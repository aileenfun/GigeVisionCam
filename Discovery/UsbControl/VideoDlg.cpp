// VideoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UsbControl.h"
#include "VideoDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CVideoDlg �Ի���

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

// CVideoDlg ��Ϣ�������


BOOL CVideoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//MoveWindow(0,0,100,100);
	//m_stcVideo.MoveWindow(0,0,g_width,g_height);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CVideoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	//m_stcVideo.MoveWindow(0,0,g_width,g_height);
	// TODO: �ڴ˴������Ϣ����������
	if(bShow)
	{
		CRect rect;
		GetParent()->GetWindowRect(rect);
		//MoveWindow(rect.left,rect.top,rect.Width()+5,rect.Height());//����ͼ����ʾ���ںͿ���̨�������λ��
		MoveWindow(0,0,rect.left,rect.top);
		//MoveWindow(rect.left-1000,rect.top-800,rect.Width()+1000,960);//��ʾ1280*960��ͼ��
	}
}


void CVideoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
// 	CRect rect;
// 	m_stcVideo.GetClientRect(rect);
// 	m_stcVideo.GetDC()->FillSolidRect(rect,RGB(0,0,255));
}


void CVideoDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	GetParent()->PostMessage(WM_COMMAND,IDC_BTN_STOPCAPTURE);
}
