
// UsbControl.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UsbControl.h"
#include "UsbControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUsbControlApp

BEGIN_MESSAGE_MAP(CUsbControlApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUsbControlApp ����

CUsbControlApp::CUsbControlApp()
{
	// ֧����������������

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CUsbControlApp ����

CUsbControlApp theApp;


// CUsbControlApp ��ʼ��

BOOL CUsbControlApp::InitInstance()
{


	CWinApp::InitInstance();

	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CUsbControlDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();//��ʾ
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

