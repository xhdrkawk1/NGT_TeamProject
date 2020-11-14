#pragma once
#include "Obj.h"
class CEnemy :
	public CObj
{
public:
	CEnemy();
	virtual ~CEnemy();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

