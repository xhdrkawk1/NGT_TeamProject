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
	bool Get_Game_Over() { return m_Bis_Gameover; }
	void KeyInput();
	void Set_AllPlayerDeadWait(bool bIsTemp) { m_bIsWaitForAllDead = bIsTemp; }
private:
	TCHAR szBuf[32]=L"";
	D3DXVECTOR3 m_player_spot[4];
	D3DXVECTOR3 m_Standard_spot[4];
	float cter;
	float arcCos;

	bool m_bIsWaitForAllDead = false;
	bool m_Bis_Gameover;
};

