#include "stdafx.h"
#include "HoldBack.h"


CHoldBack::CHoldBack()
{
}


CHoldBack::~CHoldBack()
{
}
void CHoldBack::Initialize()
{

	m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"HoleBack");
	NULL_CHECK(m_pTexInfo);
}



int CHoldBack::Update()
{


	return 0;
}

void CHoldBack::Render(HDC hDC)
{


	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배
	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);  //점들을 좌표로



	m_tInfo.mat_World = mat_Scale* mat_Translate; //SRT

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&m_tInfo.mat_World);
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX-500, fCenterY-500, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CHoldBack::Release()
{
}