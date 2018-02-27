#pragma once
#include "Mutex.h"

typedef struct UD_MESSAGE_S				//用户自定义消息
{
	int iType;							//消息类型
	int iDataSize;						//消息数据字节数
	char* pData;						//消息数据地址
	VOID * pAny;
	UD_MESSAGE_S()
	{
		memset(this,0,sizeof(*this));
	}

}UD_MESSAGE,*PUD_MESSAGE;

class CMessageQueue
{
public:
	CMessageQueue(void);
	~CMessageQueue(void);

public:
	int Open(int iQCap,int iDataCap);	//参数:队列容量;消息数据容量
	int Close();
	int PutMessage(UD_MESSAGE* pMsg);	//存放消息
	int GetMessage(UD_MESSAGE* pMsg);	//获取消息
	int Reset();						//重置队列
	int GetCount();						//队列消息个数

private:
	void ApplyResource();				//资源申请
	void ReleaseResource();				//资源释放

private:
	int			m_iQueueCapacity;		//队列容量
	int         m_iMsgCount;			//队列中消息个数
	int         m_iPutIndex;			//存放消息索引
	int         m_iGetIndex;			//获取消息索引
	int         m_iMsgDataCapacity;		//消息数据容量
	UD_MESSAGE* m_pUdMsg;				//消息缓冲区
	C_Mutex     m_Mutex;
};

