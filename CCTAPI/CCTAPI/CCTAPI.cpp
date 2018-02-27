// CCTAPI.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CCTAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
vector<camInstance*>vec_camins;


int initCCTAPI(int camNum)
{
	//addInstance(lpUser,CallBackFunc);
	if(camNum<1)return camNum;
	camNum=camNum-1;
	int status=vec_camins[camNum]->initEth();

	if(status>=0)
	{
		return vec_camins.size();
	}
	else
	{
		return -vec_camins.size();
	}
}
int addInstance(LPVOID *lpUser,LPMV_CALLBACK2 CallBackFunc)
{
	//call back func could be the same
	//you can not tell if that's the same cam by cbFunc
	//cam should be set by ip address
	camInstance*this_camInstance=new camInstance(lpUser,CallBackFunc);
	vec_camins.push_back(this_camInstance);
	return vec_camins.size();
}

int startCap(int height,int width,int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	
	if(vec_camins[camNum]->b_initstatus)
	{
		return vec_camins[camNum]->start(height,width);
	}
	return 1;
}
int setMirrorType(DataProcessType mirrortype)
{
	return 0;
}
int stopCap(int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	if(camNum>=vec_camins.size())
		return -1;
	vec_camins[camNum]->stop();
	return 1;
}
int getFrameCnt(int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	if(vec_camins[camNum]->isRunning())
	{
		return vec_camins[camNum]->getFrameCnt();
	}
	else
	{
		return -1;
	}
}
int getDataCnt(int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	if(vec_camins[camNum]->isRunning())
	{
		return vec_camins[camNum]->getDataCnt();
	}
	else
	{
		return -1;
	}
}
int sendOrder(camPropStruct camprop,int camNum,int s)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->sendOrder(camprop,s);
	return 0;
}

int sendProp(clientPropStruct prop,int camNum,int s)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->sendProp(prop,s);
	return 0;
}
int sendSoftTrig(int camNum,int s)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->sendSoftTrig(s);
	return 0;
}
int getProp(int camNum,clientPropStruct*prop)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	return vec_camins[camNum]->getProp(prop);
}
int forceIP(int camNum,std::string ipaddr)
{
	camNum = camNum - 1;
	if (vec_camins[camNum] == NULL)
		return -(camNum+1);
	vec_camins[camNum]->forceIP(ipaddr);
	return camNum;
}
int closeConnection(int &camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->closeConnection();
	
	delete vec_camins[camNum];
	vector<camInstance*>::iterator k =vec_camins.begin()+camNum;
	vec_camins.erase(k);
	camNum=-camNum;
	return 0;
}
unsigned __int32 WriteReg(unsigned __int32 addr, unsigned __int32 data,int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	return vec_camins[camNum]->WriteReg(addr,data);
}
unsigned __int32 ReadReg(unsigned __int32 addr,int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	return vec_camins[camNum]->ReadReg(addr);
}
byte * pimagebuf=NULL;
int imgready=0;
int board1=0;
#ifdef _W1280
int cs_width=1280;
int cs_height=960;
#endif
#ifdef _W640
int cs_width=640;
int cs_height=480;
#endif
void __stdcall csCallBack(LPVOID lpParam,LPVOID lpUser)
{

	imgFrame *thisFrame=(imgFrame*)lpParam;
	if(thisFrame->m_camNum==0)
	{
		memcpy(pimagebuf,thisFrame->imgBuf,thisFrame->m_width*thisFrame->m_height);
		imgready=1;
	}

}
int csInit()
{

	pimagebuf=new byte[cs_width*cs_height];
	board1=addInstance(NULL,csCallBack);
	clientPropStruct prop;
	prop.MACaddr=0xabcec0a8019b;
	prop.camIP=htonl(inet_addr("192.168.1.2"));
	prop.pcIP=htonl(inet_addr("192.168.1.3"));
	prop.interval_time=4096;
	prop.packetSize=1024;
	prop.camCnt=6;
	prop.width=cs_width;
	prop.height=cs_height;
	sendProp(prop,board1,1);
	board1=initCCTAPI(board1);
	startCap(cs_height,cs_width,board1);
	camPropStruct camprop;
	camprop.trigMode=1;//set softtrig as default
	camprop.expo=300;
	camprop.brightness=16;
	camprop.col=cs_width;
	camprop.row=cs_height;
	camprop.mirror=0;
	sendOrder(camprop,board1,0);
	sendSoftTrig(board1,1);//set soft trig as default
	return 1;
}
int csStop()
{
	stopCap(board1);
	return 1;
}
int csGetFrame(unsigned char * buff)
{
	sendSoftTrig(board1,2);
	while(!imgready)
	{
		Sleep(10);
	}
	Sleep(100);
	memcpy(buff,pimagebuf,cs_height*cs_width);
	//cv::Mat frameGray(cv_height,cv_width,CV_8UC1,pimagebuf);
	//cv::Mat frameRGB;
	//cv::cvtColor(frameGray,frameRGB,CV_BayerBG2BGR);
	imgready=0;
	return cs_height*cs_width;
}
