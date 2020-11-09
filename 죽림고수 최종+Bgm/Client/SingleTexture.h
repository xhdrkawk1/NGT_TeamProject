#pragma once

#include "Texture.h"

class CSingleTexture :
	public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

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
	TEXINFO*		m_pTexInfo;
};