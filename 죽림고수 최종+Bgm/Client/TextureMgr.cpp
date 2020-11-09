#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}

CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO* CTextureMgr::GetTexInfo(const wstring& wstrObjKey, 
	const wstring& wstrStateKey, const int& iIndex) const
{
	auto iter_find = m_MapTexture.find(wstrObjKey);

	if (m_MapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey, iIndex);
}

HRESULT CTextureMgr::InsertTexture(TEX_ID eID, const wstring& wstrFilePath, 
	const wstring& wstrObjKey, const wstring& wstrStateKey, const int& iCount)
{
	auto iter_find = m_MapTexture.find(wstrObjKey);

	if (m_MapTexture.end() == iter_find)
	{
		CTexture* pTexture = nullptr;

		switch (eID)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(wstrFilePath, wstrStateKey, iCount)))
		{
			MessageBox(nullptr, L"InsertTexture Failed", L"Error", MB_OK);
			return E_FAIL;
		}

		m_MapTexture[wstrObjKey] = pTexture;
	}
	else if (TEX_MULTI == eID)
	{
		if (FAILED(m_MapTexture[wstrObjKey]->InsertTexture(
			wstrFilePath, wstrStateKey, iCount)))
		{
			MessageBox(nullptr, L"InsertTexture Failed", L"Error", MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}


void CTextureMgr::Release()
{
	for (auto& MyPair : m_MapTexture)
		Safe_Delete(MyPair.second);

	m_MapTexture.clear();
}