#pragma once
#include "GigEMutex.h"

typedef struct UD_MESSAGE_S				//�û��Զ�����Ϣ
{
	int iType;							//��Ϣ����
	int iDataSize;						//��Ϣ�����ֽ���
	char* pData;						//��Ϣ���ݵ�ַ
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
	int Open(int iQCap,int iDataCap);	//����:��������;��Ϣ��������
	int Close();
	int PutMessage(UD_MESSAGE* pMsg);	//�����Ϣ
	int GetMessage(UD_MESSAGE* pMsg);	//��ȡ��Ϣ
	int Reset();						//���ö���
	int GetCount();						//������Ϣ����

private:
	void ApplyResource();				//��Դ����
	void ReleaseResource();				//��Դ�ͷ�

private:
	int			m_iQueueCapacity;		//��������
	int         m_iMsgCount;			//��������Ϣ����
	int         m_iPutIndex;			//�����Ϣ����
	int         m_iGetIndex;			//��ȡ��Ϣ����
	int         m_iMsgDataCapacity;		//��Ϣ��������
	UD_MESSAGE* m_pUdMsg;				//��Ϣ������
	GigeC_Mutex     m_Mutex;
};

