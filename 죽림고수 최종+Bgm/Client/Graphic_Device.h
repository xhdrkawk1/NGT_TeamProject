#pragma once

// 0.그래픽카드 장치를 초기화한다.

// 그래픽카드의 정보(장치의 지원수준)를 조사한다. 조사해주는 객체가 존재.(IDirect3D9)
// 그래픽카드 장치를 대표하는 객체를 생성한다. (IDirect3D9)
// 대표하는 객체가 존재.(IDirect3DDevice9)

class CGraphic_Device
{
	DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE { MODE_FULL, MODE_WIN };
public:
	explicit CGraphic_Device();
	virtual ~CGraphic_Device();

public:
	LPDIRECT3DDEVICE9 GetDevice() { return m_pGraphic_Device; }
	LPD3DXSPRITE GetSprite() { return m_pSprite; }

public:
	HRESULT Init_Graphic_Device(HWND hWnd, WINMODE eMode,
		const unsigned int& g_iBackCX,
		const unsigned int& g_iBackCY, LPDIRECT3DDEVICE9* ppGraphic_Device = nullptr);

	void Render_Begin();
	void Render_End();

private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

	// DirectX에서 2D 텍스쳐 렌더링 Com객체.
	LPD3DXSPRITE			m_pSprite = nullptr;
public:
	void Release();
};

