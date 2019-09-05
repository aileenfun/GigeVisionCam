// UsbControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UsbControl.h"	
#include "UsbControlDlg.h"
//#include "afxdialogex.h"
#include "propFromFileExm.h"
//#include "clientProp.h"
#include "Utility.h"
#include <cstdlib>
#include "Ce2Writer.h"
#include <fstream>
#include <iostream>
#include "Mmsystem.h"
#pragma comment(lib,"winmm.lib")
#define _CAM1
//#define _CAM2
//#define _CAM3
//#define _CAM4
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
volatile int b_softtrig=-1;
#define _ORG
cv::VideoWriter h_vw;
volatile bool snap;
int board1=-1;//board id
int board2=-1;
int board3 = -1;
int board4 = -1;
#ifdef _W640
int g_width=640;
int g_height=480;
#endif
#ifdef _W1280
#endif

unsigned long sendSoftCnt=0;
unsigned long recvSoftCnt=0;
unsigned long recvHardTrigCnt = 0;
unsigned long recvSoftCnt2 = 0;
unsigned long recvSoftCnt3 = 0;
unsigned long recvSoftCnt4 = 0;
unsigned long lastLostCnt=0;
int f_softtirg=0;
int f_hardtrig = 0;
unsigned int g_camsize = 1;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CUsbControlDlg 对话框




CUsbControlDlg::CUsbControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbControlDlg::IDD, pParent)
	, m_iProcType(0)
	, m_sEdit_Width(_T(""))
	, m_sEdit_Height(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFileRbf=NULL;
	m_bUsbOpen=FALSE;
	m_bSendData=FALSE;
	memset(m_byData,0,sizeof(BYTE)*64);
	m_pVideoDlg=NULL;

	m_hDisplayDC=NULL;
	m_hThread=NULL;
	m_pReadBuff=new char[ReadDataBytes];
	memset(m_pReadBuff,0,sizeof(char)*ReadDataBytes);

	m_bCloseWnd=FALSE;
	m_lBytePerSecond=0;
	m_pBrush=NULL;
	m_pVideoDataFile=NULL;
	m_bReview=FALSE;
	m_bSave=FALSE;
	m_Init = FALSE;
	snap=false;
	lastDataCnt = 0;
	lastFrameCnt = 0;

	lastDataCnt2 = 0;
	lastFrameCnt2 = 0;

	lastDataCnt3 = 0;
	lastFrameCnt3 = 0;

	lastDataCnt4 = 0;
	lastFrameCnt4 = 0;

	cameralist = new map_camera();
}

CUsbControlDlg::~CUsbControlDlg()
{
	if(m_pReadBuff!=NULL)
	{
		delete[] m_pReadBuff;
		m_pReadBuff=NULL;
	}
}

void CUsbControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NORMAL, m_iProcType);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, select_channel);
	DDX_Control(pDX, IDC_CHECK_SAVE, check_save_file);
	DDX_Control(pDX, IDC_EDIT_EXPO, m_eEXPO);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_eGAIN);
	DDX_Control(pDX, IDC_IPADDRESS_CAM, m_eCAMIP);
	DDX_Control(pDX, IDC_IPADDRESS_GATE, m_eGATE);
	DDX_Control(pDX, IDC_IPADDRESS_SUBNET, m_eSUBNET);
	DDX_Control(pDX, IDC_COMBO_TRIG, m_combo_trig);
	DDX_Control(pDX, IDC_LIST1, listbox);
	DDX_Control(pDX, IDC_CHECK_Gain, autogain);
	DDX_Control(pDX, IDC_CHECK_EXPO, autoexpo);
	DDX_Control(pDX, IDC_EDIT_TrigFreq, m_eFreq);
	DDX_Control(pDX, IDC_EDIT_rgain, m_eRGain);
	DDX_Control(pDX, IDC_EDIT_ggain, m_eGGain);
	DDX_Control(pDX, IDC_EDIT_bgain, m_eBGain);
	DDX_Control(pDX, IDC_static_imgctrl, gb_imgctrl);
	DDX_Control(pDX, IDC_EDIT_ggain2, m_eGGain2);
	DDX_Control(pDX, IDC_EDIT_regaddr, m_eRegAddr);
	DDX_Control(pDX, IDC_EDIT_regdata, m_eRegData);
	DDX_Control(pDX, IDC_EDIT_camsize, m_ecamsize);
	DDX_Control(pDX, IDC_CHECK_roienable, m_cb_roienable);
	DDX_Control(pDX, IDC_EDIT_roixstart, m_eroixstart);
	DDX_Control(pDX, IDC_EDIT_roixend, m_eroixend);
	DDX_Control(pDX, IDC_EDIT_roiystart, m_eroiystart);
	DDX_Control(pDX, IDC_EDIT_roiyend, m_eroiyend);
	DDX_Control(pDX, IDC_CHECK_binning, m_cb_binning);
	DDX_Control(pDX, IDC_CHECK_skip, m_cb_skip);
	DDX_Control(pDX, IDC_IPADDRESS_PCIP, m_ePCIp);
	DDX_Control(pDX, IDC_IPADDRESS_PCIP2, m_ePCIp2);
	DDX_Control(pDX, IDC_IPADDRESS_CAM3, m_eCAMIP2);
	DDX_Control(pDX, IDC_IPADDRESS_SUBNET2, m_eSUBNET2);
	DDX_Control(pDX, IDC_IPADDRESS_GATE2, m_eGATE2);
}

BEGIN_MESSAGE_MAP(CUsbControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_VIDEOCAPTURE, &CUsbControlDlg::OnBnClickedBtnVideocapture)
	ON_BN_CLICKED(IDC_BTN_STOPCAPTURE, &CUsbControlDlg::OnBnClickedBtnStopcapture)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_NORMAL, &CUsbControlDlg::OnBnClickedRadioProcType)
	ON_BN_CLICKED(IDC_RADIO_XMIRROR, &CUsbControlDlg::OnBnClickedRadioProcType)
	ON_BN_CLICKED(IDC_RADIO_YMIRROR, &CUsbControlDlg::OnBnClickedRadioProcType)
	ON_BN_CLICKED(IDC_RADIO_XYMIRROR, &CUsbControlDlg::OnBnClickedRadioProcType)
	ON_BN_CLICKED(IDC_BTN_SNAP, &CUsbControlDlg::OnBnClickedBtnSnap)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, &CUsbControlDlg::OnCbnSelchangeComboChannel)
	ON_BN_CLICKED(IDC_CHECK_SAVE, &CUsbControlDlg::OnBnClickedCheckSave)
	ON_BN_CLICKED(IDC_BUTTON_SETIP, &CUsbControlDlg::OnBnClickedButtonSetIP)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CUsbControlDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CUsbControlDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_CLOSECON, &CUsbControlDlg::OnBnClickedBtnClosecon)
    ON_BN_CLICKED(IDC_BTNW, &CUsbControlDlg::OnBnClickedBtnw)
 
	ON_LBN_SELCHANGE(IDC_LIST1, &CUsbControlDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BTN_TRIG, &CUsbControlDlg::OnBnClickedBtnTrig)
	ON_BN_CLICKED(IDC_CHECK_Gain, &CUsbControlDlg::OnBnClickedCheckGain)
	ON_BN_CLICKED(IDC_CHECK_EXPO, &CUsbControlDlg::OnBnClickedCheckExpo)
	ON_BN_CLICKED(IDC_BUTTON_SENDgain, &CUsbControlDlg::OnBnClickedButtonSendgain)
	ON_BN_CLICKED(IDC_BUTTON_SENDexpo, &CUsbControlDlg::OnBnClickedButtonSendexpo)
	ON_BN_CLICKED(IDC_BTN_WBSet, &CUsbControlDlg::OnBnClickedBtnWbset2)
	ON_BN_CLICKED(IDC_BUTTON_SENDgain2, &CUsbControlDlg::OnBnClickedButtonSendgain2)
	ON_BN_CLICKED(IDC_BTNregread, &CUsbControlDlg::OnBnClickedBtnregread)
	ON_BN_CLICKED(IDC_BTNregwrite, &CUsbControlDlg::OnBnClickedBtnregwrite)

	ON_BN_CLICKED(IDC_BUTTON_SENDcamsize, &CUsbControlDlg::OnBnClickedButtonSendcamsize)
	ON_BN_CLICKED(IDC_BTN_roiset, &CUsbControlDlg::OnBnClickedBtnroiset)
	ON_BN_CLICKED(IDC_CHECK_binning, &CUsbControlDlg::OnBnClickedCheckbinning)
	ON_BN_CLICKED(IDC_CHECK_skip, &CUsbControlDlg::OnBnClickedCheckskip)
	ON_BN_CLICKED(IDC_CHECK_roienable, &CUsbControlDlg::OnBnClickedCheckroienable)
	ON_BN_CLICKED(IDC_BUTTON_ForceIp, &CUsbControlDlg::OnBnClickedButtonForceip)
	ON_BN_CLICKED(IDC_BTN_WBSet2, &CUsbControlDlg::OnBnClickedBtnWbset2)
	ON_BN_CLICKED(btn_test, &CUsbControlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_TRIG2, &CUsbControlDlg::OnBnClickedBtnTrig2)
	ON_BN_CLICKED(IDC_BTN_minset, &CUsbControlDlg::OnBnClickedBtnminset)

	ON_BN_CLICKED(btn_resolu, &CUsbControlDlg::OnBnClickedresolu)
