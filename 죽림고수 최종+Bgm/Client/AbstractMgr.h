#pragma once
class CObj;
template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {};
	~CAbstractFactory() {};

	static CObj* CreateObj()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}
	static CObj* CreateObj(float x,float y)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(x, y);
		return pObj;
	}
	static CObj* CreateObj(D3DXMATRIX matWorld)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_WorldMatrix(matWorld);
		return pObj;



	}
};

