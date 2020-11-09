#include "stdafx.h"
#include "MultiTexture.h"


CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

TEXINFO* CMultiTexture::GetTexInfo(const wstring& wstrStateKey, 
	const int& iIndex) const
{
	auto iter_find = m_MapMultiTex.find(wstrStateKey);

	if (m_MapMultiTex.end() == iter_find)
		return nullptr;

	return iter_find->second[iIndex];
}

HRESULT CMultiTexture::InsertTexture(const wstring& wstrFilePath,
	const wstring& wstrStateKey, const int& iCount)
{
	auto iter_find = m_MapMultiTex.find(wstrStateKey);

	if (m_MapMultiTex.end() != iter_find)
		return E_FAIL;

	TEXINFO* pTexInfo = nullptr;
	TCHAR szFilePath[MIN_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFilePath, wstrFilePath.c_str(), i);

		pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->tImgInfo)))
		{
			MessageBox(nullptr, L"D3DXGetImageInfoFromFile Failed", L"Error", MB_OK);
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(GET_INSTANCE(CGraphic_Device)->GetDevice(),
			szFilePath,
			pTexInfo->tImgInfo.Width, pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels, 0, pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr,
			&pTexInfo->pTexture)))
		{
			MessageBox(nullptr, L"D3DXCreateTextureFromFileEx Failed", L"Error", MB_OK);
			return E_FAIL;
		}

		m_MapMultiTex[wstrStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for (auto& MyPair : m_MapMultiTex)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(),
			[](TEXINFO*& pTexInfo)
		{
			Safe_Release(pTexInfo->pTexture);
			Safe_Delete(pTexInfo);
		});

		MyPair.second.clear();
		MyPair.second.shrink_to_fit();
	}

	m_MapMultiTex.clear();
}