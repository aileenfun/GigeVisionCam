// -*- coding: gb2312-dos -*-
#ifndef _MVGIGEVISIONDEFINE_H
#define _MVGIGEVISIONDEFINE_H

#include "MVPixelType.h"

namespace MvCamCtrl
{
// �豸���Ͷ���
#define MV_UNKNOW_DEVICE        0x00000000          // δ֪�豸���ͣ���������
#define MV_GIGE_DEVICE          0x00000001          // GigE�豸
#define MV_1394_DEVICE          0x00000002          // 1394-a/b �豸
#define MV_USB_DEVICE           0x00000004          // USB3.0 �豸
#define MV_CAMERALINK_DEVICE    0x00000008          // CameraLink�豸

typedef struct _MV_GIGE_DEVICE_INFO_
{
    unsigned int        nIpCfgOption;
    unsigned int        nIpCfgCurrent;                   // IP configuration:bit31-static bit30-dhcp bit29-lla
    unsigned int        nCurrentIp;                      // curtent ip
    unsigned int        nCurrentSubNetMask;              // curtent subnet mask
    unsigned int        nDefultGateWay;                  // current gateway
    unsigned char       chManufacturerName[32];
    unsigned char       chModelName[32];
    unsigned char       chDeviceVersion[32];
    unsigned char       chManufacturerSpecificInfo[48];
	unsigned char       chSerialNumber[16];
    unsigned char       chUserDefinedName[16];
    unsigned int        nNetExport;        // ����IP��ַ
} MV_GIGE_DEVICE_INFO;

// �豸��Ϣ
typedef struct _MV_CC_DEVICE_INFO_
{
    // common info
    unsigned short      nMajorVer;
    unsigned short      nMinorVer;
    unsigned int        nDeviceMode;
    unsigned int        nMacAddrHigh;               // MAC ��ַ
    unsigned int        nMacAddrLow;

    unsigned int        nTLayerType;                // �豸�����Э�����ͣ�e.g. MV_GIGE_DEVICE

    union
    {
        MV_GIGE_DEVICE_INFO stGigEInfo;
        // more ...
    };

} MV_CC_DEVICE_INFO;

// ���紫��������Ϣ
typedef struct _MV_NETTRANS_INFO_
{
    long long       nReviceDataSize;    // �ѽ������ݴ�С  [ͳ��StartGrabbing��StopGrabbing֮���������]
    int             nThrowFrameCount;   // ��֡����
    int             nReserved;

}MV_NETTRANS_INFO;


// ���֧�ֵĴ����ʵ������
#define MV_MAX_TLS_NUM          8
// ���֧�ֵ��豸����
#define MV_MAX_DEVICE_NUM      256

// �豸��Ϣ�б�
typedef struct _MV_CC_DEVICE_INFO_LIST_
{
    unsigned int            nDeviceNum;                         // �����豸����
    MV_CC_DEVICE_INFO*      pDeviceInfo[MV_MAX_DEVICE_NUM];     // ֧�����256���豸

}MV_CC_DEVICE_INFO_LIST;


// ���֡����Ϣ
typedef struct _MV_FRAME_OUT_INFO_
{
    unsigned short      nWidth;             // ͼ���
    unsigned short      nHeight;            // ͼ���
    MvGvspPixelType     enPixelType;        // ���ظ�ʽ

    /*�����ֶ��ݲ�֧��*/
    unsigned int        nFrameNum;          // ֡��
    unsigned int        nDevTimeStampHigh;  // ʱ�����32λ
    unsigned int        nDevTimeStampLow;   // ʱ�����32λ
    long long           nHostTimeStamp;     // �������ɵ�ʱ���

    unsigned int        nReserved[4];       // ����
}MV_FRAME_OUT_INFO;


// �ɼ�ģʽ
typedef enum _MV_CAM_ACQUISITION_MODE_
{
    MV_ACQ_MODE_SINGLE      = 0,            // ��֡ģʽ
    MV_ACQ_MODE_MUTLI       = 1,            // ��֡ģʽ
    MV_ACQ_MODE_CONTINUOUS  = 2,            // �����ɼ�ģʽ

}MV_CAM_ACQUISITION_MODE;

// GigEVision IP Configuration
#define MV_IP_CFG_STATIC        0x01000000
#define MV_IP_CFG_DHCP          0x02000000
#define MV_IP_CFG_LLA           0x04000000


// ��Ϣ����
#define MV_MATCH_TYPE_NET_DETECT        0x00000001      // ���������Ͷ�����Ϣ

// ȫƥ���һ����Ϣ�ṹ��
typedef struct _MV_ALL_MATCH_INFO_
{
    unsigned int    nType;          // ��Ҫ�������Ϣ���ͣ�e.g. MV_MATCH_TYPE_NET_DETECT
    void*           pInfo;          // �������Ϣ���棬�ɵ����߷���
    unsigned int    nInfoSize;      // ��Ϣ����Ĵ�С

}MV_ALL_MATCH_INFO;

//  ���������Ͷ�����Ϣ�����ṹ�壬��Ӧ����Ϊ MV_MATCH_TYPE_NET_DETECT
typedef struct _MV_MATCH_INFO_NET_DETECT_
{
    long long       nReviceDataSize;    // �ѽ������ݴ�С  [ͳ��StartGrabbing��StopGrabbing֮���������]
    long long       nLostPacketCount;   // ��ʧ�İ�����
    unsigned int    nLostFrameCount;    // ��֡����
    unsigned int    nReserved;          // ����
}MV_MATCH_INFO_NET_DETECT;

//  �쳣��Ϣ����
#define MV_EXCEPTION_DEV_DISCONNECT     0x00008001      // �豸�Ͽ�����


// ���豸��Ȩ��ģʽ
#define MV_ExclusivePrivilege                                      1
#define MV_ExclusivePrivilegeWithSwitchoverKey                     2
#define MV_ControlPrivilege                                        3
#define MV_ControlPrivilegeWithSwitchoverKey                       4
#define MV_ControlSwitchoverEnablePrivilege                        5
#define MV_ControlSwitchoverEnablePrivilegeWithSwitchoverKey       6
#define MV_MonitorPrivilege                                        7
} // MvCamCtrl

#endif /* _MVGIGEVISIONDEFINE_H */
