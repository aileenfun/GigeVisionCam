#pragma once

#include <fstream>
#include <iostream>
#include <WinSock2.h>
#include "GigEDataProcess.h"
#include "GigEclientProp.h"
#include "GigEcamProp.h"
#pragma comment(lib,"WS2_32.lib")
#define PACKSIZE 8192
#define PAYLOADSIZE (PACKSIZE-16)
class GigECDataCapture
{
public:
	GigECDataCapture(GigEwqueue<GigEimgFrame*>&queue, GigECDataProcess *dp);
	virtual ~GigECDataCapture(void);
	GigEwqueue<GigEimgFrame*>&m_queue;
	GigEwqueue<GigEudp_buffer*>udp_queue;
	
public:
	int Open(int height,int width);//
	int Close();
	int Input(const LPVOID lpData,const DWORD dwSize);
	int startThread();
	int initUDP(int *socket);
	int closeUDP();
	unsigned long getFrameCnt();
	unsigned long getDataCnt();
	int sendProp(GigEclientPropStruct prop);
	int sendOrder(GigEcamPropStruct camprop,int s=0);
	int sendSoftTrig(int s=0);
	unsigned long dataCnt;
	long haveerror;
	bool f_errorpack;
	unsigned int frameCnt;
	long getData(byte * buffer,long startpos,long len,long packsize=1024);
	void get_udp_data();
	static unsigned int __stdcall get_udp_data_wrapper(void *handle);
	int getProp(GigEclientPropStruct* prop);
	unsigned __int32 WriteReg(unsigned __int32 addr, unsigned __int32 data);
private:

	GigECDataProcess *m_pDataProcess;
	int			m_iCount;		//数据计数器
	int			m_iRowIndex;	//行索引
	bool        m_bFindDbFive;	//标记是否找到55
	byte*		m_pInData;		//接收数据缓冲
	byte*	m_pOutData;		//输出数据缓冲
	GigEImageMetaStruct *imageMeta;
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
	GigEimgFrame* thisImgFrame;
	CFile* fromfile;
	struct sockaddr_in client_addr1;
	std::ofstream savefile;
	std::ofstream savefile2;
	std::ofstream savefile3;
	GigEclientProp *this_clientProp;
	GigEcamProp *this_camprop;
	int softtrigmode;
	unsigned int TOTALPACK;
	unsigned int residue;
};
