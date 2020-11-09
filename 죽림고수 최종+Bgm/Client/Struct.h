#pragma once
typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // ��ġ����
	D3DXVECTOR3 vDir; //���⺤��
	D3DXVECTOR3 vStandard; // ������ �����̵Ǵ�
	D3DXMATRIX mat_World;
}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture;

	D3DXIMAGE_INFO	   tImgInfo;
}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;

	BYTE		byDrawID;
	BYTE		byOption;
}TILE;
