// -*- coding: gb2312-dos -*-
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "DeviceGVCP.h"
#include <atlstr.h>  
#ifdef WINDOWS
#include <mstcpip.h>

#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif

using namespace std;
using namespace MVComponent;
using namespace MvCamCtrl;

DeviceGVCP::DeviceGVCP()
	: _nLastAckId(0)
{
	ReadRegRst.nRegData = -1;
	f_threadStop = false;
	f_threadStop = false;
	f_WriteRegDone = 1;
	f_ForceIPDone = 1;
	f_ReadRegDone = 1;
	f_DiscoveryDone = 1;

}

DeviceGVCP::~DeviceGVCP()
{
	f_threadStop = true;
}
/*
int DeviceGVCP::Init(int Port)
{
int nRet = MV_OK;
ReadRegRst.nRegData=-1;
f_threadStop = false;
// Init GVCP UDP socket
try
{
_UdpSkt.Open();
_UdpSkt.BindOnPort(Port);
_UdpSkt.SetBroadcast(true);
_UdpSkt.SetDontfragment(true);
}
catch (SocketException& SktEx)
{
cout << SktEx << endl;
nRet = MV_E_NETER;
return nRet;
}

#ifdef WINDOWS
// Windows UDP socket recvfrom return 10054
// ��UDP Socket��ĳ�η��ͺ��յ�һ�����ɵ����ICMP��ʱ�������������һ�������з��أ�
// ����������׽�������һ�εĽ����з�����SOCKET_ERROR��������10045��
bool bNewBehavior = false;
DWORD dwBytesReturned = 0;
::WSAIoctl(_UdpSkt.GetSocketId(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof bNewBehavior, NULL, 0, &dwBytesReturned, NULL, NULL);
#endif
getInterface();
return nRet;
}*/
int DeviceGVCP::Init(CCHCamera *c)
{
	int nRet = MV_OK;

	try
	{
		_UdpSkt.Close();
		_UdpSkt.Open();
		_UdpSkt.BindAddr(Address(c->hostaddr, 0));
		_UdpSkt.SetBroadcast(true);
		_UdpSkt.SetDontfragment(true);
		devinfo = *(c->CamInfo);
	}
	catch (SocketException& SktEx)
	{
		cout << SktEx << endl;
		nRet = MV_E_NETER;
		return nRet;
	}

#ifdef WINDOWS
	// Windows UDP socket recvfrom return 10054
	// ��UDP Socket��ĳ�η��ͺ��յ�һ�����ɵ����ICMP��ʱ�������������һ�������з��أ�
	// ����������׽�������һ�εĽ����з�����SOCKET_ERROR��������10045��
	bool bNewBehavior = false;
	DWORD dwBytesReturned = 0;
	::WSAIoctl(_UdpSkt.GetSocketId(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof bNewBehavior, NULL, 0, &dwBytesReturned, NULL, NULL);
#endif
	return nRet;
}
int DeviceGVCP::DeInit()
{
	int nRet = MV_OK;
	f_threadStop = true;
	try
	{
		_UdpSkt.Close();
	}
	catch (SocketException& SktEx)
	{
		nRet = MV_E_NETER;
		return nRet;
	}

	return nRet;
}
int DeviceGVCP::SetDeviceInfo(CCHCamera *info)
{
	devinfo = *(info->CamInfo);

	//_UdpSkt=info.udpSocket;
	//_From=info.hostAddr;
	return 1;
}
int DeviceGVCP::ReceiveMsg(MVComponent::UDP socket, unsigned int& nLen)
{
	int nRet = MV_OK;
	nLen = MV_GVCP_MAX_MSG_LEN;

	try
	{
		nLen = socket.ReceiveTimeout(500, _From, _cRecvData, nLen);

	}
	catch (SocketException& SktEx)
	{
		nRet = MV_E_NETER;
		//nRet = MV_E_NODATA;
		return nRet;
	}

	return nRet;
}
int DeviceGVCP::decodePacket(MVComponent::UDP s)
{
	for (int r = 0;r < RETRYTIME;r++)
	{
		unsigned int nLen = MV_GVCP_MAX_MSG_LEN;
		int nRet = 0;
		if ((nRet = ReceiveMsg(s, nLen)) != MV_OK)
		{
#ifdef MV_WINDOWS
			return nRet;
#endif
		}

		if (nLen > 0)
		{
			ACK_MSG_HEADER* pCmdHdr = (ACK_MSG_HEADER*)(_cRecvData);
			unsigned int code = ntohs(pCmdHdr->nAckMsgValue);
			switch (code)
			{
			case MV_GEV_DISCOVERY_ACK:
			{
				if (pCmdHdr->nStatus != MV_OK)
				{
					//error;
				}
				MV_CC_DEVICE_INFO * ptemp = new MV_CC_DEVICE_INFO();
				ptemp = (MV_CC_DEVICE_INFO *)(DiscoveryAck(_cRecvData));
				char *chartemp = new char[16];
				memcpy(chartemp, ptemp->stGigEInfo.chSerialNumber, 16);
				std::string sntemp(chartemp);
				/*	if (pDeviceGvcp->devicelist.find(sntemp) != pDeviceGvcp->devicelist.end())
				{
					pDeviceGvcp->devicelist.erase(sntemp);
				}
				(pDeviceGvcp->devicelist).insert(std::pair<std::string, MV_CC_DEVICE_INFO*>(sntemp, ptemp));*/

				CCHCamera *pcamera = new CCHCamera();
				//pcamera.CamInfo = *ptemp;
				memcpy(pcamera->CamInfo, ptemp, sizeof(MV_CC_DEVICE_INFO));
				//pcamera.udpSocket = pDeviceGvcp->currentSocket;
				pcamera->hostaddr = currentSocket.GetAddressIp();
				pcamera->camerakey = sntemp;
				if (cameralist.find(sntemp) != cameralist.end())
				{
					cameralist.erase(sntemp);
				}
				(cameralist).insert(std::pair<std::string, CCHCamera*>(sntemp, pcamera));
				break;
			}
			case MV_GEV_FORCEIP_ACK:
			{
				if (pCmdHdr->nStatus != MV_OK)
				{
					//error;
				}
				MV_CC_DEVICE_INFO * ptemp;
				ptemp = (MV_CC_DEVICE_INFO *)(ForceIPAck(_cRecvData));
			}
			case MV_GEV_READREG_ACK:
			{


				ReadRegRst = ReadRegAck(_cRecvData);
				break;
			}
			case MV_GEV_WRITEREG_ACK:
			{
				WriteRegAck();
				break;
			}
			case MV_GEV_READMEM_ACK:
			{
				//pDeviceGvcp->ReadMemAck();
				break;
			}
			case MV_GEV_WRITEMEM_ACK:
			{
				//pDeviceGvcp->WriteMemAck();
				break;
			}
			default:
			{
				break;
			}
			}
		}
		Sleep(1);
	}
}
ThreadReturnType MV_STDCALL DeviceGVCP::HandlingAckPacket(void* Arg)
{
	int nRet = MV_OK;

	DeviceGVCP* pDeviceGvcp = (DeviceGVCP*)Arg;
	unsigned int nLen;
	stringstream ss;

	do
	{
		// Start dealing CMD .......
		nLen = MV_GVCP_MAX_MSG_LEN;
		if ((nRet = pDeviceGvcp->ReceiveMsg(pDeviceGvcp->_UdpSkt, nLen)) != MV_OK)
		{
#ifdef MV_WINDOWS
			return nRet;
#endif
		}

		if (nLen > 0)
		{
			ACK_MSG_HEADER* pCmdHdr = (ACK_MSG_HEADER*)(pDeviceGvcp->_cRecvData);
			unsigned int code = ntohs(pCmdHdr->nAckMsgValue);
			switch (code)
			{
			case MV_GEV_DISCOVERY_ACK:
			{
				if (pCmdHdr->nStatus != MV_OK)
				{
					//error;
				}
				MV_CC_DEVICE_INFO * ptemp;
				ptemp = (MV_CC_DEVICE_INFO *)(pDeviceGvcp->DiscoveryAck(pDeviceGvcp->_cRecvData));
				char *chartemp = new char[16];
				memcpy(chartemp, ptemp->stGigEInfo.chSerialNumber, 16);
				std::string sntemp(chartemp);
				if (pDeviceGvcp->cameralist.find(sntemp) != pDeviceGvcp->cameralist.end())
				{
					pDeviceGvcp->cameralist.erase(sntemp);
				}
				//(pDeviceGvcp->cameralist).insert(std::pair<std::string, CCHCamera*>(sntemp, &pcamera));
				break;
			}
			case MV_GEV_FORCEIP_ACK:
			{
				if (pCmdHdr->nStatus != MV_OK)
				{
					//error;
				}
				MV_CC_DEVICE_INFO * ptemp;
				ptemp = (MV_CC_DEVICE_INFO *)(pDeviceGvcp->ForceIPAck(pDeviceGvcp->_cRecvData));
			}
			case MV_GEV_READREG_ACK:
			{
				ss << "Read register command" << endl;
				cout << ss.rdbuf();
				pDeviceGvcp->ReadRegRst = pDeviceGvcp->ReadRegAck(pDeviceGvcp->_cRecvData);
				ss << "[HandlingControlPacket] --> " << pDeviceGvcp->_From
					<< "  Read register acknowledge" << endl;
				cout << ss.rdbuf();
				break;
			}
			case MV_GEV_WRITEREG_ACK:
			{
				ss << "Write register command" << endl;
				cout << ss.rdbuf();
				pDeviceGvcp->WriteRegAck();
				ss << "[HandlingControlPacket] --> " << pDeviceGvcp->_From
					<< "  Write register acknowledge" << endl;

				cout << ss.rdbuf();
				break;
			}
			case MV_GEV_READMEM_ACK:
			{
				ss << "Read memory command" << endl;
				cout << ss.rdbuf();
				//pDeviceGvcp->ReadMemAck();
				ss << "[HandlingControlPacket] --> " << pDeviceGvcp->_From
					<< "  Read memory acknowledge" << endl;
				cout << ss.rdbuf();
				break;
			}
			case MV_GEV_WRITEMEM_ACK:
			{
				ss << "Write memory command" << endl;
				cout << ss.rdbuf();
				//pDeviceGvcp->WriteMemAck();
				ss << "[HandlingControlPacket] --> " << pDeviceGvcp->_From
					<< "  Write memory acknowledge" << endl;
				cout << ss.rdbuf();
				break;
			}
			default:
			{
				ss << "Unknown command" << endl;
				cout << ss.rdbuf();
				break;
			}
			}
		}
		Sleep(1);
	} while (!pDeviceGvcp->f_threadStop);

#ifdef MV_WINDOWS
	return nRet;
#endif
}
int DeviceGVCP::Discovery()
{
	try
	{
		DiscoveryCmd();
		if (DiscoveryDone() == -1)
		{
			return -1;
		}
		return 1;
	}
	catch (SocketException& SktEx)
	{
		return -10;
	}
	return 1;
}
int DeviceGVCP::DiscoveryCmd()
{
	return DiscoveryCmdSkt(_UdpSkt);
}
int DeviceGVCP::DiscoveryCmdSkt(MVComponent::UDP  udpskt)
{
	int nRet;
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)cSendData;
	pCmdHdr->cKeyValue = 0x42;
	pCmdHdr->nCommand = htons(MV_GEV_DISCOVERY_CMD);
	pCmdHdr->cFlg = 1;
	pCmdHdr->nReqId = _nReqId++;

	try
	{
		size_t nLen = sizeof(CMD_MSG_HEADER);
		udpskt.Send("255.255.255.255", MV_GVCP_PUBLIC_PORT, cSendData, nLen);
		f_DiscoveryDone = 0;
	}
	catch (SocketException& SktEx)
	{
		return -1;
	}
	return 1;
}



