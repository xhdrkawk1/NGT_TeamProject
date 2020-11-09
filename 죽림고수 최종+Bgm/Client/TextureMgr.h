#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

public:
	enum TEX_ID { TEX_SINGLE, TEX_MULTI, TEX_END };
	
private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* GetTexInfo(const wstring& wstrObjKey,
		const wstring& wstrStateKey = L"", const int& iIndex = 0) const;

public:
	HRESULT InsertTexture(TEX_ID eID, const wstring& wstrFilePath,
		const wstring& wstrObjKey, const wstring& wstrStateKey = L"",
		const int& iCount = 0);

public:

private:
	void Release();

private:
	map<wstring, CTexture*> m_MapTexture;
};