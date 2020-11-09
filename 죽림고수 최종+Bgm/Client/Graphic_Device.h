#pragma once

// 0.�׷���ī�� ��ġ�� �ʱ�ȭ�Ѵ�.

// �׷���ī���� ����(��ġ�� ��������)�� �����Ѵ�. �������ִ� ��ü�� ����.(IDirect3D9)
// �׷���ī�� ��ġ�� ��ǥ�ϴ� ��ü�� �����Ѵ�. (IDirect3D9)
// ��ǥ�ϴ� ��ü�� ����.(IDirect3DDevice9)

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

	// DirectX���� 2D �ؽ��� ������ Com��ü.
	LPD3DXSPRITE			m_pSprite = nullptr;
public:
	void Release();
};

