#pragma once

#define SERVERPORT 9000

#define SERVERIP   "127.0.0.1"
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#define WINCX 1000
#define WINCY 1000
#define DEAD_OBJ 1
#define PI 3.141592
#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256
#define NULL_CHECK(PTR) if(nullptr == (PTR)) return ;
#define NULL_CHECK_RETURN(PTR, VAL) if(nullptr == (PTR)) return VAL;


#define DECLARE_SINGLETON(ClassName)		\
public:										\
	static ClassName* GetInstance()			\
	{										\
		if (nullptr == m_pInstance)			\
			m_pInstance = new ClassName;	\
		return m_pInstance;					\
	}										\
	void DestroyInstance()					\
	{										\
		if (m_pInstance)					\
		{									\
			delete m_pInstance;				\
			m_pInstance = nullptr;			\
		}									\
	}										\
private:									\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)		\
ClassName* ClassName::m_pInstance = nullptr;

#define GET_INSTANCE(CLASSNAME)	\
	CLASSNAME::GetInstance()

typedef struct tagLoginData
{
	bool LoginSuccess;
	char strName[100];

}LOGINDATA;


