#include "CommonHeader.h"
#include "DataMgr.h"
#include "ObjectMgr.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>
#include "CollisionMgr.h"


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

// 소켓 함수 오류 출력
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

    if (m_eType[0] == INGAME && m_eType[1] == INGAME)
        CObjectMgr::GetInstance()->Update();
   /* else if (m_eType[0] == FINAL && m_eType[1] == FINAL)
        SaveLoadScore();*/


    m_fServerTime += CTimeMgr::GetInstance()->GetDeltaTime();
    if (m_bIsClockReset)
    {
        m_bIsClockReset = false;
        m_fServerTime = 0.f;
    }

}

void CDataMgr::SaveLoadScore()
{
  
    ifstream fin("../HighScore/HighScore.txt");
    SCOREINFO tTemp;
   
    int iNum = 0;

    vector<string> vecStr{ istream_iterator<string> {fin }, { } };
  
    int i = 0;
    for (auto& Info : vecStr)
    {
        
        if(i==0)
            tTemp.fScore = stoi(Info);
        else if (i == 1)
        {
            tTemp.Name = Info;

            SCOREINFO *Data=new SCOREINFO;
            *Data = tTemp;
            m_vecScoreInfo.push_back(Data);
            i == 0;
        }
        i++;
    }

   
    for (int i = 0; i < 2; i++)
    {
		SCOREINFO* Data = new SCOREINFO;
		Data->Name = m_tPlayerData[i].strName;
		Data->fScore = m_tPlayerData[i].fLifeTime;
		m_vecScoreInfo.push_back(Data);
	}
	sort(m_vecScoreInfo.begin(), m_vecScoreInfo.end(), [](const SCOREINFO* lhs, const SCOREINFO* rhs)
    {

        return lhs->fScore < rhs->fScore;

    });
    fin.close();

    ofstream fout("../HighScore/HighScore.txt",ios::trunc);




    for (int i = 0 ;i < m_vecScoreInfo.size(); ++i)
    {
        if (i == 5)
            break;
        fout << m_vecScoreInfo[i]->fScore << " " << m_vecScoreInfo[i]->Name << endl;

    }

 
    fout.close();

    //delete[] Data;
}



HRESULT CDataMgr::CreateThreadForClient()
{

    if (!((m_eType[0] == INGAME||m_eType[0] == COUNTDOWN ||m_eType[0]==FINAL) && ( m_eType[1] == INGAME|| m_eType[1] == COUNTDOWN||m_eType[1] == FINAL)))
        return S_OK;



    UpdatePreData();//여기서 전데이터 보내면 됨.


    HANDLE hThread[2];
    hThread[0] = CreateThread(NULL,                    // 핸들 상속과 보안 디스크립터 정보.
        0,                       // 스레드에 할당되는 스택 크기. 기본 값은 1MB.
        ClientThread,            // 스레드 함수의 시작 주소.
         (LPVOID*)0,   // 스레드 함수 전달 인자. //0번 플레이어,1번플레이어만 확인하면됨 소켓은 우리가 Vector에 들고잇음.
        0,                       // 스레드 생성을 제어하는 값.  0 또는 CREATE_SUSPENDED
        NULL);
    if (m_iConnect_Player >=1)
    {
        hThread[1] = CreateThread(NULL,                    // 핸들 상속과 보안 디스크립터 정보.
            0,                       // 스레드에 할당되는 스택 크기. 기본 값은 1MB.
            ClientThread,            // 스레드 함수의 시작 주소.
           (LPVOID*)1,   // 스레드 함수 전달 인자. //0번 플레이어,1번플레이어만 확인하면됨 소켓은 우리가 Vector에 들고잇음.
            0,                       // 스레드 생성을 제어하는 값.  0 또는 CREATE_SUSPENDED
            NULL);

    }

    WaitForSingleObject(hThread[0], INFINITE);
    CloseHandle(hThread[0]);

    if (m_iConnect_Player >=1)
    {
        WaitForSingleObject(hThread[1], INFINITE);
        CloseHandle(hThread[1]);//커널 인터페이스 가 2개를 자식 부모 두개를 다루고있기때문에 닫아야함.

    }

    if (m_iPlayerExit[0]  &&  m_iPlayerExit[1])
    {
        //초기데이터로 갱신
        cout << " 접속종료" << endl;
        m_eType[0] = LOGIN;
        m_eType[1] = LOGIN;
        m_iConnect_Player = 0;
        m_bIsClockReset = false;
        m_iPlayerExit[0] = false;
        m_iPlayerExit[1] = false;
        m_tPlayerData[0].Alive = true;
        m_tPlayerData[1].Alive = true;
        closesocket(m_ClientSocketList[0]);
        closesocket(m_ClientSocketList[1]);
        m_ClientSocketList.clear();
        m_fServerTime = 0.f;
        

        for(int i=0; i<m_vecScoreInfo.size();++i)
        Safe_Delete(m_vecScoreInfo[i]);

        m_vecScoreInfo.clear();
        m_vecScoreInfo.clear();
   
        CCollisionMgr::InitColinumber();

        GET_INSTANCE(CObjectMgr)->Clear_AllObject();


    }


    return S_OK;
}

