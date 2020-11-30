#include "stdafx.h"
#include "FrameMgr.h"
#include <iostream>
IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
	: m_szFPS(L""), m_iFPS(0), m_fDeltaTime(0.f), m_fTimeCount(0.f)
{
	m_fSecPerFrame = 1.f / MAX_FPS;

	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CpuTick);
}


CFrameMgr::~CFrameMgr()
{
}

bool CFrameMgr::LimitFrame()
{
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CpuTick);

	m_fDeltaTime += float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;
	m_OldTime = m_CurTime;

	if (m_fSecPerFrame <= m_fDeltaTime)
	{
		m_fDeltaTime = 0.f;
		++m_iFPS;
		return true;
	}

	return false;
}

void CFrameMgr::RenderFrame()
{
	m_fTimeCount += GET_INSTANCE(CTimeMgr)->GetDeltaTime();

	// 1ÃÊ Áö³µ´Ù.
	if (1.f <= m_fTimeCount)
	{
		//std::cout << "FPS:" << m_iFPS << std::endl;

		m_iFPS = 0;
		m_fTimeCount = 0.f;		
	}

}
