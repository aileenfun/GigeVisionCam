// -*- coding: gb2312-dos -*-

#ifndef _MV_WINDOWS_THREAD_H__
#define _MV_WINDOWS_THREAD_H__

#ifdef MV_WINDOWS

#include "windows.h"
#include "MV_CrossPlatformDefine.h"

// ��ʼ��һ���ٽ���
void    WINDOWS_InitMutex(MV_Mutex * mutex);
// �ͷ��ٽ���
void    WINDOWS_EnterMutex(MV_Mutex * mutex);
// �����ٽ���
void    WINDOWS_LeaveMutex(MV_Mutex * mutex);
// �˳��ٽ���
void    WINDOWS_DeleteMutex(MV_Mutex * mutex);

// �����߳�
// �ɹ����طǿ�ָ�룬ʧ�ܷ��ؿ�ָ��
void*   WINDOWS_CreateThread(MV_THREAD_ATTR* attr ,Thread_CallBack callbackFunc , void* pUser);

// �ȴ��߳������˳�
int     WINDOWS_WaitForThreadEnd(void* pThread);
// �����߳�
int     WINDOWS_EndThread();
// �����߳̾��
int     WINDOWS_DestroyThread(void* pThread);

#endif
#endif
