#pragma once
#include "define.h"

typedef struct  tagPlayerData
{
	string strName = "";
	bool   Alive = true;
	float Pos[2] = { 0.f,0.f };
}PLAYERDATA;


class CDataMgr
{
	enum SocketType {LOGIN,LOBBY,INGAME,FINAL};
	DECLARE_SINGLETON(CDataMgr)


public:
	CDataMgr() ;
	~CDataMgr() ;
	void	InitDataMgr();
	void	err_quit(char* msg);
	void	err_display(char* msg);
	int    recvn(SOCKET s, char* buf, int len, int flags, SOCKADDR_IN addr);
	void	UpdatePreData();

	HRESULT CreateThreadForClient();
	HRESULT Update(int iPlayerNum);
	HRESULT LoginUpdate(int iPlayerNum);
	HRESULT LobbyUpdate(int iPlayerNum);
	HRESULT IngameUpdate(int iPlayerNum);
	HRESULT FinalUpdate(int iPlayerNum);



public:

	int m_iConnect_Player = 0;
	SocketType m_eType[2];

	vector<SOCKET> m_ClientSocketList;
	PLAYERDATA   m_tPlayerData[2];


	CRITICAL_SECTION   m_Crt;
};

