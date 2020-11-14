#pragma once
#include "Obj.h"
class CLogo :
	public CObj
{
public:
	CLogo();
	virtual ~CLogo();


	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


private:

};

