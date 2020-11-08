#pragma once
#include "Obj.h"
class CLager :
	public CObj
{
public:
	enum eLagerType{LagerX,LagerY};
public:
	CLager();
	virtual ~CLager();
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	void Set_LagerType(eLagerType type) { m_LagerType = type; }

private:

	DWORD LagerTime;
	eLagerType m_LagerType;
};

