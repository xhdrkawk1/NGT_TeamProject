#include "ObjectMgr.h"

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

	for(int i=0; i<2; i++)
		Player_Pos[i] = { CDataMgr::GetInstance()->m_tPlayerData[i].Pos[0],CDataMgr::GetInstance()->m_tPlayerData[i].Pos[1],0.f };//매프레임 플레이어 1의 위치 갱신
	

	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x축크기 3배 y축 1배

	Arrow1_Calculate();

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
		Lager_Interval = 2;
		Arrow1_Interval = 1.f;
		Arrow1_Speed = 5.5f;
		Stage4_Interval = 5.f;

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
	OBJECT_ARROW* ArrowInfo = new OBJECT_ARROW;
	int random2 = rand() % 4;

	//OBJECT_ARROW는 초기에 방향, 위치, 크기를 잡아줘야함

	cout << "fcount ::"<<fcount << endl;
	if (fcount >= Arrow1_Interval)
	{
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
		Straight_ArrowInformation_vector.push_back(ArrowInfo);
		fcount = 0.f;
	}
	ArrowInfo->Target = Target;
	Target++; //타겟이 1씩올라간다 생성할때마다
}

void CObjectMgr::Stage2()
{
}

void CObjectMgr::Stage3()
{
}

void CObjectMgr::Stage4()
{
}

void CObjectMgr::Arrow1_Calculate()
{
	for (auto& ArrowInfo : Straight_ArrowInformation_vector)
	{

		ArrowInfo->Pos +=(  ArrowInfo->Dir * CTimeMgr::GetInstance()->GetDeltaTime() * Arrow1_Speed);
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //점들을 좌표로
		ArrowInfo->mat_World = ArrowInfo->mat_Scale * ArrowInfo->mat_Rotation * ArrowInfo->mat_Trans; //SRT

		//OBJECT_PACKET* packet = new OBJECT_PACKET();
		//packet->mat_World = ArrowInfo->mat_World;
		//Arrow_vector.push_back(packet);
	}
}

