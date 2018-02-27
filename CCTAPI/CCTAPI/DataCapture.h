#pragma once

#include <fstream>
#include <iostream>
#include <WinSock2.h>
#include "DataProcess.h"
#include "clientProp.h"
#include "camProp.h"
#pragma comment(lib,"WS2_32.lib")
#define PACKSIZE 8192
#define PAYLOADSIZE 8176
class CDataCapture
{
public:
	CDataCapture(wqueue<imgFrame*>&queue,CDataProcess *dp);
	virtual ~CDataCapture(void);
	wqueue<imgFrame*>&m_queue;
	wqueue<udp_buffer*>udp_queue;
	
public:
	int Open(int height,int width);//
	int Close();
	int Input(const LPVOID lpData,const DWORD dwSize);
	int startThread();
	int initUDP(int *socket);
	int closeUDP();
	unsigned long getFrameCnt();
	unsigned long getDataCnt();
	int sendProp(clientPropStruct prop,int s=0);
	int sendOrder(camPropStruct camprop,int s=0);
	int sendSoftTrig(int s=0);
	unsigned long dataCnt;
	unsigned int frameCnt;
	long getData(byte * buffer,long startpos,long len,long packsize=1024);
	void closeUDP(int socket);
	void get_udp_data();
	static unsigned int __stdcall get_udp_data_wrapper(void *handle);
	int getProp(clientPropStruct* prop);
	unsigned __int32 ReadReg(unsigned __int32 addr);
	unsigned __int32 WriteReg(unsigned __int32 addr, unsigned __int32 data);
private:

	CDataProcess *m_pDataProcess;
	int			m_iCount;		//���ݼ�����
	int			m_iRowIndex;	//������
	bool        m_bFindDbFive;	//����Ƿ��ҵ�55
	byte*		m_pInData;		//�������ݻ���
	byte*	m_pOutData;		//������ݻ���
	ImageMetaStruct *imageMeta;
	char* m_pReadBuff;
	long lRet;
	static unsigned int __stdcall ThreadProcess( void* handle );
	int ThreadProcessFunction();
	HANDLE m_hThread;
	volatile BOOL m_bCapture;
	int g_width_L;
	int g_width;
	int g_height;
	//UDP stufffs
	WSADATA wsd;
    SOCKET  s;
	int socketSrv;
	SOCKADDR_IN addrClient;
	SOCKADDR_IN addrSrv;
	int         len;
	imgFrame* thisImgFrame;
	CFile* fromfile;
	struct sockaddr_in client_addr1;
	std::ofstream savefile;
	std::ofstream savefile2;
	clientProp *this_clientProp;
	camProp *this_camprop;
	int softtrigmode;

};