void*  DeviceGVCP::DiscoveryAck(char* Arg)
{
	int nRet = MV_OK;
	//DeviceGVCP* pDeviceGvcp = (DeviceGVCP*)Arg;
	char * recvbuff = (char*)Arg;
	DISCOVERY_ACK_MSG* pAckMsg = (DISCOVERY_ACK_MSG*)(recvbuff + sizeof(ACK_MSG_HEADER));
	MV_CC_DEVICE_INFO* pDevInfo = new MV_CC_DEVICE_INFO();
	memcpy(&devinfo, pDevInfo, sizeof(MV_CC_DEVICE_INFO));

	pDevInfo->nMajorVer = ntohs(pAckMsg->nMajorVer);
	pDevInfo->nMinorVer = ntohs(pAckMsg->nMinorVer);
	pDevInfo->nDeviceMode = ntohl(pAckMsg->nDeviceMode);
	pDevInfo->nMacAddrHigh = ntohl(pAckMsg->nMacAddrHigh);
	pDevInfo->nMacAddrLow = ntohl(pAckMsg->nMacAddrLow);
	pDevInfo->stGigEInfo.nIpCfgOption = ntohl(pAckMsg->nIpCfgOption);
	pDevInfo->stGigEInfo.nIpCfgCurrent = ntohl(pAckMsg->nIpCfgCurrent);
	pDevInfo->stGigEInfo.nCurrentIp = ntohl(pAckMsg->nCurrentIp);
	pDevInfo->stGigEInfo.nCurrentSubNetMask = ntohl(pAckMsg->nCurrentSubNetMask);
	pDevInfo->stGigEInfo.nDefultGateWay = ntohl(pAckMsg->nDefultGateWay);

	memcpy(pDevInfo->stGigEInfo.chManufacturerName, pAckMsg->chManufacturerName, 32);
	memcpy(pAckMsg->chModelName, pDevInfo->stGigEInfo.chModelName, 32);
	memcpy(pDevInfo->stGigEInfo.chDeviceVersion, pAckMsg->chDeviceVersion, 32);
	memcpy(pDevInfo->stGigEInfo.chManufacturerSpecificInfo, pAckMsg->chManufacturerSpecificInfo, 48);
	memcpy(pDevInfo->stGigEInfo.chSerialNumber, pAckMsg->chSerialNumber, 16);
	memcpy(pDevInfo->stGigEInfo.chUserDefinedName, pAckMsg->chUserDefinedName, 16);
	f_DiscoveryDone = 1;
	return pDevInfo;
}

