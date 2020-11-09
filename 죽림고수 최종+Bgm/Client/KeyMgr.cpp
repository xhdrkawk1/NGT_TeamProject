#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::KeyPressing(int VK_Key)
{
	if (GetAsyncKeyState(VK_Key) & 0x8000)
		return true;

	return false;
}

bool CKeyMgr::KeyDown(int VK_Key)
{
	// 이전에 누른적 없고 호출시점에서 눌렀을 떄
	if (!m_bKeyState[VK_Key] && GetAsyncKeyState(VK_Key) & 0x8000)
	{
		m_bKeyState[VK_Key] = !m_bKeyState[VK_Key];
		return true;
	}

	return false;
}

bool CKeyMgr::KeyUp(int VK_Key)
{
	// 이전에 누른적 있고 호출시점에서 눌르지 않았을 때
	if (m_bKeyState[VK_Key] && !(GetAsyncKeyState(VK_Key) & 0x8000))
	{
		m_bKeyState[VK_Key] = !m_bKeyState[VK_Key];
		return true;
	}

	return false;
}

void CKeyMgr::UpdateKey()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		if (!m_bKeyState[i] && GetAsyncKeyState(i) & 0x8000)
			m_bKeyState[i] = !m_bKeyState[i];
	}
}