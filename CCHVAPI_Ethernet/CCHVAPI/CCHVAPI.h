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
#include "GigEDataCapture.h"
//#include "clientProp.h"
#include "GigEcamProp.h"
#include "DeviceGVCP.h"
#include "VirtualDevice.h"
#include <vector>

class GigEcamInstance  //= virtualGEVCam
{

private:
	VirtualDevice _VtDev;
	GigECDataProcess * m_pDataProcess;
	GigECDataCapture * m_pDataCapture;
	DeviceGVCP m_DeviceGVCP;
	volatile bool b_opened;
	bool b_closed;
	GigEwqueue<GigEimgFrame*>queue;
	LPVOID* lpUser;
	LPMV_CALLBACK2 cb;
	int socketSrv;
	void*   _pThreadGvsp;
	int width, height;
	GigEclientPropStruct *devprop;
public :
	int b_connected;
public:
	GigEcamInstance(LPVOID*lp,LPMV_CALLBACK2 CallBackFunc):m_DeviceGVCP()
	{
		m_pDataProcess=new GigECDataProcess(queue,lp);
		m_pDataCapture=new GigECDataCapture(queue,m_pDataProcess);
		b_opened=false;
		b_closed=true;
		b_connected=0;
		cb=CallBackFunc;
		socketSrv=-1;
		devprop = new GigEclientPropStruct();
		m_pDataProcess->lpcb = lp;
	}
	GigEcamInstance(LPVOID*lp,LPMV_CALLBACK2 CallBackFunc,CCHCamera *info):m_DeviceGVCP()
	{
		//m_DeviceGVCP.SetDeviceInfo(info);
		m_pDataProcess=new GigECDataProcess(queue,lp);
		m_pDataCapture=new GigECDataCapture(queue,m_pDataProcess);
		b_opened=false;
		b_closed=true;
		b_connected=0;
		cb=CallBackFunc;
		socketSrv=-1;
		devprop = new GigEclientPropStruct();
		m_pDataProcess->lpcb = lp;
	}
	GigEcamInstance() : m_DeviceGVCP()
	{
		//_VtDev.Init();
		b_opened = false;
		b_closed = true;
		b_connected = 0;
		cb = NULL;
		socketSrv = -1;
		devprop = new GigEclientPropStruct();
	}
	~GigEcamInstance()
	{
		/*if (m_pDataProcess != NULL)
		delete m_pDataProcess;
		
		if (m_pDataCapture != NULL)
		delete m_pDataCapture;*/
	}
	int initEth(CCHCamera *c,bool recv_thread=1)
	{
		if(b_connected ==1)//already opened
			return 1;
		if(b_opened)
		{
			return -2;
		}
		else
		{
			
			 if(m_DeviceGVCP.Init(c) != MV_OK)
			{
				return  -3;
			}
			 if (recv_thread)
			 {
				 _pThreadGvsp = MV_CreateThread(MV_NULL, DeviceGVCP::HandlingAckPacket, (&(this->m_DeviceGVCP)));
				 if (_pThreadGvsp == MV_NULL)
				 {
					return  -4;
				 }
			 }

				 devprop->pcIP = inet_addr(c->hostaddr.c_str());
				 uint32_t readdata;
				 int rst = ReadReg(0x0024, &readdata);//camip
				 if (rst<0)
					 if (m_DeviceGVCP.ReadRegDone() == -1)
					 {
						 return -5;
					 }
				 devprop->camIP = readdata;
				 sendProp(*devprop);
			 
		}
		b_connected = 1;
		return 1;
	}
	int updateCameraInfo()
	{
		//move connect to start.
		//connect only used to disable enthctrl(fix ip address), enable imgctrl, etc.
		//
		//
		if (b_connected <= 0)
		{
			return -1;
		}
		uint32_t readdata;
		
		int rst = ReadReg(0x33bb0010, &readdata);
		if (rst<0)//cam num
		{
			return -2;
		}

		devprop->camCnt = readdata;
		rst = ReadReg(0x33bb0014, &readdata);
		if (rst<0)//resolution
		{
			return -3;
		}

		uint32_t temp = readdata;
		devprop->width = (readdata >> 16);
		devprop->height = (temp & 0xFFFF);
		sendProp(*devprop);
		return 1;
	}
	int start()
	{

		//get height and width from reg
		//must connected
		//must know ip address from GVCP, read reg
		//send prop
		if(b_opened)
		{
			return 0;
		}
		updateCameraInfo();
		if (devprop == NULL)
		{
			return -1;
		}
		if(m_pDataCapture->initUDP(&socketSrv))
		{
			m_pDataProcess->Open(devprop->height, devprop->width, m_pDataCapture, cb);
			m_pDataCapture->Open(devprop->height, devprop->width);
			b_opened = true;
			b_closed = false;
		}
		else
		{
			return -5;
		}
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
		return 0;
	}
	int isRunning()
	{
		bool temp=b_opened;
		return temp;
	}
	int getFrameCnt()
	{
		return m_pDataCapture->getFrameCnt();
	}
	int getDataCnt()
	{
		return m_pDataCapture->getDataCnt();
	}
	long getErrPackCnt()
	{
		return m_pDataCapture->haveerror;
	}
	int sendOrder(GigEcamPropStruct camprop,int s)
	{

		return m_pDataCapture->sendOrder(camprop,s);
	}
	int sendProp(GigEclientPropStruct prop)
	{
		m_pDataCapture->sendProp(prop);
		return 0;
	}
	int setTrigMode(int s)
	{
	
		m_DeviceGVCP.WriteReg(0x33bb0004, s);
		return m_DeviceGVCP.WriteRegDone();
	}

