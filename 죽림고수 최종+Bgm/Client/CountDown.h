#pragma once
#include "Obj.h"
class CCountDown :
	public CObj
{
public:
	CCountDown();
	virtual ~CCountDown();


	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

	void SetTime(float fTime);

private:
	float m_fAccTime = 0.f;
};

