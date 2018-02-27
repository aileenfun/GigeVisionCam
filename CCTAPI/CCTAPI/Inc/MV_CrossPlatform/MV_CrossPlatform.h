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
#ifndef _MV_CROSSPLATFORM_H_
#define _MV_CROSSPLATFORM_H_

#include "MV_CrossPlatformDefine.h"

// ��ƽ̨�ӿڶ���
#ifdef __cplusplus
extern "C"
{
#endif

    /************************************************************************
     *      �߳����
     ************************************************************************/

    // ��ʼ��һ���ٽ���
    void    MV_InitMutex(MV_Mutex * mutex);
    // �ͷ��ٽ���
    void    MV_EnterMutex(MV_Mutex * mutex);
    // �����ٽ���
    void    MV_LeaveMutex(MV_Mutex * mutex);
    // �˳��ٽ���
    void    MV_DeleteMutex(MV_Mutex * mutex);
    // �����߳�
    void*   MV_CreateThread(MV_THREAD_ATTR* attr ,Thread_CallBack callbackFunc , void* pUser);
    // �ȴ��߳������˳�
    int     MV_WaitForThreadEnd(void* pThread);
    // �����߳�
    int     MV_EndThread();
    // �����߳̾��
    int     MV_DestroyThread(void* pThread);



    /************************************************************************
     *      �ڴ���������ͷ�
     ************************************************************************/

    // ���������ڴ�
    void*   MV_Aligned_Malloc(unsigned int size , unsigned int alignment);
    // �����ͷ��ڴ�
    void    MV_Aligned_Free(void* memblock);

    // ����ֵ0��ʾ�ɹ�
    int     MV_RellocBufferAdapt(unsigned char** ppBuf, unsigned int nDataLen, unsigned int* pnSize);



    /************************************************************************
     *      ��־���
     ************************************************************************/

    // ��ȡ��ǰϵͳʱ�䣬��ȷ���뼶
    void    MV_GetCurTime(char* strTime);
    // ֱ�Ӵ�ӡ��־��Ϣ
    void    MV_PrintInfo(MV_PRINT_TYPE enType, char *szFormat, ...);
    // д��־�ļ�-�������ɵ��ļ�·��
    void    MV_DumpLogBystr( char* pFile, char *fmt, ...);
    // д��־�ļ�-������־�ļ�ָ��
    void    MV_DumpLogByfp( FILE* fp, char *fmt, ...);

    double  MV_GetMilliseconds();


    // һЩ���ߺ���
    //const char* MV_CreateGUID();

#ifdef __cplusplus
}
#endif

/************************************************************************
 *      �����������ͳ�ʼ���ϲ�
 ************************************************************************/
class CMVMutex
{
  public:
    CMVMutex()
    {
        MV_InitMutex(&m_Mutex);
    }
    ~CMVMutex()
    {
        MV_DeleteMutex(&m_Mutex);
    }
    void Lock()
    {
        MV_EnterMutex(&m_Mutex);
    }
    void Unlock()
    {
        MV_LeaveMutex(&m_Mutex);
    }

  private:
    MV_Mutex    m_Mutex;
};

class CMVLock
{
  public:
    CMVLock(CMVMutex* pMutex = MV_NULL)
    {
        pMutex->Lock();
        m_pMutex = pMutex;
    }
    ~CMVLock()
    {
        m_pMutex->Unlock();
    }
  private:
    CMVMutex*   m_pMutex;
};

#endif //_MV_CROSSPLATFORM_H_
