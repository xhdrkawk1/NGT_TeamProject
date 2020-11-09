#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture()
{
}

CSingleTexture::~CSingleTexture()
{
	Release();
}

TEXINFO* CSingleTexture::GetTexInfo(const wstring& wstrStateKey,
	const int& iIndex) const
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::InsertTexture(const wstring& wstrFilePath, 
	const wstring& wstrStateKey, const int& iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTexInfo->tImgInfo)))
	{
		MessageBox(nullptr, L"D3DXGetImageInfoFromFile Failed", L"Error", MB_OK);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(GET_INSTANCE(CGraphic_Device)->GetDevice(),
		wstrFilePath.c_str(),
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0, m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr,
		&m_pTexInfo->pTexture)))
	{
		MessageBox(nullptr, L"D3DXCreateTextureFromFileEx Failed", L"Error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}