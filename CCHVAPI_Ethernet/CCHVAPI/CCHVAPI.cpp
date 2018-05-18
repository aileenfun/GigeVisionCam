//

#include "stdafx.h"
#include "CCHVAPI.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
vector<GigEcamInstance*>vec_camins;
GigEcamInstance searchCamIns;
int GigEaddInstance(LPVOID *lpUser, LPMV_CALLBACK2 CallBackFunc, CCHCamera *info)
{
	GigEcamInstance*this_camInstance = new GigEcamInstance(lpUser, CallBackFunc, info );
	int rst = 0;
	if (this_camInstance->initEth(info)<0)
	{
		//rst=this_camInstance->connect(info);//move to start()
		return -1;
	}
		vec_camins.push_back(this_camInstance);
		
		return vec_camins.size();

	
}

int GigEstartCap(int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	
	if(vec_camins[camNum]->b_connected)
	{
		return vec_camins[camNum]->start();
	}
	return -1;
}
int GigEsetMirrorType(GigEDataProcessType mirrortype,int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->setMirrorType(mirrortype);
	return 0;
}
int GigEstopCap(int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	if(camNum>=vec_camins.size())
		return -1;
	
	return vec_camins[camNum]->stop();
}
int GigEgetFrameCnt(int camNum)
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
int GigEgetDataCnt(int camNum)
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
long GigEgetErrPackCnt(int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	if(vec_camins[camNum]->isRunning())
	{
		return vec_camins[camNum]->getErrPackCnt();
	}
	else
	{
		return -1;
	}
}
int GigEsendOrder(GigEcamPropStruct camprop,int camNum,int s)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->sendOrder(camprop,s);
	return 0;
}

int GigEsendProp(GigEclientPropStruct prop,int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->sendProp(prop);
	return 0;
}
int GigEsetTrigMode(int s,int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->setTrigMode(s);
	return 0;
}

int GigEcloseConnection(int &camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	vec_camins[camNum]->closeConnection();
	delete vec_camins[camNum];
	vector<GigEcamInstance*>::iterator k =vec_camins.begin()+camNum;
	vec_camins.erase(k);
	//camNum=-camNum;
	return 0;
}
int GigEsetIP(CCHCamera *devinfo,int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setIP(devinfo);
}
int GigEforceIP(CCHCamera *devinfo)
{
	return searchCamIns.forceIP(devinfo);
}
int GigEsearchCamera(map_camera * camlist)
{
	//may need to restart searchCamIns
	 searchCamIns.searchCamera(camlist);
	return camlist->size();
}
unsigned __int32 GigEWriteReg(unsigned __int32 addr, unsigned __int32 data,int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	return vec_camins[camNum]->WriteReg(addr,data);
}
unsigned __int32 GigEReadReg(unsigned __int32 addr,uint32_t *data,int camNum)
{
	if(camNum<1)return camNum;
	camNum=camNum-1;
	return vec_camins[camNum]->ReadReg(addr,data);
}
int GigEsendSoftTrig(int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->sendSoftTrig();
}
int GigEsetGain(uint32_t value,int isauto,int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setGain(value,isauto);
}
int GigEsetExpo(uint32_t value,int isauto,int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setExpo(value,isauto);
}
int GigEsetFreq(uint32_t value,int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setFreq(value);
}
int GigEsetWB(uint32_t rvalue,uint32_t gvalue,uint32_t g2value,uint32_t bvalue,int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setWB(rvalue,gvalue,g2value,bvalue);
}
int GigEsetCamSize(int camsize,int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setCamSize(camsize);
}
int GigEsetROI(int xstart, int xend, int ystart, int yend, int enable, int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setROI(xstart, xend, ystart, yend, enable);
}
int GigEsetSkip(int enable,int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setSkip(enable);
}
int GigEsetBinning(int enable,int camNum)
{
	if (camNum<1)return camNum;
	camNum = camNum - 1;
	return vec_camins[camNum]->setBinning(enable);
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
map_camera *cameralist;

csCallBackFuncDel Handler = 0;

void __stdcall csCallBack(LPVOID lpParam, LPVOID lpUser)
{

	GigEimgFrame *thisFrame = (GigEimgFrame*)lpParam;
	if (thisFrame->m_camNum == 0)
	{
		memcpy(pimagebuf, thisFrame->imgBuf, thisFrame->m_width*thisFrame->m_height);
		//imgready=1;
		Handler(pimagebuf);
	}

}
int csInit(csCallBackFuncDel cb, int w = 1280, int h = 960)
{
	Handler = cb;
	cs_width = w;
	cs_height = h;
	pimagebuf = new byte[cs_width*cs_height];

	cameralist = new map_camera();
	GigEsearchCamera(cameralist);//ö�����
	int count = 0;
	map_camera::iterator itr;
	itr = cameralist->begin();


	if (itr != cameralist->end())
	{
		CCHCamera *c0 = itr->second;
		board1 = GigEaddInstance(NULL, csCallBack, c0);
		return board1;
	}
	else
	{
		return -1;
	}
}
int csSetROI(int xstart, int xend, int ystart, int yend, int enable)
{
	GigEsetROI(xstart, xend, ystart, yend, enable,board1);

	return 1;
}
int csSetExpo(uint32_t value, int isauto)
{
	GigEsetGain(value, isauto, board1);
	return 1;
}
int csStart()
{
	int temp = 0;
	if (board1>0)
		temp = GigEstartCap(board1);
	return temp;
}
int csStop()
{
	GigEstopCap(board1);
	return 1;
}
int csGetFrame(unsigned char * buff)
{
	//sendSoftTrig(board1,2);
	while (!imgready)
	{
		Sleep(1);
	}
	//Sleep(100);
	memcpy(buff, pimagebuf, cs_height*cs_width);
	//cv::Mat frameGray(cv_height,cv_width,CV_8UC1,pimagebuf);
	//cv::Mat frameRGB;
	//cv::cvtColor(frameGray,frameRGB,CV_BayerBG2BGR);
	imgready = 0;
	return cs_height*cs_width;
}