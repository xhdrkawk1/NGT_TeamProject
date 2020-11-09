#pragma once

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
	HRESULT Init_Graphic_Device();

	void Stage1();
	void Stage2();
	void Stage3();
	void Stage4();
	void Time_Render(float x, float y);

	void Game_Over();
 private:
	HDC m_hDC;

	float fcount = 0.f;
	float fcount2 = 0.f;
	float fcount3 = 0.f;
	float fcount4 = 0.f;
	const TEXINFO*		m_pTexInfo;
	const TEXINFO*		m_pTimeTexInfo[10];
	const TEXINFO*		m_pScoreImage;
	DWORD MainGame_Time;
	int Game_Stage;

	float Arrow1_Interval;
	float Arrow1_Speed;
	float Arrow2_Interval;
	float Arrow2_Speed;
	float Lager_Interval;
	float Stage4_Interval;

	bool m_Game_Over = false;

};

