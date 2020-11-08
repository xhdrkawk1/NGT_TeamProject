#include "stdafx.h"
#include "MainGame.h"
#include "Obj.h"
#include "Player.h"
#include "Arrow.h"
#include "Arrow2.h"
#include "Warning.h"
#include "HoldBack.h"
CMainGame::CMainGame()
{
	Game_Stage = 0;
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	MainGame_Time = GetTickCount();
	m_hDC = GetDC(g_hWnd);
	Init_Graphic_Device();
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/UI/BackGround.jpg", L"BackGround");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/UI/HoleBack.png", L"HoleBack"); //구멍배경
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Player/Player.png", L"Player");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Arrow/Arrow1.png", L"Arrow1");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Arrow/Arrow2.png", L"Arrow2");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Lager/WarningX.png", L"WarningX");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Lager/WarningY.png", L"WarningY");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Lager/LagerX.png", L"LagerX");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Lager/LagerY.png", L"LagerY");

	//Time
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/0.png", L"0");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/1.png", L"1");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/2.png", L"2");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/3.png", L"3");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/4.png", L"4");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/5.png", L"5");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/6.png", L"6");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/7.png", L"7");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/8.png", L"8");
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/Time/9.png", L"9");
	//Score
	GET_INSTANCE(CTextureMgr)->InsertTexture(CTextureMgr::TEX_SINGLE, L"../Texture/UI/GameOver.png", L"GameOver");
	CObj *m_pPlayer = CAbstractFactory<CPlayer>::CreateObj();
	CObjMgr::GetInstance()->AddObject(m_pPlayer, CObjMgr::PLAYER);

	CObj *pObj = CAbstractFactory<CHoldBack>::CreateObj();
	CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::HOLEBACK);

	Arrow1_Interval=40.f;
	Arrow1_Speed = 3.f;
	Arrow2_Interval=100.f;
	Arrow2_Speed = 2.f;
	Lager_Interval = 200.f;
	Stage4_Interval = 500.f;
	Game_Stage = 4;
}

void CMainGame::Update()
{
	//if (m_Game_Over == true)
	//{
	//	if (GetAsyncKeyState('R') & 0x8000)
	//	{
	//		m_Game_Over == false;
	//		dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer())->Set_Game_Over_false();
	//		MainGame_Time = GetTickCount();
	//	}
	//}

	if (m_Game_Over == false)
	{
		CObjMgr::GetInstance()->Update();

		if (GetTickCount() - MainGame_Time > 0 && GetTickCount() - MainGame_Time <= 10000)
		{
			Game_Stage = 1;

		}
		else if (GetTickCount() - MainGame_Time > 10000 && GetTickCount() - MainGame_Time <= 20000)
		{
			Game_Stage = 2;
		}
		else if (GetTickCount() - MainGame_Time > 20000 && GetTickCount() - MainGame_Time <= 30000)
		{
			Game_Stage = 3;
		}
		else if (GetTickCount() - MainGame_Time > 30000 && GetTickCount() - MainGame_Time <= 45000)
		{
			Game_Stage = 4;

		}
		else if (GetTickCount() - MainGame_Time > 45000 && GetTickCount() - MainGame_Time <= 60000)
		{
			Game_Stage = 5;
		}
		if (Game_Stage == 1)
		{
			Stage1();
		}
		else if (Game_Stage == 2)
		{
			Stage1();
			Stage2();
		}
		else if (Game_Stage == 3)
		{
			Stage1();
			Stage2();
			Stage3();
		}
		else if (Game_Stage == 4)
		{
			Stage1();
			Stage2();
			Stage3();
			Stage4();
		}
		else if (Game_Stage == 5)
		{
			Lager_Interval = 100;
			Arrow1_Interval = 15.f;
			Arrow1_Speed = 5.5f;
			Stage4_Interval = 350.f;

			Stage1();
			Stage2();
			Stage3();
			Stage4();
		}

		fcount++;
		fcount2++;
		fcount3++;
		fcount4++;
	}
}

