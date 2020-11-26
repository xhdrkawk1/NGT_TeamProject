#include "CommonHeader.h"
#include "DataMgr.h"
#include "ObjectMgr.h"

IMPLEMENT_SINGLETON(CDataMgr)

DWORD __stdcall ClientThread(LPVOID arg);
DWORD __stdcall ClientLoginThread(LPVOID arg);

CDataMgr::CDataMgr()
{
   
}


CDataMgr::~CDataMgr()
{
}

void CDataMgr::err_quit(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPTSTR)msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// ���� �Լ� ���� ���
void CDataMgr::err_display(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}
void CDataMgr::InitDataMgr()
{
    InitializeCriticalSection(&m_Crt);
    m_eType[0] = LOGIN;
    m_eType[1] = LOGIN;
    CObjectMgr::GetInstance()->Initialize();
}

void CDataMgr::UpdatePreData()
{
    CTimeMgr::GetInstance()->UpdateTime();
    CKeyMgr::GetInstance()->KeyCheck();

    CObjectMgr::GetInstance()->Update();

    m_fServerTime += CTimeMgr::GetInstance()->GetDeltaTime();
    cout << m_fServerTime << endl;
}



HRESULT CDataMgr::CreateThreadForClient()
{

    if (!(m_eType[0] == INGAME && m_eType[1] == INGAME))
        return S_OK;

    UpdatePreData();//���⼭ �������� ������ ��.


    HANDLE hThread[2];
    hThread[0] = CreateThread(NULL,                    // �ڵ� ��Ӱ� ���� ��ũ���� ����.
        0,                       // �����忡 �Ҵ�Ǵ� ���� ũ��. �⺻ ���� 1MB.
        ClientThread,            // ������ �Լ��� ���� �ּ�.
         (LPVOID*)0,   // ������ �Լ� ���� ����. //0�� �÷��̾�,1���÷��̾ Ȯ���ϸ�� ������ �츮�� Vector�� �������.
        0,                       // ������ ������ �����ϴ� ��.  0 �Ǵ� CREATE_SUSPENDED
        NULL);
    if (m_iConnect_Player >=1)
    {
        hThread[1] = CreateThread(NULL,                    // �ڵ� ��Ӱ� ���� ��ũ���� ����.
            0,                       // �����忡 �Ҵ�Ǵ� ���� ũ��. �⺻ ���� 1MB.
            ClientThread,            // ������ �Լ��� ���� �ּ�.
           (LPVOID*)1,   // ������ �Լ� ���� ����. //0�� �÷��̾�,1���÷��̾ Ȯ���ϸ�� ������ �츮�� Vector�� �������.
            0,                       // ������ ������ �����ϴ� ��.  0 �Ǵ� CREATE_SUSPENDED
            NULL);

    }

    WaitForSingleObject(hThread[0], INFINITE);
    CloseHandle(hThread[0]);

    if (m_iConnect_Player >=1)
    {
        WaitForSingleObject(hThread[1], INFINITE);
        CloseHandle(hThread[1]);//Ŀ�� �������̽� �� 2���� �ڽ� �θ� �ΰ��� �ٷ���ֱ⶧���� �ݾƾ���.

    }
    return S_OK;
}

HRESULT CDataMgr::CreateThareadForLobby()
{

 
    HANDLE hThread;
    hThread = CreateThread(NULL,                    // �ڵ� ��Ӱ� ���� ��ũ���� ����.
        0,                       // �����忡 �Ҵ�Ǵ� ���� ũ��. �⺻ ���� 1MB.
        ClientLoginThread,            // ������ �Լ��� ���� �ּ�.
        (LPVOID*)m_iConnect_Player,   // ������ �Լ� ���� ����. //0�� �÷��̾�,1���÷��̾ Ȯ���ϸ�� ������ �츮�� Vector�� �������.
        0,                       // ������ ������ �����ϴ� ��.  0 �Ǵ� CREATE_SUSPENDED
        NULL);
    CloseHandle(hThread);

    return S_OK;


}

HRESULT CDataMgr::Update(int iPlayerNum)
{
    switch (m_eType[iPlayerNum])
    {
    case CDataMgr::LOGIN:
        if (FAILED(LoginUpdate(iPlayerNum)))
            return E_FAIL;
        break;
    case CDataMgr::LOBBY:
        if (FAILED(LobbyUpdate(iPlayerNum)))
            return E_FAIL;

        m_fServerTime = 0.f;
        break;
    case CDataMgr::INGAME:
        if (FAILED(IngameUpdate(iPlayerNum)))
            return E_FAIL;
        break;
    case CDataMgr::FINAL:
        if (FAILED(FinalUpdate(iPlayerNum)))
            return E_FAIL;
        break;
    default:
        break;
    }



    return S_OK;
}

