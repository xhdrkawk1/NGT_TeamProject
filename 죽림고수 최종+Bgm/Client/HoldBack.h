#pragma once
#include "Obj.h"
class CHoldBack :
	public CObj
{
public:
	CHoldBack();
	virtual ~CHoldBack();


	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


private:

};

