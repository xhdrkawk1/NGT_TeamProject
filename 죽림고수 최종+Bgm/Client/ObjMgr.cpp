#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CollisionMgr.h"
IMPLEMENT_SINGLETON(CObjMgr)
CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::AddObject(CObj * pObj, OBJ_ID eID)
{
	m_ObjLst[eID].push_back(pObj);
}

void CObjMgr::Update()
{

	for (int i = LOGO; i < END; i++)
	{
		for (OBJITER iter = m_ObjLst[i].begin(); iter != m_ObjLst[i].end();)
		{
			int iEvent = (*iter)->Update();

			if (m_ObjLst[i].empty())
				break;

			if (DEAD_OBJ == iEvent)
			{
				Safe_Delete(*iter);
				iter = m_ObjLst[i].erase(iter);
			}
			else
				++iter;
		}
	}
	//if (m_ObjLst[LAGER].size() >= 1)
	//{
	//	CCollisionMgr::CollisionRect(m_ObjLst[LAGER], m_ObjLst[ARROW]);
	//	CCollisionMgr::CollisionRect(m_ObjLst[LAGER], m_ObjLst[ARROW2]);


	//	//플레이어 사망모드
	//	CCollisionMgr::Collision_TO_player(CObjMgr::GetInstance()->GetPlayer(), m_ObjLst[LAGER]);

	//}

	//////플레이어 무적 모드
	//CCollisionMgr::CollisionRect(m_ObjLst[PLAYER], m_ObjLst[ARROW]);
	//CCollisionMgr::CollisionRect(m_ObjLst[PLAYER], m_ObjLst[ARROW2]);

	//플레이어 사망모드
	//CCollisionMgr::Collision_TO_player(CObjMgr::GetInstance()->GetPlayer(), m_ObjLst[ARROW]);
	//CCollisionMgr::Collision_TO_player(CObjMgr::GetInstance()->GetPlayer(), m_ObjLst[ARROW2]);
}

void CObjMgr::Render(HDC hDC)
{
	for (int i = 0; i < END; ++i)
	{
		for (OBJITER iter = m_ObjLst[i].begin(); iter != m_ObjLst[i].end(); iter++)
		{
			(*iter)->Render(hDC);
		}
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < END; ++i)
	{
		for_each(m_ObjLst[i].begin(), m_ObjLst[i].end(),
			[](auto& ptr)
		{
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
			}
		});
		m_ObjLst[i].clear();
	}
}

void CObjMgr::ReleaseGroup(OBJ_ID eID)
{
}