END_MESSAGE_MAP()


// CUsbControlDlg 消息处理程序
BOOL CUsbControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	CRect cRect,wRect,mRect;
	GetDesktopWindow()->GetWindowRect(wRect);
	GetWindowRect(cRect);
	mRect.right=wRect.right-50;
	mRect.bottom=wRect.bottom-50;
	mRect.left=mRect.right-cRect.Width();
	mRect.top=mRect.bottom-cRect.Height();
	MoveWindow(mRect);

	m_pBrush=new CBrush[2];

	select_channel.InsertString(0,_T("0"));
	select_channel.InsertString(1,_T("1"));
	select_channel.InsertString(2,_T("2"));
	select_channel.InsertString(3,_T("3"));
	select_channel.InsertString(4,_T("4"));
	select_channel.InsertString(5,_T("5"));
	select_channel.SetCurSel(0);
	check_save_file.SetCheck(0);

	m_combo_trig.InsertString(0,_T("Auto"));
	m_combo_trig.InsertString(1,_T("Fpga Trig"));
	m_combo_trig.InsertString(2,_T("Soft Trig"));
	m_combo_trig.InsertString(3,_T("Out Trig"));
	m_combo_trig.SetCurSel(0);

	m_eEXPO.SetWindowTextW(_T("300"));
	m_eGAIN.SetWindowTextW(_T("16"));

	m_ecamsize.SetWindowTextW(_T("6"));
	
	SetDlgItemText(IDC_EDITA, _T("152"));
	SetDlgItemText(IDC_EDITB, _T("121"));
	SetDlgItemText(IDC_EDITC, _T("97"));
	SetDlgItemText(IDC_EDITMAXP, _T("200"));
	SetDlgItemText(IDC_EDITMAXW, _T("150"));
	SetDlgItemText(IDC_EDITMINW, _T("1"));
	autogain.SetCheck(1);
	autoexpo.SetCheck(1);
	m_eEXPO.EnableWindow(0);
	m_eGAIN.EnableWindow(0);
	gb_imgctrl.EnableWindow(0);
	m_cb_roienable.SetCheck(0);
	m_eroixstart.EnableWindow(1);
	m_eroixend.EnableWindow(1);
	m_eroiystart.EnableWindow(1);
	m_eroiyend.EnableWindow(1);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUsbControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();//加载控制台窗口模型
		int temp1 = 1;
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CUsbControlDlg::BMPHeader(int lWidth, int lHeight,byte* m_buf,BITMAPINFO* m_bmi)
{
	int mlBpp=8;
	bool lReverse=false;
	BITMAPFILEHEADER bhh;
	BITMAPINFOHEADER bih;
	
	memset(m_bmi,0,sizeof(BITMAPINFO));
	memset(&bhh,0,sizeof(BITMAPFILEHEADER));
	memset(&bih,0,sizeof(BITMAPINFOHEADER));

	int widthStep				=	(((lWidth * mlBpp) + 31) & (~31)) / 8; //每行实际占用的大小（每行都被填充到一个4字节边界）
	int QUADSize 				= 	mlBpp==8?sizeof(RGBQUAD)*256:0;

	//构造彩色图的文件头
	bhh.bfOffBits				=	(DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + QUADSize; 
	bhh.bfSize					=	sizeof(m_bmi->bmiHeader);//(DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + QUADSize + widthStep*lHeight;  
	bhh.bfReserved1				=	0; 
	bhh.bfReserved2				=	0;
	bhh.bfType					=	0x4d42;  

	//构造彩色图的信息头
	bih.biBitCount				=	mlBpp;
	bih.biSize					=	sizeof(BITMAPINFOHEADER);
	bih.biHeight				=	(lReverse?-1:1)*lHeight;
	bih.biWidth					=	lWidth;  
	bih.biPlanes				=	1;
	bih.biCompression			=	BI_RGB;
	bih.biSizeImage				=	widthStep*lHeight;  
	bih.biXPelsPerMeter			=	0;  
	bih.biYPelsPerMeter			=	0;  
	bih.biClrUsed				=	0;  
	bih.biClrImportant			=	0;  
	
	//构造灰度图的调色版
	RGBQUAD rgbquad[256];
	if(mlBpp==8)
	{
		for(int i=0;i<256;i++)
		{
		rgbquad[i].rgbBlue=i;
		rgbquad[i].rgbGreen=i;
		rgbquad[i].rgbRed=i;
		rgbquad[i].rgbReserved=0;
		m_bmi->bmiColors[i].rgbRed=i;
		m_bmi->bmiColors[i].rgbGreen=i;
		m_bmi->bmiColors[i].rgbBlue=i;
		m_bmi->bmiColors[i].rgbReserved=0;
		}
		
	}

	int DIBSize = widthStep * lHeight;

	byte* bmpbuffer=new byte[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256+DIBSize];
	memcpy(bmpbuffer,(LPSTR)&bhh,sizeof(BITMAPFILEHEADER));
	memcpy(bmpbuffer+sizeof(BITMAPFILEHEADER),(LPSTR)&bih,sizeof(BITMAPINFOHEADER));
	memcpy(bmpbuffer+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER),&rgbquad,sizeof(RGBQUAD)*256);
	//memcpy(bmpbuffer+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256,m_buf,DIBSize);
	//m_picCtrl.Load(bmpbuffer,sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256+DIBSize);

	m_bmi->bmiHeader=bih;
	
	//StretchDIBits(m_pDisplay->GetMemDC()->m_hDC,0,0,g_width,g_height,0,0,g_width,g_height,m_buf,m_bmi,DIB_RGB_COLORS,SRCCOPY);
	//m_pDisplay->Display();
	delete bmpbuffer;
	bool b_save_file	=false;
	if(b_save_file)
	{
		CString strName;
		CString camFolder;
		camFolder.Format(L"d:\\c6UDP\\cam%d",0);
		if(CreateDirectory(camFolder,NULL)||ERROR_ALREADY_EXISTS == GetLastError())
		{
			int iFileIndex=1;
			do 
			{
				strName.Format(L"d:\\c6UDP\\cam%d\\V_%d.bmp",0,iFileIndex);
				++iFileIndex;
			} while (_waccess(strName,0)==0);
			CT2CA pszConvertedAnsiString (strName);
			std::string cvfilename(pszConvertedAnsiString);
			
			CFile file;  
	if(file.Open(strName,CFile::modeWrite | CFile::modeCreate))  
	{
		file.Write((LPSTR)&bhh,sizeof(BITMAPFILEHEADER));  
		file.Write((LPSTR)&bih,sizeof(BITMAPINFOHEADER));  
		if(mlBpp==8) file.Write(&rgbquad,sizeof(RGBQUAD)*256);
		file.Write(m_buf,DIBSize);  
		file.Close();  
		return ;  
	}  
		}
	}
}

void CUsbControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUsbControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

volatile int show_channel;
bool b_save_file;
int trigsource;
unsigned long imgtime;
int camera_height = 0;
int camera_width = 0;
byte* imgBuf = NULL;
byte* imgBuf2 = NULL;
byte* imgBuf3 = NULL;
byte* imgBuf4 = NULL;
#ifdef _ORG
void _stdcall RawCallBack(LPVOID lpParam, LPVOID lpUser)
{
	/*相机传上来的数据保存在thisFrame中，数据的长度为height*width*camsize,
	例如，假如您的系统为6个相机，每个相机的分辨率为1280*960，那么传上来的数据总长度就是1280*960*6，
	各个相机的图像数据从0号相机到5号相机依次排列，例如2号相机的图像就位于1280*960*2,长度为1280*960,
	只需要对该数据块按照分辨率进行切割，就是对应的相机的图像。*/
	GigEimgFrame *thisFrame = (GigEimgFrame*)lpParam;
	if (thisFrame == NULL)
		return;
	CUsbControlDlg *pDlg = (CUsbControlDlg*)lpUser;
	int dispheight = thisFrame->m_height / g_camsize;
	//图像的高度为m_height，因此单个图像的高度就是m_height/g_camsize
	int dispwidth = thisFrame->m_width;
	camera_height = thisFrame->m_height;
	camera_width = thisFrame->m_width;
	//数据块的宽度与图像宽度相等。
	if (imgBuf == NULL)
	{
		imgBuf = new byte[dispheight*dispwidth];
	}

	int offset = 0;
	if (show_channel <= g_camsize - 1)//选择显示哪一个相机的图像。
	{
		offset = show_channel;
	}
	else
	{
		offset = g_camsize - 1;
	}
	offset = dispheight*dispwidth*offset;
	trigsource = thisFrame->m_camNum;
	imgtime=thisFrame->imgtime;
	memcpy(imgBuf, thisFrame->imgBuf + offset, dispheight*dispwidth);//从数据块中只拷贝需要显示的图像	byte *coords=new byte[dispheight];
	cv::Mat frame(dispheight, dispwidth, CV_8UC1, imgBuf);
	cv::Mat frame2(thisFrame->m_height, thisFrame->m_width, CV_8UC1, thisFrame->imgBuf);
	//752*480
	//xie
	cv::Rect rect1(245, 240, 70, 120);
	cv::Rect rect2(400, 240, 70, 120);
	cv::Rect rect7(370, 153, 40, 60);
	//zhi
	cv::Rect rect3(95, 240+480, 70, 120);
	cv::Rect rect4(280, 240+480, 70, 120);
	cv::Rect rect5(440, 53+480, 60, 100);
	cv::Rect rect6(145, 150+480, 40, 60);

	cv::rectangle(frame, rect3, cv::Scalar(255, 255, 255), 2);
	cv::rectangle(frame, rect1, cv::Scalar(255, 255, 255), 2);
	cv::rectangle(frame, rect2, cv::Scalar(255, 255, 255), 2);
	cv::rectangle(frame, rect4, cv::Scalar(255, 255, 255), 2);
	cv::rectangle(frame, rect5, cv::Scalar(255, 255, 255), 2);
	cv::rectangle(frame, rect6, cv::Scalar(255, 255, 255), 2);
	cv::rectangle(frame, rect7, cv::Scalar(255, 255, 255), 2);
	cv::imshow("disp", frame);
	cv::imshow("all", frame2);
	cv::waitKey(1);
	
	if (f_hardtrig)
	{
		recvHardTrigCnt++;
	}

	if (b_save_file)
	{
		CString strName;
		CString camFolder;

		for (int cameranumber = 0; cameranumber < g_camsize; cameranumber++)
		{
			camFolder.Format(L"c:\\c6UDP\\cam%d", cameranumber);
			if (CreateDirectory(camFolder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				int iFileIndex = 1;
				do
				{
					strName.Format(L"c:\\c6UDP\\cam%d\\V_%d.bmp", cameranumber, thisFrame->timestamp);
					++iFileIndex;
				} while (_waccess(strName, 0) == 0);
				CT2CA pszConvertedAnsiString(strName);
				std::string cvfilename(pszConvertedAnsiString);
				offset = cameranumber * dispheight*dispwidth;
				memcpy(imgBuf, thisFrame->imgBuf + offset, dispheight*dispwidth);
				cv::Mat framesave(dispheight, dispwidth, CV_8UC1, imgBuf);
				cv::imwrite(cvfilename, framesave);

			}
		}
	}
		if (snap == true)
		{
			CString strName;
			CString camFolder;
			camFolder.Format(L"c:\\c6UDP\\snap");
			if (CreateDirectory(camFolder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				int iFileIndex = 1;
				do
				{
					strName.Format(L"c:\\c6UDP\\snap\\V_%d.bmp", thisFrame->timestamp);
					++iFileIndex;
				} while (_waccess(strName, 0) == 0);
				CT2CA pszConvertedAnsiString(strName);
				std::string cvfilename(pszConvertedAnsiString);
				
				cv::Mat framesave(thisFrame->m_height, thisFrame->m_width, CV_8UC1, thisFrame->imgBuf);
				cv::imwrite(cvfilename, framesave);

			}

			for (int cameranumber = 0; cameranumber < g_camsize; cameranumber++)
			{
				camFolder.Format(L"c:\\c6UDP\\cam%d", cameranumber);
				if (CreateDirectory(camFolder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
				{
					int iFileIndex = 1;
					do
					{
						strName.Format(L"c:\\c6UDP\\cam%d\\V_%d.bmp", cameranumber, thisFrame->timestamp);
						++iFileIndex;
					} while (_waccess(strName, 0) == 0);
					CT2CA pszConvertedAnsiString(strName);
					std::string cvfilename(pszConvertedAnsiString);
					offset = cameranumber * dispheight*dispwidth;
					memcpy(imgBuf, thisFrame->imgBuf + offset, dispheight*dispwidth);
					cv::Mat framesave(dispheight, dispwidth, CV_8UC1, imgBuf);
					cv::imwrite(cvfilename, framesave);

				}
			}
			snap = false;
		}
		if (f_softtirg)
		{
			recvSoftCnt++;
		}
	
		//h_vw.write(frame);

	
}
#else
void _stdcall RawCallBack(LPVOID lpParam, LPVOID lpUser)
{
	/*相机传上来的数据保存在thisFrame中，数据的长度为height*width*camsize,
	例如，假如您的系统为6个相机，每个相机的分辨率为1280*960，那么传上来的数据总长度就是1280*960*6，
	各个相机的图像数据从0号相机到5号相机依次排列，例如2号相机的图像就位于1280*960*2,长度为1280*960,
	只需要对该数据块按照分辨率进行切割，就是对应的相机的图像。*/
	GigEimgFrame *thisFrame = (GigEimgFrame*)lpParam;
	if (thisFrame == NULL)
		return;
	CUsbControlDlg *pDlg = (CUsbControlDlg*)lpUser;
	int dispheight = thisFrame->m_height / g_camsize;
	//图像的高度为m_height，因此单个图像的高度就是m_height/g_camsize
	int dispwidth = thisFrame->m_width;
	//数据块的宽度与图像宽度相等。
	if (imgBuf == NULL)
	{
		imgBuf = new byte[dispheight*dispwidth];
	}

	int offset = 0;
	if (show_channel <= g_camsize - 1)//选择显示哪一个相机的图像。
	{
		offset = show_channel;
	}
	else
	{
		offset = g_camsize - 1;
	}
	offset = dispheight*dispwidth*offset;
	memcpy(imgBuf, thisFrame->imgBuf + offset, dispheight*dispwidth);//从数据块中只拷贝需要显示的图像	byte *coords=new byte[dispheight];
	cv::Mat frame(dispheight, dispwidth, CV_8UC1, imgBuf);
	cv::Mat frameRGB;
	cv::cvtColor(frame, frameRGB, CV_GRAY2BGR);

	int coords_height = 4;
	int imageheight = dispheight - coords_height;
	int coords_len = imageheight * 4;
	int imagewidth = dispwidth;
	byte * coords = new byte[coords_len];//1
	memcpy(coords, thisFrame->imgBuf + offset + imageheight *imagewidth, coords_len);

	cv::Point pt;
	int xtemp, ytemp;
	for (int i = 0; i < coords_len; i += 4)
	{
		ytemp = coords[i];
		ytemp = ytemp << 8;
		ytemp += coords[i + 1];
		xtemp = coords[i + 2];
		xtemp = xtemp << 8;
		xtemp += coords[i + 3];
		if (xtemp > imagewidth)
		{
			continue;
		}
		pt.x = xtemp;
		pt.y = ytemp - 1;
		circle(frameRGB, pt, 1, cv::Scalar(0, 0, 255));
	}
	//cv::imshow("disp", frame);
	cv::imshow("RGB", frameRGB);
	cv::waitKey(1);

	if (b_save_file)
	{
		CString strName;
		CString camFolder;

		for (int cameranumber = 0; cameranumber < g_camsize; cameranumber++)
		{
			camFolder.Format(L"c:\\c6UDP\\cam%d", cameranumber);
			if (CreateDirectory(camFolder, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				int iFileIndex = 1;
				do
				{
					strName.Format(L"c:\\c6UDP\\cam%d\\V_%d.bmp", cameranumber, thisFrame->timestamp);
					++iFileIndex;
				} while (_waccess(strName, 0) == 0);
				CT2CA pszConvertedAnsiString(strName);
				std::string cvfilename(pszConvertedAnsiString);
				offset = cameranumber*dispheight*dispwidth;
				memcpy(imgBuf, thisFrame->imgBuf + offset, dispheight*dispwidth);
				cv::Mat framesave(dispheight, dispwidth, CV_8UC1, imgBuf);
				cv::imwrite(cvfilename, framesave);

			}
		}
		if (snap == true)
		{
			//cv::imwrite("snap.jpg",frame);
			snap = false;
		}
		if (f_softtirg)
		{
			recvSoftCnt++;
		}
		//h_vw.write(frame);

	}
}
#endif
#ifdef _CAM2
	void _stdcall RawCallBack2(LPVOID lpParam, LPVOID lpUser)
	{
		GigEimgFrame *thisFrame = (GigEimgFrame*)lpParam;
		if (thisFrame == NULL)
			return;

		CUsbControlDlg *pDlg = (CUsbControlDlg*)lpUser;

		int dispheight = thisFrame->m_height / g_camsize;
		int dispwidth = thisFrame->m_width;
		if (imgBuf2 == NULL)
		{
			imgBuf2 = new byte[dispheight*dispwidth];
		}
		int offset = 0;
		if (show_channel <= g_camsize - 1)
		{
			offset = show_channel;
		}
		else
		{
			offset = g_camsize - 1;
		}
		offset = dispheight*dispwidth*offset;
		memcpy(imgBuf2, thisFrame->imgBuf + offset, dispheight*dispwidth);
		cv::Mat frame(dispheight, dispwidth, CV_8UC1, imgBuf2);
		cv::imshow("disp2", frame);
		cv::waitKey(1);
		if (f_softtirg)
		{
			recvSoftCnt2++;
		}

	}
#endif
#ifdef _CAM3
	void _stdcall RawCallBack3(LPVOID lpParam, LPVOID lpUser)
	{
		GigEimgFrame *thisFrame = (GigEimgFrame*)lpParam;
		if (thisFrame == NULL)
			return;

		CUsbControlDlg *pDlg = (CUsbControlDlg*)lpUser;

		int dispheight = thisFrame->m_height / g_camsize;
		int dispwidth = thisFrame->m_width;
		if (imgBuf3 == NULL)
		{
			imgBuf3 = new byte[dispheight*dispwidth];
		}
		int offset = 0;
		if (show_channel <= g_camsize - 1)
		{
			offset = show_channel;
		}
		else
		{
			offset = g_camsize - 1;
		}
		offset = dispheight*dispwidth*offset;
		memcpy(imgBuf3, thisFrame->imgBuf + offset, dispheight*dispwidth);
		cv::Mat frame(dispheight, dispwidth, CV_8UC1, imgBuf3);
		cv::imshow("disp3", frame);
		cv::waitKey(1);
		if (f_softtirg)
		{
			recvSoftCnt3++;
		}

	}
#endif
#ifdef _CAM4
	void _stdcall RawCallBack4(LPVOID lpParam, LPVOID lpUser)
	{
		GigEimgFrame *thisFrame = (GigEimgFrame*)lpParam;
		if (thisFrame == NULL)
			return;

		CUsbControlDlg *pDlg = (CUsbControlDlg*)lpUser;

		int dispheight = thisFrame->m_height / g_camsize;
		int dispwidth = thisFrame->m_width;
		if (imgBuf4 == NULL)
		{
			imgBuf4 = new byte[dispheight*dispwidth];
		}
		int offset = 0;
		if (show_channel <= g_camsize - 1)
		{
			offset = show_channel;
		}
		else
		{
			offset = g_camsize - 1;
		}
		offset = dispheight*dispwidth*offset;
		memcpy(imgBuf4, thisFrame->imgBuf + offset, dispheight*dispwidth);
		cv::Mat frame(dispheight, dispwidth, CV_8UC1, imgBuf4);
		cv::imshow("disp4", frame);
		cv::waitKey(1);
		if (f_softtirg)
		{
			recvSoftCnt4++;
		}

	}
#endif

	void  CUsbControlDlg::OnBnClickedBtnVideocapture()
{
		if (GigEstartCap(board1) < 1)
		{
			SetDlgItemText(IDC_STATIC_TEXT, L"设备打开失败！");
			return;
		}
	if (GigEstartCap(board2) < 0)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"设备2打开失败！");
	}
	SetDlgItemText(IDC_STATIC_TEXT, L"采集中...");
	CheckRadioButton(IDC_RADIO_NORMAL, IDC_RADIO_XYMIRROR, IDC_RADIO_NORMAL);
	SetTimer(1, 1000, NULL);
	cv::namedWindow("disp");

	//sendSoftTrig(1);
	//m_bmi= (BITMAPINFO*)alloca( sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);
	//BMPHeader(g_width,g_height,NULL,m_bmi);
	//////save video///////
	//std::string filename="videofile.avi";
	//cv::Size videosize=cv::Size(g_width,g_height);
	//h_vw.open(filename,CV_FOURCC('X','V','I','D'),15,videosize,0);
	//if(!h_vw.isOpened())
	//{
	//	SetDlgItemText(IDC_STATIC_TEXT,L"保存视频失败。");
	//	return;
	//}
	/////save as file////
}


void CUsbControlDlg::OnBnClickedBtnStopcapture()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	KillTimer(2);
	if(GigEstopCap(board1)<0)
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"尚未采集");
	}
	if(GigEstopCap(board2)!=0)
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"尚未采集");
		
	}
	f_softtirg=0;
	f_hardtrig = 0;
	UpdateData(TRUE);
	//cv::destroyWindow("disp");
	SetDlgItemText(IDC_STATIC_TEXT,L" ");
}

