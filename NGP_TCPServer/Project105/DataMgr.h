#pragma once
#include "define.h"

class CDataMgr
{
	DECLARE_SINGLETON(CDataMgr)
public:
	CDataMgr() ;
	~CDataMgr() ;

public:

	int m_iConnect_Player = 0;

	vector<SOCKET> m_ClientSocketList;
};

