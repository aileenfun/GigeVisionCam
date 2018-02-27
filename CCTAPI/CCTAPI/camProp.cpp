#include "StdAfx.h"
#include "camProp.h"


camProp::camProp(void)
{
	sendbuff=new char[32];
	memCamProp.trigMode=0;
	memCamProp.expo=0x40;
	memCamProp.brightness=0x10;
	memCamProp.mirror=0;
	memCamProp.row=0x01e0;
	memCamProp.col=0x0280;
	initBuffer();
}

camProp::~camProp(void)
{
	delete sendbuff;
}
int camProp::initBuffer()
{
	int i=0;
	sendbuff[i++]=0x56;//0
	sendbuff[i++]=0xab;//1
	sendbuff[i++]=0x01;//2
	sendbuff[i++]=memCamProp.trigMode;//3
	sendbuff[i++]=(memCamProp.expo&(0xff<<8))>>8;//4
	sendbuff[i++]=memCamProp.expo&(0xff);//5
	sendbuff[i++]=memCamProp.brightness;//6
	sendbuff[i++]=memCamProp.mirror;//7
	sendbuff[i++]=(memCamProp.row&(0xff<<8))>>8;//8
	sendbuff[i++]=memCamProp.row&(0xff);//9
	sendbuff[i++]=(memCamProp.col&(0xff<<8))>>8;//10
	sendbuff[i++]=memCamProp.col&(0xff);//11
	i=30;
	sendbuff[i++]=0x57;
	sendbuff[i++]=0xac;
	return 0;
}
int camProp::setCamProp(camPropStruct prop)
{
	memcpy(&memCamProp,&prop,sizeof(camPropStruct));
	initBuffer();
	return 0;
}
int camProp::getCamProp(camPropStruct *prop)
{
	memcpy(prop,&memCamProp,sizeof(camPropStruct));
	return 0;
}
int camProp::getBuffer(char* destbuffer)
{
	memcpy(destbuffer,sendbuff,32);
	return 32;
}