void CUsbControlDlg::OnDestroy()
{
	OnBnClickedBtnClosecon();
	CDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	KillTimer(1);

	m_bCloseWnd=TRUE;

	Sleep(100);
	if(m_pVideoDlg!=NULL)
	{
		delete m_pVideoDlg;
		m_pVideoDlg=NULL;
	}
	if(m_pBrush!=NULL)
	{
		for(int i=0;i<2;++i)
		{
			if(m_pBrush[i].m_hObject!=NULL)
			{
				m_pBrush[i].DeleteObject();
			}
		}
		delete[] m_pBrush;
		m_pBrush=NULL;
	}
}

void CUsbControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int iFrame=0;
	CString str;
	stringstream sstemp;
	switch(nIDEvent)
	{
	case 1://1s timer
		{
		unsigned int framecnttemp = GigEgetFrameCnt(board1);
		unsigned int framecnttemp2 = GigEgetFrameCnt(board2);
		unsigned int framecnttemp3 = GigEgetFrameCnt(board3);
		unsigned int framecnttemp4 = GigEgetFrameCnt(board4);

		unsigned int bps = GigEgetDataCnt(board1);
		unsigned int bps2 = GigEgetDataCnt(board2);
		unsigned int bps3 = GigEgetDataCnt(board3);
		unsigned int bps4 = GigEgetDataCnt(board4);

#ifdef _CAM1
		sstemp << "cam1: " << framecnttemp - lastFrameCnt << " FPS,"
			<< float(bps - lastDataCnt) / 1024 / 1024 << "MB/s"
			//<< ",recv: " << recvSoftCnt << ",send: " << sendSoftCnt << ",diff: " << sendSoftCnt - recvSoftCnt
			//<< ",ErrPack: " << GigEgetErrPackCnt(board1)
			<< ",ImgTime:" << imgtime
				<< endl;

			lastFrameCnt = framecnttemp;
			lastDataCnt = bps;
#endif
#ifdef _CAM2
			sstemp << "cam2: " << framecnttemp2 - lastFrameCnt2 << " FPS," 
				<< float(bps2 - lastDataCnt2) / 1024 / 1024 << "MB/s"
				<< ",recv: " << recvSoftCnt2 << ",send: " << sendSoftCnt << ",diff: " << sendSoftCnt - recvSoftCnt2 
				<< ",ErrPack: " << GigEgetErrPackCnt(board2) << endl;

			lastFrameCnt2 = framecnttemp2;
			lastDataCnt2 = bps2;
#endif
#ifdef _CAM3
			sstemp << "cam3: " << framecnttemp3 - lastFrameCnt3 << " FPS,"
				<< float(bps3 - lastDataCnt3) / 1024 / 1024 << "MB/s"
				<< ",recv: " << recvSoftCnt3 << ",send: " << sendSoftCnt << ",diff: " << sendSoftCnt - recvSoftCnt3
				<< ",ErrPack: " << GigEgetErrPackCnt(board3) << endl;

			lastFrameCnt3 = framecnttemp3;
			lastDataCnt3 = bps3;
#endif
#ifdef _CAM4
			sstemp << "cam4: " << framecnttemp4 - lastFrameCnt4 << " FPS,"
				<< float(bps4 - lastDataCnt4) / 1024 / 1024 << "MB/s"
				<< ",recv: " << recvSoftCnt4 << ",send: " << sendSoftCnt << ",diff: " << sendSoftCnt - recvSoftCnt4
				<< ",ErrPack: " << GigEgetErrPackCnt(board4) << endl;

			lastFrameCnt4 = framecnttemp4;
			lastDataCnt4 = bps4;
#endif
			str=sstemp.str().c_str();

			SetDlgItemText(IDC_STATIC_TEXT,str);
			break;
		}
	case 2:
		{
			if(0)
			{
#ifdef _CAM1
				GigEsendSoftTrig(board1);
#endif
#ifdef _CAM2
				GigEsendSoftTrig(board2);
#endif
#ifdef _CAM3
				GigEsendSoftTrig(board3);
#endif
#ifdef _CAM4
				GigEsendSoftTrig(board4);
#endif
				sendSoftCnt++;
			}
		}
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CUsbControlDlg::OnBnClickedRadioProcType()
{
	UpdateData(TRUE);
	GigEsetMirrorType(GigEDataProcessType(m_iProcType),board1);
}


HBRUSH CUsbControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	int ID=pWnd->GetDlgCtrlID();
	if(ID==IDC_STATIC_TEXT)
	{
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
		return m_pBrush[0];
	case CTLCOLOR_STATIC:
		return m_pBrush[0];
	default:
		return hbr;
	}
}

