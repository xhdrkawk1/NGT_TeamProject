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
public:
	float Arrow1_Interval;
	float Arrow1_Speed;
	float Arrow2_Interval;
	float Arrow2_Speed;
	float Lager_Interval;
	float Stage4_Interval;

	int Game_Stage = 0;

	vector<OBJECT_ARROW*> Straight_ArrowInformation_vector;

	//vector<OBJECT_PACKET*> Arrow_vector; // vector  // size
	//vector<OBJECT_PACKET*> Arrow2_vector;// vector //size
	//vector<OBJECT_PACKET*> Warning_vector;

	float fcount = 0.f;
	float fcount2 = 0.f;
	float fcount3 = 0.f;
	float fcount4 = 0.f;


	int Target = 0;
	D3DXVECTOR3 vStandard;
	D3DXVECTOR3 Player_Pos[2];

};

