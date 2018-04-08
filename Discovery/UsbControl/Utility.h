#pragma once
#include "stdafx.h"
#include <string>
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
class CMacAddr
{
private :
	CString temp;
public :
	unsigned char macaddrIdx[6];
	unsigned long long macAddrNum;
	CString  macAddrStr;
	CMacAddr()
	{
	}
	CMacAddr(unsigned long long num)
	{
		setAddress(num);
	}
	int setAddress(unsigned long long num)
	{
		
		macAddrNum = num;
		unsigned long long mask = 0xff;
		for (int i = 0; i < 6; i++)
		{
			macaddrIdx[i] = (macAddrNum&(mask << (i * 8)))>>(i*8);
			temp.Format(L"%.2X", macaddrIdx[i]);
			if(i==0)
				macAddrStr = temp;
			else
			{
				macAddrStr = temp+L":"+macAddrStr;
			}
		}
		return 1;
	}
	int setAddress(unsigned char *addr)
	{
		return 1;
	}
	
	
};