#pragma once

template <typename T>
void Safe_Delete(T& ptr)
{
	if (nullptr != ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
unsigned long Safe_Release(T& pInstance)
{
	unsigned long dwRefCnt = 0;

	if (nullptr != pInstance)
	{
		dwRefCnt = pInstance->Release();

		if (0 == dwRefCnt)
			pInstance = nullptr;
	}

	return dwRefCnt;
}