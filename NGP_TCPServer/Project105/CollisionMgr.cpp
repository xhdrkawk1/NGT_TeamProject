#include "CommonHeader.h"
#include "CollisionMgr.h"
CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

static int Colinumber=0;
void CCollisionMgr::CollisionRect(PLAYERDATA* pdata, list<OBJECT_WARNING*>& srcLst)
{
	RECT rc = {};
	for (auto& pSrc : srcLst)
	{
		if (pdata->Alive == false)
			continue;

		if (IntersectRect(&rc, &pdata->tRect, &pSrc->tRect))
		{
			pdata->Alive = false;//플레이어 사망
			Colinumber++;

			if(Colinumber==2)
				CDataMgr::GetInstance()->SaveLoadScore();
			//cout << "충돌" << endl;
		}
	}
}

void CCollisionMgr::CollisionRect(PLAYERDATA* pdata, list<OBJECT_ARROW*>& srcLst)
{
	RECT rc = {};
	for (auto& pSrc : srcLst)
	{
		if (pSrc->isDead == true)
			continue;
		if (pdata->Alive == false)
			continue;
		if (IntersectRect(&rc, &pdata->tRect, &pSrc->tRect))
		{
			pdata->Alive = false;//플레이어 사망
			pdata->fLifeTime = GET_INSTANCE(CDataMgr)->m_fServerTime;
			//cout << "충돌" << endl;
			pSrc->isDead = true;

			Colinumber++;

			if (Colinumber == 2)
				CDataMgr::GetInstance()->SaveLoadScore();
		}
	}
}

void CCollisionMgr::CollisionRect(list<OBJECT_WARNING*>& LagerLst, list<OBJECT_ARROW*>& ArrowLst)
{
	RECT rc = {};
	for (auto& Lager : LagerLst)
	{
		for (auto& Arrow : ArrowLst)
		{
			if (IntersectRect(&rc, &Lager->tRect, &Arrow->tRect))
			{
				Arrow->isDead = true;		
			}
		}
	}
}

void CCollisionMgr::InitColinumber()
{
	Colinumber = 0;
}


//
//void CCollisionMgr::CollisionRect(OBJLIST& dstLst, OBJLIST& srcLst)
//{
//	RECT rc = {};
//
//	// auto == CObj*
//	for (auto& pDst : dstLst)
//	{
//		for (auto& pSrc : srcLst)
//		{
//			// IntersectRect(1번 렉트, 2번 렉트, 3번 렉트): 두 사각형이 충돌하면 TRUE를 반환.
//			// 1번 렉트: 두 사각형이 충돌하여 생겨난 겹친 영역을 기록.
//			// 2번 렉트: 충돌 대상1
//			// 3번 렉트: 충돌 대상2
//			if (IntersectRect(&rc, &(pDst->GetRect()), &(pSrc->GetRect())))
//				//if(CheckRect(pDst, pSrc))
//			{
//				//pDst->IsDead();
//				pSrc->IsDead();
//			}
//		}
//	}
//}

//
//bool CCollisionMgr::CheckRect(CObj* pDst, CObj* pSrc)
//{
//	// x축 반지름 합
//	float fSumRadX = (pDst->GetImgInfo().tImgInfo.Width + pSrc->GetImgInfo().tImgInfo.Width) * 0.5f;
//
//	// x축 거리
//	// fabs(X): X의 절대 값을 계산
//	float fDistX = fabs(pDst->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);
//
//	// y축 반지름 합
//	float fSumRadY = (pDst->GetImgInfo().tImgInfo.Height + pSrc->GetImgInfo().tImgInfo.Height) * 0.5f;
//
//	// y축 거리
//	float fDistY = fabs(pDst->Get_Info().vPos.y - pSrc->Get_Info().vPos.y);
//
//	if (fSumRadX > fDistX || fSumRadY > fDistY)
//	{
//
//		return true;
//	}
//
//	return false;
//}
