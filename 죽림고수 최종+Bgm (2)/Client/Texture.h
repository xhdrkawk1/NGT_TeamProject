#pragma once

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual TEXINFO* GetTexInfo(const wstring& wstrStateKey = L"",
		const int& iIndex = 0) const PURE;

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath,
		const wstring& wstrStateKey = L"", const int& iCount = 0) PURE;

protected:
	virtual void Release() PURE;
};