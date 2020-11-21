#pragma once
#include "Obj.h"
class CWarning :
	public CObj
{
public:
	enum Warn_Dir{WARNX,WARNY};
public:
	CWarning();
	virtual ~CWarning();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

	void Set_Dir(Warn_Dir Dir) { m_Warn_Dir = Dir; }
private:
	Warn_Dir m_Warn_Dir;
	DWORD WarningTime;
	float Angle;
	bool m_bIsInit;
};

