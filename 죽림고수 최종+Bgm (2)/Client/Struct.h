#pragma once
typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // ��ġ����
	D3DXVECTOR3 vDir; //���⺤��
	D3DXVECTOR3 vStandard; // ���������� �����̵Ǵ��� {-1,0,0}
	D3DXMATRIX mat_World; // ������ǥ
}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture;

	D3DXIMAGE_INFO	   tImgInfo;
}TEXINFO;