	int closeConnection()
	{
		m_DeviceGVCP.DeInit();
		m_pDataCapture->closeUDP();
		return 0;
	}
	int ReadReg(unsigned __int32 addr,uint32_t *data)
	{
		int rst= m_DeviceGVCP.ReadReg(addr, data);
		return rst;
	}
	unsigned __int32 WriteReg(unsigned __int32 addr,unsigned __int32 data)
	{

		return m_DeviceGVCP.WriteReg(addr, data);;
	}
	int forceIP(CCHCamera *devinfo)
	{

		m_DeviceGVCP.SetDeviceInfo(devinfo);
		return m_DeviceGVCP.ForceIP(devinfo);
	}
	int setIP(CCHCamera *devinfo)
	{
		//persistence ip ram and ee
		//subnet
		//gateway
		if (m_DeviceGVCP.WriteReg(0x064C, devinfo->CamInfo->stGigEInfo.nCurrentIp) < 0)//persistence ip
			return -1;
			Sleep(100);
			if (m_DeviceGVCP.WriteReg(0x065C, devinfo->CamInfo->stGigEInfo.nCurrentSubNetMask) < 0)//subnet
				return -2;
			Sleep(100);
			if (m_DeviceGVCP.WriteReg(0x066C, devinfo->CamInfo->stGigEInfo.nDefultGateWay) < 0)//gateway
				return -3;
			Sleep(100);
			return 1;
	
	}
	
