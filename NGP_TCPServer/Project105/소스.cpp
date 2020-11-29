#include "CommonHeader.h"




DWORD WINAPI ClientThread(LPVOID arg);


int main(int argc, char* argv[])
{

    /*__________________________________________________________________________________________________________
   [ Ready Manager ]
    ____________________________________________________________________________________________________________*/

    CDataMgr::GetInstance()->InitDataMgr();
    
    CKeyMgr::GetInstance();

    int retval = 0;
    /*__________________________________________________________________________________________________________
   [ Ready WSA ]
    ____________________________________________________________________________________________________________*/
    WSADATA tWSA;
    if (WSAStartup(MAKEWORD(2, 2), &tWSA))
        return 1;

    /*__________________________________________________________________________________________________________
   [ Ready ListenSocket ]
    ____________________________________________________________________________________________________________*/
    SOCKET pListenSocket = socket(AF_INET,      // IPv4
        SOCK_STREAM,  // TCP ��������
        0);
    if (INVALID_SOCKET == pListenSocket)
        GET_INSTANCE(CDataMgr)->err_quit("socket()");

    /*__________________________________________________________________________________________________________
   [ Bind ListenSocket ]
    ____________________________________________________________________________________________________________*/
    SOCKADDR_IN tServerAddr;
    ZeroMemory(&tServerAddr, sizeof(tServerAddr));
    tServerAddr.sin_family = AF_INET;             // IPv4
    tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);   // Ŭ���̾�Ʈ�� � IP�ּҷε� ���� ���.
    tServerAddr.sin_port = htons(SERVERPORT);  // ������ ���� ��Ʈ ��ȣ ����.

    retval = bind(pListenSocket,                       // Ŭ���̾�Ʈ ������ ������ �������� ���� ����.
        (SOCKADDR*)&tServerAddr,             // ���� ���� ����ü.
        sizeof(tServerAddr));                // ���� ���� ����ü�� ����.
    if (SOCKET_ERROR == retval)
        GET_INSTANCE(CDataMgr)->err_quit("bind()");

    /*__________________________________________________________________________________________________________
    [ Make ListenState ]
    ____________________________________________________________________________________________________________*/
    retval = listen(pListenSocket, // Ŭ���̾�Ʈ ������ ������ �������� ���� ����.
        SOMAXCONN);    // ���� ������ Ŭ���̾�Ʈ ����. �Ϻ� �������ݿ��� ���� ������ �ִ�.
    if (SOCKET_ERROR == retval)
        GET_INSTANCE(CDataMgr)->err_quit("listen()");

   
    /*__________________________________________________________________________________________________________
    [ Ready ClientSocket ]
    ____________________________________________________________________________________________________________*/
    int         iAddrLen = 0;
    SOCKET      pClientSocket = NULL;
    SOCKADDR_IN tClientAddr;
    ZeroMemory(&tClientAddr, sizeof(SOCKADDR_IN));


    /*__________________________________________________________________________________________________________
     [ Server MainLoop ]
    ____________________________________________________________________________________________________________*/
    while (true)
    {
        if (CDataMgr::GetInstance()->m_iConnect_Player < 2)
        {
            cout << "[ Server ] - listening ... " << endl;
            // Accept Client
            iAddrLen = sizeof(tClientAddr);
            pClientSocket = accept(pListenSocket,           // Ŭ���̾�Ʈ ������ ������ �������� ���� ����.
                (SOCKADDR*)&tClientAddr, // ������ Ŭ���̾�Ʈ�� �ּ� ������ ä������.
                &iAddrLen);              // accept() �Լ��� ä������ �ּ� ������ ũ��.
            if (INVALID_SOCKET == pClientSocket)
            {
               GET_INSTANCE(CDataMgr)-> err_display("accept()");
                break;
            }

            // Print ClientInfo.
            cout << "[TCP Server] Client Connect \t IP Address = " << inet_ntoa(tClientAddr.sin_addr)
                << "\t Port Number = " << ntohs(tClientAddr.sin_port) << endl;
            cout << endl;
         
          

            CDataMgr::GetInstance()->m_ClientSocketList.push_back(pClientSocket);
            CDataMgr::GetInstance()->CreateThareadForLobby();
            CDataMgr::GetInstance()->m_iConnect_Player++;
        }
        else
        CDataMgr::GetInstance()->CreateThreadForClient();
    
    }


    

    // CloseSocket.
    for (int i = 0; i < CDataMgr::GetInstance()->m_ClientSocketList.size();++i)
    {
        closesocket(CDataMgr::GetInstance()->m_ClientSocketList[i]);
    }

    closesocket(pListenSocket);
    WSACleanup();
    GET_INSTANCE(CDataMgr)->DestroyInstance();
}



