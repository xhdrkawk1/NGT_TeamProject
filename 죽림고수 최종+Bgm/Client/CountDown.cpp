#include "stdafx.h"
#include "CountDown.h"
#include <iostream>

CCountDown::CCountDown()
{
}

CCountDown::~CCountDown()
{
	Release();
}

void CCountDown::Initialize()
{
	m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"CountDown",L"UI",2);
	NULL_CHECK(m_pTexInfo);
}

int CCountDown::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;
	return 0;
}

void CCountDown::Render(HDC hDC)
{
	if (m_fAccTime <= 0)
		return;
	int iTemp = m_fAccTime;
	if (iTemp == 3)
		iTemp = 2;


	m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"CountDown", L"UI", (int)iTemp);
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 2.f, 2.f, 0.f);  //x축크기 3배 y축 1배
	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);  //점들을 좌표로



	m_tInfo.mat_World = mat_Scale * mat_Translate; //SRT

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&m_tInfo.mat_World);
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - 250, fCenterY - 250, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCountDown::Release()
{
}

void CCountDown::SetTime(float fTime)
{
	m_fAccTime = 3 - fTime;
}
