// -*- coding: gb2312-dos -*-
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "DeviceGVCP.h"

#ifdef WINDOWS
#include <mstcpip.h>

#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif

using namespace std;
using namespace MVComponent;
using namespace MvCamCtrl;

DeviceGVCP::DeviceGVCP(Device* pDev)
	: _nLastAckId(0), _pDevice(pDev)
{
	DeviceGVCP* pDeviceGvcp = (DeviceGVCP*)_pDevice;

	_cp = cp_init();
}

DeviceGVCP::~DeviceGVCP()
{
	cp_close(_cp);
}

int DeviceGVCP::Init(int Port)
{
	int nRet = MV_OK;

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
		//cp_print(_cp, CP_FG_RED, "[WARN]");
		cout << SktEx << endl;
		nRet = MV_E_NETER;
		return nRet;
	}

#ifdef WINDOWS
	// Windows UDP socket recvfrom return 10054
	// 当UDP Socket在某次发送后收到一个不可到达的ICMP包时，这个错误将在下一个接收中返回，
	// 所以上面的套接字在下一次的接收中返回了SOCKET_ERROR，错误是10045。
	bool bNewBehavior = false;
	DWORD dwBytesReturned = 0;
	::WSAIoctl(_UdpSkt.GetSocketId(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof bNewBehavior, NULL, 0, &dwBytesReturned, NULL, NULL);
#endif

	return nRet;
}

int DeviceGVCP::DeInit()
{
	int nRet = MV_OK;

	try
	{
		_UdpSkt.Close();
	}
	catch (SocketException& SktEx)
	{
		cp_print(_cp, CP_FG_RED, "[WARN]");
		cout << SktEx << endl;
		nRet = MV_E_NETER;
		return nRet;
	}

	return nRet;
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
		if ((nRet = pDeviceGvcp->ReceiveMsg(nLen)) != MV_OK)
		{
			cp_print(pDeviceGvcp->_cp, CP_FG_RED, "[WARN]");
			ss << "[HandlingControlPacket] ReceiveMsg fail!!" << endl;
			cout << ss.rdbuf();
#ifdef MV_WINDOWS
			return nRet;
#endif
		}

		if (nLen > 0)
		{
			//ss << "[HandlingAckPacket] <-- " << pDeviceGvcp->_From << "  ";

			ACK_MSG_HEADER* pCmdHdr = (ACK_MSG_HEADER*)(pDeviceGvcp->_cRecvData);

			switch (ntohs(pCmdHdr->nAckMsgValue))
			{
			case MV_GEV_DISCOVERY_ACK:
			{
				if (pCmdHdr->nStatus != MV_OK)
				{
					//error;
				}
				MV_CC_DEVICE_INFO * ptemp;
				 ptemp=(MV_CC_DEVICE_INFO *)(pDeviceGvcp->DiscoveryAck(pDeviceGvcp->_cRecvData));
				 char *chartemp = new char[16];
				 memcpy(chartemp, ptemp->stGigEInfo.chSerialNumber, 16);
				 std::string sntemp(chartemp);
				 if (pDeviceGvcp->devicelist.find(sntemp) == pDeviceGvcp->devicelist.end())
				 {
					(pDeviceGvcp->devicelist).insert(std::pair<std::string, MV_CC_DEVICE_INFO*>(sntemp, ptemp));
				 }

				break;
			}
			case MV_GEV_READREG_ACK:
			{
				ss << "Read register command" << endl;
				cout << ss.rdbuf();
				pDeviceGvcp->ReadRegAck();
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
				pDeviceGvcp->ReadMemAck();
				ss << "[HandlingControlPacket] --> " << pDeviceGvcp->_From
					<< "  Read memory acknowledge" << endl;
				cout << ss.rdbuf();
				break;
			}
			case MV_GEV_WRITEMEM_ACK:
			{
				ss << "Write memory command" << endl;
				cout << ss.rdbuf();
				pDeviceGvcp->WriteMemAck();
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
		Sleep(500);
	} while (!pDeviceGvcp->_pDevice->IsCancel());

#ifdef MV_WINDOWS
	return nRet;
#endif
}
int DeviceGVCP::DiscoveryCmd()
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
		//_UdpSkt.Send(_From, cSendData, nLen);
		_UdpSkt.Send("255.255.255.255",MV_GVCP_PUBLIC_PORT,cSendData, nLen);
	}
	catch (SocketException& SktEx)
	{
		/*
		cp_print(_cp, CP_FG_RED, "[WARN]");
		cout << SktEx << endl;
		nRet = MV_E_NETER;*/
		return nRet;
	}
	return 1;
}
int DeviceGVCP::forceIP(std::string ip)
{
	int nRet;
	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)cSendData;
	pCmdHdr->cKeyValue = 0x42;
	pCmdHdr->nCommand = htons(MV_GEV_FORCEIP_CMD);
	pCmdHdr->cFlg = 0x11;
	pCmdHdr->nReqId = _nReqId++;
	FORCEIP_CMD_MSG *forceIPMsg = (FORCEIP_CMD_MSG*)(cSendData + sizeof(CMD_MSG_HEADER));
	MV_CC_DEVICE_INFO* deviceinfo = (devicelist.find("")->second);
	forceIPMsg->nMacAddrHigh = htons(deviceinfo->nMacAddrHigh);
	forceIPMsg->nMacAddrLow = htons(deviceinfo->nMacAddrLow);
	forceIPMsg->nStaticDefaultGateWay = htons(deviceinfo->stGigEInfo.nDefultGateWay);
	forceIPMsg->nStaticIp = htons(inet_addr(ip.c_str()));
	forceIPMsg->nStaticSubNetMask = htons(deviceinfo->stGigEInfo.nCurrentSubNetMask);
	try
	{
		size_t nLen = sizeof(CMD_MSG_HEADER);
		_UdpSkt.Send("255.255.255.255", MV_GVCP_PUBLIC_PORT, cSendData, nLen);
	}
	catch (SocketException& SktEx)
	{

		return -2;
	}
	return 1;
}
int DeviceGVCP::ReceiveMsg(unsigned int& nLen)
{
	int nRet = MV_OK;
	nLen = MV_GVCP_MAX_MSG_LEN;

	try
	{
		nLen = _UdpSkt.ReceiveTimeout(500, _From, _cRecvData, nLen);
	}
	catch (SocketException& SktEx)
	{
		cp_print(_cp, CP_FG_RED, "[WARN]");
		cout << SktEx << endl;
		nRet = MV_E_NETER;
		return nRet;
	}

	return nRet;
}

