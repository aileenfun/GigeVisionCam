#pragma once
#include "ini.h"
#include <sstream>
#include <iostream>
#include "..\..\CCTAPI\CCTAPI\CCTAPI.h"
typedef
VOID
(WINAPI * EECALLBACK)(LPVOID lpParam1,LPVOID lpParam2);
typedef unsigned long DWORD;
class Ce2Writer
{
public:
	
	Ce2Writer(CCHCamera*deviceinfo)
	{
	
		burnerInstance = addInstance((LPVOID*)this, 0, deviceinfo);
		//initCCTAPI(burnerInstance);
		counter = 0;
	}
	int startWriteEE(const char * filename)
	{
		INI::Parser _info(filename);
		INI::Level ee_info = _info.top()("EE_INFO");
		INI::Level::value_map_t::const_iterator it_map;
		counter = 0;
		for (INI::Level::values_t::iterator it_value = ee_info.ordered_values.begin();it_value != ee_info.ordered_values.end();it_value++)
		{
			it_map = (*it_value);
			std::stringstream ss_addr, ss_value;
			unsigned int ee_addr, ee_value;
			ss_addr << std::hex << it_map->first;
			ss_value << std::hex << it_map->second;

			ss_addr >> ee_addr;
			ss_value >> ee_value;

			WriteReg(ee_addr, ee_value, burnerInstance);
			Sleep(100);
			counter++;
		}
		return counter;
	}
		~Ce2Writer();
	
private:
	EECALLBACK h_callbackFunc;
	int burnerInstance;
	int counter;
};

