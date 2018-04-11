#ifndef _old
#pragma once

#include <list>
#include "GigEMutex.h"
#include <windows.h>
using namespace std;
template <typename T>class GigEwqueue
{
private:
	list<T>   m_queue;
	GigeC_Mutex     m_Mutex;
	GigeC_Mutex sizelock;
	bool stop;
	int maxlimit;// 0 for no limit
	CONDITION_VARIABLE noempty;
	CONDITION_VARIABLE nofull;
	CRITICAL_SECTION qlock;
public:
	inline GigEwqueue(void);
	inline ~GigEwqueue(void);
public:
	inline void add(T item);
	inline void insert_front(T item);//for get data use only
	inline T remove();
	inline int size();
	inline void reset();
	inline int setlimit(int l);
	inline void clear();
};

template<typename T> 
inline GigEwqueue<T>::GigEwqueue()
 {
	 InitializeConditionVariable(&noempty);
	 InitializeConditionVariable(&nofull);
	 InitializeCriticalSection(&qlock);
	 maxlimit=0;
	 stop=false;
 }

 template<typename T> 
 GigEwqueue<T>::~GigEwqueue()
 {
	EnterCriticalSection(&qlock);
	stop=true;
	LeaveCriticalSection(&qlock);
	Sleep(1000);
	WakeAllConditionVariable(&nofull);
	WakeAllConditionVariable(&noempty);
 }

template<typename T> 
 void GigEwqueue<T>::add(T item)
{
	EnterCriticalSection (&qlock);
	if(maxlimit!=0&&m_queue.size()>maxlimit&&stop==false)
	{
		SleepConditionVariableCS(&nofull,&qlock,1);
		//return;
	}

	m_queue.push_back(item);
	LeaveCriticalSection(&qlock);
	WakeConditionVariable(&noempty);
}

template<class T> T GigEwqueue<T>::remove()
{
	int wait_cnt=0;
	//CGuard guardsize(sizelock);
	EnterCriticalSection(&qlock);
	while(m_queue.size()==0)
	{
		if(stop||wait_cnt>6)
		{
			LeaveCriticalSection(&qlock);
			return NULL;
		}
		wait_cnt++;
		//Sleep(1);
		SleepConditionVariableCS(&noempty,&qlock,1);
	}
	//delete guardsize;
	//CGuard guard(m_Mutex);
	T item=m_queue.front();
	m_queue.pop_front();
	LeaveCriticalSection(&qlock);
	WakeConditionVariable(&nofull);
	return item;
}
template<class T>
void  GigEwqueue<T>::clear()
{
	EnterCriticalSection(&qlock);
	for(int i=0;i<m_queue.size();i++)
	{
		T item=m_queue.front();
		m_queue.pop_front();
		delete item;
	}
	LeaveCriticalSection(&qlock);
}
template<class T>
void  GigEwqueue<T>::reset()
{

	//CGuard guard(m_Mutex);
	//CGuard guardsize(sizelock);
	EnterCriticalSection(&qlock);
	stop=true;
	LeaveCriticalSection(&qlock);
	Sleep(1000);
	WakeAllConditionVariable(&nofull);
	WakeAllConditionVariable(&noempty);
	for(int i=0;i<m_queue.size();i++)
	{
		T item=m_queue.front();
		m_queue.pop_front();
		delete item;
	}
}
template <typename T>
inline int GigEwqueue<T>::size()
{
	CGuard guard(m_Mutex);
	int tempsize=m_queue.size();
	std::conditional_variable d1;
	return tempsize;
}
template<typename T> 
 void GigEwqueue<T>::insert_front(T item)
{
	CGuard guard(m_Mutex);
	m_queue.push_front(item);
 }
 template<typename T> 
 int GigEwqueue<T>::setlimit(int l)
 {
	 maxlimit=l;
	 return maxlimit;
 }
#endif

#ifdef _old
#pragma once

#include <list>
#include "Mutex.h"
#include <windows.h>
using namespace std;
template <typename T>class wqueue
{
private:
	list<T>   m_queue;
	C_Mutex     m_Mutex;
	C_Mutex sizelock;
	bool stop;
	int maxlimit;// 0 for no limit
public:
	inline wqueue(void);
	inline ~wqueue(void);
public:
	inline void add(T item);
	inline void insert_front(T item);//for get data use only
	inline T remove();
	inline int size();
	inline void reset();
	inline int setlimit(int l);
};

template<typename T> 
inline wqueue<T>::wqueue()
 {
	 maxlimit=0;
	 stop=false;
 }

 template<typename T> 
 wqueue<T>::~wqueue()
 {
	 stop=true;
 }

template<typename T> 
 void wqueue<T>::add(T item)
{
	CGuard guard(m_Mutex);
	if(maxlimit!=0&&m_queue.size()>maxlimit)
	{
		return;
	}

	m_queue.push_back(item);
	
}

template<class T> T wqueue<T>::remove()
{
	int wait_cnt=0;
	CGuard guardsize(sizelock);
	while(m_queue.size()==0)
	{
		if(stop||wait_cnt>10)
			return NULL;
		wait_cnt++;
		Sleep(1);
	}
	//delete guardsize;
	CGuard guard(m_Mutex);
	T item=m_queue.front();
	m_queue.pop_front();
	return item;
}
template<class T>
void  wqueue<T>::reset()
{
	CGuard guard(m_Mutex);
	CGuard guardsize(sizelock);
	stop=true;
	Sleep(1000);
	for(int i=0;i<m_queue.size();i++)
	{
		T item=m_queue.front();
		m_queue.pop_front();
		delete item;
	}
}
template <typename T>
inline int wqueue<T>::size()
{
	CGuard guard(m_Mutex);
	int tempsize=m_queue.size();
	std::conditional_variable d1;
	return tempsize;
}
template<typename T> 
 void wqueue<T>::insert_front(T item)
{
	CGuard guard(m_Mutex);
	m_queue.push_front(item);
 }
 template<typename T> 
 int wqueue<T>::setlimit(int l)
 {
	 maxlimit=l;
	 return maxlimit;
 }

#endif