int DeviceGVCP::DiscoveryDone()
{
	int count = RETRYTIME;

	while (this->f_DiscoveryDone == -1)
	{
		if (count < 0)
		{
			return -1;
		}
		Sleep(100);
		count--;
	}
	return 1;
}
int DeviceGVCP::ForceIP(CCHCamera *info)

{

	UDP tempudp;

	tempudp.Open();

	Address tempaddr;

	tempaddr.SetAddressIp(info->hostaddr);

	tempaddr.SetAddressPort(0);

	tempudp.BindAddr(tempaddr);

	tempudp.SetBroadcast(true);

	tempudp.SetDontfragment(true);

	try

	{

		ForceIP(tempudp);

		decodePacket(tempudp);

		if (ForceIPDone() == -1)

		{

			return -1;

		}

		return 1;

	}

	catch (SocketException& SktEx)

	{

		return -10;

	}

}

int DeviceGVCP::ForceIP(MVComponent::UDP udpskt)

{

	int nRet;
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)cSendData;
	pCmdHdr->cKeyValue = 0x42;
	pCmdHdr->nCommand = htons(MV_GEV_FORCEIP_CMD);
	pCmdHdr->cFlg = 0x11;
	pCmdHdr->nReqId = _nReqId++;
	FORCEIP_CMD_MSG *forceIPMsg = (FORCEIP_CMD_MSG*)(cSendData + sizeof(CMD_MSG_HEADER));

	forceIPMsg->nMacAddrHigh = htonl(devinfo.nMacAddrHigh);
	forceIPMsg->nMacAddrLow = htonl(devinfo.nMacAddrLow);
	forceIPMsg->nStaticDefaultGateWay = htonl(devinfo.stGigEInfo.nDefultGateWay);
	forceIPMsg->nStaticIp = htonl(devinfo.stGigEInfo.nCurrentIp);
	forceIPMsg->nStaticSubNetMask = htonl(devinfo.stGigEInfo.nCurrentSubNetMask);

	f_ForceIPDone = -1;
	try
	{
		size_t nLen = sizeof(CMD_MSG_HEADER) + sizeof(FORCEIP_CMD_MSG);
		udpskt.Send("255.255.255.255", MV_GVCP_PUBLIC_PORT, cSendData, nLen);

	}
	catch (SocketException& SktEx)
	{

		return -2;
	}
	return 1;
}
int DeviceGVCP::ForceIPAck(char* Arg)
{
	char * recvbuff = (char*)Arg;
	ACK_MSG_HEADER *header = (ACK_MSG_HEADER*)recvbuff;
	f_ForceIPDone = header->nAckId;
	return f_ForceIPDone;
}
int DeviceGVCP::ForceIPDone()
{
	int count = RETRYTIME;
	while (f_ForceIPDone < 0)
	{
		if (count < 0)
		{
			break;
		}
		Sleep(100);
		count--;
	}
	return f_ForceIPDone;
}
int DeviceGVCP::ReadReg(unsigned int addr, uint32_t *data)
{
	try
	{
		ReadRegCmd(addr);
		if (ReadRegDone() == -1)
		{
			return -1;
		}
		*data = ntohl(ReadRegRst.nRegData);
		return 1;
	}
	catch (SocketException& SktEx)
	{
		return -10;
	}

}
unsigned int DeviceGVCP::ReadRegCmd(unsigned int addr)
{
	int nRet;
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)cSendData;
	pCmdHdr->cKeyValue = 0x42;
	pCmdHdr->nCommand = htons(MV_GEV_READREG_CMD);
	pCmdHdr->cFlg = 1;
	pCmdHdr->nReqId = _nReqId++;
	READREG_CMD_MSG *ReadRegMsg = (READREG_CMD_MSG*)(cSendData + sizeof(CMD_MSG_HEADER));
	ReadRegMsg->nRegAddress = htonl(addr);
	f_ReadRegDone = -1;
	try
	{
		size_t nLen = sizeof(CMD_MSG_HEADER) + sizeof(READREG_CMD_MSG);
		unsigned int ipaddr = ntohl(devinfo.stGigEInfo.nCurrentIp);
		sockaddr_in socktemp;

		char str[INET_ADDRSTRLEN];
		socktemp.sin_addr.S_un.S_addr = ipaddr;
		inet_ntop(AF_INET, &(socktemp.sin_addr), str, INET_ADDRSTRLEN);
		std::string ipaddrstr(str);
		_UdpSkt.Send(ipaddrstr, MV_GVCP_PUBLIC_PORT, cSendData, nLen);

	}
	catch (SocketException& SktEx)
	{
		return -1;
	}
	return 1;
}

