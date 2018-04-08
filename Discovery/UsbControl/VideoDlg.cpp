// VideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UsbControl.h"
#include "VideoDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CVideoDlg 对话框

IMPLEMENT_DYNAMIC(CVideoDlg, CDialog)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoDlg::IDD, pParent)
{

}

CVideoDlg::~CVideoDlg()
{
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_VIDEO, m_stcVideo);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
END_MESSAGE_MAP()

CDC* CVideoDlg::GetDisplayDC()
{
	if(m_stcVideo.m_hWnd!=NULL)
	{
		return m_stcVideo.GetDC();
	}
	return NULL;
}

// CVideoDlg 消息处理程序


BOOL CVideoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//MoveWindow(0,0,100,100);
	//m_stcVideo.MoveWindow(0,0,g_width,g_height);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CVideoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	//m_stcVideo.MoveWindow(0,0,g_width,g_height);
	// TODO: 在此处添加消息处理程序代码
	if(bShow)
	{
		CRect rect;
		GetParent()->GetWindowRect(rect);
		//MoveWindow(rect.left,rect.top,rect.Width()+5,rect.Height());//代表图像显示窗口和控制台窗口相对位置
		MoveWindow(0,0,rect.left,rect.top);
		//MoveWindow(rect.left-1000,rect.top-800,rect.Width()+1000,960);//显示1280*960大图像
	}
}


void CVideoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
// 	CRect rect;
// 	m_stcVideo.GetClientRect(rect);
// 	m_stcVideo.GetDC()->FillSolidRect(rect,RGB(0,0,255));
}


void CVideoDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	GetParent()->PostMessage(WM_COMMAND,IDC_BTN_STOPCAPTURE);
}
