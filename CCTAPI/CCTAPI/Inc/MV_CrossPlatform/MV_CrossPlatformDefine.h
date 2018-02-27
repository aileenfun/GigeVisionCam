// -*- coding: gb2312-dos -*-
/** @file       MV_CrossPlatform.h
 *  @note       HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *  @brief      Machine Vision CrossPlatform define
 *
 *  @author     Machine Vision Team
 *  @date       2014/12/12
 *  @note       ����;
 *
 *  @warning
 */
#ifndef _MV_CROSSPLATFORM_DEFINE_H_
#define _MV_CROSSPLATFORM_DEFINE_H_

#include <stdio.h>

#if defined(WIN32)
#define MV_WINDOWS
#endif

// ͷ�ļ�����
#if defined(MV_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#else
#include "stdint.h"
#endif

typedef int MV_BOOL;
#ifndef MV_TRUE
#define MV_TRUE 1
#endif

#ifndef MV_FALSE
#define MV_FALSE 0
#endif

// �������Ͷ���
// ��ָ�붨��
#ifndef MV_NULL
#ifdef __cplusplus
#define MV_NULL    0
#else
#define MV_NULL    ((void *)0)
#endif
#endif

// �ٽ�������
#if defined(MV_WINDOWS)
#define     MV_Mutex    CRITICAL_SECTION
#elif defined(MV_LINUX)
#define     MV_Mutex    pthread_mutex_t
#elif defined(MV_APPLE)
#define     MV_Mutex    pthread_mutex_t
#endif

// �߳����ȼ�����
typedef enum
{
    MV_THREAD_PRIORITY_LOWEST = 0,
    MV_THREAD_PRIORITY_BELOW_NORMAL,
    MV_THREAD_PRIORITY_NORMAL,
    MV_THREAD_PRIORITY_ABOVE_NORMAL,
    MV_THREAD_PRIORITY_HIGHEST,

    MV_THREAD_CREATE_JOINABLE = 0,
    MV_THREAD_CREATE_DETACHED = 1,

}MV_THREAD_Property;

typedef struct
{
    MV_THREAD_Property threadPriority;  //�߳����ȼ�

#if defined(MV_LINUX) || defined(MV_ANDROID)
    MV_THREAD_Property detachState;     // �̷߳���״̬����ʹ��pthreadʱ����Ч��
#endif

}MV_THREAD_ATTR;

// �̻߳ص��������Ͷ���
#if defined(MV_WINDOWS)

#ifndef MV_STDCALL
#define MV_STDCALL __stdcall
#endif

typedef unsigned int ThreadReturnType ;

#ifndef MV_SLEEP
#define MV_SLEEP(x) Sleep(x)
#endif

#else  // NOT WINDOWS

#ifndef MV_STDCALL
#define MV_STDCALL
#endif

typedef void* ThreadReturnType;

#ifndef MV_SLEEP
#define MV_SLEEP(x) usleep(x*1000)
#endif
#endif

// �߳�ִ�к�������
typedef ThreadReturnType(MV_STDCALL *Thread_CallBack)( void* pUser);

typedef enum
{
    MV_PRINT_DEBUG  = 0,
    MV_PRINT_WHENEVER,
}MV_PRINT_TYPE;

typedef struct MV_SYSTEMTIME_T
{
    unsigned short  wYear;
    unsigned short  wMonth;
    unsigned short  wDayOfWeek;
    unsigned short  wDay;
    unsigned short  wHour;
    unsigned short  wMinute;
    unsigned short  wSecond;
    unsigned short  wMilliseconds;
}MV_SYSTEMTIME;

//#define MAX_GUID_LENGTH     64

#endif //_MV_CROSSPLATFORM_DEFINE_H_
