#include "StdAfx.h"
//#include "wqueue.h"


//template<typename T> 
//wqueue<T>::wqueue()
// {
// }
//
// template<typename T> 
// wqueue<T>::~wqueue()
// {
// }

//template<typename T> 
// void inline wqueue<T>::add(T item)
//{
//	CGuard guard(m_Mutex);
//	m_queue.push_back(item);
//}

//template<class T> T wqueue<T>::remove()
//{
//	CGuard guard(m_Mutex);
//	while(m_queue.size()==0)
//	{
//		Sleep(1);
//	}
//	T item=m_queue.front();
//	m_queue.pop_front();
//	return item;
//}
//inline template <typename T>
//int wqueue<T>::size()
//{
//	CGuard guard(m_Mutex);
//	int tempsize=m_queue.size();
//	return tempsize;
//	std::conditional_variable d1;
//}