READREG_CMD_MSG_ACK DeviceGVCP::ReadRegAck(char* arg)
{
	char * recvbuff = (char*)arg;
	ACK_MSG_HEADER* pAckHdr = (ACK_MSG_HEADER*)recvbuff;
	this->f_ReadRegDone = pAckHdr->nAckId;
	if (pAckHdr->nStatus == MV_OK)
	{
		memcpy(&ReadRegRst, recvbuff + sizeof(ACK_MSG_HEADER), sizeof(READREG_CMD_MSG_ACK));
	}
	return ReadRegRst;
}
int DeviceGVCP::ReadRegDone()
{
	int count = RETRYTIME;

	while (this->f_ReadRegDone == -1)
	{
		if (count < 0)
		{
			return -1;
		}
		Sleep(10);
		count--;
	}
	return 1;
}
int DeviceGVCP::WriteReg(unsigned int addr, unsigned int data)
{
	try
	{
		WriteRegCmd(addr, data);
		if ((addr >= 0x064C && addr <= 0x066C)||(addr>=
			0000&&addr<=0x33CC07FF))
		{//IP address and EE range
			//EEROM need more time to delay
			Sleep(50);
		}
		else
		{
			Sleep(1);
		}
		if (WriteRegDone() == -1)
		{
			return -1;
		}
		return 1;
	}
	catch (SocketException& SktEx)
	{
		return -10;
	}
}
int DeviceGVCP::WriteRegCmd(unsigned int addr, unsigned int data)
{
	int nRet;
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)cSendData;
	pCmdHdr->cKeyValue = 0x42;
	pCmdHdr->nCommand = htons(MV_GEV_WRITEREG_CMD);
	pCmdHdr->cFlg = 1;
	pCmdHdr->nReqId = _nReqId++;

	WRITEREG_CMD_MSG *WriteRegMsg = (WRITEREG_CMD_MSG*)(cSendData + sizeof(CMD_MSG_HEADER));
	WriteRegMsg->nRegAddress = htonl(addr);
	WriteRegMsg->nRegData = htonl(data);
	try
	{
		size_t nLen = sizeof(CMD_MSG_HEADER) + sizeof(WRITEREG_CMD_MSG);
		unsigned int ipaddr = ntohl(devinfo.stGigEInfo.nCurrentIp);
		sockaddr_in socktemp;
		char str[INET_ADDRSTRLEN];

		socktemp.sin_addr.S_un.S_addr = ipaddr;
		inet_ntop(AF_INET, &(socktemp.sin_addr), str, INET_ADDRSTRLEN);
		std::string ipaddrstr(str);
		WriteRegDone();
		_UdpSkt.Send(ipaddrstr, MV_GVCP_PUBLIC_PORT, cSendData, nLen);
		f_WriteRegDone = 0;
	}
	catch (SocketException& SktEx)
	{
		return -1;
	}
	return 1;
}
int DeviceGVCP::WriteRegDone()
{
	int count = RETRYTIME;
	while (f_WriteRegDone != 1)
	{
		if (count < 0)
		{
			return -1;
		}
		Sleep(10);
		count--;
	}
	return 1;
}
int DeviceGVCP::WriteRegAck()
{
	f_WriteRegDone = 1;
	return 1;
}
CStringA DeviceGVCP::STR_T2A(LPCTSTR in)
{
	CStringA result;

	ASSERT(in != NULL);
	if (in == NULL)
		return result;

	int bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, in, -1, NULL, 0, NULL, NULL);
	if (bytesNeeded == 0)
		return result;

	LPSTR buf = result.GetBuffer(bytesNeeded);
	int res = WideCharToMultiByte(CP_UTF8, 0, in, -1, buf, bytesNeeded, NULL, NULL);
	result.ReleaseBuffer();
	if (res == 0)
	{
		result.Empty();
		return result;
	}

	return result;
}

