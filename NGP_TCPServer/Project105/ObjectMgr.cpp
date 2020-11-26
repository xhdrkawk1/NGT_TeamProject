#include "ObjectMgr.h"
#include "CollisionMgr.h"
IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{

}

CObjectMgr::~CObjectMgr()
{
}

void CObjectMgr::Initialize()
{
	Arrow1_Interval = 2.f;// 2초당 하나
	Arrow1_Speed = 150.f; // 1초에 50이동
	Arrow2_Interval = 4.f;//4초에 하나
	Arrow2_Speed = 50.f; // 1초에 50이동
	Lager_Interval = 4.f; //4초에하나
	Stage4_Interval = 10.f;

	vStandard= { -1.f,0.f,0.f };

}

void CObjectMgr::Update()
{

	for (int i = 0; i < 2; i++)
	{
		PLAYERDATA pdata = CDataMgr::GetInstance()->m_tPlayerData[i];
		Player_Pos[i] = { pdata.Pos[0],pdata.Pos[1],0.f };//매프레임 플레이어 1의 위치 갱신
		UpdateRect_Player(&pdata.tRect, Player_Pos[i]);
		CCollisionMgr::CollisionRect( &pdata, Straight_ArrowInformation_list);
		CCollisionMgr::CollisionRect(&pdata, Guide_ArrowInformation_list);//
		CCollisionMgr::CollisionRect(&pdata, LagerX_list);
		CCollisionMgr::CollisionRect(&pdata, LagerY_list);//
	}

	

	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배

	Arrow1_Calculate();
	Arrow2_Calculate();
	Warning_Calculate();
	Lager_Calculate();

	CCollisionMgr::CollisionRect(LagerX_list, Straight_ArrowInformation_list);
	CCollisionMgr::CollisionRect(LagerX_list, Guide_ArrowInformation_list);
	CCollisionMgr::CollisionRect(LagerY_list, Straight_ArrowInformation_list);
	CCollisionMgr::CollisionRect(LagerY_list, Guide_ArrowInformation_list);

	float Time = CDataMgr::GetInstance()->m_fServerTime;
	if (Time > 0 && Time <= 10)
		Game_Stage = 1;
	else if (Time > 10 && Time <= 20)
		Game_Stage = 2;
	else if (Time > 20 && Time <= 30)
		Game_Stage = 3;
	else if (Time > 30 && Time <= 45)
		Game_Stage = 4;
	else if (Time > 45 && Time <= 60)
		Game_Stage = 5;


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
	/*	Lager_Interval = 2;
		Arrow1_Interval = 1.f;
		Arrow1_Speed = 5.5f;
		Stage4_Interval = 5.f;*/

		Stage1();
		Stage2();
		Stage3();
		Stage4();

	}

	fcount  += CTimeMgr::GetInstance()->GetDeltaTime();
	fcount2 += CTimeMgr::GetInstance()->GetDeltaTime();
	fcount3 += CTimeMgr::GetInstance()->GetDeltaTime();
	fcount4 += CTimeMgr::GetInstance()->GetDeltaTime();
}

void CObjectMgr::Stage1()
{
	int random = rand() % 1000;

	int random2 = rand() % 4;

	//OBJECT_ARROW는 초기에 방향, 위치, 크기를 잡아줘야함

	if (fcount >= Arrow1_Interval)
	{
		OBJECT_ARROW* ArrowInfo = new OBJECT_ARROW;
		int random = rand() % 1000;
		int random2 = rand() % 4;

	
		D3DXVECTOR3 Position;
		switch (random2)
		{
		case 0:	
			Position = { (float)random,0,0 };
			break;
		case 1:
			Position = { (float)random, (float)1000,0 };
			break;
		case 2:		
			Position = { 0, (float)random,0 };
			break;
		case 3:
			Position = { (float)1000, (float)random,0 };
			break;
		}
		

		Target = Target % 2; // 타겟은 0또는 1이겠지 그래서 플레이어 인덱스 0 기준 1기준을 계속 연산할것이다.
		ArrowInfo->Pos = Position;
		ArrowInfo->Dir = Player_Pos[Target] - ArrowInfo->Pos; // 플레이어에서 위치를빼서 플레이어를 향하는 방향
		D3DXVec3Normalize(&ArrowInfo->Dir, &ArrowInfo->Dir);   //크기를 정규화
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //점들을 좌표로

		D3DXVECTOR3 vStandard2 = -ArrowInfo->Dir;
		float cter = D3DXVec3Dot(&vStandard2, &vStandard); //cos cter  화살이 가지는 방향벡터와 -x축 방향벡터를 내적함 (라디안)
		float Angle = acosf(cter);
		D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));

		if (ArrowInfo->Pos.y <= Player_Pos[Target].y)
		{
			D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, (Angle));
		}
		else
		{
			D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배
			D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));
		}

		ArrowInfo->mat_World = ArrowInfo->mat_Scale * ArrowInfo->mat_Rotation * ArrowInfo->mat_Trans; //SRT
		Straight_ArrowInformation_list.push_back(ArrowInfo);
		fcount = 0.f;
		ArrowInfo->Target = Target;

	    Target++; //타겟이 1씩올라간다 생성할때마다
	}
	

}

