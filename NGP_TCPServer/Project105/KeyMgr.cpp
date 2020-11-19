#include "CommonHeader.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwCurKey(0), m_dwKeyPressed(0), m_dwKeyDown(0)
{

}

CKeyMgr::~CKeyMgr()
{
}

// 매 프레임마다 호출
void CKeyMgr::KeyCheck()
{
	m_dwCurKey = 0;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwCurKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwCurKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwCurKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwCurKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwCurKey |= KEY_ENTER;
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
		m_dwCurKey |= KEY_MBUTTON;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurKey |= KEY_A;
	if (GetAsyncKeyState('T') & 0x8000)
		m_dwCurKey |= KEY_T;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwCurKey |= KEY_Q;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwCurKey |= KEY_E;
	if (GetAsyncKeyState('Z') & 0x8000)
		m_dwCurKey |= KEY_Z;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurKey |= KEY_D;
	if (GetAsyncKeyState('K') & 0x8000)
		m_dwCurKey |= KEY_K;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwCurKey |= KEY_P;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurKey |= KEY_S;
	if (GetAsyncKeyState('V') & 0x8000)
		m_dwCurKey |= KEY_V;
	if (GetAsyncKeyState('O') & 0x8000)
		m_dwCurKey |= KEY_O;
	if (GetAsyncKeyState('C') & 0x8000)
		m_dwCurKey |= KEY_C;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwCurKey |= KEY_R;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurKey |= KEY_W;
	if (GetAsyncKeyState('I') & 0x8000)
		m_dwCurKey |= KEY_I;
	if (GetAsyncKeyState('M') & 0x8000)
		m_dwCurKey |= KEY_M;
	if (GetAsyncKeyState('N') & 0x8000)
		m_dwCurKey |= KEY_N;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	if (m_dwCurKey & dwKey)
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}
	else if (m_dwKeyPressed & dwKey)
	{
		m_dwKeyPressed ^= dwKey;
		return true;
	}

	return false;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// 이전에 누른적 없고 현재 눌렸을 때
	if (!(m_dwKeyDown & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	// 현재 누른적 없으나 기존에 눌렸을 때
	else if (!(m_dwCurKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwCurKey & dwKey)
		return true;

	return false;
}

// 두 키가 순서대로 눌렸을 경우
bool CKeyMgr::KeyCombined(DWORD dwFirst, DWORD dwSecond)
{
	if (KeyDown(dwSecond) && (m_dwCurKey & dwFirst))
		return true;

	return false;
}

bool CKeyMgr::KeyImit()
{
	m_dwCurKey = 0;
	m_dwKeyPressed = 0;
	m_dwKeyDown = 0;
	return false;
}
