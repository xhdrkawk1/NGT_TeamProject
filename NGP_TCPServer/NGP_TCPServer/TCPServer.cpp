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

// ���� �Լ� ���� ���
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
		SOCK_STREAM,  // TCP ��������
		0);
	if (INVALID_SOCKET == pListenSocket)
		err_quit("socket()");

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
		err_quit("bind()");

	/*__________________________________________________________________________________________________________
	[ Make ListenState ]
	____________________________________________________________________________________________________________*/
	retval = listen(pListenSocket, // Ŭ���̾�Ʈ ������ ������ �������� ���� ����.
		SOMAXCONN);    // ���� ������ Ŭ���̾�Ʈ ����. �Ϻ� �������ݿ��� ���� ������ �ִ�.
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

		pClientSocket = accept(pListenSocket,           // Ŭ���̾�Ʈ ������ ������ �������� ���� ����.
			(SOCKADDR*)&tClientAddr, // ������ Ŭ���̾�Ʈ�� �ּ� ������ ä������.
			&iAddrLen);              // accept() �Լ��� ä������ �ּ� ������ ũ��.
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
		hThread = CreateThread(NULL,                    // �ڵ� ��Ӱ� ���� ��ũ���� ����.
			0,                       // �����忡 �Ҵ�Ǵ� ���� ũ��. �⺻ ���� 1MB.
			ClientThread,            // ������ �Լ��� ���� �ּ�.
			(LPVOID)pClientSocket,   // ������ �Լ� ���� ����. 
			0,                       // ������ ������ �����ϴ� ��.  0 �Ǵ� CREATE_SUSPENDED
			NULL);                   // DWORD������ �����ϸ� ������ ID�� �����. �ʿ� ���ٸ� NULL.

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
		received = recv(s,      // ����� ���� ����� ����.
			ptr,    // ���� �����͸� ������ ������ �ּ�.
			left,   // ���� ���۷κ��� ������ �ִ� �������� ũ��.
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