void CObjectMgr::Stage2()
{
	Arrow1_Interval = 1.5f;// 2초당 하나
	Arrow1_Speed = 200.f; // 1초에 50이동

	if (fcount2 >= Arrow2_Interval)
	{
		OBJECT_ARROW* ArrowInfo = new OBJECT_ARROW;
		int random = rand() % 1000;
		int random2 = rand() % 4;

		Target2 = Target2 % 2;
		D3DXVECTOR3 Position;
		switch (random2)
		{
		case 0:
			Position = { (float)random,0,0 };
			break;
		case 1:
			Position = { (float)random, (float)1000,0 };
			break;
		case 2:
			Position = { 0, (float)random,0 };
			break;
		case 3:
			Position = { (float)1000, (float)random,0 };
			break;
		}
		ArrowInfo->Pos = Position;

		fcount2 = 0.f;
		Guide_ArrowInformation_list.push_back(ArrowInfo);
		ArrowInfo->Target = Target2;
		Target2++; //타겟이 1씩올라간다 생성할때마다
	}
	
}

void CObjectMgr::Stage3()
{
	Arrow1_Interval = 1.f;
	Arrow1_Speed = 250.f;

	float random_cter = (float)(rand() % 314) / 100;
	if (fcount3 >= Lager_Interval)
	{
		int random = rand() % 5 + 1;
		int random2 = rand() % 2;
		int random3;
		OBJECT_WARNING* warning = new OBJECT_WARNING;
		D3DXVECTOR3 Pos = { 0,0,0 };
		random3 = rand() % 2;
		switch (random2)
		{
		case 0:
		{
			Pos = { 500 + 350 * cosf(random_cter) + 50 ,500 - 350 * sinf(random_cter) ,0.f };
		}
		break;
		case 1:
		{
			Pos = { 500 + 350 * cosf(random_cter) + 50 ,500 + 350 * sinf(random_cter) ,0.f };
		
		}
		break;
		}
		D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
		D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배
		D3DXMatrixTranslation(&mat_Translate, Pos.x, Pos.y, 0.f);  //점들을 좌표로
		warning->mat_World = mat_Scale * mat_Translate;
		if (random3 == 0)
			WarningX_Information_list.push_back(warning);
		else if (random3 == 1)
			WarningY_Information_list.push_back(warning);
		fcount3 = 0.f;
	}
}

void CObjectMgr::Stage4()
{
	Arrow1_Interval = 0.5f;
	Arrow1_Speed = 300.f;
	if (fcount4 >= Stage4_Interval)
	{
		fcount4 = 0.f;
		for (int i = 0; i < 5; i++)
		{
			D3DXVECTOR3 Pos[8];
			Pos[0] = { 400.f * i,0 ,0 };
			Pos[1] = { 400.f * i,1000.f ,0 };
			Pos[2] = { 0,400.f * i ,0 };
			Pos[3] = { 1000.f,400.f * i,0 };
			Pos[4] = { 400.f * i + 200.f ,0 ,0 };
			Pos[5] = { 400.f * i + 200.f,1000.f ,0 };
			Pos[6] = { 0,400.f * i + 200.f ,0 };
			Pos[7] = { 1000.f,400.f * i + 200.f ,0 };
		
			for (int j = 0; j < 8; j++)
			{
				OBJECT_ARROW *ArrowInfo = new OBJECT_ARROW;
				ArrowInfo->Pos = Pos[j];
				ArrowInfo->Dir = Player_Pos[j / 4] - ArrowInfo->Pos;
				D3DXVec3Normalize(&ArrowInfo->Dir, &ArrowInfo->Dir);   //크기를 정규화
				D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //점들을 좌표로
				D3DXVECTOR3 vStandard2 = -ArrowInfo->Dir;
				float cter = D3DXVec3Dot(&vStandard2, &vStandard); //cos cter  화살이 가지는 방향벡터와 -x축 방향벡터를 내적함 (라디안)
				float Angle = acosf(cter);
				D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));

				if (ArrowInfo->Pos.y <= Player_Pos[j / 4].y)
				{
					D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);
					D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, (Angle));
				}
				else
				{
					D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배
					D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));
				}

				ArrowInfo->mat_World = ArrowInfo->mat_Scale * ArrowInfo->mat_Rotation * ArrowInfo->mat_Trans; //SRT
				ArrowInfo->Target = int(j / 4);
				Straight_ArrowInformation_list.push_back(ArrowInfo);



			}
		}

	}

}

