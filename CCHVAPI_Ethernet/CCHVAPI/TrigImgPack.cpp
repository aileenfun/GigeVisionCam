#include "StdAfx.h"
#include "GigETrigImgPack.h"
#include "GigEDataCapture.h"
std::map<UINT_PTR, GigETrigImgPack*> GigETrigImgPack::m_TrigImgPackMap;
void GigETrigImgPack::add(GigEimgFrame* imgFrame)
{
	if(imgFrame==NULL)
	{
		Sleep(1);
		return;
	}
	if(imgFrame->m_camNum>=camSize)
	{
		delete imgFrame;
		return;
	}
	if(imgPack.size()>0&&imgPack[imgFrame->m_camNum]!=NULL)// frame lost
	{
		resetPack();
	}
	if(recved==0)
	{
		InitPack();
	}

	if(imgFrame!=NULL&&(imgPack[imgFrame->m_camNum]==NULL))
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
		
		if(softtrigmode)
		{
			if(softtrig)
			{
				softtrig--;
				consumePack(h_callback,this);
			}
		}
		else
		{
			consumePack(h_callback,this);
		}
		resetPack();
	}
}
bool  GigETrigImgPack::getAllPack()
{
	if(recved==camSize)
	{
		long ts=imgPack[0]->timestamp;
		//for(int i=1;i<camSize-1;i++)
		//{
		//	//if(imgPack[i]->status!=1)
		//	if(0)
		//	{
		//		b_getallpack=false;
		//		retry++;
		//		return false;
		//		break;
		//	}
		//}
		b_getallpack=true;
		return true;
	}
	return false;
}