void CUsbControlDlg::Split(CString in, CString *out, int &outcnt, CString spliter,int maxn)
{

	int d_len=spliter.GetLength();
	int j=0;
	int n=0;
	int m_pos;
	while(1)
	{
		m_pos= in.Find(spliter,j);
		if(m_pos==-1 && j==0)
		{
			out[0]=in.Mid(0);
			outcnt=0;//-1
			break;
		}
		if((m_pos==-1 && j!=0)||(n==maxn))
		{
			out[n]=in.Mid(j,in.GetLength()-j);
			outcnt=n;
			break;
		}

		if(j==0)
		{
			out[n]=in.Mid(0,m_pos);
			j=m_pos+d_len;
		}
		else
		{
			out[n]=in.Mid(j,m_pos-j);
			j=m_pos+d_len;
		}
		n++;
	}
}

void CUsbControlDlg::OnBnClickedBtnSnap()
{
	// TODO: Add your control notification handler code here
	snap=true;
}


void CUsbControlDlg::OnCbnSelchangeComboChannel()
{
	// TODO: Add your control notification handler code here
	show_channel=select_channel.GetCurSel();
}


void CUsbControlDlg::OnBnClickedCheckSave()
{
	// TODO: Add your control notification handler code here
	b_save_file=check_save_file.GetCheck();
}

