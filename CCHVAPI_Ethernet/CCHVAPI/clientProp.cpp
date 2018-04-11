#include "stdafx.h"
#include "GigEclientProp.h"

#include "GigEcamProp.h"
#include <string.h>
GigEclientProp::GigEclientProp(void)
{
	sendbuffer=new char[32];
	memClientProp.camIP=33663168;//0xc0a80002;
	memClientProp.pcIP=50440384;//inet0xc0a80003;
	//camIP=0;
	//pcIP=0;
	memClientProp.camCnt=0;
	memClientProp.MACaddr=0xabcdc0a8019b;
	memClientProp.packetSize=1024;
	memClientProp.interval_time=0x1000;
	initBuffer();
}
int GigEclientProp::initBuffer()
{
	int i=0;
	long long ff=0xff;
	sendbuffer[i++]=0x56;//0
	sendbuffer[i++]=0xab;//1
	sendbuffer[i++]=0x00;//2 func
	
	sendbuffer[i++]=(memClientProp.MACaddr&(ff<<5*8))>>5*8;//0xab;//3 MAC H
	sendbuffer[i++]=(memClientProp.MACaddr&(ff<<4*8))>>4*8;//0xcd;//4
	sendbuffer[i++]=(memClientProp.MACaddr&(ff<<3*8))>>3*8;//0xc0;//5
	sendbuffer[i++]=(memClientProp.MACaddr&(ff<<2*8))>>2*8;//0xa8;//6
	sendbuffer[i++]=(memClientProp.MACaddr&(ff<<1*8))>>8;//0x01;//7
	sendbuffer[i++]=memClientProp.MACaddr&(ff<<0);//0x9b;//8 MAC L
	sendbuffer[i++]=(memClientProp.camIP&(0xff<<3*8))>>3*8;//0xc0;//9 IP H
	sendbuffer[i++]=(memClientProp.camIP&(0xff<<2*8))>>2*8;//0xa8;//10
	sendbuffer[i++]=(memClientProp.camIP&(0xff<<1*8))>>8;//0x00;//11
	sendbuffer[i++]=memClientProp.camIP&(0xff);//0x02;//12 IP L 
	sendbuffer[i++]=(memClientProp.pcIP&(ff<<3*8))>>3*8;//0xc0;//13 IP H
	sendbuffer[i++]=(memClientProp.pcIP&(ff<<2*8))>>2*8;//0xa8;//14
	sendbuffer[i++]=(memClientProp.pcIP&(ff<<1*8))>>8;//0x00;//15
	sendbuffer[i++]=memClientProp.pcIP&(ff);//0x03;//16 IP L
	sendbuffer[i++]=(memClientProp.packetSize&(0xff<<8))>>8;//0x04;//17 pack_size H
	sendbuffer[i++]=memClientProp.packetSize&(0xff<<0);//0x00;//18
	sendbuffer[i++]=(memClientProp.interval_time&(0xff<<3*8))>>3*8;//0x00;//19 interval time H
	sendbuffer[i++]=(memClientProp.interval_time&(0xff<<2*8))>>2*8;//0x00;//20
	sendbuffer[i++]=(memClientProp.interval_time&(0xff<<1*8))>>8;//0x10;//21
	sendbuffer[i++]=memClientProp.interval_time&(0xff<<0*8);//0x00;//22 interval time L
	sendbuffer[i++]=0x00;//23 resv
	sendbuffer[i++]=0x00;//24
	sendbuffer[i++]=0x00;//25
	sendbuffer[i++]=0x00;//26
	sendbuffer[i++]=0x00;//27
	sendbuffer[i++]=0x00;//28
	sendbuffer[i++]=0x00;//29
	sendbuffer[i++]=0x57;//30 frame end
	sendbuffer[i++]=0xac;//31 
	return 0;
}

GigEclientProp::~GigEclientProp(void)
{
}

int GigEclientProp::setClientProp(GigEclientPropStruct prop)
{
	memcpy(&memClientProp,&prop,sizeof(GigEclientPropStruct));
	initBuffer();
	return 0;
}
int GigEclientProp::getClientProp(GigEclientPropStruct *prop)
{
	memcpy(prop,&memClientProp,sizeof(GigEclientPropStruct));
	return 0;
}
int GigEclientProp::setpcIP(DWORD IP)
{
	memClientProp.pcIP=IP;
	return 0;
}
unsigned long GigEclientProp::getpcIP()
{
	return memClientProp.pcIP;
}
int GigEclientProp::setcamIP(DWORD IP)
{
	memClientProp.camIP=IP;
	return 0;
}
unsigned long GigEclientProp::getcamIP()
{
	return memClientProp.camIP;
}
int GigEclientProp::getBuffer(char * destbuffer)
{
	memcpy(destbuffer,sendbuffer,32);
	return 32;
}
int GigEclientProp::setCamCnt(int cnt)
{
	memClientProp.camCnt=cnt;
	return cnt;
}
int GigEclientProp::getCamCnt()
{
	return memClientProp.camCnt;
}