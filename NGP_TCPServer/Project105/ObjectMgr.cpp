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
	Arrow1_Interval = 2.f;// 2�ʴ� �ϳ�
	Arrow1_Speed = 150.f; // 1�ʿ� 50�̵�
	Arrow2_Interval = 4.f;//4�ʿ� �ϳ�
	Arrow2_Speed = 50.f; // 1�ʿ� 50�̵�
	Lager_Interval = 4.f; //4�ʿ��ϳ�
	Stage4_Interval = 10.f;

	vStandard= { -1.f,0.f,0.f };

}

void CObjectMgr::Update()
{

	for(int i=0; i<2; i++)
		Player_Pos[i] = { CDataMgr::GetInstance()->m_tPlayerData[i].Pos[0],CDataMgr::GetInstance()->m_tPlayerData[i].Pos[1],0.f };//�������� �÷��̾� 1�� ��ġ ����
	

	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
	D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x��ũ�� 3�� y�� 1��

	Arrow1_Calculate();
	Arrow2_Calculate();
	Warning_Calculate();
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

	//OBJECT_ARROW�� �ʱ⿡ ����, ��ġ, ũ�⸦ ��������

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
		

		Target = Target % 2; // Ÿ���� 0�Ǵ� 1�̰��� �׷��� �÷��̾� �ε��� 0 ���� 1������ ��� �����Ұ��̴�.
		ArrowInfo->Pos = Position;
		ArrowInfo->Dir = Player_Pos[Target] - ArrowInfo->Pos; // �÷��̾�� ��ġ������ �÷��̾ ���ϴ� ����
		D3DXVec3Normalize(&ArrowInfo->Dir, &ArrowInfo->Dir);   //ũ�⸦ ����ȭ
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //������ ��ǥ��

		D3DXVECTOR3 vStandard2 = -ArrowInfo->Dir;
		float cter = D3DXVec3Dot(&vStandard2, &vStandard); //cos cter  ȭ���� ������ ���⺤�Ϳ� -x�� ���⺤�͸� ������ (����)
		float Angle = acosf(cter);
		D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));

		if (ArrowInfo->Pos.y <= Player_Pos[Target].y)
		{
			D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, (Angle));
		}
		else
		{
			D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);  //x��ũ�� 3�� y�� 1��
			D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));
		}

		ArrowInfo->mat_World = ArrowInfo->mat_Scale * ArrowInfo->mat_Rotation * ArrowInfo->mat_Trans; //SRT
		Straight_ArrowInformation_list.push_back(ArrowInfo);
		fcount = 0.f;
		ArrowInfo->Target = Target;

	    Target++; //Ÿ���� 1���ö󰣴� �����Ҷ�����
	}
	

}

void CObjectMgr::Stage2()
{
	Arrow1_Interval = 1.5f;// 2�ʴ� �ϳ�
	Arrow1_Speed = 200.f; // 1�ʿ� 50�̵�

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
		Target2++; //Ÿ���� 1���ö󰣴� �����Ҷ�����
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
		D3DXMatrixScaling(&mat_Scale, 1.f, 1.f, 0.f);  //x��ũ�� 3�� y�� 1��
		D3DXMatrixTranslation(&mat_Translate, Pos.x, Pos.y, 0.f);  //������ ��ǥ��
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
}

void CObjectMgr::Arrow1_Calculate()//������
{
	list<OBJECT_ARROW*>::iterator iterBegin = Straight_ArrowInformation_list.begin();
	list<OBJECT_ARROW*>::iterator iterEnd = Straight_ArrowInformation_list.end();
	for (; iterBegin!=iterEnd; )
	{
		OBJECT_ARROW* ArrowInfo = (*iterBegin);

	
		ArrowInfo->Pos +=(  ArrowInfo->Dir * CTimeMgr::GetInstance()->GetDeltaTime() * Arrow1_Speed);
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //������ ��ǥ��
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

void CObjectMgr::Arrow2_Calculate()//����
{
	list<OBJECT_ARROW*>::iterator iterBegin = Guide_ArrowInformation_list.begin();
	list<OBJECT_ARROW*>::iterator iterEnd = Guide_ArrowInformation_list.end();
	for (; iterBegin != iterEnd; )
	{
		OBJECT_ARROW* ArrowInfo = (*iterBegin);

		ArrowInfo->Dir = Player_Pos[ArrowInfo->Target] - ArrowInfo->Pos; // �÷��̾�� ��ġ������ �÷��̾ ���ϴ� ����
		D3DXVec3Normalize(&ArrowInfo->Dir, &ArrowInfo->Dir);   //ũ�⸦ ����ȭ
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //������ ��ǥ��

		D3DXVECTOR3 vStandard2 = -ArrowInfo->Dir;
		float cter = D3DXVec3Dot(&vStandard2, &vStandard); //cos cter  ȭ���� ������ ���⺤�Ϳ� -x�� ���⺤�͸� ������ (����)
		float Angle = acosf(cter);
		D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));

		if (ArrowInfo->Pos.y <= Player_Pos[ArrowInfo->Target].y)
		{
			D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, (Angle));
		}
		else
		{
			D3DXMatrixScaling(&ArrowInfo->mat_Scale, 1.f, 1.f, 0.f);  //x��ũ�� 3�� y�� 1��
			D3DXMatrixRotationZ(&ArrowInfo->mat_Rotation, -(Angle));
		}

		ArrowInfo->Pos += (ArrowInfo->Dir * CTimeMgr::GetInstance()->GetDeltaTime() * Arrow2_Speed);
		D3DXMatrixTranslation(&ArrowInfo->mat_Trans, ArrowInfo->Pos.x, ArrowInfo->Pos.y, 0.f);  //������ ��ǥ��
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

		cout << "!!!! :"<<WarnInfo->DeltaTime << endl;
		if (WarnInfo->DeltaTime >=3.f)
		{
			Safe_Delete(*iterBegin);
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
			Safe_Delete(*iterBegin);
			iterBegin = WarningY_Information_list.erase(iterBegin);
		}
		else
			iterBegin++;
	}
}

