#pragma once

#define VK_MAX 0x100

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr();
	~CKeyMgr();

public:
	bool KeyPressing(int VK_Key);
	bool KeyDown(int VK_Key);
	bool KeyUp(int VK_Key);

public:
	void UpdateKey();

private:
	bool m_bKeyState[VK_MAX];
};