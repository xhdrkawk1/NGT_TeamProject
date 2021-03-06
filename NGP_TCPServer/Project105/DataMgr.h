#pragma once

#include "define.h"


typedef struct  tagPlayerData
{
	string strName = "";
	bool   Alive = true;
	float  fLifeTime = 0.f;
	float Pos[2] = { 0.f,0.f };
	RECT tRect;
}PLAYERDATA;

typedef struct tagHighScoreInfo
{
	float fScore;
	string Name;
}SCOREINFO;




class CDataMgr
{
	DECLARE_SINGLETON(CDataMgr)
public:
	enum SocketType {LOGIN,LOBBY,COUNTDOWN,INGAME,FINAL};


public:
	CDataMgr() ;
	~CDataMgr() ;
	void	InitDataMgr();
	void	err_quit(char* msg);
	void	err_display(char* msg);
	int    recvn(SOCKET s, char* buf, int len, int flags, SOCKADDR_IN addr);
	void	UpdatePreData();
	void    SaveLoadScore();


	HRESULT CreateThreadForClient();
	HRESULT CreateThareadForLobby();


	HRESULT Update(int iPlayerNum);
	HRESULT LoginUpdate(int iPlayerNum);
	HRESULT LobbyUpdate(int iPlayerNum);
	HRESULT CountDownUpdate(int iPlayerNum);
	HRESULT IngameUpdate(int iPlayerNum);
	HRESULT FinalUpdate(int iPlayerNum);



public:

	int m_iConnect_Player = 0;
	SocketType m_eType[2];

	vector<SCOREINFO*> m_vecScoreInfo;
	vector<SOCKET> m_ClientSocketList;
	PLAYERDATA   m_tPlayerData[2];


	CRITICAL_SECTION   m_Crt;

	float m_fStartTime = 0.f;

	float m_fServerTime = 0.f;

	bool m_bIsClockReset = false;

	bool m_iPlayerExit[2] = { 0, };
};

