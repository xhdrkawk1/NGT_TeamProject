#pragma once
class CObj;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)
public:
	enum OBJ_ID {PLAYER,ARROW,ARROW2,WARNING,LAGER,HOLEBACK,END };
public:
	CObjMgr();
	~CObjMgr();

	CObj* GetPlayer() { return m_ObjLst[PLAYER].front(); }
	
	void AddObject(CObj* pObj, OBJ_ID eID);
	void Update();
	void Render(HDC hDC);
	void Release();
	void ReleaseGroup(OBJ_ID eID);
private:
	OBJLIST	m_ObjLst[END];
public:
	SOCKET sock;
};

