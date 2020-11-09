#include "stdafx.h"
#include "Lager.h"


CLager::CLager()
{
}


CLager::~CLager()
{
}
void CLager::Initialize()
{
	LagerTime = GetTickCount();

}



int CLager::Update()
{
	if (m_LagerType == LagerX)
	{
		m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"LagerX");
	}
	else if (m_LagerType == LagerY)
	{
		m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"LagerY");
	}
	CObj::UpdateRect();


	if (GetTickCount() - LagerTime>500)
	{
		return DEAD_OBJ;
	}
	return 0;
}

void CLager::Render(HDC hDC)
{


	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배
	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);  //점들을 좌표로



	m_tInfo.mat_World = mat_Scale* mat_Translate; //SRT

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&m_tInfo.mat_World);
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CLager::Release()
{
}
