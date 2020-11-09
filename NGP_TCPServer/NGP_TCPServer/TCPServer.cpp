#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

#define SERVERPORT 9000

using namespace std;

void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char* msg)
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

DWORD WINAPI ClientThread(LPVOID arg);
int ReceiveData(SOCKET s, char* buf, int len, int flags, u_short client_portnum);

int main(int argc, char* argv[])
{
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
		SOCK_STREAM,  // TCP 프로토콜
		0);
	if (INVALID_SOCKET == pListenSocket)
		err_quit("socket()");

	/*__________________________________________________________________________________________________________
   [ Bind ListenSocket ]
	____________________________________________________________________________________________________________*/
	SOCKADDR_IN tServerAddr;
	ZeroMemory(&tServerAddr, sizeof(tServerAddr));
	tServerAddr.sin_family = AF_INET;             // IPv4
	tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);   // 클라이언트의 어떤 IP주소로든 접속 허용.
	tServerAddr.sin_port = htons(SERVERPORT);  // 서버의 지역 포트 번호 설정.

	retval = bind(pListenSocket,                       // 클라이언트 접속을 수용할 목적으로 만든 소켓.
		(SOCKADDR*)&tServerAddr,             // 소켓 구조 구조체.
		sizeof(tServerAddr));                // 소켓 구조 구조체의 길이.
	if (SOCKET_ERROR == retval)
		err_quit("bind()");

	/*__________________________________________________________________________________________________________
	[ Make ListenState ]
	____________________________________________________________________________________________________________*/
	retval = listen(pListenSocket, // 클라이언트 접속을 수용할 목적으로 만든 소켓.
		SOMAXCONN);    // 접속 가능한 클라이언트 개수. 하부 프로토콜에서 지원 가능한 최댓값.
	if (SOCKET_ERROR == retval)
		err_quit("listen()");


	/*__________________________________________________________________________________________________________
	[ Ready ClientSocket ]
	____________________________________________________________________________________________________________*/
	int         iAddrLen = 0;
	SOCKET      pClientSocket = NULL;
	SOCKADDR_IN tClientAddr;
	ZeroMemory(&tClientAddr, sizeof(SOCKADDR_IN));

	HANDLE hThread;

	/*__________________________________________________________________________________________________________
	 [ Server MainLoop ]
	____________________________________________________________________________________________________________*/
	while (true)
	{
		cout << "[ Server ] - listening ... " << endl;

		// Accept Client
		iAddrLen = sizeof(tClientAddr);

		pClientSocket = accept(pListenSocket,           // 클라이언트 접속을 수용할 목적으로 만든 소켓.
			(SOCKADDR*)&tClientAddr, // 접속한 클라이언트의 주소 정보가 채워진다.
			&iAddrLen);              // accept() 함수가 채워넣은 주소 정보의 크기.
		if (INVALID_SOCKET == pClientSocket)
		{
			err_display("accept()");
			break;
		}

		// Print ClientInfo.
		cout << "[TCP Server] Client Connect \t IP Address = " << inet_ntoa(tClientAddr.sin_addr)
			<< "\t Port Number = " << ntohs(tClientAddr.sin_port) << endl;
		cout << endl;


		// 1Client - 1Thread Context Model.
		// Create Thread Context.
		hThread = CreateThread(NULL,                    // 핸들 상속과 보안 디스크립터 정보.
			0,                       // 스레드에 할당되는 스택 크기. 기본 값은 1MB.
			ClientThread,            // 스레드 함수의 시작 주소.
			(LPVOID)pClientSocket,   // 스레드 함수 전달 인자. 
			0,                       // 스레드 생성을 제어하는 값.  0 또는 CREATE_SUSPENDED
			NULL);                   // DWORD변수를 전달하면 스레드 ID가 저장됨. 필요 없다면 NULL.

		if (NULL == hThread)
			closesocket(pClientSocket);
		else
			CloseHandle(hThread);
	}

	// CloseSocket.
	closesocket(pListenSocket);
	WSACleanup();
}

DWORD __stdcall ClientThread(LPVOID arg)
{


	return 0;
}

int ReceiveData(SOCKET s, char* buf, int len, int flags, u_short client_portnum)
{
	int     received = 0;
	char* ptr = buf;
	int     left = len;

	while (left > 0)
	{
		received = recv(s,      // 통신할 대상과 연결된 소켓.
			ptr,    // 받은 데이터를 저장할 버퍼의 주소.
			left,   // 수신 버퍼로부터 복사할 최대 데이터의 크기.
			flags);

		if (SOCKET_ERROR == received)
			return SOCKET_ERROR;

		else if (0 == received)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}
