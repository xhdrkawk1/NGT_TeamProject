#pragma once
class CObj;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)
public:
	enum OBJ_ID {LOGO,PLAYER,OBJECT,HOLEBACK,COUNTDOWN,END };
public:
	CObjMgr();
	~CObjMgr();

	CObj* GetPlayer() { return m_ObjLst[PLAYER].front(); }
	OBJLIST& GetObjList(OBJ_ID eId) { return m_ObjLst[eId]; };


	void AddObject(CObj* pObj, OBJ_ID eID);
	void Update();
	void Render(HDC hDC);
	void Release();
	void ReleaseGroup(OBJ_ID eID);
	void ClearServerRender();
private:
	OBJLIST	m_ObjLst[END];

};

