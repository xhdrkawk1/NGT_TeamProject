#include "stdafx.h"
#include "Obj.h"


CObj::CObj():m_bIsDead(false)
{
}


CObj::~CObj()
{
}

void CObj::UpdateRect()
{
	m_tRect.left = LONG(m_tInfo.vPos.x - m_pTexInfo->tImgInfo.Width*0.5);
	m_tRect.top = LONG(m_tInfo.vPos.y - m_pTexInfo->tImgInfo.Height*0.5);
	m_tRect.right = LONG(m_tInfo.vPos.x + m_pTexInfo->tImgInfo.Width*0.5);
	m_tRect.bottom = LONG(m_tInfo.vPos.y + m_pTexInfo->tImgInfo.Height*0.5);
}

void CObj::UpdateArrowRect()
{
	m_tRect.left = LONG(m_tInfo.vPos.x - m_pTexInfo->tImgInfo.Width*0.5);
	m_tRect.top = LONG(m_tInfo.vPos.y - m_pTexInfo->tImgInfo.Height*0.2);
	m_tRect.right = LONG(m_tInfo.vPos.x - m_pTexInfo->tImgInfo.Width*0.4);
	m_tRect.bottom = LONG(m_tInfo.vPos.y + m_pTexInfo->tImgInfo.Height*0.2);
}

void CObj::Set_Pos(float x,float y)
{
	m_tInfo.vPos.x = x;
	m_tInfo.vPos.y = y;
	m_tInfo.vPos.z = 0.f;
}
void CObj::Set_WorldMatrix(D3DXMATRIX matWorld)
{
	m_tInfo.mat_World = matWorld;

}