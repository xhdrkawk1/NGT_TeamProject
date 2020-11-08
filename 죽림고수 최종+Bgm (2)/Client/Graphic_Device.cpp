#include "stdafx.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_pSDK(nullptr)
	, m_pGraphic_Device(nullptr)
{

}

CGraphic_Device::~CGraphic_Device()
{
	Release();
}


HRESULT CGraphic_Device::Init_Graphic_Device(HWND hWnd, WINMODE eMode,
	const unsigned int& iBackCX, const unsigned int& iBackCY,
	LPDIRECT3DDEVICE9* ppGraphic_Device)
{
	// 장치를 조사할 수 있는 객체를 생성한다.
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_pSDK)
		return E_FAIL;

	// 장치 기능을 조사한다.
	D3DCAPS9		DevCaps;
	ZeroMemory(&DevCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DevCaps)))
		return E_FAIL;

	DWORD		dwBehaviorFlag = 0;

	// 여러가지 조사 중.
	// 장치를 통한 렌더링연산(정점에 대한 변환, 조명연산)이 가능한지에 대한 조사
	if (DevCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwBehaviorFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwBehaviorFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS			d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	/*
	UINT                FullScreen_RefreshRateInHz;
	UINT                PresentationInterval;
	*/

	d3dpp.BackBufferWidth = iBackCX;
	d3dpp.BackBufferHeight = iBackCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	d3dpp.Windowed = eMode;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, dwBehaviorFlag, &d3dpp, &m_pGraphic_Device)))
	{
		MessageBox(0,L"CreateDevice Failed!!", L"Error", MB_OK);
		return E_FAIL;
	}

	if (nullptr != ppGraphic_Device)
		*ppGraphic_Device = m_pGraphic_Device;

	// 스프라이트 Com객체를 할당
	if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
	{
		MessageBox(0, L"D3DXCreateSprite Failed!!", L"Error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CGraphic_Device::Render_Begin()
{
	// 렌더링 과정
	// 1. 후면버퍼를 비운다.(clear)
	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);


	// 2. 후면버퍼에 그린다.
	m_pGraphic_Device->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphic_Device::Render_End()
{
	m_pSprite->End();
	m_pGraphic_Device->EndScene();

	// 3. 후면버퍼와 전면버퍼를 교체하여 화면에 출력한다.(present)
	m_pGraphic_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void CGraphic_Device::Release()
{
	unsigned long		dwRefCnt = 0;

	if (dwRefCnt = Safe_Release(m_pSprite))
		MessageBox(0, L"ID3DXSprite Release Failed", L"Error", MB_OK);

	if (dwRefCnt = Safe_Release(m_pGraphic_Device))
		MessageBox(0, L"IDirect3DDevice9 Release Failed", L"Error", MB_OK);

	if (dwRefCnt = Safe_Release(m_pSDK))
		MessageBox(0, L"IDirect3D9 Release Failed", L"Error", MB_OK);
}

