#include "StdAfx.h"
#include "DataProcess.h"
#define MSG_CMD_DESTORY		0
#define MSG_CMD_SINGLE		1
#define MSG_CMD_RUN			2
#define MSG_CMD_STOP		3

#define MSG_CMD_CHANGDisplayDataBytes	7
#define MSG_CMD_CHANGEAMPLIFIER		8
#define MSG_CMD_GETAVGCOUNT  9
#define MSG_DATA_HANDALE	10

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDataProcess::CDataProcess(wqueue<imgFrame*>&queue,LPVOID *lpUser)
	:cpm_queue(queue)
{
	m_hThread=NULL;
	//m_pPutMsg=NULL;
	//m_pGetMsg=NULL;
	
	m_hDC=0;
	lpcb=lpUser;
	memset(&m_BitmapInfo,0,sizeof(BITMAPINFO));
	m_BitmapInfo.bmiHeader.biBitCount=24;
	m_BitmapInfo.bmiHeader.biClrImportant=0;
	m_BitmapInfo.bmiHeader.biClrUsed=0;
	m_BitmapInfo.bmiHeader.biCompression=0;

	m_BitmapInfo.bmiHeader.biPlanes=1;
	m_BitmapInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

	m_BitmapInfo.bmiHeader.biXPelsPerMeter=0;
	m_BitmapInfo.bmiHeader.biYPelsPerMeter=0;

	m_bEnd=FALSE;
	m_lFrameCount=0;

	m_ProcType=Normal_Proc;

	m_ChangeType=Normal_Change;
	m_bCreateBmp=FALSE;
	m_temp=NULL;
	imData=NULL;
	camsize=6;	
}

CDataProcess::~CDataProcess(void)
{
		delete []m_Out;
		delete []m_In;
}

int CDataProcess::Open(int height,int width,CDataCapture* dp,LPMV_CALLBACK2 CallBackFunc)
{
	g_height=height;
	g_width=width;
	g_width_L=width;
	h_callback=CallBackFunc;
	m_bEnd=FALSE;
	m_In = new byte[g_height*g_width_L];
	m_processed=new byte[g_height*g_width_L];
	m_Out = new tagRGB[g_height*g_width];
	memset(m_In,0,sizeof(m_In));
	memset(m_Out,0,sizeof(m_Out));
	m_bEnd=FALSE;
	m_hThread = (HANDLE)_beginthreadex(NULL,0,ThreadProcess,this,0,NULL);
	m_temp=new byte[g_width*g_height];
	this_dp=dp;
	//this_trigimgpack=new TrigImgPack(6,2000,this_dp,h_callback);
	return 0;
}
int CDataProcess::Close()
{
	CGuard guard(m_Mutex);
	cpm_queue.reset();
	m_bEnd=TRUE;
	if(m_temp!=NULL)
	delete m_temp;
	return 0;
}

int CDataProcess::Input( VOID * pData,int dwSizes )
{
	CGuard guard(m_Mutex);
	if(m_bEnd)
		return -1;
	//m_pPutMsg->iDataSize = dwSizes;
	//m_pPutMsg->pAny=pData;
	//memcpy(m_pPutMsg->pData,pData,m_pPutMsg->iDataSize);
	//m_pPutMsg->iType = MSG_DATA_HANDALE;
	++m_lFrameCount;
	//return PutMessage(m_pPutMsg);
	return 0;
}

UINT_PTR hTimer=0;
VOID   __stdcall   TimerProc(HWND   hwnd,UINT   uMsg,UINT_PTR   idEvent,DWORD   dwTime)   
{   
	//KillTimer(NULL,hTimer);   
	//MessageBox(NULL,"Speak   in   Timer!",":)",MB_OK);   
	//MessageBeep(0xFFFFFFFF);
	//OutputDebugString(_T("timer1"));
} 

void CDataProcess::ThreadProcessFunction()
{
	int iRet=0;
	int endflag = 1;
	m_bEnd=false;
		
	MSG msg;
	 PeekMessage(&msg,   NULL,   NULL,   NULL,   PM_NOREMOVE);   
	 this_trigimgpack=new TrigImgPack(camsize,500,this_dp,h_callback);
	// hTimer=SetTimer(NULL,0,500,(TIMERPROC)TimerProc);
	while (m_bEnd==false)
	{
		/*if(PeekMessage(&msg,NULL,WM_TIMER,WM_USER+1,PM_NOREMOVE))
		{
			if(msg.message==WM_TIMER)
			OutputDebugString(_T("timer1"));
			DispatchMessage(&msg);
		}*/
		 CImgFrame=(imgFrame*)cpm_queue.remove();
		 this_trigimgpack->add(CImgFrame);
		//this_trigimgpack->onTimer(&msg);
		//OutPutWrapper(h_callback,this);
		//delete CImgFrame;

	}
}

