
// UsbControl.cpp : 定义应用程序的类行为。
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


// CUsbControlApp 构造

CUsbControlApp::CUsbControlApp()
{
	// 支持重新启动管理器

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CUsbControlApp 对象

CUsbControlApp theApp;


// CUsbControlApp 初始化

BOOL CUsbControlApp::InitInstance()
{


	CWinApp::InitInstance();

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CUsbControlDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();//显示
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//“确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

