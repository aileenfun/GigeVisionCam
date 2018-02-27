
// UsbControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UsbControl.h"	
#include "UsbControlDlg.h"
//#include "afxdialogex.h"
#include "propFromFileExm.h"
//#include "clientProp.h"
#include <cstdlib>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
volatile int b_softtrig=-1;

cv::VideoWriter h_vw;
volatile bool snap;
int board1=-1;//board id
int board2=-1;
#ifdef _W640
int g_width=640;
int g_height=480;
#endif
#ifdef _W1280
int g_width=1280;
int g_height=960;
#endif

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
	m_sUsbOrder.pData=m_byData;
	m_pVideoDlg=NULL;

	m_hDisplayDC=NULL;
	m_hThread=NULL;
	m_pReadBuff=new char[ReadDataBytes];
	memset(m_pReadBuff,0,sizeof(char)*ReadDataBytes);

	m_bCloseWnd=FALSE;
	m_lBytePerSecond=0;
	m_CyDriver=NEW_DRIVER;
	m_pBrush=NULL;
	m_pVideoDataFile=NULL;
	m_bReview=FALSE;
	m_bSave=FALSE;
	m_Init = FALSE;
	snap=false;
	lastDataCnt=0;
	lastFrameCnt=0;
	
	//system("arp -s 192.168.1.2 ab-cd-c0-a8-01-9b 192.168.1.3");
	//system("arp -s 192.168.2.8 ab-cd-c0-a8-01-9b 192.168.2.9");
	
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
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Width);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_Height);
	DDX_Text(pDX, IDC_EDIT1, m_sEdit_Width);
	DDX_Text(pDX, IDC_EDIT2, m_sEdit_Height);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, select_channel);
	DDX_Control(pDX, IDC_CHECK_SAVE, check_save_file);
	DDX_Control(pDX, IDC_EDIT_MAC, m_eMAC);
	DDX_Control(pDX, IDC_EDIT_PACK, m_ePACK);
	DDX_Control(pDX, IDC_EDIT_INTV, m_eINTV);
	DDX_Control(pDX, IDC_EDIT_EXPO, m_eEXPO);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_eGAIN);
	DDX_Control(pDX, IDC_IPADDRESS_CAM, m_eCAMIP);
	DDX_Control(pDX, IDC_IPADDRESS_PC, m_ePCIP);
	DDX_Control(pDX, IDC_EDITCAMSIZE, m_editcamsize);
	DDX_Control(pDX, IDC_COMBO_TRIG, m_combo_trig);
	DDX_Text(pDX, IDC_EDITCAMSIZE, m_scamsize);
	DDX_Control(pDX, IDC_EDIT_REG__ADDR, m_eRegAddr);
	DDX_Control(pDX, IDC_EDIT_REG_DATA, m_eRegData);
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
	ON_BN_CLICKED(IDC_BUTTON_UDPROP, &CUsbControlDlg::OnBnClickedButtonUdprop)
	ON_BN_CLICKED(IDC_BUTTON_CAM_ORDER, &CUsbControlDlg::OnBnClickedButtonCamOrder)
	ON_BN_CLICKED(IDC_BUTTON4, &CUsbControlDlg::OnBnClickedButton4)
//	ON_EN_CHANGE(IDC_EDITCAMSIZE, &CUsbControlDlg::OnEnChangeEditcamsize)
ON_BN_CLICKED(IDC_BTN_READDEV, &CUsbControlDlg::OnBnClickedBtnReaddev)
ON_BN_CLICKED(IDC_BTN_INIT, &CUsbControlDlg::OnBnClickedBtnInit)
ON_BN_CLICKED(IDC_BTN_CLOSECON, &CUsbControlDlg::OnBnClickedBtnClosecon)
ON_BN_CLICKED(IDC_BTNW, &CUsbControlDlg::OnBnClickedBtnw)
ON_BN_CLICKED(IDC_BTNR, &CUsbControlDlg::OnBnClickedBtnr)
END_MESSAGE_MAP()