HRESULT CDataMgr::LoginUpdate(int iPlayerNum)
{
    
    SOCKET client_sock = m_ClientSocketList[iPlayerNum];
    int retval;
    int len;
    SOCKADDR_IN clientaddr;
    int addrlen;
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    char* Buf = nullptr;
    bool LoginSuccess = false;
    while (!LoginSuccess)
    {
        while (1)
        {
            retval = recvn(client_sock, (char*)&len, sizeof(int), 0, clientaddr);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;;
            Buf = new char[len + 1];
            retval = recvn(client_sock, Buf, len, 0, clientaddr);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                delete[] Buf;
                Buf = nullptr;
                break;
            }
            else if (retval == 0)
            {
                delete[] Buf;
                Buf = nullptr;
                break;

            }
            Buf[retval] = '\0';
            cout << Buf << endl;
            break;
        }


        int iAnotherPlayer = 0;
        if (iPlayerNum == 0)
            iAnotherPlayer = 1;
        string strName = Buf;


        EnterCriticalSection(&m_Crt);
        if (m_tPlayerData[iAnotherPlayer].strName != strName)
        {
            LoginSuccess = true;
            m_tPlayerData[iPlayerNum].strName = strName;
        }
        else
            LoginSuccess = false;
        LeaveCriticalSection(&m_Crt);



        send(client_sock, (char*)&LoginSuccess, sizeof(bool), 0);
        if (LoginSuccess)
        {
            m_eType[iPlayerNum] = LOBBY;
            cout << iPlayerNum << "�� �÷��̾� ����~" << endl;

        }

    }
    return S_OK;
}

HRESULT CDataMgr::LobbyUpdate(int iPlayerNum)
{
    bool bIsGameStart = false;
    int iAnotherPlayer = 0;
    if (iPlayerNum == 0)
        iAnotherPlayer = 1;

  
        if (m_eType[iAnotherPlayer] == INGAME || m_eType[iAnotherPlayer] == LOBBY)
        {
            bIsGameStart = true;
            m_eType[iPlayerNum] = INGAME;
        }



        SOCKET client_sock = m_ClientSocketList[iPlayerNum];
        int retval;
        float Pos[2];
        SOCKADDR_IN clientaddr;
        int addrlen;
        addrlen = sizeof(clientaddr);
        getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
        bool LoginSuccess = false;
        while (1)
        {

            retval = recvn(client_sock, (char*)&Pos, sizeof(float) * 2, 0, clientaddr);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            memcpy(&m_tPlayerData[iPlayerNum].Pos, Pos, sizeof(float) * 2);//�츮�÷��̾���ǥ
            break;
        }

     send(m_ClientSocketList[iPlayerNum], (char*)&bIsGameStart, sizeof(bool), 0);
  
 

    return S_OK;
}

HRESULT CDataMgr::IngameUpdate(int iPlayerNum)
{
    if (iPlayerNum == 0)
    {
        int i = 0;

    }
    else
        int i = 1;

    SOCKET client_sock = m_ClientSocketList[iPlayerNum];
    int retval;
    float Pos[2];
    SOCKADDR_IN clientaddr;
    int addrlen;
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    bool LoginSuccess = false;

    int iAnotherPlayer = 0;
    if (iPlayerNum == 0)
        iAnotherPlayer = 1;

    while (1)
    {
        
        retval = recvn(client_sock, (char*)&Pos, sizeof(float)*2, 0, clientaddr);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        memcpy(&m_tPlayerData[iPlayerNum].Pos, Pos, sizeof(float) * 2);//�츮�÷��̾���ǥ

        break;
    }
      
    send(client_sock, (char*)&m_tPlayerData[iAnotherPlayer].Pos, sizeof(float) * 2, 0);//�ӽ� ���÷��̾���ǥ
    send(client_sock, (char*)&m_fServerTime, sizeof(float), 0);//����Ÿ��



    int size = CObjectMgr::GetInstance()->Straight_ArrowInformation_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//������ ������
    for(auto &vec: CObjectMgr::GetInstance()->Straight_ArrowInformation_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//ȭ�� �����Ʈ����

    size = CObjectMgr::GetInstance()->Guide_ArrowInformation_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//������ ������
    for (auto& vec : CObjectMgr::GetInstance()->Guide_ArrowInformation_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//����ȭ�� �����Ʈ����


    size = CObjectMgr::GetInstance()->WarningX_Information_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//������ ������
    for (auto& vec : CObjectMgr::GetInstance()->WarningX_Information_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//����X �����Ʈ����

    size = CObjectMgr::GetInstance()->WarningY_Information_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//������ ������
    for (auto& vec : CObjectMgr::GetInstance()->WarningY_Information_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//����Y �����Ʈ����



    size = CObjectMgr::GetInstance()->LagerX_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//������ ������
    for (auto& vec : CObjectMgr::GetInstance()->LagerX_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//������X �����Ʈ����

    size = CObjectMgr::GetInstance()->LagerY_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//������ ������
    for (auto& vec : CObjectMgr::GetInstance()->LagerY_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//������Y �����Ʈ����


    return S_OK;
}

HRESULT CDataMgr::FinalUpdate(int iPlayerNum)
{
    return S_OK;
}

DWORD __stdcall ClientThread(LPVOID arg)
{
    int iPlayerId = (int)arg;

        if (FAILED(GET_INSTANCE(CDataMgr)->Update(iPlayerId)))
            return E_FAIL;

    return S_OK;
}

DWORD __stdcall ClientLoginThread(LPVOID arg)
{
    int iPlayerId = (int)arg;
     while (GET_INSTANCE(CDataMgr) -> m_eType[iPlayerId]==CDataMgr::LOBBY || GET_INSTANCE(CDataMgr)->m_eType[iPlayerId] == CDataMgr::LOGIN)
     {
    if (FAILED(GET_INSTANCE(CDataMgr)->Update(iPlayerId)))
        return E_FAIL;
     }
    return S_OK;
}

int CDataMgr::recvn(SOCKET s, char* buf, int len, int flags, SOCKADDR_IN addr)
{
    int received;
    char* ptr = buf;
    int left = len;
    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        else if (received == 0)
            break;
        left -= received;
        ptr += received;
    }

    return (len - left);
}