#include "stdafx.h"
#include "Warning.h"
#include "Lager.h"

CWarning::CWarning()
{
}


CWarning::~CWarning()
{
}
void CWarning::Initialize()
{
	WarningTime = GetTickCount();
	
	m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"WarningY");
}



int CWarning::Update()
{

	if (m_Warn_Dir == WARNY)
	{
		m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"WarningY");	
	}
	else if (m_Warn_Dir == WARNX)
	{
		m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"WarningX");
	}
	if (GetTickCount()-WarningTime>3000)
	{
		
		CObj *pObj = CAbstractFactory<CLager>::CreateObj(m_tInfo.vPos.x, m_tInfo.vPos.y);
		if (m_Warn_Dir==WARNY)
			dynamic_cast<CLager*>(pObj)->Set_LagerType(CLager::LagerY);
		else if(m_Warn_Dir == WARNX)
			dynamic_cast<CLager*>(pObj)->Set_LagerType(CLager::LagerX);
		
		CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::LAGER);

		return DEAD_OBJ;
	}
	return 0;
}

void CWarning::Render(HDC hDC)
{


	
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x��ũ�� 3�� y�� 1��
	D3DXMatrixTranslation(&mat_Translate, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);  //������ ��ǥ��



	m_tInfo.mat_World = mat_Scale* mat_Translate; //SRT

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&m_tInfo.mat_World);
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CWarning::Release()
{
}