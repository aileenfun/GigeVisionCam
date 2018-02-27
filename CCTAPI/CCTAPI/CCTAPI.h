// CCTAPI.h : main header file for the CCTAPI DLL
//TODO:
//softTrig needs to be updated

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef COMPILE_API
	#define CCT_API extern "C" __declspec(dllexport) 
#else 
	#define CCT_API extern "C" __declspec(dllimport)
#endif

#include "resource.h"		// main symbols
#include "DataCapture.h"
#include "CyUsb.h"
//#include "clientProp.h"
#include "camProp.h"
#include "DeviceGVCP.h"
#include "VirtualDevice.h"
#include <vector>

class camInstance  //= virtualGEVCam
{

private:
	VirtualDevice _VtDev;
	CDataProcess * m_pDataProcess;
	CDataCapture * m_pDataCapture;
	DeviceGVCP m_DeviceGVCP;
	volatile bool b_opened;
	bool b_closed;
	wqueue<imgFrame*>queue;
	LPVOID* lpUser;
	LPMV_CALLBACK2 cb;
	int socketSrv;
	void*   _pThreadGvsp;
public :
	int b_initstatus;
public:
	camInstance(LPVOID*lp,LPMV_CALLBACK2 CallBackFunc):_VtDev("virtual-camera.xml","VirtualDevice.ini"),m_DeviceGVCP((Device*)&_VtDev)
	{

		_VtDev.Init();
		m_pDataProcess=new CDataProcess(queue,lp);
		m_pDataCapture=new CDataCapture(queue,m_pDataProcess);
		b_opened=false;
		b_closed=true;
		b_initstatus=0;
		cb=CallBackFunc;
		socketSrv=-1;
	}
	~camInstance()
	{
		delete m_pDataProcess;
		delete m_pDataCapture;
	}
	int initEth()
	{
		b_initstatus = 1;
		if(b_opened)
		{
			b_initstatus=-2;
		}
		else
		{
			//b_initstatus=m_pDataCapture->initUDP(&socketSrv)==0?TRUE:FALSE;
			if (m_DeviceGVCP.Init() != MV_OK)
			{
				b_initstatus = -2;
			}
			if(b_initstatus==0)
				b_initstatus=-1;
		}
		return b_initstatus;
	}
	int start(int height,int width)
	{
		if(b_initstatus<=0)
		{
			return -1;
		}
		_pThreadGvsp = MV_CreateThread(MV_NULL, DeviceGVCP::HandlingAckPacket, (&(this->m_DeviceGVCP)));
		if (_pThreadGvsp == MV_NULL)
		{
			return -1;
		}

		//m_pDataProcess->Open(height,width,m_pDataCapture,cb);
		//m_pDataCapture->Open(height,width);
		b_opened=true;
		b_closed=false;
		return 1;

	}
	int stop()
	{
		if(!b_opened)
			return -2;
		b_opened=false;
		m_pDataCapture->Close();
		//m_pDataCapture=NULL;
		m_pDataProcess->Close();
		//m_pDataProcess=NULL;
		//CyUsb_Destroy();
		return 0;
	}
	int isRunning()
	{
		bool temp=b_opened;
		bool temp1=!b_closed;
		return temp1;
	}
	int getFrameCnt()
	{
		return m_pDataCapture->getFrameCnt();
	}
	int getDataCnt()
	{
		return m_pDataCapture->getDataCnt();
	}
	int sendOrder(camPropStruct camprop,int s)
	{

		return m_pDataCapture->sendOrder(camprop,s);
	}
	int sendProp(clientPropStruct prop,int s)
	{
		m_pDataCapture->sendProp(prop,s);
		return 0;
	}
	int sendSoftTrig(int s)
	{
		//m_pDataCapture->sendSoftTrig(s);

		return 0;
	}
	std::map<std::string, MvCamCtrl::MV_CC_DEVICE_INFO*> getProp(clientPropStruct*prop)
	{
		
		m_DeviceGVCP.DiscoveryCmd();
		//setIP("192.168.1.5");
		Sleep(2000);
		return m_DeviceGVCP.devicelist;
	}
	int closeConnection()
	{
		m_DeviceGVCP.DeInit();
		m_pDataCapture->closeUDP(socketSrv);
		return 0;
	}
	unsigned __int32 ReadReg(unsigned __int32 addr)
	{
		return m_pDataCapture->ReadReg(addr);
	}
	unsigned __int32 WriteReg(unsigned __int32 addr,unsigned __int32 data)
	{
		return m_pDataCapture->WriteReg(addr,data);
	}
	int setIP(std::string ip)
	{
		return m_DeviceGVCP.forceIP(ip);
	}
};
CCT_API int addInstance(LPVOID *lpUser,LPMV_CALLBACK2 CallBackFunc);
CCT_API int initCCTAPI(int camNum);
CCT_API int startCap(int height,int width,int camNum=1);
CCT_API int stopCap(int camNum);
CCT_API int setMirrorType(DataProcessType mirrortype);
CCT_API int getFrameCnt(int camNum=1);
CCT_API int getDataCnt(int camNum=1);
CCT_API int sendProp(clientPropStruct prop,int camNum=1,int s=0);
CCT_API int sendOrder(camPropStruct camprop,int camNum=1,int s=0);
CCT_API int sendSoftTrig(int camNum=1,int s=0);
CCT_API std::map<std::string, MvCamCtrl::MV_CC_DEVICE_INFO*> getProp(int camNum,clientPropStruct*prop);
CCT_API int setIP(std::string ipaddr);
CCT_API int closeConnection(int &camNum);
CCT_API int csInit();
CCT_API int csStop();
CCT_API int csGetFrame(unsigned char * buff);
CCT_API unsigned __int32 WriteReg(unsigned __int32 addr, unsigned __int32 data,int camNum);
CCT_API unsigned __int32 ReadReg(unsigned __int32 addr,int camNum);