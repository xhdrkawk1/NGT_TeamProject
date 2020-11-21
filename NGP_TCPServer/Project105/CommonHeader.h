#pragma once
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>


using namespace std;
#include <locale>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <time.h>
#define _CRT_NONSTDC_NO_WARNINGS
//Header
#include <locale>

//manager
#include "DataMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
//Direct X9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

// User Header
#include "define.h"
#include "Constant.h"

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;  // 위치벡터
	D3DXVECTOR3 vDir; //방향벡터
	D3DXVECTOR3 vStandard; // 각도의 기준이되는
	D3DXMATRIX mat_World;
}INFO;


class CObj;
typedef list<CObj*>			OBJLIST;
typedef OBJLIST::iterator	OBJITER;