	map_camera searchCamera(map_camera *camlist)
	{
		//m_DeviceGVCP.Discovery();
		CCHCamera*devinfo = new CCHCamera();
		devinfo->hostaddr = "0.0.0.0";
		initEth(devinfo, 0);//should not be used, just use getinterface. 
		m_DeviceGVCP.getInterface();
		*camlist = m_DeviceGVCP.cameralist;
		delete devinfo;
		return m_DeviceGVCP.cameralist;
	}
	int sendSoftTrig()
	{
		
		return m_DeviceGVCP.WriteReg(0x33bb0008, 1);;
	}
	int setGain(uint32_t value,int isauto)
	{
		m_DeviceGVCP.WriteReg(0x33bb0034,isauto);
		if(!isauto)
		{
			return m_DeviceGVCP.WriteReg(0x33bb003C,value);
		}
		
	}
	int setExpo(uint32_t value,int isauto)
	{
		m_DeviceGVCP.WriteReg(0x33bb0034,isauto);
		if(!isauto)
		{
			return m_DeviceGVCP.WriteReg(0x33bb0038,value);
		}
		
	}
	int setFreq(uint32_t value)
	{
		return m_DeviceGVCP.WriteReg(0x33bb000C,value);

	}
	int setWB(uint32_t rvalue,uint32_t g1value,uint32_t g2value,uint32_t bvalue)
	{
		if(m_DeviceGVCP.WriteReg(0x33bb0040,g1value) \
			&&m_DeviceGVCP.WriteReg(0x33bb0044,bvalue) \
			&&m_DeviceGVCP.WriteReg(0x33bb0048,rvalue)\
		&&m_DeviceGVCP.WriteReg(0x33bb004C,g2value))
		{
			return 1;
		}
		
	}
	int setMirrorType(GigEDataProcessType mirrortype)
	{
		int temp = 0;
		switch (mirrortype)
		{
		case 0:
			temp = 0;
			break;
		case 1:
			temp = 0x4000;
			break;
		case 2:
			temp = 0x8000;
			break;
		case 3:
			temp = 0xc000;
			break;
		default:
			temp = 0;
			break;
		}
		return m_DeviceGVCP.WriteReg(0x33bb0050, temp);
		 
	}
	int setCamSize(int camsize)
	{
		m_pDataProcess->setCamSize(camsize);
		return (m_DeviceGVCP.WriteReg(0x33bb0010,camsize));
	}
	int setROI(int xstart, int xend, int ystart, int yend, int enable)
	{
		if (!enable)
		{
			xstart = 0;
			xend = 1279;
			ystart = 0;
			yend = 959;
		}
		if (m_DeviceGVCP.WriteReg(0x33bb0018, ystart) &&
			m_DeviceGVCP.WriteReg(0x33bb001C, xstart) &&
			m_DeviceGVCP.WriteReg(0x33bb0020, yend) &&
			m_DeviceGVCP.WriteReg(0x33bb0024, xend) &&
			m_DeviceGVCP.WriteReg(0x33bb0028, enable))
		{
			return 1;
		}
		return -1;

	}
	int setBinning(int enable)
	{
		return m_DeviceGVCP.WriteReg(0x33bb002C, enable);
	}
	int setSkip(int enable)
	{
		return m_DeviceGVCP.WriteReg(0x33bb0030, enable);
	}
};
CCT_API int GigEaddInstance(LPVOID *lpUser,LPMV_CALLBACK2 CallBackFunc,CCHCamera *info);
//CCT_API int initCCTAPI(int camNum);
CCT_API int GigEstartCap(int camNum=1);
CCT_API int GigEstopCap(int camNum=1);
CCT_API int GigEsetMirrorType(GigEDataProcessType mirrortype,int camNum=1);
CCT_API int GigEgetFrameCnt(int camNum=1);
CCT_API int GigEgetDataCnt(int camNum=1);
CCT_API long GigEgetErrPackCnt(int camNum=1);
CCT_API int GigEsetTrigMode(int s=0,int camNum=1);
CCT_API int GigEsendSoftTrig(int camNum=1);
CCT_API int GigEsetIP(CCHCamera *devinfo,int camNum=1);
CCT_API int GigEforceIP(CCHCamera *devinfo);
CCT_API int GigEcloseConnection(int &camNum);
CCT_API	int GigEsearchCamera(map_camera * info);
CCT_API unsigned __int32 GigEWriteReg(unsigned __int32 addr, unsigned __int32 data,int camNum=1);
CCT_API unsigned __int32 GigEReadReg(unsigned __int32 addr,uint32_t *data,int camNum=1);
CCT_API int GigEsetGain(uint32_t value,int isauto,int camNum=1);
CCT_API int GigEsetExpo(uint32_t value,int isauto,int camNum=1);
CCT_API int GigEsetFreq(uint32_t value,int camNum=1);
CCT_API int GigEsetWB(uint32_t rvalue,uint32_t gvalue,uint32_t g2value,uint32_t bvalue,int camNum=1);
CCT_API int GigEsetCamSize(int camsize,int camNum=1);
CCT_API int GigEsetROI(int xstart, int xend, int ystart, int yend, int enable, int camNum = 1);
CCT_API int GigEsetBinning(int enable,int camNum=1);
CCT_API int GigEsetSkip(int enable,int camNum=1);

CCT_API int csInit();
CCT_API int csStart();
CCT_API int csStop();
CCT_API int csGetFrame(unsigned char * buff);
//#endif 