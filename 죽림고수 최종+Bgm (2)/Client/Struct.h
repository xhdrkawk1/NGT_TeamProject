#pragma once
typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // 위치벡터
	D3DXVECTOR3 vDir; //방향벡터
	D3DXVECTOR3 vStandard; // 각도선정의 기준이되는축 {-1,0,0}
	D3DXMATRIX mat_World; // 월드좌표
}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9 pTexture;

	D3DXIMAGE_INFO	   tImgInfo;
}TEXINFO;
