#pragma once



class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr();
	~CKeyMgr();

public:
	// 매 프레임마다 호출
	void KeyCheck();

public:
	bool KeyUp(DWORD dwKey);
	bool KeyDown(DWORD dwKey);
	bool KeyPressing(DWORD dwKey);
	bool KeyCombined(DWORD dwFirst, DWORD dwSecond);
	bool KeyImit();
private:
	DWORD m_dwCurKey;
	DWORD m_dwKeyPressed;
	DWORD m_dwKeyDown;
};

