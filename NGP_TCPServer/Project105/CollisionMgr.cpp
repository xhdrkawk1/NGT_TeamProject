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
			pdata->Alive = false;//�÷��̾� ���
			Colinumber++;

			if(Colinumber==2)
				CDataMgr::GetInstance()->SaveLoadScore();
			//cout << "�浹" << endl;
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
			pdata->Alive = false;//�÷��̾� ���
			pdata->fLifeTime = GET_INSTANCE(CDataMgr)->m_fServerTime;
			//cout << "�浹" << endl;
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
//			// IntersectRect(1�� ��Ʈ, 2�� ��Ʈ, 3�� ��Ʈ): �� �簢���� �浹�ϸ� TRUE�� ��ȯ.
//			// 1�� ��Ʈ: �� �簢���� �浹�Ͽ� ���ܳ� ��ģ ������ ���.
//			// 2�� ��Ʈ: �浹 ���1
//			// 3�� ��Ʈ: �浹 ���2
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
//	// x�� ������ ��
//	float fSumRadX = (pDst->GetImgInfo().tImgInfo.Width + pSrc->GetImgInfo().tImgInfo.Width) * 0.5f;
//
//	// x�� �Ÿ�
//	// fabs(X): X�� ���� ���� ���
//	float fDistX = fabs(pDst->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);
//
//	// y�� ������ ��
//	float fSumRadY = (pDst->GetImgInfo().tImgInfo.Height + pSrc->GetImgInfo().tImgInfo.Height) * 0.5f;
//
//	// y�� �Ÿ�
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
