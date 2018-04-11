#pragma once
#include "TrigImgPack.h"
#include "GigEimgFrame.h"
#include "GigEwqueue.h"
#include <process.h>
#include <vector>
//extern int g_width_L;

//#define RGB565
//#define	Array2_Index(i,j)	  i*Video_Width+j
#ifdef RGB565
#define g_width_L                       g_width*2
#else

#endif
#define Sig_L							4
#define ReadDataBytes                  8192*512
typedef
VOID
(WINAPI * LPMV_CALLBACK2)(LPVOID lpParam, LPVOID lpUser);
enum GigEDataProcessType
{
	GigE_Normal_Proc, GigE_Xmirror_Proc, GigE_Ymirror_Proc, GigE_XYmirror_Proc
};
//void _stdcall RawCallBack(LPVOID lpParam,LPVOID lpUser);
enum GigERgbChangeType//R G B三元素互换处理
{
	GigE_Normal_Change, GigE_RG_Change, GigE_RB_Change, GigE_GB_Change
};
struct GigEImageMetaStruct
{
	int camNum;
	int row;
	int col;
	~GigEImageMetaStruct()
	{
		//delete p_image;
	}
};
struct tagRGB
{
	byte B;
	byte G;
	byte R;
	tagRGB()
	{
		memset(this,0,sizeof(*this));
	}
};
class GigECDataCapture;
class GigECDataProcess
{
	GigEwqueue<GigEimgFrame*>& cpm_queue;
public:
	GigECDataProcess(GigEwqueue<GigEimgFrame*>&queue,LPVOID *lpUser);
	~GigECDataProcess(void);

public:
	int Open(int height,int width, GigECDataCapture *dp,LPMV_CALLBACK2 CallBackFunc);
	int Close();
	int	Input(VOID * pData,int dwSizes);
	int GetFrameCount(int& fCount);
	int SetProcType(GigEDataProcessType type);
	int SetChangeType(GigERgbChangeType type);
	GigEimgFrame* CImgFrame;
	void setsofttrigmode(int m);
	void softtrigonce();
	int softtrigtimes();
	int setCamSize(int cs);
	//LPVOID lpcb;
private:
	void ThreadProcessFunction();
	static unsigned int __stdcall ThreadProcess(void *handle);
	LPVOID *lpcb;
private:
	//int ByteToRGB(byte pIn[Video_Height][Video_Width_L] ,tagRGB pOut[Video_Height][Video_Width]);
	int	ProcessData();
	int ByteToRGB(byte *pIn ,tagRGB* pOut);
	//int PutMessage(UD_MESSAGE *msg);
	void CloseMsgQueue();
	void DoNormal(byte* pIn,byte *pOut,int h,int w);
	void DoXmirrorProc();//X轴镜像处理
	void DoXmirrorProc(byte* pIn,byte *pOut,int h,int w);
	void DoYmirrorProc();//Y轴镜像处理
	void DoYmirrorProc(byte* pIn,byte* pOut,int h,int w);
	void RgbChangeProc(tagRGB& DestRgb,const tagRGB& OrgRgb);//RGB元素互换处理
	void CreateBmpFile();
	GigeC_Mutex	m_Mutex;
private:
	HANDLE m_hThread;
	//UD_MESSAGE *m_pPutMsg, *m_pGetMsg;
	//CMessageQueue m_MsgQueue;

	BOOL m_bEnd;
	int m_lFrameCount;//图像帧计数
	GigEDataProcessType m_ProcType;
	GigERgbChangeType m_ChangeType;
	int OutPutWrapper(LPMV_CALLBACK2 CallBackFunc,LPVOID lpUser);
	LPMV_CALLBACK2 h_callback;
		
private:
	BITMAPINFO	m_BitmapInfo;
	HDC			m_hDC;
	byte*       m_In;
	byte* m_processed;
	tagRGB*     m_Out;
	int g_height,g_width,g_width_L;
	BOOL        m_bCreateBmp;
	byte* m_temp;
	GigEImageMetaStruct *imData;
	TrigImgPack *this_trigimgpack;
	GigECDataCapture* this_dp;
	vector<LPMV_CALLBACK2> vec_callbackfunc;
	int camsize;
};