// CUsbControlDlg 消息处理程序
BOOL CUsbControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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

	// TODO: 在此添加额外的初始化代码
	//------------------------------------
	CRect cRect,wRect,mRect;
	GetDesktopWindow()->GetWindowRect(wRect);
	GetWindowRect(cRect);
	mRect.right=wRect.right-50;
	mRect.bottom=wRect.bottom-50;
	mRect.left=mRect.right-cRect.Width();
	mRect.top=mRect.bottom-cRect.Height();
	MoveWindow(mRect);
	//------------------------------------
	m_pFileRbf=new CFile();
	m_pVideoDlg=new CVideoDlg();

	m_pVideoDlg->Create(IDD_DLG_VIDEO,this);
	m_pVideoDlg->ShowWindow(FALSE);
	m_hDisplayDC=m_pVideoDlg->GetDisplayDC()->m_hDC;
	m_pDisplay=new CDisplay();
	m_pDisplay->Open(CDC::FromHandle(m_hDisplayDC),CRect(0,0,g_width,g_height));
	m_iRdoDriver=(int)m_CyDriver;

	m_pBrush=new CBrush[2];
	m_pBrush[0].CreateSolidBrush(RGB(99,208,242));
	m_pBrush[1].CreateSolidBrush(RGB(174,238,250));
	select_channel.InsertString(0,_T("0"));
	select_channel.InsertString(1,_T("1"));
	select_channel.InsertString(2,_T("2"));
	select_channel.InsertString(3,_T("3"));
	select_channel.InsertString(4,_T("4"));
	select_channel.InsertString(5,_T("5"));
	select_channel.SetCurSel(0);
	check_save_file.SetCheck(0);

	m_combo_trig.InsertString(0,_T("FPGA Trig"));
	m_combo_trig.InsertString(1,_T("Ext Trig"));
	m_combo_trig.InsertString(2,_T("Soft Trig"));
	m_combo_trig.SetCurSel(0);

	m_eMAC.SetWindowTextW(_T("AB:CE:C0:A8:01:9B"));
	m_ePACK.SetWindowTextW(_T("1024"));
	m_eINTV.SetWindowTextW(_T("4096"));
	m_eCAMIP.SetWindowTextW(_T("192.168.1.2"));

	m_ePCIP.SetWindowTextW(_T("192.168.1.3"));
	m_eEXPO.SetWindowTextW(_T("300"));
	m_eGAIN.SetWindowTextW(_T("16"));

	

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
byte* imgBuf=new byte[g_width*g_height];
void _stdcall RawCallBack(LPVOID lpParam,LPVOID lpUser)
{
	imgFrame *thisFrame=(imgFrame*)lpParam;
	if(thisFrame==NULL)
		return;
	//sendSoftTrig(board1,2);
	
	CUsbControlDlg *pDlg=(CUsbControlDlg*)lpUser;
	memcpy(imgBuf,thisFrame->imgBuf,thisFrame->m_height*thisFrame->m_width);
	cv::Mat frame(thisFrame->m_height,thisFrame->m_width,CV_8UC1,imgBuf);
	
		//stringstream ss;
		//ss<<thisFrame->m_camNum;
		//string str=ss.str();
		//cv::imshow(str,frame);
	if(show_channel==thisFrame->m_camNum)
	{
		//BITMAPINFO* m_bmi;
		//pDlg->BMPHeader(thisFrame->m_width,thisFrame->m_height,thisFrame->imgBuf,pDlg->m_bmi);
		//StretchDIBits(pDlg->m_pDisplay->GetMemDC()->m_hDC,0,0,g_width,g_height,0,0,g_width,g_height,thisFrame->imgBuf,pDlg->m_bmi,DIB_RGB_COLORS,SRCCOPY);
		//pDlg->m_pDisplay->Display();
		//cv::Mat frameRGB;
	//cv::cvtColor(frame,frameRGB,CV_BayerBG2BGR);
		//cv::imshow("RGB",frameRGB);
		cv::imshow("RAW",frame);
		cv::waitKey(1);
		//memset(imgBuf,0,g_width*g_height);
	}
	
	if(b_save_file)
	{
		CString strName;
		CString camFolder;
		camFolder.Format(L"c:\\c6UDP\\cam%d",thisFrame->m_camNum);
		if(CreateDirectory(camFolder,NULL)||ERROR_ALREADY_EXISTS == GetLastError())
		{
			int iFileIndex=1;
			do 
			{
				strName.Format(L"c:\\c6UDP\\cam%d\\V_%d.bmp",thisFrame->m_camNum,thisFrame->timestamp);
				++iFileIndex;
			} while (_waccess(strName,0)==0);
			CT2CA pszConvertedAnsiString (strName);
			std::string cvfilename(pszConvertedAnsiString);
			cv::imwrite(cvfilename,frame);
		}
	}
	if(snap==true)
	{
		//cv::imwrite("snap.jpg",frame);
		snap=false;
	}
		//h_vw.write(frame);
		
}
void _stdcall RawCallBack2(LPVOID lpParam,LPVOID lpUser)
{
	imgFrame *thisFrame=(imgFrame*)lpParam;
	if(thisFrame==NULL)
		return;
	
	CUsbControlDlg *pDlg=(CUsbControlDlg*)lpUser;
	
		cv::Mat frame(thisFrame->m_height,thisFrame->m_width,CV_8UC1,thisFrame->imgBuf);
	if(show_channel==thisFrame->m_camNum)
	{
		
		//memcpy(imgBuf,thisFrame->imgBuf,thisFrame->m_height*thisFrame->m_width);
		//BITMAPINFO* m_bmi;
		//pDlg->BMPHeader(thisFrame->m_width,thisFrame->m_height,thisFrame->imgBuf,pDlg->m_bmi);
		//StretchDIBits(pDlg->m_pDisplay->GetMemDC()->m_hDC,0,0,g_width,g_height,0,0,g_width,g_height,thisFrame->imgBuf,pDlg->m_bmi,DIB_RGB_COLORS,SRCCOPY);
		//pDlg->m_pDisplay->Display();

		//cv::Mat frame1=frame.clone();
		cv::imshow("disp2",frame);
		cv::waitKey(1);
		//memset(imgBuf,0,g_width*g_height);
	}
			
	if(b_save_file)
	{
		CString strName;
		CString camFolder;
		camFolder.Format(L"c:\\c6UDP\\cam%d",thisFrame->m_camNum);
		if(CreateDirectory(camFolder,NULL)||ERROR_ALREADY_EXISTS == GetLastError())
		{
			int iFileIndex=1;
			do 
			{
				strName.Format(L"c:\\c6UDP\\cam%d\\V_%d.bmp",thisFrame->m_camNum,thisFrame->timestamp);
				++iFileIndex;
			} while (_waccess(strName,0)==0);
			CT2CA pszConvertedAnsiString (strName);
			std::string cvfilename(pszConvertedAnsiString);
			cv::imwrite(cvfilename,frame);
		}
	}
	if(snap==true)
	{
		//cv::imwrite("snap.jpg",frame);
		snap=false;
	}
		//h_vw.write(frame);
}
void  CUsbControlDlg::OnBnClickedBtnVideocapture()
{

	CUsbControlDlg *temp=this;
	if(startCap(g_height,g_width,board1)<1)
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"设备打开失败！");
		return;
	}
	else
	{

		SetDlgItemText(IDC_STATIC_TEXT,L"采集中...");
		CheckRadioButton(IDC_RADIO_NORMAL,IDC_RADIO_XYMIRROR,IDC_RADIO_NORMAL);
		SetTimer(1,1000,NULL);
		
		cv::namedWindow("disp");
	}

	/*if(startCap(g_height,g_width,board2)<0)
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"设备打开失败！");
		return;
	}*/
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
	if(stopCap(board1)!=0)
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"尚未采集");
		return;
	}
	//if(stopCap(board2)!=0)
	//{
	//	SetDlgItemText(IDC_STATIC_TEXT,L"尚未采集");
	//	return;
	//}
	UpdateData(TRUE);
	cv::destroyWindow("disp");
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
	//CloseUsb();
	//CyUsb_Destroy();
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
	switch(nIDEvent)
	{
	case 1://1s timer
		{
			iFrame=getFrameCnt()-lastFrameCnt;
			m_lBytePerSecond=getDataCnt()-lastDataCnt;
			str.Format(L"%d Fps     %0.4f MBs",iFrame,float(m_lBytePerSecond)/1024.0/1024.0);
			lastFrameCnt=getFrameCnt();
			lastDataCnt=getDataCnt();
			SetDlgItemText(IDC_STATIC_TEXT,str);
			
			
		}
	case 2:
		{
			if(m_combo_trig.GetCurSel()==1)
			{
				//sendSoftTrig(board1,2);
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
	setMirrorType(DataProcessType(m_iProcType));
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
void CUsbControlDlg::modifyProp()
{

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
			if (pWndWithFocus == GetDlgItem(IDC_EDIT_MAC))
			{ 
				
				//AfxMessageBox("Yess!");
				return TRUE;
			}
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

void CUsbControlDlg::OnBnClickedButtonUdprop()
{
	// TODO: Add your control notification handler code here
	CString s_temp;
	CStringA s_tempA;
	m_eMAC.GetWindowTextW(s_temp);
	s_tempA=s_temp;
	const size_t newsizea = (s_tempA.GetLength()+1);
	char * pc=new char [newsizea];
	
	strcpy_s(pc,newsizea,s_tempA);
	unsigned char byAddress[6] = {'\0'};
	prop.MACaddr=ConverMacAddressStringIntoByte(pc,byAddress);
	//unsigned long tempval = _tstol(s_temp);
	//prop.MACaddr=tempval;

	DWORD addr;
	m_eCAMIP.GetAddress(addr);
	prop.camIP=addr;
	m_ePCIP.GetAddress(addr);
	prop.pcIP=addr;
	
	m_ePACK.GetWindowTextW(s_temp);
	int tempint=_tstoi(s_temp);
	if(tempint!=0)
	prop.packetSize=tempint;

	m_eINTV.GetWindowTextW(s_temp);
	tempint=_tstoi(s_temp);
	if(tempint!=0)
	prop.interval_time=tempint;

	sendProp(prop,board1);
//	sendProp(prop,board2);
}

void CUsbControlDlg::OnBnClickedButtonCamOrder()
{
		
	camPropStruct camprop;
	bool mode2=0;
	camprop.trigMode=m_combo_trig.GetCurSel();	


	if(camprop.trigMode==TM_SOFT)//2
	{
		mode2=1;
		camprop.trigMode=1;
	}
	CString s_temp;
	int tempint;
	
	m_eEXPO.GetWindowTextW(s_temp);
	tempint=_tstoi(s_temp);
	camprop.expo=tempint;
	
	m_eGAIN.GetWindowTextW(s_temp);
	tempint=_tstoi(s_temp);
	camprop.brightness=tempint;
	
	camprop.col=g_width;
	camprop.row=g_height;

	camprop.mirror=m_iProcType;
	sendOrder(camprop,board1);
	Sleep(1000);
	
	if(mode2==1)
	{
		sendSoftTrig(board1,1);
	}
	else
	{
		sendSoftTrig(board1,m_combo_trig.GetCurSel());
		//SetTimer(2,1000,NULL);
	}
//	sendOrder(camprop,board2);
}


void CUsbControlDlg::OnBnClickedButton4()
{
	sendSoftTrig(board1,4);
	sendSoftTrig(board1,2);
}

void CUsbControlDlg::OnBnClickedBtnReaddev()
{
	clientPropStruct *devprop=new clientPropStruct();
	
	if(getProp(board1,devprop)<1)
	{
		//error return 
		SetDlgItemText(IDC_STATIC_TEXT,L"error open camera");
		return;
	}
	CString temp;
	temp.Format(L"%d",devprop->height);
	m_Edit_Height.SetWindowTextW(temp);
	temp.Format(L"%d",devprop->width);
	m_Edit_Width.SetWindowTextW(temp);
	temp.Format(L"%d",devprop->camCnt);
	m_editcamsize.SetWindowTextW(temp);
	g_width=devprop->width;
	g_height=devprop->height;

	if(devprop->camCnt>0&&devprop->camCnt)
	prop.camCnt=devprop->camCnt;
	sendProp(prop,board1,1);
	SetDlgItemText(IDC_STATIC_TEXT,L"camera opened");
	UpdateData(TRUE);
}


void CUsbControlDlg::OnBnClickedBtnInit()
{
	board1=addInstance((LPVOID*)this,RawCallBack);
	CString s_temp;
	CStringA s_tempA;
	m_eMAC.GetWindowTextW(s_temp);
	s_tempA=s_temp;
	const size_t newsizea = (s_tempA.GetLength()+1);
	char * pc=new char [newsizea];
	
	strcpy_s(pc,newsizea,s_tempA);
	unsigned char byAddress[6] = {'\0'};
	prop.MACaddr=ConverMacAddressStringIntoByte(pc,byAddress);

	DWORD addr;
	m_eCAMIP.GetAddress(addr);
	prop.camIP=addr;
	m_ePCIP.GetAddress(addr);
	prop.pcIP=addr;
	
	m_ePACK.GetWindowTextW(s_temp);
	int tempint=_tstoi(s_temp);
	if(tempint!=0)
	prop.packetSize=tempint;

	m_eINTV.GetWindowTextW(s_temp);
	tempint=_tstoi(s_temp);
	if(tempint!=0)
	prop.interval_time=tempint;

	prop.camCnt=6;

	sendProp(prop,board1,1);
	board1=initCCTAPI(board1);

	prop.camIP=htonl(inet_addr("192.168.1.2"));
	prop.pcIP=htonl(inet_addr("192.168.1.3"));
}


void CUsbControlDlg::OnBnClickedBtnClosecon()
{
	OnBnClickedBtnStopcapture();
	closeConnection(board1);
}


void CUsbControlDlg::OnBnClickedBtnw()
{
		CString s_temp;
	
	m_eRegAddr.GetWindowTextW(s_temp);
	
	int tempaddr=_tstoi(s_temp);
	m_eRegData.GetWindowTextW(s_temp);
	int tempdata=_tstoi(s_temp);
	WriteReg(tempaddr,tempdata,1);
}


void CUsbControlDlg::OnBnClickedBtnr()
{
	// TODO: 在此添加控件通知处理程序代码
}
