#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
	m_Bis_Gameover = false;
	m_fspeed = 5.f;
	cter = PI/24.f;
	ZeroMemory(m_player_spot, sizeof(m_player_spot));
}


CPlayer::~CPlayer()
{
	Release();
}


void CPlayer::Initialize()
{
	m_tInfo.vPos = { 400.f, 300.f, 0.f };
	m_tInfo.vDir = { 1,0,0 };

	
	memcpy(m_Standard_spot, m_player_spot, sizeof(m_Standard_spot));
	m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"Player");
	NULL_CHECK(m_pTexInfo);
}

int CPlayer::Update()
{//
	CObj::UpdateRect();

	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	
	m_tInfo.mat_World = mat_Scale*mat_Translate;



	KeyInput();

	if (m_bIsDead)
		return DEAD_OBJ;
	return 0;
}

void CPlayer::Render(HDC hDC)
{
	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 1.f,1.f, 0.f);  //x축크기 3배 y축 1배
	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);  //점들을 좌표로

	
	
	

	m_tInfo.mat_World = mat_Scale  * mat_Translate; //SRT
	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&m_tInfo.mat_World);
	m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"Player");
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	
//	Rectangle(hDC, m_tRect.left - 100, m_tRect.top - 100, m_tRect.right + 100, m_tRect.bottom + 100);
}

void CPlayer::Release()
{

}

void CPlayer::KeyInput()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_tInfo.vPos.x -= 5.f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_tInfo.vPos.x += 5.f;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_tInfo.vPos.y -= 5.f;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_tInfo.vPos.y += 5.f;

}