CString DeviceGVCP::STR_A2T(LPCSTR in)
{
	CString result;

	ASSERT(in != NULL);
	if (in == NULL)
		return result;

	int numCharsNeeded = MultiByteToWideChar(CP_UTF8, 0, in, -1, NULL, 0);
	if (numCharsNeeded == 0)
		return result;

	LPTSTR buf = result.GetBuffer(numCharsNeeded);
	int res = MultiByteToWideChar(CP_UTF8, 0, in, -1, buf, numCharsNeeded);
	result.ReleaseBuffer();
	if (res == 0)
	{
		result.Empty();
		return result;
	}

	return result;
}
int DeviceGVCP::getInterface()
{
	CStringArray baList;
	baList.RemoveAll();

	ULONG bufSz = 0;
	if (GetAdaptersInfo(NULL, &bufSz) == ERROR_BUFFER_OVERFLOW)
	{
		vector<BYTE> buf;
		buf.resize(bufSz, 0);
		if (GetAdaptersInfo((IP_ADAPTER_INFO*)&buf[0], &bufSz) == ERROR_SUCCESS)
		{
			IP_ADAPTER_INFO* pAdapterInfo = (IP_ADAPTER_INFO*)&buf[0];
			for (; pAdapterInfo != NULL; pAdapterInfo = pAdapterInfo->Next)
			{
				unsigned long ip = inet_addr(pAdapterInfo->IpAddressList.IpAddress.String);
				//unsigned long mask = inet_addr(pAdapterInfo->IpAddressList.IpMask.String);
				//unsigned long bcip = ip | ~mask;
				//struct in_addr ia;
				//ia.S_un.S_addr = bcip;
				//CString broadcastAddr;
				//broadcastAddr = STR_A2T(inet_ntoa(ia));
				//baList.Add(broadcastAddr);
				if (ip == 0)
				{
					continue;
				}
				UDP tempudp;
				tempudp.Open();
				Address tempaddr;
				tempaddr.SetAddressIp(pAdapterInfo->IpAddressList.IpAddress.String);
				tempaddr.SetAddressPort(0);
				tempudp.BindAddr(tempaddr);
				tempudp.SetBroadcast(true);
				tempudp.SetDontfragment(true);
				currentSocket = tempudp;//update global udp
				DiscoveryCmdSkt(tempudp);
				decodePacket(tempudp);
				tempudp.Close();
			}
		}
	}
	return 1;
}
/*
int DeviceGVCP::ReadMemAck()
{
int nRet = MV_OK;

char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)_cRecvData;
ACK_MSG_HEADER* pAckHdr = (ACK_MSG_HEADER*)cSendData;
pAckHdr->nStatus = htons(MV_GEV_STATUS_SUCCESS);
pAckHdr->nAckMsgValue = htons(MV_GEV_READMEM_ACK);
pAckHdr->nAckId = htons(ntohs(pCmdHdr->nReqId));

if (_nLastAckId != ntohs(pCmdHdr->nReqId))
{
// Update last ack_id
_nLastAckId = ntohs(pCmdHdr->nReqId);
}

READMEM_CMD_MSG* pCmdMsg = (READMEM_CMD_MSG*)(_cRecvData + sizeof(CMD_MSG_HEADER));
READMEM_ACK_MSG* pAckMsg = (READMEM_ACK_MSG*)(cSendData + sizeof(ACK_MSG_HEADER));
pAckHdr->nLength = htons(sizeof(pAckMsg->nMemAddress) + ntohs(pCmdMsg->nMemLen));
pAckMsg->nMemAddress = pCmdMsg->nMemAddress;
_pDevice->GetMem(reinterpret_cast<Device::virtual_addr_t>(ntohl(pCmdMsg->nMemAddress)),
(unsigned char*)pAckMsg->chReadMemData, ntohs(pCmdMsg->nMemLen));

try
{
size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->nLength);
_UdpSkt.Send(_From, cSendData, nLen);
}
catch (SocketException& SktEx)
{
nRet = MV_E_NETER;
return nRet;
}

return nRet;
}

int DeviceGVCP::WriteMemAck()
{
int nRet = MV_OK;

char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)_cRecvData;
ACK_MSG_HEADER* pAckHdr = (ACK_MSG_HEADER*)cSendData;
pAckHdr->nStatus = htons(MV_GEV_STATUS_SUCCESS);
pAckHdr->nAckMsgValue = htons(MV_GEV_WRITEMEM_ACK);
pAckHdr->nLength = htons(sizeof(WRITEREG_ACK_MSG));
pAckHdr->nAckId = htons(ntohs(pCmdHdr->nReqId));

int nIndex = ntohs(pCmdHdr->nLength) - sizeof(char*);
if (_nLastAckId != ntohs(pCmdHdr->nReqId))
{
// Update last ack_id
_nLastAckId = ntohs(pCmdHdr->nReqId);

// Write to memory
WRITEMEM_CMD_MSG* pCmdMsg = (WRITEMEM_CMD_MSG*)(_cRecvData + sizeof(CMD_MSG_HEADER));
size_t nCount = ntohs(pCmdHdr->nLength) - sizeof(pCmdMsg->nMemAddress);
if (_pDevice->SetMem(reinterpret_cast<Device::virtual_addr_t>(ntohl(pCmdMsg->nMemAddress)),
pCmdMsg->chWriteMemData, nCount) != MV_OK)
{
nIndex = 0;
}
}
WRITEMEM_ACK_MSG* pAckMsg = (WRITEMEM_ACK_MSG*)(cSendData + sizeof(ACK_MSG_HEADER));
pAckMsg->nReserved = 0;
pAckMsg->nIndex = htons(nIndex);

try
{
size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->nLength);
_UdpSkt.Send(_From, cSendData, nLen);
}
catch (SocketException& SktEx)
{
nRet = MV_E_NETER;
return nRet;
}

return nRet;
}
*/