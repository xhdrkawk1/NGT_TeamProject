#include "stdafx.h"
#include "Arrow.h"


CArrow::CArrow()
{

	cter = 0.f;
}


CArrow::~CArrow()
{
}

void CArrow::Initialize()
{
	m_tInfo.vStandard = { -1.f,0.f,0.f };
	float size = 10.f;
	
	
	m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"Arrow1");
	NULL_CHECK(m_pTexInfo);

}

int CArrow::Update()
{
	CObj::UpdateArrowRect();
	

	m_tInfo.vPos.x += m_fspeed*m_tInfo.vDir.x;
	m_tInfo.vPos.y += m_fspeed*m_tInfo.vDir.y;
	if (m_bIsDead)
		return DEAD_OBJ;
	return 0;
}

void CArrow::Render(HDC hDC)
{
	m_tInfo.vDir = CObjMgr::GetInstance()->GetPlayer()->Get_Info().vPos - m_tInfo.vPos;  // 플레이어에서 화살좌표를 뺌 
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);   //크기를 1로 조정

	
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배
	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);  //점들을 좌표로


	D3DXVECTOR3 vStandard2 = -m_tInfo.vDir;
	cter = D3DXVec3Dot(&vStandard2, &m_tInfo.vStandard); //cos cter  화살이 가지는 방향벡터와 -x축 방향벡터를 내적함 (라디안)

	if (m_tInfo.vPos.y <= CObjMgr::GetInstance()->GetPlayer()->Get_Info().vPos.y)
	{
		D3DXMatrixScaling(&mat_Scale, -1.f, 1.f, 0.f);
		cter *= -1.f;
	}

	float Angle = acosf(cter);
	D3DXMatrixRotationZ(&mat_Rotation, -(Angle));


	m_tInfo.mat_World = mat_Scale * mat_Rotation * mat_Translate; //SRT

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&m_tInfo.mat_World);
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CArrow::Release()
{
}

