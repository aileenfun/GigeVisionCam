#pragma once
//#include "DataCapture.h"
#include <vector>
#include "imgFrame.h"
#include <map>
#include "Mutex.h"
using namespace std;
typedef
VOID
	(WINAPI * LPMV_CALLBACK2)(LPVOID lpParam, LPVOID lpUser);
class CDataCapture;
class TrigImgPack
{
private:
	
	vector<imgFrame*> imgPack;
	int camSize;
	int retry;
	int alarmPeriod;
	int recved;
	UINT_PTR id;
	LPMV_CALLBACK2 h_callback;
	C_Mutex m_Mutex_msg;
	int lastget;
	int softtrigmode;
	int softtrig;
public:
	bool timeup;
	 static std::map<UINT_PTR, TrigImgPack*> m_TrigImgPackMap; //declaration
	 bool b_getallpack;
	 CDataCapture* this_dp;
	TrigImgPack(int cs,int ap,CDataCapture *dp,LPMV_CALLBACK2 CallBackFunc)
	{
		recved=0;
		camSize=cs;
		alarmPeriod=ap;
		h_callback=CallBackFunc;
		this_dp=dp;
		softtrigmode=0;
		softtrig=0;
		m_TrigImgPackMap[id]=this;
	}
	~TrigImgPack(void);
	void InitPack()
	{
		recved=0;
		retry=0;
		b_getallpack=false;
		timeup=false;
		for(int i=0;i<camSize;i++)
		{
			imgFrame *this_imgFrame=NULL;
			imgPack.push_back(this_imgFrame);
		}
				
	}
	void add(imgFrame* imgFrame);
	bool getAllPack();
	void settrigmode(int trigmode)
	{
		switch(trigmode)
		{
		case 0://free run
			softtrigmode=0;
			break;
		case 1://extern trig
			softtrigmode=0;
			resetPack();
			break;
		case 2://
			softtrigmode=1;
			break;
		}
		
	}
	int softtrigonce()
	{
		softtrig>0?0:softtrig++;
		return softtrig;
		
	}
	int softtrigtimes()
	{
		return softtrig;
	}
	void consumePack(LPMV_CALLBACK2 CallBackFunc,LPVOID lpUser)
	{
		
		for(int i=0;i<camSize;i++)
		{
			if(imgPack.size()!=0)
			CallBackFunc(imgPack[i],this);
		}
		
	}
	void resetPack()
	{
		recved=0;
		retry=0;
		//Sleep(1);
		for(int i=0;i<imgPack.size();i++)
		{
			if(imgPack[i]!=NULL)
			delete imgPack[i];
		}
		imgPack.clear();
	}

	
};

