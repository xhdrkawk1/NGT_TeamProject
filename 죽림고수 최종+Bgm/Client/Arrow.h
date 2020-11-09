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
	D3DXVECTOR3 m_Arrow_spot[4];
	D3DXVECTOR3 m_Standard_spot[4];
	float cter;
};

