

#ifndef _DEVICEGVCP_H
#define _DEVICEGVCP_H

#if defined __WIN32 || defined __WIN64 || defined WIN32 || defined WIN64
#define WINDOWS
#endif

#include <string>
#include <iostream>
#include "Device.h"
#include "Inc/Socket/Socket.h"
//#include "Inc/color_print/color_print.h"
#include "Inc/MVErrorDefine.h"
#include "Inc/MVGigEVisionGVCPDefine.h"
#include "Inc/MVGigEVisionDeviceDefine.h"
#include "Inc/MVGiGEVisionGVCPRegisterDefine.h"
#include "Inc/MV_CrossPlatform/MV_CrossPlatform.h"

#include <map>
using namespace MvCamCtrl;
typedef std::map<std::string, MvCamCtrl::MV_CC_DEVICE_INFO*> map_deviceInfo;

#define RETRYTIME 10

class CCHCamera
{
public:
	MV_CC_DEVICE_INFO *CamInfo;
	//MVComponent::UDP udpSocket;
	//MVComponent::Address hostAddr;
	std::string hostaddr;
	std::string camerakey;
	CCHCamera()
	{
		CamInfo = new MV_CC_DEVICE_INFO();
	}

	CCHCamera(const CCHCamera& b):hostaddr(b.hostaddr), camerakey(b.camerakey)
	{
		if (nullptr != b.CamInfo)
		{
			int count = 3;
			do
			{
				CamInfo = new(std::nothrow) MV_CC_DEVICE_INFO();
				count--;
			} while (count || (nullptr == CamInfo));

			if (nullptr != CamInfo)
			{
				memcpy((void*)CamInfo, (void*)b.CamInfo, sizeof(MV_CC_DEVICE_INFO));
			}
			else
			{
				CamInfo = nullptr;
			}
		}
		else
		{
			CamInfo = nullptr;
		}
	}

	CCHCamera& operator=(const CCHCamera& b)
	{
		if (&b == this) { return *this; }

		hostaddr = b.hostaddr;
		camerakey = b.camerakey;
		if (nullptr != b.CamInfo)
		{
			int count = 3;
			do
			{
				CamInfo = new(std::nothrow) MV_CC_DEVICE_INFO();
				count--;
			} while (count || (nullptr == CamInfo));

			if (nullptr != CamInfo)
			{
				memcpy((void*)CamInfo, (void*)b.CamInfo, sizeof(MV_CC_DEVICE_INFO));
			}
			else
			{
				CamInfo = nullptr;
			}
		}
		else
		{
			CamInfo = nullptr;
		}

		if (CamInfo != nullptr) { delete CamInfo; CamInfo = nullptr; }

		return *this;
	}
	~CCHCamera()
	{
		delete CamInfo;
	}
};
typedef std::map<std::string, CCHCamera*> map_camera;
class DeviceGVCP
{
  public:
    DeviceGVCP();
    virtual ~DeviceGVCP();

    // No copying allowed
private:
	DeviceGVCP(const DeviceGVCP&) {};
	DeviceGVCP& operator = (const DeviceGVCP&) { return *this; };
public:
	int Init(CCHCamera *c);
    int DeInit();
	int SetDeviceInfo(CCHCamera *info);
    static ThreadReturnType MV_STDCALL HandlingAckPacket(void* Arg);
	int Discovery();
	int DiscoveryCmd();
	int DiscoveryCmdSkt(MVComponent::UDP udpskt);
	void*  DiscoveryAck(char * Arg);
	int DiscoveryDone();
	MvCamCtrl::MV_CC_DEVICE_INFO devinfo;
	map_camera cameralist;
	int ForceIP(MVComponent::UDP udpskt);

	int ForceIP(CCHCamera *info);

	int ForceIPDone();
	
	//success:1
	//socket error -1
	int ReadReg(unsigned int addr,uint32_t *value);
	unsigned int ReadRegCmd(unsigned int addr);
	//success: return ReadRegRst.nRegData
	//time out:-1
	int ReadRegDone();

	//success:1
	//socket error:-1
	int WriteReg(unsigned int addr, unsigned int data);
	int WriteRegCmd(unsigned int addr, unsigned int data);
	int WriteRegAck();
	int WriteRegDone();
	int ReadMemAck();
	int WriteMemAck();
	int getInterface();
	CStringA STR_T2A(LPCTSTR in);
	CString  STR_A2T(LPCSTR in);
	READREG_CMD_MSG_ACK ReadRegRst;
	bool f_threadStop;
  private:
	  int ReceiveMsg(MVComponent::UDP socket, unsigned int& nLen);
  private:
	int ForceIPAck(char* Arg);
	int decodePacket(MVComponent::UDP s);
	READREG_CMD_MSG_ACK ReadRegAck(char* Arg);

    MVComponent::UDP     _UdpSkt;
    MVComponent::Address _From;
	MVComponent::Address _devAddr;
    char                 _cRecvData[MV_GVCP_MAX_MSG_LEN];
    unsigned int         _nLastAckId;
	unsigned int		 _nReqId;
    Device*              _pDevice;
	int					f_WriteRegDone;
	int					f_ForceIPDone;
	int					f_ReadRegDone;
	int					f_DiscoveryDone;
	MVComponent::UDP currentSocket;
	
};


#endif /* _DEVICEGVCP_H */
