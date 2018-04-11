#include "StdAfx.h"
#include "MessageQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMessageQueue::CMessageQueue(void)
{
	m_iQueueCapacity=0;	
	m_iMsgCount=0;
	m_iPutIndex=0;		
	m_iGetIndex=0;		
	m_iMsgDataCapacity=0;	
	m_pUdMsg=NULL;			
}

CMessageQueue::~CMessageQueue(void)
{
	ReleaseResource();
}

int CMessageQueue::Open( int iQCap,int iDataCap )
{
	CGuard guard(m_Mutex);
	if(iQCap<=0||iDataCap<=0)
		return -1;
	m_iQueueCapacity=iQCap;
	m_iMsgDataCapacity=iDataCap;
	ReleaseResource();
	ApplyResource();
	return 0;
}

int CMessageQueue::Close()
{
	CGuard guard(m_Mutex);
	ReleaseResource();
	return 0;
}

int CMessageQueue::PutMessage( UD_MESSAGE* pMsg )
{
	CGuard guard(m_Mutex);
	if(m_iMsgCount>=m_iQueueCapacity)
		return -1;
	m_pUdMsg[m_iPutIndex].iType=pMsg->iType;
	m_pUdMsg[m_iPutIndex].iDataSize=min(pMsg->iDataSize,m_iMsgDataCapacity);
	memcpy(m_pUdMsg[m_iPutIndex].pData,pMsg->pData,m_pUdMsg[m_iPutIndex].iDataSize);
	++m_iMsgCount;
	++m_iPutIndex;
	m_iPutIndex%=m_iQueueCapacity;
	return 0;
}

int CMessageQueue::GetMessage( UD_MESSAGE* pMsg )
{
	CGuard guard(m_Mutex);
	if(m_iMsgCount<=0)
		return -1;
	pMsg->iType=m_pUdMsg[m_iGetIndex].iType;
	pMsg->iDataSize=m_pUdMsg[m_iGetIndex].iDataSize;
	memcpy(pMsg->pData,m_pUdMsg[m_iGetIndex].pData,m_pUdMsg[m_iGetIndex].iDataSize);
	--m_iMsgCount;
	++m_iGetIndex;
	m_iGetIndex%=m_iQueueCapacity;
	return 0;
}

int CMessageQueue::Reset()
{
	CGuard guard(m_Mutex);
	m_iPutIndex=0;
	m_iGetIndex=0;
	m_iMsgCount=0;
	return 0;
}

int CMessageQueue::GetCount()
{
	CGuard guard(m_Mutex);
	return m_iMsgCount;
}

void CMessageQueue::ReleaseResource()
{
	if(m_pUdMsg!=NULL)
	{
		for(int i=0;i<m_iQueueCapacity;++i)
		{
			if(m_pUdMsg[i].pData!=NULL)
			{
				delete[] m_pUdMsg[i].pData;
				m_pUdMsg[i].pData=NULL;
			}
		}
		delete[] m_pUdMsg;
		m_pUdMsg=NULL;
	}
}

void CMessageQueue::ApplyResource()
{
	m_pUdMsg=new UD_MESSAGE[m_iQueueCapacity];
	for(int i=0;i<m_iQueueCapacity;++i)
	{
		m_pUdMsg[i].pData=new char[m_iMsgDataCapacity];
		memset(m_pUdMsg[i].pData,0,sizeof(char)*m_iMsgDataCapacity);
	}
}
