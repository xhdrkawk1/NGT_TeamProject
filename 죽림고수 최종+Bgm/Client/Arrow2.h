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
	D3DXVECTOR3 m_Arrow_spot[4];
	D3DXVECTOR3 m_Standard_spot[4];
	float cter;
	float Angle;
	D3DXMATRIX mat_Scale, mat_Rotation, mat_Translate;
};