void*  DeviceGVCP::DiscoveryAck(char* Arg)
{
	int nRet = MV_OK;
	//DeviceGVCP* pDeviceGvcp = (DeviceGVCP*)Arg;
	char * recvbuff = (char*)Arg;
	DISCOVERY_ACK_MSG* pAckMsg = (DISCOVERY_ACK_MSG*)(recvbuff + sizeof(ACK_MSG_HEADER));
	MV_CC_DEVICE_INFO* pDevInfo = new MV_CC_DEVICE_INFO();

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

	memcpy(pDevInfo->stGigEInfo.chManufacturerName,pAckMsg->chManufacturerName,32);
	memcpy(pAckMsg->chModelName, pDevInfo->stGigEInfo.chModelName, 32);
	memcpy(pDevInfo->stGigEInfo.chDeviceVersion,pAckMsg->chDeviceVersion, 32);
	memcpy(pDevInfo->stGigEInfo.chManufacturerSpecificInfo, pAckMsg->chManufacturerSpecificInfo,  48);
	memcpy(pDevInfo->stGigEInfo.chSerialNumber, pAckMsg->chSerialNumber, 16);
	memcpy(pDevInfo->stGigEInfo.chUserDefinedName, pAckMsg->chUserDefinedName,  16);
	
	return pDevInfo;
}

