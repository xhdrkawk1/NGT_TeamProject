#pragma once
class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;

	void UpdateRect();
	void UpdateArrowRect();
	void Set_Pos(float x, float y);
	void Set_speed(float speed) { m_fspeed = speed; }
	void Set_WorldMatrix(D3DXMATRIX matWorld);
public:
	INFO &Get_Info() { return m_tInfo; }
	const RECT& GetRect() const { return m_tRect; }
	TEXINFO GetImgInfo() { return m_texInfo; }
	bool m_bIsDead;
	void IsDead() { m_bIsDead = true; }
protected:
	RECT m_tRect;
	INFO m_tInfo;
	TEXINFO m_texInfo;

	float m_fspeed;
	const TEXINFO*		m_pTexInfo;

};