void CObjectMgr::Arrow1_Calculate()//일직선
{
	list<OBJECT_ARROW*>::iterator iterBegin = Straight_ArrowInformation_list.begin();
	list<OBJECT_ARROW*>::iterator iterEnd = Straight_ArrowInformation_list.end();
	for (; iterBegin!=iterEnd; )
	{
		OBJECT_ARROW* ArrowInfo = (*iterBegin);

	
		ArrowInfo->Pos +=(  ArrowInfo->Dir * CTimeMgr::GetInstance()->GetDeltaTime() * Arrow1_Speed);
		UpdateRect_Arrow(&ArrowInfo->tRect, ArrowInfo->Pos);
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //점들을 좌표로
		ArrowInfo->mat_World = ArrowInfo->mat_Scale * ArrowInfo->mat_Rotation * ArrowInfo->mat_Trans; //SRT

		if ((ArrowInfo->Pos.x<-10 || ArrowInfo->Pos.x>WINCX + 10) || (ArrowInfo->Pos.y<-10 || ArrowInfo->Pos.y>WINCY + 10))
			ArrowInfo->isDead = true;

		if (ArrowInfo->isDead == true)
		{
			Safe_Delete(*iterBegin);
			iterBegin = Straight_ArrowInformation_list.erase(iterBegin);
		}
		else
			iterBegin++;
		//OBJECT_PACKET* packet = new OBJECT_PACKET();
		//packet->mat_World = ArrowInfo->mat_World;
		//Arrow_vector.push_back(packet);
	}
}

void CObjectMgr::Arrow2_Calculate()//유도
{
	list<OBJECT_ARROW*>::iterator iterBegin = Guide_ArrowInformation_list.begin();
	list<OBJECT_ARROW*>::iterator iterEnd = Guide_ArrowInformation_list.end();
	for (; iterBegin != iterEnd; )
	{
		OBJECT_ARROW* ArrowInfo = (*iterBegin);

		ArrowInfo->Dir = Player_Pos[ArrowInfo->Target] - ArrowInfo->Pos; // 플레이어에서 위치를빼서 플레이어를 향하는 방향
		D3DXVec3Normalize(&ArrowInfo->Dir, &ArrowInfo->Dir);   //크기를 정규화
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //점들을 좌표로

		D3DXVECTOR3 vStandard2 = -ArrowInfo->Dir;
		float cter = D3DXVec3Dot(&vStandard2, &vStandard); //cos cter  화살이 가지는 방향벡터와 -x축 방향벡터를 내적함 (라디안)
		float Angle = acosf(cter);
		D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));

		if (ArrowInfo->Pos.y <= Player_Pos[ArrowInfo->Target].y)
		{
			D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, (Angle));
		}
		else
		{
			D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배
			D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));
		}

		ArrowInfo->Pos += (ArrowInfo->Dir * CTimeMgr::GetInstance()->GetDeltaTime() * Arrow2_Speed);
		UpdateRect_Arrow(&ArrowInfo->tRect, ArrowInfo->Pos);
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //점들을 좌표로
		ArrowInfo->mat_World = ArrowInfo->mat_Scale * ArrowInfo->mat_Rotation * ArrowInfo->mat_Trans; //SRT
		

		if ((ArrowInfo->Pos.x<-10 || ArrowInfo->Pos.x>WINCX + 10) || (ArrowInfo->Pos.y<-10 || ArrowInfo->Pos.y>WINCY + 10))
			ArrowInfo->isDead = true;

		if (ArrowInfo->isDead == true)
		{
			Safe_Delete(*iterBegin);
			iterBegin = Guide_ArrowInformation_list.erase(iterBegin);
		}
		else
			iterBegin++;
	}
}