unsigned int __stdcall CDataProcess::ThreadProcess( void *handle )
{
	CDataProcess *pThis = (CDataProcess *)handle;
	pThis->ThreadProcessFunction();
	return NULL;
}
int CDataProcess::OutPutWrapper(LPMV_CALLBACK2 CallBackFunc,LPVOID lpUser)
{
	CallBackFunc(CImgFrame,lpcb);
	return 0;
}



int CDataProcess::ProcessData()
{
	if(!m_bEnd)
	{
		switch(m_ProcType)
		{
		case Xmirror_Proc:
			DoXmirrorProc(m_In,m_processed,g_height,g_width);
			break;
		case Ymirror_Proc:
			DoYmirrorProc(m_In,m_processed,g_height,g_width);
			break;
		case XYmirror_Proc:
			
			DoXmirrorProc(m_In,m_temp,g_height,g_width);
			DoYmirrorProc(m_temp,m_processed,g_height,g_width);
			
			break;
		case Normal_Proc:
		default:
			DoNormal(m_In,m_processed,g_height,g_width);
			break;
		}
		//OutPutWrapper(h_callback,this);
		//CreateBmpFile();
		//m_BitmapInfo.bmiHeader.biSizeImage=g_width*g_height*3;//图片实际数据字节数
		//m_BitmapInfo.bmiHeader.biWidth=g_width;
		//m_BitmapInfo.bmiHeader.biHeight= g_height;
		//StretchDIBits(m_pDisplay->GetMemDC()->m_hDC,0,0,g_width,g_height,0,0,g_width,g_height,m_Out,&m_BitmapInfo,DIB_RGB_COLORS,SRCCOPY);
		////StretchDIBits(m_pDisplay->GetMemDC()->m_hDC,0,0,1280,720,0,0,g_height,g_width,m_Out,&m_BitmapInfo,DIB_RGB_COLORS,SRCCOPY);
		//m_pDisplay->Display();
		memset(m_In,0,g_width*g_height);
		memset(m_Out,0,g_width*g_height);
		memset(m_processed,0,g_width*g_height);
	}
	return 0;
}


int CDataProcess::ByteToRGB( byte *pIn ,tagRGB* pOut )
{
	static tagRGB sTempRgb;
#ifdef RGB565
	static tagRGB s565Rgb;
	byte maskGH=0x7;//0000,0111,low 3 bits
	byte maskGL=0xE0;//1110,0000,high 3 bits
	byte maskB=0x1F;//0001,1111,
	int x6=255/63;//4
	int x5=255/31;//8
	for(int i=0;i<g_height;i++)
	{
		for(int j=0;j<g_width_L;j+=2)//16bit data length 
		{
			s565Rgb.R=pIn[i * g_width_L + j]>>3;//R5
			s565Rgb.G=((pIn[i * g_width_L + j]&0x7)<<3)+((pIn[i * g_width_L + j+1]&0xE0)>>5);//G6
			s565Rgb.B=pIn[i * g_width_L + j+1]&0x1F;//B5
			
			sTempRgb.R=s565Rgb.R*x5;
			sTempRgb.G=s565Rgb.G*x6;
			sTempRgb.B=s565Rgb.B*x5;
			
			RgbChangeProc(pOut[i * g_width_L + j/2],sTempRgb);
		}
	}
#else
	for(int i=0;i<g_height;i+=2)
	{
		for(int j=0;j<g_width;j+=2)
		{
			sTempRgb.R= pIn[i * g_width + j];
			sTempRgb.G= (pIn[i * g_width + j+1]>>1)+(pIn[(i+1) * g_width + j]>>1);
			sTempRgb.B= pIn[(i+1) * g_width + j+1];
			RgbChangeProc(pOut[i * g_width + j],sTempRgb);
		}
	}
	for(int i=0;i<g_height;i+=2)
	{
		for(int j=1;j<g_width-1;j+=2)
		{
			sTempRgb.R= pIn[i * g_width + j+1];
			sTempRgb.G= (pIn[i * g_width + j]>>1)+(pIn[(i+1) * g_width + j+1]>>1);
			sTempRgb.B= pIn[(i+1) * g_width + j];
			RgbChangeProc(pOut[i * g_width + j],sTempRgb);
		}
	}
	for(int i=1;i<g_height-1;i+=2)
	{
		for(int j=0;j<g_width;j+=2)
		{
			sTempRgb.R= pIn[(i+1) * g_width + j];
			sTempRgb.G= (pIn[i * g_width + j]>>1)+(pIn[(i+1) * g_width + j+1]>>1);
			sTempRgb.B= pIn[i * g_width + j+1];
			RgbChangeProc(pOut[i * g_width + j],sTempRgb);
		}
	}
	for(int i=1;i<g_height-1;i+=2)
	{
		for(int j=1;j<g_width-1;j+=2)
		{
			sTempRgb.R= pIn[(i+1) * g_width + j+1];
			sTempRgb.G= (pIn[i * g_width + j+1]>>1)+(pIn[(i+1) * g_width + j]>>1);
			sTempRgb.B= pIn[i * g_width + j];
			RgbChangeProc(pOut[i * g_width + j],sTempRgb);
		}
	}
#endif
	return 0;
}

