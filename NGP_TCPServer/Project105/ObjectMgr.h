#pragma once
#include "CommonHeader.h"
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)
	enum OBJ_ID {ARROW, ARROW2, WARNING, END };

public:
	CObjectMgr();
	~CObjectMgr();


	void Initialize();
	void Update();
	void Stage1();
	void Stage2();
	void Stage3();
	void Stage4();


	void Arrow1_Calculate();
	void Arrow2_Calculate();
	void Warning_Calculate();
	void Lager_Calculate();
	void MakeLager(D3DXVECTOR3 POS,int Type);

	void UpdateRect_Player(RECT* Rect, D3DXVECTOR3 POS);
	void UpdateRect_Arrow(RECT* Rect, D3DXVECTOR3 POS);

	void UpdateRect_RagerX(RECT* Rect, D3DXVECTOR3 POS);
	void UpdateRect_RagerY(RECT* Rect, D3DXVECTOR3 POS);

	void Player_Collsion(int iPlayerNum);
public:
	float Arrow1_Interval;
	float Arrow1_Speed;
	float Arrow2_Interval;
	float Arrow2_Speed;
	float Lager_Interval;
	float Stage4_Interval;

	int Game_Stage = 0;

	list<OBJECT_ARROW*> Straight_ArrowInformation_list;
	list<OBJECT_ARROW*> Guide_ArrowInformation_list;
	list<OBJECT_WARNING*> WarningX_Information_list;
	list<OBJECT_WARNING*> WarningY_Information_list;

	list<OBJECT_WARNING*>LagerX_list;
	list<OBJECT_WARNING*>LagerY_list;
	//vector<OBJECT_PACKET*> Arrow_vector; // vector  // size
	//vector<OBJECT_PACKET*> Arrow2_vector;// vector //size
	//vector<OBJECT_PACKET*> Warning_vector;

	float fcount = 0.f;
	float fcount2 = 0.f;
	float fcount3 = 0.f;
	float fcount4 = 0.f;


	int Target = 0;
	int Target2 = 0;
	D3DXVECTOR3 vStandard;
	D3DXVECTOR3 Player_Pos[2];

};