void CObjectMgr::Warning_Calculate()
{
	list<OBJECT_WARNING*>::iterator iterBegin = WarningX_Information_list.begin();
	list<OBJECT_WARNING*>::iterator iterEnd = WarningX_Information_list.end();

	for (; iterBegin != iterEnd; )
	{
		OBJECT_WARNING* WarnInfo = (*iterBegin);
		WarnInfo->DeltaTime += CTimeMgr::GetInstance()->GetDeltaTime();

		if (WarnInfo->DeltaTime >=3.f)
		{
			D3DXVECTOR3 pos = { WarnInfo->mat_World._41, WarnInfo->mat_World._42, WarnInfo->mat_World._43 };
			MakeLager(pos, 0);
			Safe_Delete(*iterBegin);//레이저 X 생성
			iterBegin = WarningX_Information_list.erase(iterBegin);

		}
		else
			iterBegin++;
	}
	iterBegin = WarningY_Information_list.begin();
	iterEnd = WarningY_Information_list.end();

	for (; iterBegin != iterEnd; )
	{
		OBJECT_WARNING* WarnInfo = (*iterBegin);
		WarnInfo->DeltaTime += CTimeMgr::GetInstance()->GetDeltaTime();
		if (WarnInfo->DeltaTime >= 3.f)
		{
			D3DXVECTOR3 pos = { WarnInfo->mat_World._41, WarnInfo->mat_World._42, WarnInfo->mat_World._43 };
			MakeLager(pos, 1);
			Safe_Delete(*iterBegin);//레이저 Y 생성
			iterBegin = WarningY_Information_list.erase(iterBegin);
		}
		else
			iterBegin++;
	}
}

void CObjectMgr::Lager_Calculate()
{
	list<OBJECT_WARNING*>::iterator iterBegin = LagerX_list.begin();
	list<OBJECT_WARNING*>::iterator iterEnd = LagerX_list.end();

	for (; iterBegin != iterEnd; )
	{
		OBJECT_WARNING* WarnInfo = (*iterBegin);
		WarnInfo->DeltaTime += CTimeMgr::GetInstance()->GetDeltaTime();

		if (WarnInfo->DeltaTime >= 1.f)
		{
			Safe_Delete(*iterBegin);
			iterBegin = LagerX_list.erase(iterBegin);
		}
		else
			iterBegin++;
	}
	iterBegin = LagerY_list.begin();
	iterEnd = LagerY_list.end();
	for (; iterBegin != iterEnd; )
	{
		OBJECT_WARNING* WarnInfo = (*iterBegin);
		WarnInfo->DeltaTime += CTimeMgr::GetInstance()->GetDeltaTime();
		if (WarnInfo->DeltaTime >= 1.f)
		{
			Safe_Delete(*iterBegin);//레이저 Y 생성
			iterBegin = LagerY_list.erase(iterBegin);
		}
		else
			iterBegin++;
	}
}

void CObjectMgr::MakeLager(D3DXVECTOR3 POS, int Type)
{
	OBJECT_WARNING * Lager = new OBJECT_WARNING;

	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배
	D3DXMatrixTranslation(&mat_Translate, POS.x, POS.y, 0.f);  //점들을 좌표로
	Lager->mat_World = mat_Scale * mat_Translate;
	
	if (Type == 0)
	{
		LagerX_list.push_back(Lager);
		UpdateRect_RagerX(&Lager->tRect, POS);
	}
	else
	{
		LagerY_list.push_back(Lager);
		UpdateRect_RagerY(&Lager->tRect, POS);
	}

}

void CObjectMgr::UpdateRect_Player(RECT* Rect, D3DXVECTOR3 POS)
{
	Rect->left = LONG(POS.x - (35 * 0.5));
	Rect->top = LONG(POS.y - (35 * 0.5));
	Rect->right = LONG(POS.x + (35 * 0.5));
	Rect->bottom = LONG(POS.y + (35 * 0.5));
}

void CObjectMgr::UpdateRect_Arrow(RECT* Rect,D3DXVECTOR3 POS)
{
	Rect->left = LONG(POS.x - (35* 0.5));
	Rect->top =  LONG(POS.y - (35* 0.5));
	Rect->right =LONG(POS.x + (35* 0.5));
	Rect->bottom=LONG(POS.y + (35 *0.5));
}

void CObjectMgr::UpdateRect_RagerX(RECT* Rect, D3DXVECTOR3 POS)
{
	Rect->left = LONG(POS.x - (2000 * 0.5));
	Rect->top = LONG(POS.y - (20 * 0.5));
	Rect->right = LONG(POS.x + (2000 * 0.5));
	Rect->bottom = LONG(POS.y + (20 * 0.5));
}

void CObjectMgr::UpdateRect_RagerY(RECT* Rect, D3DXVECTOR3 POS)
{
	Rect->left = LONG(POS.x - (20 * 0.5));
	Rect->top = LONG(POS.y - (2000 * 0.5));
	Rect->right = LONG(POS.x + (20 * 0.5));
	Rect->bottom = LONG(POS.y + (2000 * 0.5));
}

