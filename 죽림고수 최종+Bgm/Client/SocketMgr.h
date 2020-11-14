#pragma once
class CSocketMgr
{
	DECLARE_SINGLETON(CSocketMgr)
public:
	enum SocketType{LOGIN,LOBBY,INGAME,FINAL};
public:
	CSocketMgr();
	~CSocketMgr();
public:
	SocketType Get_SocketType() { return m_eType; };
public:
	HRESULT InitSocketMgr();
	void err_quit(char* msg);
	void err_display(char* msg);

	HRESULT Update_SocketMgr();
	HRESULT UpdateLogin();
	HRESULT UpdateLobby();
	HRESULT UpdateIngame();
	HRESULT UpdateFinal();
private:
	
	SocketType m_eType;

};

