#pragma once
class CFrameMgr
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	CFrameMgr();
	~CFrameMgr();

public:
	bool LimitFrame();
	void RenderFrame();

private:
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_CpuTick;

	// 1/60√ 
	float	m_fSecPerFrame;
	float	m_fDeltaTime;
	float	m_fTimeCount;

	TCHAR	m_szFPS[MIN_STR];
	int		m_iFPS;

};