BOOL CUsbControlDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Check if return key is pressed
		if (pMsg->wParam == VK_RETURN)
		{ 
			// Get current window that has focus
			CWnd* pWndWithFocus = GetFocus();
			int ctrlNum=pWndWithFocus->GetDlgCtrlID();
			// Check if focus belongs to edit control
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}
unsigned long long  CUsbControlDlg::ConverMacAddressStringIntoByte(const char *pszMACAddress, unsigned char* pbyAddress)
{
	unsigned long long rstmacaddr=0;//0xffffffffffff;
	const char cSep = ':';
	for (int iConunter = 0; iConunter < 6; ++iConunter)
	{
		unsigned int iNumber = 0;
		char ch;

		//Convert letter into lower case.
		ch = tolower (*pszMACAddress++);

		if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
		{
			return NULL;
		}

		//Convert into number. 
		//         a. If chareater is digit then ch - '0'
		//		   b. else (ch - 'a' + 10) it is done because addition of 10 takes correct value.
		iNumber = isdigit (ch) ? (ch - '0') : (ch - 'a' + 10);
		ch = tolower (*pszMACAddress);

		if ((iConunter < 5 && ch != cSep) || (iConunter == 5 && ch != '\0' && !isspace (ch)))
		{
			++pszMACAddress;

			if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
			{
				return NULL;
			}

			iNumber <<= 4;
			iNumber += isdigit (ch) ? (ch - '0') : (ch - 'a' + 10);
			ch = *pszMACAddress;

			if (iConunter < 5 && ch != cSep)
			{
				return NULL;
			}
		}
		/* Store result.  */
		pbyAddress[iConunter] = (unsigned char) iNumber;
		
		unsigned long long temp=iNumber;
			//temp=temp<<(8*iConunter);
		rstmacaddr+=temp<<(8*(5-iConunter));
		/* Skip cSep.  */
		++pszMACAddress;
	}
	return rstmacaddr;
}

void CUsbControlDlg::OnBnClickedButtonSetIP()
{
	// TODO: Add your control notification handler code here

	DWORD ipaddr, subnet, gateway;
	m_eCAMIP.GetAddress(ipaddr);
	m_eSUBNET.GetAddress(subnet);
	m_eGATE.GetAddress(gateway);
	CCHCamera *c = new CCHCamera();
	MV_CC_DEVICE_INFO* deviceinfo;
	int idx = listbox.GetCurSel();
	if (idx <0)
	{
		SetDlgItemText(IDC_STATIC_TEXT, _T("please select an item"));
		return;
	}
	CString strtemp;
	listbox.GetText(idx, strtemp);
	CT2CA pszConvertedAnsiString(strtemp);
	std::string strtemp2(pszConvertedAnsiString);
	c = cameralist->find(strtemp2)->second;
	deviceinfo = c->CamInfo;
	deviceinfo->stGigEInfo.nCurrentIp = ipaddr;
	deviceinfo->stGigEInfo.nDefultGateWay = gateway;
	deviceinfo->stGigEInfo.nCurrentSubNetMask = subnet;

	c->CamInfo = deviceinfo;
	m_ePCIp.GetAddress(ipaddr);
	c->hostaddr = ipaddr;
	int rst= GigEsetIP(c,board1);
	CString str;
	if (rst < 0)
	{
		str.Format(L"IP set error: %d",rst);
		
	}
	else
	{
		str.Format(L"IP set success");
	}
	SetDlgItemText(IDC_STATIC_TEXT, str);
	OnBnClickedBtnSearch();
}
std::string WStringToString(const std::wstring& wstr)
{
	std::string str(wstr.length(), ' ');

	//std::copy(wstr.begin(), wstr.end(), str.begin());
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), (int)wstr.length(), (LPSTR)str.c_str(), (int)str.length(), NULL, NULL);

	return str;
}
void CUsbControlDlg::OnBnClickedButtonForceip()
{
	DWORD ipaddr, subnet, gateway;
	m_eCAMIP2.GetAddress(ipaddr);
	m_eSUBNET2.GetAddress(subnet);
	m_eGATE2.GetAddress(gateway);
	CCHCamera *c = new CCHCamera();
	MV_CC_DEVICE_INFO* deviceinfo;
	int idx = listbox.GetCurSel();
	if (idx < 0)
	{
		SetDlgItemText(IDC_STATIC_TEXT, _T("please select a item"));
		return;
	}
	CString strtemp;
	listbox.GetText(idx, strtemp);
	CT2CA pszConvertedAnsiString(strtemp);
	std::string strtemp2(pszConvertedAnsiString);
	
	c = cameralist->find(strtemp2)->second;
	deviceinfo=c->CamInfo;
	deviceinfo->stGigEInfo.nCurrentIp = ipaddr;
	deviceinfo->stGigEInfo.nDefultGateWay = gateway;
	deviceinfo->stGigEInfo.nCurrentSubNetMask = subnet;
	c->CamInfo = deviceinfo;
	TCHAR szBuf[24] = { 0 };
	m_ePCIp2.GetWindowTextW(szBuf, 24);
	c->hostaddr = WStringToString(szBuf);

	int rst = GigEforceIP(c);
	CString str;
	if (rst < 0)
	{
		str.Format(L"IP set error: %d", rst);

	}
	else
	{
		str.Format(L"IP set success");
	}
	SetDlgItemText(IDC_STATIC_TEXT, str);
	OnBnClickedBtnSearch();
}

void CUsbControlDlg::OnBnClickedBtnConnect()
{
	//get info from list selection
	int idx = listbox.GetCurSel();
	CString str;

	if (idx <= -1)
	{
		SetDlgItemText(IDC_STATIC_TEXT, _T("please select an item"));

	}
	else
	{

		CString strtemp;
		listbox.GetText(idx, strtemp);
		CT2CA pszConvertedAnsiString(strtemp);
		std::string strtemp2(pszConvertedAnsiString);
		cameralist->find(strtemp2);

		CCHCamera* c = cameralist->find(strtemp2)->second;

		//connect button
		board1 = GigEaddInstance((LPVOID*)this, RawCallBack, c);
		//GigEgetCamSize(&g_camsize, board1);
		if (board1 > 0)
		{
			str.Format(L"Device connected");
			SetDlgItemText(IDC_STATIC_TEXT, str);
			gb_imgctrl.EnableWindow(1);

		}
		else
		{
			str.Format(L"connect error %d", board1);
			SetDlgItemText(IDC_STATIC_TEXT, str);
		}
	}
}
void CUsbControlDlg::OnLbnSelchangeList1()
{

	int idx=listbox.GetCurSel();
	if(idx<=-1)
	{
		return;
	}
	CString strtemp;
	listbox.GetText(idx,strtemp);
	CT2CA pszConvertedAnsiString(strtemp);
	std::string strtemp2(pszConvertedAnsiString);
	cameralist->find(strtemp2);
	CCHCamera* camerainfo=cameralist->find(strtemp2)->second;
	//show pcip address
	unsigned int ipaddr = inet_addr(camerainfo->hostaddr.c_str());
	sockaddr_in socktemp;
	socktemp.sin_addr.S_un.S_addr = ipaddr;
	CString addrstrpc(inet_ntoa(socktemp.sin_addr));
	m_ePCIp.SetWindowTextW(addrstrpc);
	m_ePCIp2.SetWindowTextW(addrstrpc);
	//show camip address
	ipaddr = ntohl(camerainfo->CamInfo->stGigEInfo.nCurrentIp);
	//unsigned int ipaddr = inet_addr("192.168.0.1");
	socktemp.sin_addr.S_un.S_addr = ipaddr;
	CString addrstr(inet_ntoa(socktemp.sin_addr));
	m_eCAMIP.SetWindowTextW(addrstr);
	m_eCAMIP2.SetWindowTextW(addrstr);
	//show subnet
	ipaddr=ntohl(camerainfo->CamInfo->stGigEInfo.nCurrentSubNetMask);
	socktemp.sin_addr.S_un.S_addr=ipaddr;
	CString addrstr2(inet_ntoa(socktemp.sin_addr));
	m_eSUBNET.SetWindowTextW(addrstr2);
	m_eSUBNET2.SetWindowTextW(addrstr2);
	//show Gateway
	ipaddr=ntohl(camerainfo->CamInfo->stGigEInfo.nDefultGateWay);
	socktemp.sin_addr.S_un.S_addr=ipaddr;
	CString addrstr3(inet_ntoa(socktemp.sin_addr));
	m_eGATE.SetWindowTextW(addrstr3);
	m_eGATE2.SetWindowTextW(addrstr3);
	//show mac address
	unsigned long long macaddr = (camerainfo->CamInfo->nMacAddrHigh);
	macaddr=macaddr<<(2*8);
	macaddr+=camerainfo->CamInfo->nMacAddrLow;
	CMacAddr macAddrHelper(macaddr);
	//m_eMAC.SetWindowTextW(macAddrHelper.macAddrStr);

	CString str;
	str.Format(L"Manufactor Name: %s \nModel Name: %s \nDeviceVer %s \nMAC Addr: %s"
	, CString(camerainfo->CamInfo->stGigEInfo.chManufacturerName) \
	,CString(camerainfo->CamInfo->stGigEInfo.chModelName)\
	,CString(camerainfo->CamInfo->stGigEInfo.chDeviceVersion)\
	,macAddrHelper.macAddrStr);
	SetDlgItemText(IDC_STATIC_TEXT, str);
}

