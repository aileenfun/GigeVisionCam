#pragma once
#include <afxmt.h>

class GigeC_Mutex
{
public:
	inline GigeC_Mutex::GigeC_Mutex()
	{
	};
	inline GigeC_Mutex::~GigeC_Mutex()
	{
	};
	inline void GigeC_Mutex::Lock()
	{
		m_Sem.Lock();
	};
	inline void GigeC_Mutex::UnLock()
	{
		m_Sem.Unlock();
	};
private:
	CSemaphore m_Sem;
};

class CGuard
{
public:

	inline CGuard::CGuard(GigeC_Mutex &mutex):m_Mutex(mutex)
	{
		m_Mutex.Lock();
	};
	inline CGuard::~CGuard()
	{
		m_Mutex.UnLock();
	};
private:
	GigeC_Mutex &m_Mutex;
};
