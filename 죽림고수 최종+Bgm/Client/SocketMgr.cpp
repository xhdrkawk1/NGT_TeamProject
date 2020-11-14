#include "stdafx.h"
#include "Logo.h"
#include "SocketMgr.h"
#include "iostream"
#include "Player.h"
#include "HoldBack.h"
#include "Enemy.h"
IMPLEMENT_SINGLETON(CSocketMgr)
SOCKET m_Socket;
SOCKADDR_IN m_serveraddr;
int recvn(SOCKET s, char* buf, int len, int flags, SOCKADDR_IN addr);
CSocketMgr::CSocketMgr()
{
}


CSocketMgr::~CSocketMgr()
{
	
}

HRESULT CSocketMgr::InitSocketMgr()
{

	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return E_FAIL;
	
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)err_quit("socket()");
    

	ZeroMemory(&m_serveraddr, sizeof(m_serveraddr));
	m_serveraddr.sin_family = AF_INET;
	m_serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	m_serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(m_Socket, (SOCKADDR*)&m_serveraddr, sizeof(m_serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	m_eType = LOGIN;
    
	CObj* pObj = nullptr;
	pObj = CAbstractFactory<CLogo>::CreateObj();
	CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::LOGO);
	

	return S_OK;
}

void CSocketMgr::err_quit(char* msg)
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

void CSocketMgr::err_display(char* msg)
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

HRESULT CSocketMgr::Update_SocketMgr()
{
	switch (m_eType)
	{
	case CSocketMgr::LOGIN:
		UpdateLogin();
		break;
	case CSocketMgr::LOBBY:
		UpdateLobby();
		break;
	case CSocketMgr::INGAME:
		UpdateIngame();
		break;
	case CSocketMgr::FINAL:
		UpdateFinal();
		break;
	default:
		break;
	}



	return S_OK;
}

HRESULT CSocketMgr::UpdateLogin()
{
	string strName;
	std::cout << "닉네임을 입력하세요: " << std::endl;
	cin >> strName;
	
	int iSize = strName.length();
	send(m_Socket, (char*)&iSize, sizeof(int) , 0);
	send(m_Socket, (char*)strName.c_str(), sizeof(char) * iSize, 0);


	int addrlen;

	addrlen = sizeof(m_serveraddr);
	getpeername(m_Socket, (SOCKADDR*)&m_serveraddr, &addrlen);
	int retval;
	bool bIs;
	while (1)
	{
		retval = recvn(m_Socket, (char*)&bIs, sizeof(bool), 0, m_serveraddr);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;
		bool LoginSuccess = (bool)bIs;
	
		if (LoginSuccess == true)
		{
			m_eType = LOBBY;
			cout << "Login Success" << endl;

			GET_INSTANCE(CObjMgr)->GetObjList(CObjMgr::LOGO).front()->IsDead();

			CObj *m_pPlayer = CAbstractFactory<CPlayer>::CreateObj();
			CObjMgr::GetInstance()->AddObject(m_pPlayer, CObjMgr::PLAYER);

			CObj *pObj = CAbstractFactory<CHoldBack>::CreateObj();
			CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::HOLEBACK);

		}
		else
		{
			cout << "아이디 중복입니다" << endl;
		}
		return S_OK;
	}


	





	return S_OK;
}

HRESULT CSocketMgr::UpdateLobby()
{
	bool bIsTemp;
	int retval;

	while (1)
	{
		retval = recvn(m_Socket, (char*)&bIsTemp, sizeof(bool), 0, m_serveraddr);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;
		bool LoginSuccess = (bool)bIsTemp;
		if (LoginSuccess)
		{
			m_eType = INGAME;
			cout << "GameStart" << endl;

			CObj* pobj = CAbstractFactory<CEnemy>::CreateObj(0.f,0.f);
			CObjMgr::GetInstance()->AddObject(pobj, CObjMgr::ENEMY);

			break;
		}
		break;
	}


	return S_OK;
}

HRESULT CSocketMgr::UpdateIngame()
{

	float vEnemyPos[2];
	float vMyPos[2];
	int retval;
	D3DXVECTOR3 vec3= GET_INSTANCE(CObjMgr)->GetPlayer()->Get_Info().vPos;;
	memcpy(&vMyPos, &vec3, sizeof(float) * 2);

	send(m_Socket, (char*)&vMyPos, sizeof(float) * 2, 0);


	while (1)
	{
		retval = recvn(m_Socket, (char*)&vEnemyPos, sizeof(float)*2.f, 0, m_serveraddr);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		break;
	}

	GET_INSTANCE(CObjMgr)->GetObjList(CObjMgr::ENEMY).front()->Set_Pos(vEnemyPos[0], vEnemyPos[1]);


	return S_OK;
}

HRESULT CSocketMgr::UpdateFinal()
{
	return S_OK;
}
int recvn(SOCKET s, char* buf, int len, int flags, SOCKADDR_IN addr)
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
		printf("[TCP/%s:%d] %f\n", inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port), ((float(len - left) / (float)len)) * 100.f);
	}

	return (len - left);
}