void CUsbControlDlg::OnBnClickedBtnSearch()
{
	//search camera
	
	listbox.ResetContent();

	try
	{
		GigEsearchCamera(cameralist);
	}
	catch(exception &e)
	{
		CString error(e.what());
		SetDlgItemText(IDC_STATIC_TEXT, error);
	}
	
	map_camera ::iterator itr;
	for(itr=cameralist->begin();itr!=cameralist->end();++itr)
	{
		std::string temp=itr->first;
		CString tempc(temp.c_str());
		listbox.AddString(tempc);
	}
	UpdateData(TRUE);
	
	
}

void CUsbControlDlg::OnBnClickedBtnClosecon()
{
	gb_imgctrl.EnableWindow(0);
	OnBnClickedBtnStopcapture();
	GigEcloseConnection(board1);
	GigEcloseConnection(board2);
}


void CUsbControlDlg::OnBnClickedBtnw()
{
	int idx = listbox.GetCurSel();
	if(idx>-1)
	{
		CString strtemp;
		listbox.GetText(idx, strtemp);
		CT2CA pszConvertedAnsiString(strtemp);
		std::string strtemp2(pszConvertedAnsiString);
		cameralist->find(strtemp2);

		CCHCamera* c = cameralist->find(strtemp2)->second;

		//connect button

	Ce2Writer * eewriter = new Ce2Writer(c);
	eewriter->startWriteEE("eefile.ini");
	}
}

void CUsbControlDlg::OnCbnSelchangeComboTrig()
{
	//auto ,fpga,soft,extern

}

void CUsbControlDlg::OnBnClickedBtnTrig()
{
	recvSoftCnt=0;
	recvSoftCnt2 = 0;
	recvSoftCnt3 = 0;
	recvSoftCnt4 = 0;
	sendSoftCnt=0;
	GigEsendSoftTrig(board1);
}


void CUsbControlDlg::OnBnClickedCheckGain()
{
	m_eGAIN.EnableWindow(!autogain.GetCheck());
	UpdateData();
}


void CUsbControlDlg::OnBnClickedCheckExpo()
{
	m_eEXPO.EnableWindow(!autoexpo.GetCheck());
	UpdateData();

}


void CUsbControlDlg::OnBnClickedButtonSendgain()
{
	CString cs_gain;
	m_eGAIN.GetWindowTextW(cs_gain);
	int rst=_ttoi(cs_gain);
	int autovalue=autogain.GetCheck();
		autovalue=autovalue<<1;
		autovalue=autovalue+autoexpo.GetCheck();
		//GigEsetGain(rst,autovalue,board1);
		GigEsetGain_HZC(rst, show_channel,board1);
}


void CUsbControlDlg::OnBnClickedButtonSendexpo()
{
	// TODO: Add your control notification handler code here
	CString cs_expo;
	m_eEXPO.GetWindowTextW(cs_expo);
	int rst=_ttoi(cs_expo);
	int autovalue=autogain.GetCheck();
	autovalue=autovalue<<1;
	autovalue=autovalue+autoexpo.GetCheck();
	GigEsetExpo(rst,autovalue,board1);
}


void CUsbControlDlg::OnBnClickedBtnWbset()
{

}
void CUsbControlDlg::OnBnClickedButtonSendgain2()
{
	int rst=0;
	int idx=m_combo_trig.GetCurSel();
	
	if(idx>=0&&idx<4)
	 rst= GigEsetTrigMode(idx);


	CString cs_freq;
	m_eFreq.GetWindowTextW(cs_freq);
	uint32_t temp=_ttoi(cs_freq);
	GigEsetFreq(temp,board1);
	if (rst < 0)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"Set trig mode error.");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"Success");
	}
}

void CUsbControlDlg::OnBnClickedBtnregread()
{
  /*
  std::stringstream ss_addr, ss_value;
  unsigned int ee_addr, ee_value;
  ss_addr << std::hex << it_map->first;
  ss_value << std::hex << it_map->second;
  ss_addr >> ee_addr;
  ss_value >> ee_value;
  */
  std::stringstream ss_addr, ss_value;
  uint32_t addr;
  CString cs_regaddr;
  m_eRegAddr.GetWindowTextW(cs_regaddr);
  CT2CA pszConvertedAnsiString(cs_regaddr);
  std::string str_addr(pszConvertedAnsiString);
  ss_addr << std::hex << str_addr;
  ss_addr >> addr;
  uint32_t regdata;
  if (GigEReadReg(addr, &regdata, board1))
  {
    CString str;
    str.Format(L"%x", regdata);
    m_eRegData.SetWindowTextW(str);
  }
  else
  {
    m_eRegData.SetWindowTextW(L"error");
  }
}
 
void CUsbControlDlg::OnBnClickedBtnregwrite()
{
  std::stringstream ss_addr, ss_value;
  uint32_t regaddr, regdata;
  CString cs_regaddr, cs_regdata;
  m_eRegAddr.GetWindowTextW(cs_regaddr);
  CT2CA pszConvertedAnsiString(cs_regaddr);
  std::string str_addr(pszConvertedAnsiString);
  ss_addr << std::hex << str_addr;
  ss_addr >> regaddr;
  m_eRegData.GetWindowTextW(cs_regaddr);
  CT2CA pszConvertedAnsiString1(cs_regaddr);
  std::string str_data(pszConvertedAnsiString1);
  ss_value << std::hex << str_data;
  ss_value >> regdata;
  if (GigEWriteReg(regaddr, regdata, board1))
  {
    SetDlgItemText(IDC_STATIC_TEXT, L"Success");
  }
  else
  {
    SetDlgItemText(IDC_STATIC_TEXT, L"error");
  }
}
 


void CUsbControlDlg::OnBnClickedButtonSendcamsize()
{
	CString cs_camsize;
	m_ecamsize.GetWindowTextW(cs_camsize);
	int camsize = _ttoi(cs_camsize);
	GigEsetCamSize(camsize,board1);
	g_camsize = camsize;
}


