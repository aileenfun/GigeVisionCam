#pragma once
//#include "DataCapture.h"
struct clientPropStruct
{
	int camCnt;
	DWORD pcIP;
	DWORD camIP;
	int packetSize;
	int interval_time;
	unsigned long long MACaddr;
	int width;
	int height;
};
class clientProp
{
public:
	clientProp(void);
	~clientProp(void);
	int initBuffer();
	int getCamCnt();
	int setCamCnt(int camCnt);
	int setpcIP(DWORD IP);
	DWORD getpcIP();
	int setcamIP(DWORD IP);
	DWORD getcamIP();
	int getBuffer(char * destbuffer);
	int setClientProp(clientPropStruct prop);
	int getClientProp(clientPropStruct *prop);
	clientPropStruct memClientProp;
	
private:
	char *sendbuffer;
	
		
	int camCnt;
	DWORD pcIP;
	DWORD camIP;
	int packetSize;
	int interval_time;
	unsigned long MACaddr;
};

