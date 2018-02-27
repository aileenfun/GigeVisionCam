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
	/*clientPropStruct():camCnt(6),
	hostIP(0xc0a800003),
	clientIP(0xc0a80002),
	MACaddr(0xabcdc0a8019b),
	packetSize(1024),
	interval_time(0x1000){}*/
	
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

