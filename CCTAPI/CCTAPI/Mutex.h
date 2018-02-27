#pragma once
#include <afxmt.h>

class C_Mutex
{
public:
	inline C_Mutex::C_Mutex()
	{
	};
	inline C_Mutex::~C_Mutex()
	{
	};
	inline void C_Mutex::Lock()
	{
		m_Sem.Lock();
	};
	inline void C_Mutex ::UnLock()
	{
		m_Sem.Unlock();
	};
private:
	CSemaphore m_Sem;
};

class CGuard
{
public:

	inline CGuard::CGuard(C_Mutex &mutex):m_Mutex(mutex)
	{
		m_Mutex.Lock();
	};
	inline CGuard::~CGuard()
	{
		m_Mutex.UnLock();
	};
private:
	C_Mutex &m_Mutex;
};
