#pragma once
typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // 위치벡터
	D3DXVECTOR3 vDir; //방향벡터
	D3DXVECTOR3 vStandard; // 각도의 기준이되는
	D3DXMATRIX mat_World;
}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture;

	D3DXIMAGE_INFO	   tImgInfo;
}TEXINFO;
typedef struct tagObjectInfo
{

	D3DXMATRIX mat_World;
}OB_INFO;