void CMainGame::Render()
{

	if (m_Game_Over == false)
	{
		m_pTexInfo = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"BackGround");
		NULL_CHECK(m_pTexInfo);

		GET_INSTANCE(CGraphic_Device)->Render_Begin();

		float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);

		matWorld = matScale * matTrans;

		GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&matWorld);
		GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		CObjMgr::GetInstance()->Render(m_hDC);

		Time_Render(0.f,0.f);

		Game_Over();
		GET_INSTANCE(CGraphic_Device)->Render_End();
	}
}

void CMainGame::Release()
{
	CObjMgr::GetInstance()->DestroyInstance();
	GET_INSTANCE(CTextureMgr)->DestroyInstance();
	GET_INSTANCE(CGraphic_Device)->DestroyInstance();
	
	ReleaseDC(g_hWnd, m_hDC);
}

HRESULT CMainGame::Init_Graphic_Device()
{
	if (FAILED(GET_INSTANCE(CGraphic_Device)->Init_Graphic_Device(g_hWnd, CGraphic_Device::MODE_WIN, WINCX, WINCY)))
	{
		MessageBox(nullptr, L"Init Graphic Device Failed", L"Get Instance Error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CMainGame::Stage1()
{
	if (fcount >= Arrow1_Interval)
	{
			int random = rand() % 1000;
			CObj *pObj=nullptr;
			int random2 = rand() % 4;
			
			switch (random2)
			{
			case 0:	pObj = CAbstractFactory<CArrow2>::CreateObj(random, 0);
				break;
			case 1:	pObj = CAbstractFactory<CArrow2>::CreateObj(random, 1000);
				break;
			case 2:	pObj = CAbstractFactory<CArrow2>::CreateObj(0, random);
				break;
			case 3:	pObj = CAbstractFactory<CArrow2>::CreateObj(1000, random);
				break;
			}
		
			CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::ARROW2);
			dynamic_cast<CArrow2*>(pObj)->LateInit();
			pObj->Set_speed(Arrow1_Speed);

		fcount = 0.f;
	}
}

void CMainGame::Stage2()
{
	if (Game_Stage == 2)
	{
		Arrow1_Interval = 30.f;
		Arrow1_Speed = 3.5f;
	}
	if (fcount2 >= Arrow2_Interval)
	{
		int random = rand() % 1000;
		CObj *pObj = nullptr;
		int random2 = rand() % 4;

		switch (random2)
		{
		case 0:	pObj = CAbstractFactory<CArrow>::CreateObj(random, 0);
			break;
		case 1:	pObj = CAbstractFactory<CArrow>::CreateObj(random, 1000);
			break;
		case 2:	pObj = CAbstractFactory<CArrow>::CreateObj(0, random);
			break;
		case 3:	pObj = CAbstractFactory<CArrow>::CreateObj(1000, random);
			break;
		}
		CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::ARROW);
	
		pObj->Set_speed(Arrow2_Speed);
		fcount2 = 0.f;
	}
}
void CMainGame::Stage3()
{
	if (Game_Stage == 3)
	{
		Arrow1_Interval = 25.f;
		Arrow1_Speed = 4.f;
	}
	float random_cter = (float)(rand() % 314) / 100;
	
	if (fcount3 >= Lager_Interval)
	{
		int random = rand() % 5+1;
		CObj *pObj = nullptr;

		int random2 = rand() % 2;
		int random3;
		switch (random2)
		{

		case 0:
			random3 = rand() % 2;
			pObj = CAbstractFactory<CWarning>::CreateObj(500 + 350*cosf(random_cter)+50, 500 - 350 * sinf(random_cter));
			dynamic_cast<CWarning*>(pObj)->Set_Dir(CWarning::Warn_Dir(random3));
			break;
		case 1:
			random3 = rand() % 2;
			pObj = CAbstractFactory<CWarning>::CreateObj(500 + 350 * cosf(random_cter)+50, 500 + 350 * sinf(random_cter));
			dynamic_cast<CWarning*>(pObj)->Set_Dir(CWarning::Warn_Dir(random3));
			break;


		}
		CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::WARNING);

		fcount3 = 0.f;
	}
}
void CMainGame::Stage4()
{
	if (Game_Stage == 4)
	{
		Arrow1_Interval = 20.f;
		Arrow1_Speed = 4.5f;
	}
	if (fcount4 >= Stage4_Interval)
	{

		for (int i = 0; i < 10; i++)
		{
			CObj *pObj = CAbstractFactory<CArrow2>::CreateObj(200*i,0);
			CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::ARROW2);
			dynamic_cast<CArrow2*>(pObj)->LateInit();
			pObj->Set_speed(Arrow1_Speed);

			pObj = CAbstractFactory<CArrow2>::CreateObj(0, 200 * i);
			CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::ARROW2);
			dynamic_cast<CArrow2*>(pObj)->LateInit();
			pObj->Set_speed(Arrow1_Speed);

			pObj = CAbstractFactory<CArrow2>::CreateObj(1000,200 * i);
			CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::ARROW2);
			dynamic_cast<CArrow2*>(pObj)->LateInit();
			pObj->Set_speed(Arrow1_Speed);

			pObj = CAbstractFactory<CArrow2>::CreateObj(200 * i, 1000);
			CObjMgr::GetInstance()->AddObject(pObj, CObjMgr::ARROW2);
			dynamic_cast<CArrow2*>(pObj)->LateInit();
			pObj->Set_speed(Arrow1_Speed);
		}
		fcount4 = 0.f;
	}
}