HRESULT CDataMgr::CreateThareadForLobby()
{

 
    HANDLE hThread;
    hThread = CreateThread(NULL,                    // 핸들 상속과 보안 디스크립터 정보.
        0,                       // 스레드에 할당되는 스택 크기. 기본 값은 1MB.
        ClientLoginThread,            // 스레드 함수의 시작 주소.
        (LPVOID*)m_iConnect_Player,   // 스레드 함수 전달 인자. //0번 플레이어,1번플레이어만 확인하면됨 소켓은 우리가 Vector에 들고잇음.
        0,                       // 스레드 생성을 제어하는 값.  0 또는 CREATE_SUSPENDED
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
    case CDataMgr::COUNTDOWN:
        if(FAILED(CountDownUpdate(iPlayerNum)))
            return E_FAIL;
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
            cout << iPlayerNum << "번 플레이어 입장~" << endl;

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

  
        if (m_eType[iAnotherPlayer] == COUNTDOWN || m_eType[iAnotherPlayer] == LOBBY)
        {
            bIsGameStart = true;
            m_eType[iPlayerNum] = COUNTDOWN;
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

            memcpy(&m_tPlayerData[iPlayerNum].Pos, Pos, sizeof(float) * 2);//우리플레이어좌표
            break;
        }

     send(m_ClientSocketList[iPlayerNum], (char*)&bIsGameStart, sizeof(bool), 0);
  
 

    return S_OK;
}

HRESULT CDataMgr::CountDownUpdate(int iPlayerNum)
{
    

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

        retval = recvn(client_sock, (char*)&Pos, sizeof(float) * 2, 0, clientaddr);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        memcpy(&m_tPlayerData[iPlayerNum].Pos, Pos, sizeof(float) * 2);//우리플레이어좌표

        break;
    }

    send(client_sock, (char*)&m_tPlayerData[iAnotherPlayer].Pos, sizeof(float) * 2, 0);//임시 적플레이어좌표
    send(client_sock, (char*)&m_fServerTime, sizeof(float), 0);//서버타임

    if (m_fServerTime > 3.f || m_eType[iAnotherPlayer] == INGAME)
    {
        m_eType[iPlayerNum] = INGAME;
        m_bIsClockReset = true;
    }

    return S_OK;
}