void CUsbControlDlg::OnBnClickedBtnroiset()
{
	m_cb_binning.EnableWindow(!m_cb_roienable.GetCheck());
	m_cb_skip.EnableWindow(!m_cb_roienable.GetCheck());

	CString cs_xstart, cs_xend, cs_ystart, cs_yend;
	m_eroixstart.GetWindowTextW(cs_xstart);
	m_eroixend.GetWindowTextW(cs_xend);
	m_eroiystart.GetWindowTextW(cs_ystart);
	m_eroiyend.GetWindowTextW(cs_yend);
	int xstart = _ttoi(cs_xstart);
	int xend = _ttoi(cs_xend);
	int ystart = _ttoi(cs_ystart);
	int yend = _ttoi(cs_yend);
	int roienable = m_cb_roienable.GetCheck();

	GigEsetROI(xstart,xend,ystart, yend, roienable,board1);
}


void CUsbControlDlg::OnBnClickedCheckbinning()
{
	m_cb_roienable.EnableWindow(!m_cb_binning.GetCheck());
	m_cb_skip.EnableWindow(!m_cb_binning.GetCheck());
	GigEsetBinning(m_cb_binning.GetCheck(),board1);
}


void CUsbControlDlg::OnBnClickedCheckskip()
{
	m_cb_roienable.EnableWindow(!m_cb_skip.GetCheck());
	m_cb_binning.EnableWindow(!m_cb_skip.GetCheck());
	GigEsetSkip(m_cb_skip.GetCheck(),board1);
}


void CUsbControlDlg::OnBnClickedCheckroienable()
{
	m_cb_binning.EnableWindow(!m_cb_roienable.GetCheck());
	m_cb_skip.EnableWindow(!m_cb_roienable.GetCheck());
	GigEsetROIEn(m_cb_roienable.GetCheck(),board1);
}


void CUsbControlDlg::OnBnClickedBtnWbset2()
{
	CString cs_r, cs_g, cs_b, cs_g2;
	m_eRGain.GetWindowTextW(cs_r);
	m_eGGain.GetWindowTextW(cs_g);
	m_eBGain.GetWindowTextW(cs_b);
	m_eGGain2.GetWindowTextW(cs_g2);
	GigEsetWB(_ttoi(cs_r), _ttoi(cs_g),_ttoi(cs_g2), _ttoi(cs_b), board1);
}


void CUsbControlDlg::OnBnClickedButton1()
{

	CString str;
		map_camera::iterator itr;
		itr = cameralist->begin();
#ifdef _CAM1
		if (itr != cameralist->end())
		{
			CCHCamera *c0 = itr->second;
			board1 = GigEaddInstance((LPVOID*)this, RawCallBack, c0);
			if (board1 <= 0)
			{
				str.Format(L"connect error cam1");
				SetDlgItemText(IDC_STATIC_TEXT, str);
				return;
			}
			itr++;
		}
#endif

#ifdef _CAM2
		if (itr != cameralist->end())
		{
			CCHCamera *c1 = itr->second;
			board2 = GigEaddInstance((LPVOID*)this, RawCallBack2, c1);
			if (board2 <= 0)
			{
				str.Format(L"connect error cam2");
				SetDlgItemText(IDC_STATIC_TEXT, str);
				return;
			}
			itr++;
		}
#endif

#ifdef _CAM3
		if (itr != cameralist->end())
		{
			CCHCamera *c1 = itr->second;
			board3 = GigEaddInstance((LPVOID*)this, RawCallBack3, c1);
			if (board3 <= 0)
			{
				str.Format(L"connect error cam3");
				SetDlgItemText(IDC_STATIC_TEXT, str);
				return;
			}
			itr++;
		}
#endif

#ifdef _CAM4
		if (itr != cameralist->end())
		{
			CCHCamera *c1 = itr->second;
			board4 = GigEaddInstance((LPVOID*)this, RawCallBack4, c1);
			if (board4 <= 0)
			{
				str.Format(L"connect error cam4");
				SetDlgItemText(IDC_STATIC_TEXT, str);
				return;
			}

		}
#endif
		str.Format(L"Device connected");
		SetDlgItemText(IDC_STATIC_TEXT, str);
		gb_imgctrl.EnableWindow(1);
	
		sendSoftCnt = 0;
		f_softtirg = 1;
		

		
#ifdef _CAM1
		recvSoftCnt = 0;
		GigEsetTrigMode(2, board1);
		if (GigEstartCap(board1)<1)
		{
			SetDlgItemText(IDC_STATIC_TEXT, L"设备打开失败！");
			return;
		}
#endif

#ifdef _CAM2
		recvSoftCnt2 = 0;
		GigEsetTrigMode(2, board2);
		if (GigEstartCap(board2)<0)
		{
			SetDlgItemText(IDC_STATIC_TEXT, L"设备2打开失败！");
			return;
		}
#endif

#ifdef _CAM3
		recvSoftCnt3 = 0;
		GigEsetTrigMode(2, board3);
		if (GigEstartCap(board3)<0)
		{
			SetDlgItemText(IDC_STATIC_TEXT, L"设备2打开失败！");
			return;
		}
#endif

#ifdef _CAM4
		recvSoftCnt4 = 0;
		GigEsetTrigMode(2, board4);
		if (GigEstartCap(board4)<0)
		{
			SetDlgItemText(IDC_STATIC_TEXT, L"设备2打开失败！");
			return;
		}
#endif
		SetDlgItemText(IDC_STATIC_TEXT, L"采集中...");
		CheckRadioButton(IDC_RADIO_NORMAL, IDC_RADIO_XYMIRROR, IDC_RADIO_NORMAL);
		SetTimer(1, 1000, NULL);
		
}


void CUsbControlDlg::OnBnClickedBtnTrig2()
{
	/*
	int rst = 0;
	int idx = m_combo_trig.GetCurSel();

	CString cs_freq;
	m_eFreq.GetWindowTextW(cs_freq);
	uint32_t temp = _ttoi(cs_freq);
	if(idx==2)
	SetTimer(2, temp, NULL);

	recvSoftCnt = 0;
	recvSoftCnt2 = 0;
	recvSoftCnt3 = 0;
	recvSoftCnt4 = 0;
	sendSoftCnt = 0;
	f_softtirg = 1;
	GigEsendSoftTrig(board1);
	*/
	f_hardtrig = 1;
	recvHardTrigCnt = 0;
}


void CUsbControlDlg::OnBnClickedBtnminset()
{
	CString str;
	int data;
	GetDlgItemText(IDC_EDITA,str);
	data = _ttoi(str);
	csSetGaussianA(data);

	GetDlgItemText(IDC_EDITB, str);
	data = _ttoi(str);
	csSetGaussianB(data);

	GetDlgItemText(IDC_EDITC, str);
	data = _ttoi(str);
	csSetGaussianC(data);

	GetDlgItemText(IDC_EDITMAXP, str);
	data = _ttoi(str);
	csSetMaxBrightnessThreshold(data);

	GetDlgItemText(IDC_EDITMAXW, str);
	data = _ttoi(str);
	csSetMaxLineWidth(data);

	GetDlgItemText(IDC_EDITMINW, str);
	data = _ttoi(str);
	csSetMinLineWidth(data);

}


int resolu=0;
void CUsbControlDlg::OnBnClickedresolu()
{
	// TODO: Add your control notification handler code here
	if(resolu==0)
	{	resolu=1;}
	else
	{
		resolu=0;
	}
	GigEsetResolu_HZC(resolu);
}