void CMainGame::Time_Render(float x,float y)
{
	m_pTimeTexInfo[0] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"0");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[1] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"1");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[2] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"2");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[3] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"3");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[4] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"4");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[5] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"5");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[6] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"6");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[7] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"7");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[8] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"8");
	NULL_CHECK(m_pTexInfo);
	m_pTimeTexInfo[9] = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"9");
	NULL_CHECK(m_pTexInfo);


	int TimerTenSec = (GetTickCount() - MainGame_Time) / 10000;
	int TenSecindex = TimerTenSec;

	if ((TenSecindex / 10)>0)
	{
		TenSecindex -= (TenSecindex / 10) * 10;
	}
	int TimerSec = (GetTickCount() - MainGame_Time) / 1000;
	int Secindex = TimerSec;

	if ((Secindex / 10)>0)
	{
		Secindex -= (Secindex / 10) * 10;
	}

	int TimerMliSec = (GetTickCount() - MainGame_Time) / 100;

	int MilSecindex = TimerMliSec;
	if ((MilSecindex / 10)>0)
	{
		MilSecindex -= (MilSecindex / 10)*10;
	}
	int TimerMicreSec = (GetTickCount() - MainGame_Time) / 10;

	int MicroSecindex = TimerMicreSec;
	if ((MicroSecindex / 10)>0)
	{
		MicroSecindex -= (MicroSecindex / 10) * 10;
	}

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 800, 800, 0.f);

	matWorld = matScale * matTrans;

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&matWorld);  //마이크로초
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTimeTexInfo[MicroSecindex]
		->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - 610-x, fCenterY + 290-y, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&matWorld);  //밀리초
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTimeTexInfo[MilSecindex]
		->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX-580-x, fCenterY+290-y, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&matWorld);  //초
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTimeTexInfo[Secindex]
		->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - 535 - x, fCenterY + 290 - y, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&matWorld);  //10초
	GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pTimeTexInfo[TenSecindex]
		->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - 505 - x, fCenterY + 290 - y, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMainGame::Game_Over()
{
	if (dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer())->Get_Game_Over()==true)
	{
		m_Game_Over = true;
		m_pScoreImage = GET_INSTANCE(CTextureMgr)->GetTexInfo(L"GameOver");
		NULL_CHECK(m_pScoreImage);


		float fCenterX = m_pScoreImage->tImgInfo.Width * 0.5f;
		float fCenterY = m_pScoreImage->tImgInfo.Height * 0.5f;

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);

		matWorld = matScale * matTrans;

		GET_INSTANCE(CGraphic_Device)->GetSprite()->SetTransform(&matWorld);
		GET_INSTANCE(CGraphic_Device)->GetSprite()->Draw(m_pScoreImage->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		Time_Render(-310.f, 515.f);
	}
}



