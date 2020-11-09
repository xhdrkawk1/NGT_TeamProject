#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Player.h"
CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionRect(OBJLIST & dstLst, OBJLIST & srcLst)
{
	RECT rc = {};

	// auto == CObj*
	for (auto& pDst : dstLst)
	{
		for (auto& pSrc : srcLst)
		{
			// IntersectRect(1번 렉트, 2번 렉트, 3번 렉트): 두 사각형이 충돌하면 TRUE를 반환.
			// 1번 렉트: 두 사각형이 충돌하여 생겨난 겹친 영역을 기록.
			// 2번 렉트: 충돌 대상1
			// 3번 렉트: 충돌 대상2
			if (IntersectRect(&rc, &(pDst->GetRect()), &(pSrc->GetRect())))
			//if(CheckRect(pDst, pSrc))
			{
				//pDst->IsDead();
				pSrc->IsDead();
			}
		}
	}
}

void CCollisionMgr::Collision_TO_player(CObj* dst, OBJLIST & srcLst) //dst //플레이어  소스가 오브젝트
{
	RECT rc = {};

	// auto == CObj*
		for (auto& pSrc : srcLst)
		{
			// IntersectRect(1번 렉트, 2번 렉트, 3번 렉트): 두 사각형이 충돌하면 TRUE를 반환.
			// 1번 렉트: 두 사각형이 충돌하여 생겨난 겹친 영역을 기록.
			// 2번 렉트: 충돌 대상1
			// 3번 렉트: 충돌 대상2
			if (IntersectRect(&rc, &(dst->GetRect()), &(pSrc->GetRect())))
				//if(CheckRect(pDst, pSrc))
			{
				//pDst->IsDead();
				pSrc->IsDead();
				dynamic_cast<CPlayer*>(dst)->Set_Game_Over();
			}
		}
	}
//void CCollisionMgr::CollisionSphere(OBJLIST & dstLst, OBJLIST & srcLst)
//{
//	// auto == CObj*
//	for (auto& pDst : dstLst)
//	{
//		for (auto& pSrc : srcLst)
//		{
//			if (CheckSphere(pDst, pSrc))
//			{
//				pDst->IsDead();
//				pSrc->IsDead();
//			}
//		}
//	}
//}

//void CCollisionMgr::CollisionItem(OBJLIST & dstLst, OBJLIST & srcLst)
//{
//
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
//			{
//				pDst->PlusSpeed(pSrc->GetSpeed());
//				pSrc->IsDead();
//
//			}
//		}
//	}
//}

//bool CCollisionMgr::CheckSphere(CObj* pDst, CObj* pSrc)
//{
//	// 두 원의 반지름 합을 구한다.
//	float fSumRad = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
//
//	// 두 원의 거리를 구한다.
//	float a = pDst->Get_Info().fX - pSrc->Get_Info().fX;
//	float b = pDst->Get_Info().fY - pSrc->Get_Info().fY;
//
//	// sqrtf(x): x에 대한 제곱근을 구하는 함수. <cmath>에서 제공한다.
//	float c = sqrtf(a * a + b * b);	// 피타고라스
//
//									// 반지름의 합이 거리보다 커진다면 true 반환.
//	return fSumRad >= c;
//}

bool CCollisionMgr::CheckRect(CObj* pDst, CObj* pSrc)
{
	// x축 반지름 합
	float fSumRadX = (pDst->GetImgInfo().tImgInfo.Width + pSrc->GetImgInfo().tImgInfo.Width) * 0.5f;

	// x축 거리
	// fabs(X): X의 절대 값을 계산
	float fDistX = fabs(pDst->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);

	// y축 반지름 합
	float fSumRadY = (pDst->GetImgInfo().tImgInfo.Height + pSrc->GetImgInfo().tImgInfo.Height) * 0.5f;

	// y축 거리
	float fDistY = fabs(pDst->Get_Info().vPos.y - pSrc->Get_Info().vPos.y);

	if (fSumRadX > fDistX || fSumRadY > fDistY)
	{
	
		return true;
	}

	return false;
}
