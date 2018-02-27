#pragma once
#include "afxwin.h"
//#include "afxdialogex.h"

// CVideoDlg �Ի���
extern int g_width;
extern int g_height;
class CVideoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVideoDlg)
	// �Ի�������
	enum { IDD = IDD_DLG_VIDEO };
	DECLARE_MESSAGE_MAP()
public:
	CVideoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoDlg();

public:
	CDC* GetDisplayDC();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();

private:
	CStatic m_stcVideo;
	virtual void OnCancel();
};
