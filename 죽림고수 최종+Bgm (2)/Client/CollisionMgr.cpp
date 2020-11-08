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
			// IntersectRect(1�� ��Ʈ, 2�� ��Ʈ, 3�� ��Ʈ): �� �簢���� �浹�ϸ� TRUE�� ��ȯ.
			// 1�� ��Ʈ: �� �簢���� �浹�Ͽ� ���ܳ� ��ģ ������ ���.
			// 2�� ��Ʈ: �浹 ���1
			// 3�� ��Ʈ: �浹 ���2
			if (IntersectRect(&rc, &(pDst->GetRect()), &(pSrc->GetRect())))
			//if(CheckRect(pDst, pSrc))
			{
				//pDst->IsDead();
				pSrc->IsDead();
			}
		}
	}
}

void CCollisionMgr::Collision_TO_player(CObj* dst, OBJLIST & srcLst) //dst //�÷��̾�  �ҽ��� ������Ʈ
{
	RECT rc = {};

	// auto == CObj*
		for (auto& pSrc : srcLst)
		{
			// IntersectRect(1�� ��Ʈ, 2�� ��Ʈ, 3�� ��Ʈ): �� �簢���� �浹�ϸ� TRUE�� ��ȯ.
			// 1�� ��Ʈ: �� �簢���� �浹�Ͽ� ���ܳ� ��ģ ������ ���.
			// 2�� ��Ʈ: �浹 ���1
			// 3�� ��Ʈ: �浹 ���2
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
//			// IntersectRect(1�� ��Ʈ, 2�� ��Ʈ, 3�� ��Ʈ): �� �簢���� �浹�ϸ� TRUE�� ��ȯ.
//			// 1�� ��Ʈ: �� �簢���� �浹�Ͽ� ���ܳ� ��ģ ������ ���.
//			// 2�� ��Ʈ: �浹 ���1
//			// 3�� ��Ʈ: �浹 ���2
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
//	// �� ���� ������ ���� ���Ѵ�.
//	float fSumRad = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
//
//	// �� ���� �Ÿ��� ���Ѵ�.
//	float a = pDst->Get_Info().fX - pSrc->Get_Info().fX;
//	float b = pDst->Get_Info().fY - pSrc->Get_Info().fY;
//
//	// sqrtf(x): x�� ���� �������� ���ϴ� �Լ�. <cmath>���� �����Ѵ�.
//	float c = sqrtf(a * a + b * b);	// ��Ÿ���
//
//									// �������� ���� �Ÿ����� Ŀ���ٸ� true ��ȯ.
//	return fSumRad >= c;
//}

bool CCollisionMgr::CheckRect(CObj* pDst, CObj* pSrc)
{
	// x�� ������ ��
	float fSumRadX = (pDst->GetImgInfo().tImgInfo.Width + pSrc->GetImgInfo().tImgInfo.Width) * 0.5f;

	// x�� �Ÿ�
	// fabs(X): X�� ���� ���� ���
	float fDistX = fabs(pDst->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);

	// y�� ������ ��
	float fSumRadY = (pDst->GetImgInfo().tImgInfo.Height + pSrc->GetImgInfo().tImgInfo.Height) * 0.5f;

	// y�� �Ÿ�
	float fDistY = fabs(pDst->Get_Info().vPos.y - pSrc->Get_Info().vPos.y);

	if (fSumRadX > fDistX || fSumRadY > fDistY)
	{
	
		return true;
	}

	return false;
}
