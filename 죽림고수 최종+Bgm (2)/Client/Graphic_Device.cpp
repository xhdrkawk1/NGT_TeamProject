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
	// ��ġ�� ������ �� �ִ� ��ü�� �����Ѵ�.
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == m_pSDK)
		return E_FAIL;

	// ��ġ ����� �����Ѵ�.
	D3DCAPS9		DevCaps;
	ZeroMemory(&DevCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DevCaps)))
		return E_FAIL;

	DWORD		dwBehaviorFlag = 0;

	// �������� ���� ��.
	// ��ġ�� ���� ����������(������ ���� ��ȯ, ������)�� ���������� ���� ����
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

	// ��������Ʈ Com��ü�� �Ҵ�
	if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
	{
		MessageBox(0, L"D3DXCreateSprite Failed!!", L"Error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CGraphic_Device::Render_Begin()
{
	// ������ ����
	// 1. �ĸ���۸� ����.(clear)
	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);


	// 2. �ĸ���ۿ� �׸���.
	m_pGraphic_Device->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphic_Device::Render_End()
{
	m_pSprite->End();
	m_pGraphic_Device->EndScene();

	// 3. �ĸ���ۿ� ������۸� ��ü�Ͽ� ȭ�鿡 ����Ѵ�.(present)
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

