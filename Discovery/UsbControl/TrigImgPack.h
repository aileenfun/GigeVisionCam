#pragma once
#include <vector>
#include <imgFrame.h>
#include <map>
#include <CCTAPI.h>
#include <CCTAPI.h>
using namespace std;

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
public:
	bool timeup;
	 static std::map<UINT_PTR, TrigImgPack*> m_TrigImgPackMap; //declaration
	 bool b_getallpack;
	TrigImgPack(int cs,int ap,LPMV_CALLBACK2 CallBackFunc)
	{
		recved=0;
		camSize=cs;
		alarmPeriod=ap;
		h_callback=CallBackFunc;
	}
	~TrigImgPack(void);
	void InitPack()
	{
		recved=0;
		retry=0;
		b_getallpack=false;
		timeup=false;
		for(int i=0;i<camSize-1;i++)
		{
			imgFrame *this_imgFrame=NULL;
			imgPack.push_back(this_imgFrame);
		}
		id=SetTimer(0,1,alarmPeriod,(TIMERPROC)timerCallback);
				m_TrigImgPackMap[id]=this;
	}
	void add(imgFrame* imgFrame)
	{
		if(recved==0)
		{
			InitPack();
		}
		
		if(imgPack[imgFrame->m_camNum]==NULL)
		{
			imgPack[imgFrame->m_camNum]=imgFrame;
			recved++;
		}
		else
		{
			delete imgFrame;
		}
		if(getAllPack())
		{
			KillTimer(0,id);
			consumePack(h_callback,this);
			resetPack();
			//sendSoftTrig(1);
			
		}
	}
	void consumePack(LPMV_CALLBACK2 CallBackFunc,LPVOID lpUser)
	{
		for(int i=0;i<camSize-1;i++)
		{
			CallBackFunc(imgPack.front(),this);
		}
	}
	void resetPack()
	{
		recved=0;
		retry=0;
		imgPack.clear();
	}
	bool getAllPack()
	{
		if(recved==6)
		{
			long ts=imgPack[0]->timestamp;
			for(int i=1;i<camSize-1;i++)
			{
				if(0)//imgPack[i]->timestamp!=ts)
				{
					b_getallpack=false;
					retry++;
					//sendSoftTrig(2);
					return false;
					break;
				}
			}
			b_getallpack=true;
			return true;
		}
		return false;
	}
	static VOID CALLBACK timerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		sendSoftTrig(2);
	}
};

