#pragma once

class CObj;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	
	
	//static void CollisionRectEX(OBJLIST& dstLst, OBJLIST& srcLst);
	//static void CollisionSphere(OBJLIST& dstLst, OBJLIST& srcLst);
	static void CollisionRect(OBJLIST& dstLst, OBJLIST& srcLst);
	//static void CollisionItem(OBJLIST& dstLst, OBJLIST& srcLst);
	static void CCollisionMgr::Collision_TO_player(CObj* dst, OBJLIST & srcLst);

private:
	//static bool CheckSphere(CObj* pDst, CObj* pSrc);
	//static bool CheckRect(CObj* pDst, CObj* pSrc, float* pMoveX, float* pfMoveY);
	static bool CheckRect(CObj * pDst, CObj * pSrc);
};

