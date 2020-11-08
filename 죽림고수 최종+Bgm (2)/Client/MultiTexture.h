#pragma once

#include "Texture.h"

class CMultiTexture :
	public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual TEXINFO * GetTexInfo(const wstring& wstrStateKey = L"", 
		const int& iIndex = 0) const override;

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", const int& iCount = 0) override;

private:
	virtual void Release() override;

private:
	map<wstring, vector<TEXINFO*>> m_MapMultiTex;
};