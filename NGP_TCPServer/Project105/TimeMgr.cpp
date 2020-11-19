#include "CommonHeader.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
	: m_fDeltaTime(0.f)
{
	// �ϵ����� ���ػ� Ÿ�̸Ӹ� ������ �Լ�. 1/10000��
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// ���� 1�ʴ� CPU�� ����� �����ϴ��� �������� ������ �Լ�.
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
