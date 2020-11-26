#pragma once

struct OBJECT_WARNING;
struct OBJECT_ARROW;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:

	static void CollisionRect(PLAYERDATA* pdata, list<OBJECT_WARNING*>& srcLst);
	static void CollisionRect(PLAYERDATA* pdata, list<OBJECT_ARROW *>& srcLst);
	
	static void CollisionRect(list<OBJECT_WARNING*>& LagerLst, list<OBJECT_ARROW*>& ArrowLst);
private:
	
	//static bool CheckRect(CObj* pDst, CObj* pSrc);
};

