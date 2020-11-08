#pragma once
#include "Obj.h"
class CArrow :
	public CObj
{
public:
	CArrow();
	virtual ~CArrow();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


private:
	float cter;
};

