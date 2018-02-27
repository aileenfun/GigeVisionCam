#pragma once
#include "afxwin.h"
//#include "afxdialogex.h"

// CVideoDlg 对话框
extern int g_width;
extern int g_height;
class CVideoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoDlg)
	// 对话框数据
	enum { IDD = IDD_DLG_VIDEO };
	DECLARE_MESSAGE_MAP()
public:
	CVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoDlg();

public:
	CDC* GetDisplayDC();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();

private:
	CStatic m_stcVideo;
	virtual void OnCancel();
};
