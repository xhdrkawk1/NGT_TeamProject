#include "stdafx.h"
#include "Arrow2.h"


CArrow2::CArrow2()
{
	
	cter = 0.f;
}


CArrow2::~CArrow2()
{
}

void CArrow2::Initialize()
{
	m_tInfo.vStandard = { -1.f,0.f,0.f };
	m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"Arrow2");
	NULL_CHECK(m_pTexInfo);
}

void CArrow2::LateInit()
{
	m_tInfo.vDir = CObjMgr::GetInstance()->GetPlayer()->Get_Info().vPos - m_tInfo.vPos;  // �÷��̾�� ȭ����ǥ�� �� 
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);   //ũ�⸦ 1�� ����


	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);  //������ ��ǥ��

	D3DXVECTOR3 vStandard2 = -m_tInfo.vDir;
	cter = D3DXVec3Dot(&vStandard2, &m_tInfo.vStandard); //cos cter  ȭ���� ������ ���⺤�Ϳ� -x�� ���⺤�͸� ������ (����)
	Angle = acosf(cter);

	if (m_tInfo.vPos.y <= CObjMgr::GetInstance()->GetPlayer()->Get_Info().vPos.y)
	{
		D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);
		D3DXMatrixRotationZ(&mat_Rotation, (Angle));
	}
	else
	{
		D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x��ũ�� 3�� y�� 1��
		D3DXMatrixRotationZ(&mat_Rotation, -(Angle));
	}
	


	m_tInfo.mat_World = mat_Scale * mat_Rotation * mat_Translate; //SRT



}

int CArrow2::Update()
{
	CObj::UpdateArrowRect();
	m_tInfo.vPos.x += m_fspeed*m_tInfo.vDir.x;
	m_tInfo.vPos.y += m_fspeed*m_tInfo.vDir.y;


	if ((m_tInfo.vPos.x<-10 || m_tInfo.vPos.x>WINCX + 10) || (m_tInfo.vPos.y<-10 || m_tInfo.vPos.y>WINCY + 10))
	{
		return DEAD_OBJ;
	}
	if (m_bIsDead)
		return DEAD_OBJ;
	return 0;
}

void CArrow2::Render(HDC hDC)
{


	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);  //������ ��ǥ��

	m_tInfo.mat_World = mat_Scale * mat_Rotation * mat_Translate; //SRT

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&m_tInfo.mat_World);
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CArrow2::Release()
{
}

