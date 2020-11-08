#pragma once
#include "Obj.h"
class CPlayer :
	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	void Set_Game_Over() { m_Bis_Gameover = true; }
	void Set_Game_Over_false() { m_Bis_Gameover = false; }
	bool Get_Game_Over() { return m_Bis_Gameover; }
	void KeyInput();
private:
	TCHAR szBuf[32]=L"";

	float cter;
	float arcCos;

	bool m_Bis_Gameover;
};