void CDataProcess::CloseMsgQueue()
{
}

int CDataProcess::GetFrameCount( int& fCount )
{
	CGuard guard(m_Mutex);
	fCount=m_lFrameCount;
	m_lFrameCount=0;
	return 0;
}

void CDataProcess::DoYmirrorProc(byte* pIn,byte * pOut,int h,int w)
{
	volatile byte tempbuf=0;
	for(int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			int idx=i*w+w-1-j;
			pOut[i*w+j]=pIn[idx];
		}
	}
}
void CDataProcess::DoYmirrorProc()
{
	tagRGB TempRgb;
	for(int i=0;i<g_height;++i)
	{
		for(int j=0;j<g_width/2;++j )
		{
			memcpy(&TempRgb,&m_Out[i*g_width + j],sizeof(tagRGB));
			memcpy(&m_Out[i*g_width + j],&m_Out[i*g_width + g_width-1-j],sizeof(tagRGB));
			memcpy(&m_Out[i*g_width + g_width-1-j],&TempRgb,sizeof(tagRGB));
		}
	}
}
void CDataProcess::DoNormal(byte* pIn,byte* pOut,int h,int w)
{
	memcpy(pOut,pIn,w*h);
}
void CDataProcess::DoXmirrorProc(byte* pIn,byte *pOut,int h,int w)
{
	//byte *tempbuf=new byte[w];
	
	for(int i=0;i<h;i++)
	{
		/*memcpy(tempbuf,&pIn[i*w],w);
		memcpy(&pIn[i*w],&pIn[(h-1-i)*w],w);
		memcpy(&pIn[(h-i-1)*w],tempbuf,w);
		memset(tempbuf,0,w);
		*/
		memcpy(&pOut[i*w],&pIn[(h-1-i)*w],w);
	}
	//delete []tempbuf;
}
void CDataProcess::DoXmirrorProc()
{
	tagRGB* TempRgbBuf = new tagRGB[g_width];//如果用到，考虑指针地址循环使用
	for(int i=0;i<g_height/2;++i)
	{
		memcpy(TempRgbBuf,&m_Out[i*g_width],sizeof(tagRGB)*g_width);
		memcpy(&m_Out[i*g_width],&m_Out[(g_height-1-i)*g_width],sizeof(tagRGB)*g_width);
		memcpy(&m_Out[(g_height-1-i)*g_width],TempRgbBuf,sizeof(tagRGB)*g_width);
		memset(TempRgbBuf,0,sizeof(TempRgbBuf));
	}
	delete []TempRgbBuf;
}

int CDataProcess::SetProcType( DataProcessType type )
{
	CGuard guard(m_Mutex);
	m_ProcType=type;
	return 0;
}

int CDataProcess::SetChangeType( RgbChangeType type )
{
	CGuard guard(m_Mutex);
	m_ChangeType=type;
	return 0;
}

void CDataProcess::RgbChangeProc( tagRGB& DestRgb,const tagRGB& OrgRgb )
{
	switch(m_ChangeType)
	{
	case RB_Change:
		DestRgb.R=OrgRgb.B;
		DestRgb.G=OrgRgb.G;
		DestRgb.B=OrgRgb.R;
		break;
	case RG_Change:
		DestRgb.R=OrgRgb.G;
		DestRgb.G=OrgRgb.R;
		DestRgb.B=OrgRgb.B;
		break;
	case GB_Change:
		DestRgb.R=OrgRgb.R;
		DestRgb.G=OrgRgb.B;
		DestRgb.B=OrgRgb.G;
		break;
	case Normal_Change:
	default:
		DestRgb.R=OrgRgb.R;
		DestRgb.G=OrgRgb.G;
		DestRgb.B=OrgRgb.B;
		break;
	}
} 
	void CDataProcess::setsofttrigmode(int m)
	{
		//this_trigimgpack->resetPack();
		this_trigimgpack->settrigmode(m);

	}
	void CDataProcess::softtrigonce()
	{
		this_trigimgpack->softtrigonce();
	}
	int CDataProcess::softtrigtimes()
	{
		return this_trigimgpack->softtrigtimes();
	}
int CDataProcess::setCamSize(int cs)
{
	camsize=cs;
	return camsize;
}