HRESULT CDataMgr::IngameUpdate(int iPlayerNum)
{
 


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

        memcpy(&m_tPlayerData[iPlayerNum].Pos, Pos, sizeof(float) * 2);//우리플레이어좌표
        break;
    }
    EnterCriticalSection(&m_Crt);

    GET_INSTANCE(CObjectMgr)->Player_Collsion(iPlayerNum);

    send(client_sock, (char*)&m_tPlayerData[iPlayerNum].Alive, sizeof(bool), 0);
    send(client_sock, (char*)&m_tPlayerData[iAnotherPlayer].Pos, sizeof(float) * 2, 0);//임시 적플레이어좌표
    send(client_sock, (char*)&m_tPlayerData[iAnotherPlayer].Alive, sizeof(bool), 0);

    LeaveCriticalSection(&m_Crt);


    send(client_sock, (char*)&m_fServerTime, sizeof(float), 0);//서버타임


    int size = CObjectMgr::GetInstance()->Straight_ArrowInformation_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//벡터의 사이즈
    for(auto &vec: CObjectMgr::GetInstance()->Straight_ArrowInformation_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//화살 월드매트릭스

    size = CObjectMgr::GetInstance()->Guide_ArrowInformation_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//벡터의 사이즈
    for (auto& vec : CObjectMgr::GetInstance()->Guide_ArrowInformation_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//유도화살 월드매트릭스


    size = CObjectMgr::GetInstance()->WarningX_Information_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//벡터의 사이즈
    for (auto& vec : CObjectMgr::GetInstance()->WarningX_Information_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//워닝X 월드매트릭스

    size = CObjectMgr::GetInstance()->WarningY_Information_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//벡터의 사이즈
    for (auto& vec : CObjectMgr::GetInstance()->WarningY_Information_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//워닝Y 월드매트릭스



    size = CObjectMgr::GetInstance()->LagerX_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//벡터의 사이즈
    for (auto& vec : CObjectMgr::GetInstance()->LagerX_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//레이저X 월드매트릭스

    size = CObjectMgr::GetInstance()->LagerY_list.size();
    send(client_sock, (char*)&size, sizeof(int), 0);//벡터의 사이즈
    for (auto& vec : CObjectMgr::GetInstance()->LagerY_list)
        send(client_sock, (char*)&vec->mat_World, sizeof(D3DXMATRIX), 0);//레이저Y 월드매트릭스

    bool bIsGotoNextChapter = false;


    if ((m_tPlayerData[iPlayerNum].Alive == false && m_tPlayerData[iAnotherPlayer].Alive == false)||m_eType[iAnotherPlayer]==FINAL)
        bIsGotoNextChapter = true;
    send(client_sock, (char*)&bIsGotoNextChapter, sizeof(bool), 0);//레이저Y 월드매트릭스


    if (bIsGotoNextChapter)
        m_eType[iPlayerNum] = FINAL;



    
    return S_OK;
}

HRESULT CDataMgr::FinalUpdate(int iPlayerNum)
{
    
    if (m_iPlayerExit[iPlayerNum])
        return S_OK;

    SOCKET client_sock = m_ClientSocketList[iPlayerNum];
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    int iAnotherPlayer = 0;
    if (iPlayerNum == 0)
        iAnotherPlayer = 1;

    send(client_sock, (char*)&m_tPlayerData[iPlayerNum].fLifeTime, sizeof(float), 0);//플레이어 시간 보냄
    send(client_sock, (char*)&m_tPlayerData[iAnotherPlayer].fLifeTime, sizeof(float), 0);//플레이어 시간 보냄

    
    int Size = m_vecScoreInfo.size();
    send(client_sock, (char*)&Size, sizeof(int), 0);//벡터 사이즈 보냄

    for (auto& Info : m_vecScoreInfo)
    {
        Size = Info->Name.size();
        send(client_sock, (char*)&Size, sizeof(int), 0);//스트링 사이즈 보냄
        char str[100] = { 0, };
        strcpy(str, Info->Name.c_str());
        send(client_sock, (char*)&str, sizeof(char)*Size, 0);

        send(client_sock, (char*)&Info->fScore, sizeof(float), 0);//스트링 사이즈 보냄
    }



    EnterCriticalSection(&m_Crt);
    m_iPlayerExit[iPlayerNum] = true;
    LeaveCriticalSection(&m_Crt);
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