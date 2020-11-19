#include "CommonHeader.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
	: m_fDeltaTime(0.f)
{
	// 하드웨어에서 고해상도 타이머를 얻어오는 함수. 1/10000초
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// 현재 1초당 CPU가 몇번을 진동하는지 진동수를 얻어오는 함수.
	QueryPerformanceFrequency(&m_CpuTick);
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CpuTick);

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;
	m_OldTime = m_CurTime;
}
