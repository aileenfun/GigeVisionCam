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
	GigEcamInstance*this_camInstance = new GigEcamInstance(lpUser, CallBackFunc, info);
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
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
		
	camNum = camNum - 1;
	
	if (vec_camins[camNum]->b_connected)
	{
		return vec_camins[camNum]->start();
	}
	return -1;
}
int GigEstartCap_HZC(int camNum)
{
	if (camNum < 1)return camNum;
	if (camNum > vec_camins.size())return -2;

	camNum = camNum - 1;

	if (vec_camins[camNum]->b_connected)
	{
		return vec_camins[camNum]->startHZC();
	}
	return -1;
}
int GigEsetMirrorType(GigEDataProcessType mirrortype, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	vec_camins[camNum]->setMirrorType(mirrortype);
	return 0;
}
int GigEstopCap(int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;

	return vec_camins[camNum]->stop();
}
int GigEgetFrameCnt(int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	if (vec_camins[camNum]->isRunning())
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
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	if (vec_camins[camNum]->isRunning())
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
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	if (vec_camins[camNum]->isRunning())
	{
		return vec_camins[camNum]->getErrPackCnt();
	}
	else
	{
		return -1;
	}
}
int GigEsendOrder(GigEcamPropStruct camprop, int camNum, int s)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	vec_camins[camNum]->sendOrder(camprop, s);
	return 0;
}

int GigEsendProp(GigEclientPropStruct prop, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	vec_camins[camNum]->sendProp(prop);
	return 0;
}
int GigEsetTrigMode(int s, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	vec_camins[camNum]->setTrigMode(s);
	return 0;
}

int GigEcloseConnection(int &camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	vec_camins[camNum]->closeConnection();
	delete vec_camins[camNum];
	vector<GigEcamInstance*>::iterator k = vec_camins.begin() + camNum;
	vec_camins.erase(k);
	//camNum=-camNum;
	return 0;
}
int GigEsetIP(CCHCamera *devinfo, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
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
unsigned __int32 GigEWriteReg(unsigned __int32 addr, unsigned __int32 data, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->WriteReg(addr, data);
}
unsigned __int32 GigEReadReg(unsigned __int32 addr, uint32_t *data, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->ReadReg(addr, data);
}
int GigEsendSoftTrig(int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->sendSoftTrig();
}
int GigEsetAuto(int isauto, int camNum)
{
	return vec_camins[camNum]->setAuto(isauto);
}
int GigEsetGain(uint32_t value, int isauto, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setGain(value, isauto);
}
int GigEsetExpo(uint32_t value, int isauto, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setExpo(value, isauto);
}
int GigEsetFreq(uint32_t value, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setFreq(value);
}
int GigEsetWB(uint32_t rvalue, uint32_t gvalue, uint32_t g2value, uint32_t bvalue, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setWB(rvalue, gvalue, g2value, bvalue);
}
int GigEsetCamSize(int camsize, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setCamSize(camsize);
}
int GigEgetCamSize(unsigned int *camsize, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->getCamSize(camsize);
}
int GigEsetROI(int xstart, int xend, int ystart, int yend, int enable, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setROI(xstart, xend, ystart, yend, enable);
}
int GigEsetSkip(int enable, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setSkip(enable);
}
int GigEsetBinning(int enable, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setBinning(enable);
}
int GigEsetTrigThreshold(int n,int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setTrigThreshold(n);
}
int GigEsetPWM(int perc, int freq, int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setPWM(perc,freq);
}
int GigEsetROIEn(int enable,int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setROIEn(enable);

}
int GigEsetGain_HZC(uint32_t value, int idx,int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setGain_HZC(value, idx);
}
int GigEsetResolu_HZC(int value,int camNum)
{
	if (camNum<1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	if(value>1)
		value=1;
	if(value<0)
		value=0;
	return vec_camins[camNum]->setResolu_HZC(value);
}
int GigEsetIOLength_MY(uint32_t us, int camNum)
{
	if (camNum < 1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setIOLength_MY(us);
}
int GigEsetLightOn_XD(int s, int camNum)
{
	if (camNum < 1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setLightOn_XD(s);
}
int GigEsetLightLen_XD(uint32_t len, int camNum)
{
	if (camNum < 1)return camNum;
	if (camNum > vec_camins.size())return -2;
	camNum = camNum - 1;
	return vec_camins[camNum]->setLightLen_XD(len);
}
byte * pimagebuf = NULL;
int imgready = 0;
int board1 = 1;
#ifdef _W1280
int cs_width = 1280;
int cs_height = 1028;
#endif
#ifdef _W640
int cs_width = 640;
int cs_height = 480;
#endif
map_camera *cameralist;

csCallBackFuncDel Handler = 0;

void __stdcall csCallBack(LPVOID lpParam, LPVOID lpUser)
{

	GigEimgFrame *thisFrame = (GigEimgFrame*)lpParam;
	if (thisFrame->m_camNum == 0)
	{
		//memset(pimagebuf, 11, thisFrame->m_height*thisFrame->m_width);
		memcpy(pimagebuf, thisFrame->imgBuf, thisFrame->m_width*thisFrame->m_height);
		//imgready=1;
		Handler(pimagebuf);
	}

}
int csInit(csCallBackFuncDel cb, int w = 1280, int h = 1028)
{
	Handler = cb;
	cs_width = w;
	cs_height = h;
	pimagebuf = new byte[cs_width*cs_height];
	memset(pimagebuf, 0, cs_width*cs_height);
	cameralist = new map_camera();
	GigEsearchCamera(cameralist);//Ã¶¾ÙÏà»ú
	int count = 0;
	map_camera::iterator itr;
	itr = cameralist->begin();
	if (itr != cameralist->end())
	{
		CCHCamera *c0 = itr->second;
		board1 = GigEaddInstance(NULL, csCallBack, c0);
		if (board1>0)
			GigEsetCamSize(1, board1);
		return board1;
	}
	else
	{
		return -1;
	}
}
int csSetROI(int xstart, int xend, int ystart, int yend, int enable)
{
	GigEsetROI(xstart, xend, ystart, yend, enable, board1);

	return 1;
}
int csSetExpo(uint32_t value, int isauto)
{
	GigEsetExpo(value, isauto, board1);
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

///////parameter setting for Laser center point
/*
Gaussian parameters:

----------------
| c  | b | c  |
----------------
| b  | a | b  |
----------------
| c  | b | c  |
----------------
*/

int csSetGaussianA(uint8_t a)
{

	return GigEWriteReg(0x33bb0100, a, board1);
}
int csSetGaussianB(uint8_t b)
{
	return GigEWriteReg(0x33bb0104, b, board1);
}
int csSetGaussianC(uint8_t c)
{
	return GigEWriteReg(0x33bb0108, c, board1);
}
int csSetMaxBrightnessThreshold(uint8_t data)
{
	return GigEWriteReg(0x33bb010c, data, board1);
}
int csSetMaxLineWidth(uint32_t data)
{
	return GigEWriteReg(0x33bb0110, data, board1);
}
int csSetMinLineWidth(uint32_t data)
{
	return GigEWriteReg(0x33bb0114, data, board1);
}