// -*- coding: gb2312-dos -*-
/** @file           DeviceGVCP.h
 *  @copyright      HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief          �豸�� GVCP Э��ʵ��
 *
 *  @author         zhenglinjun
 *  @date           2015-09-21
 *
 *  @note
 *
 *  @warning
 **/

#ifndef _DEVICEGVCP_H
#define _DEVICEGVCP_H

#if defined __WIN32 || defined __WIN64 || defined WIN32 || defined WIN64
#define WINDOWS
#endif

#include <string>
#include <iostream>

#include "Device.h"
#include "Inc/Socket/Socket.h"
#include "Inc/color_print/color_print.h"

#include "Inc/MVErrorDefine.h"
#include "Inc/MVGigEVisionGVCPDefine.h"
#include "Inc/MVGigEVisionDeviceDefine.h"
#include "Inc/MVGiGEVisionGVCPRegisterDefine.h"
#include "Inc/MV_CrossPlatform/MV_CrossPlatform.h"

#include <map>
using namespace MvCamCtrl;
//extern struct MV_CC_DEVICE_INFO;
class DeviceGVCP
{
  public:
    DeviceGVCP(Device* pDev);
    virtual ~DeviceGVCP();

    // No copying allowed
    DeviceGVCP(const DeviceGVCP&);
    void operator=(const DeviceGVCP&);

    int Init(int Port = MV_GVCP_PUBLIC_PORT);
    int DeInit();

    static ThreadReturnType MV_STDCALL HandlingAckPacket(void* Arg);
	int DiscoveryCmd();
	void*  DiscoveryAck(char * Arg);
	//MvCamCtrl::MV_CC_DEVICE_INFO devinfo;
	std::map<std::string, MvCamCtrl::MV_CC_DEVICE_INFO*> devicelist;
	int forceIP(std::string ip);
  private:
    int ReceiveMsg(unsigned int& nLen);

	
    int ReadRegAck();
    int WriteRegAck();
    int ReadMemAck();
    int WriteMemAck();

  private:
    MVComponent::UDP     _UdpSkt;
    MVComponent::Address _From;
    char                 _cRecvData[MV_GVCP_MAX_MSG_LEN];
    unsigned int         _nLastAckId;
	unsigned int		 _nReqId;
    Device*              _pDevice;
    cp_state_ptr         _cp;
};


#endif /* _DEVICEGVCP_H */
