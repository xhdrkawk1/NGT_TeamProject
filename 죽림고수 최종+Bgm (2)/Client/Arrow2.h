#pragma once
#include "Obj.h"
class CArrow2 :
	public CObj
{
public:
	CArrow2();
	virtual ~CArrow2();

	virtual void Initialize() override;
	void LateInit();
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


private:
	float cter;
	float Angle;

};