int DeviceGVCP::ReadRegAck()
{
	int nRet = MV_OK;

	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)_cRecvData;
	ACK_MSG_HEADER* pAckHdr = (ACK_MSG_HEADER*)cSendData;
	pAckHdr->nStatus = htons(MV_GEV_STATUS_SUCCESS);
	pAckHdr->nAckMsgValue = htons(MV_GEV_READREG_ACK);
	pAckHdr->nLength = htons(ntohs(pCmdHdr->nLength));
	pAckHdr->nAckId = htons(ntohs(pCmdHdr->nReqId));

	if (_nLastAckId != ntohs(pCmdHdr->nReqId))
	{
		// Update last ack_id
		_nLastAckId = ntohs(pCmdHdr->nReqId);
	}

	long* pCurRegAddr = (long*)(_cRecvData + sizeof(CMD_MSG_HEADER));
	long* pCurRegData = (long*)(cSendData + sizeof(ACK_MSG_HEADER));
	uint32_t nTmp;
	for (unsigned int i = 0; i < ntohs(pCmdHdr->nLength) / sizeof(long); ++i)
	{
		pCurRegAddr = (long*)pCurRegAddr + i;
		pCurRegData = (long*)pCurRegData + i;
		_pDevice->GetReg(reinterpret_cast<Device::virtual_addr_t>(ntohl(*pCurRegAddr)), nTmp);
		*pCurRegData = htonl(nTmp);
	}

	try
	{
		size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->nLength);
		_UdpSkt.Send(_From, cSendData, nLen);
	}
	catch (SocketException& SktEx)
	{
		cp_print(_cp, CP_FG_RED, "[WARN]");
		cout << SktEx << endl;
		nRet = MV_E_NETER;
		return nRet;
	}

	return nRet;
}

int DeviceGVCP::WriteRegAck()
{
	int nRet = MV_OK;

	char cSendData[MV_GVCP_MAX_MSG_LEN] = { 0 };
	CMD_MSG_HEADER* pCmdHdr = (CMD_MSG_HEADER*)_cRecvData;
	ACK_MSG_HEADER* pAckHdr = (ACK_MSG_HEADER*)cSendData;
	pAckHdr->nStatus = htons(MV_GEV_STATUS_SUCCESS);
	pAckHdr->nAckMsgValue = htons(MV_GEV_WRITEREG_ACK);
	pAckHdr->nLength = htons(sizeof(WRITEREG_ACK_MSG));
	pAckHdr->nAckId = htons(ntohs(pCmdHdr->nReqId));

	int nIndex = ntohs(pCmdHdr->nLength) / sizeof(WRITEREG_CMD_MSG);
	if (_nLastAckId != ntohs(pCmdHdr->nReqId))
	{
		// Update last ack_id
		_nLastAckId = ntohs(pCmdHdr->nReqId);

		// Write to register
		WRITEREG_CMD_MSG* pCurCmdMsg = (WRITEREG_CMD_MSG*)((char*)_cRecvData + sizeof(CMD_MSG_HEADER));
		for (unsigned int i = 0; i < ntohs(pCmdHdr->nLength) / sizeof(WRITEREG_CMD_MSG); ++i)
		{
			pCurCmdMsg = (WRITEREG_CMD_MSG*)pCurCmdMsg + i;

			if (_pDevice->SetReg(reinterpret_cast<Device::virtual_addr_t>(ntohl(pCurCmdMsg->nRegAddress)),
				(uint32_t)(ntohl(pCurCmdMsg->nRegData))) != MV_OK)
			{
				nIndex = i;
				break;
			}

		}
	}

	WRITEREG_ACK_MSG* pAckMsg = (WRITEREG_ACK_MSG*)(cSendData + sizeof(ACK_MSG_HEADER));
	pAckMsg->nReserved = 0;
	pAckMsg->nIndex = htons(nIndex);

	try
	{
		size_t nLen = sizeof(ACK_MSG_HEADER) + ntohs(pAckHdr->nLength);
		_UdpSkt.Send(_From, cSendData, nLen);
	}
	catch (SocketException& SktEx)
	{
		cp_print(_cp, CP_FG_RED, "[WARN]");
		cout << SktEx << endl;
		nRet = MV_E_NETER;
		return nRet;
	}

	return nRet;
}

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
		cp_print(_cp, CP_FG_RED, "[WARN]");
		cout << SktEx << endl;
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
		cp_print(_cp, CP_FG_RED, "[WARN]");
		cout << SktEx << endl;
		nRet = MV_E_NETER;
		return nRet;
	}

	return nRet;
}
