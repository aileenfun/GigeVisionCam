
// UsbControlDlg.h : 头文件
//

#pragma once
//#include "DataProcess.h"
//#include "DataCapture.h"
#include "VideoDlg.h"
#include "Display.h"
#include "afxwin.h"
#include "..\..\CCTAPI\CCTAPI\CCTAPI.h"
#include <cv.hpp>
#include <opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "MACAddrEdit.h"
#include "afxcmn.h"
#include "atlbase.h"  
#include "atlstr.h"  
#include "comutil.h" 
#include "PictureCtrl.h"
// CUsbControlDlg 对话框
typedef
	VOID
	(WINAPI * LPMV_CALLBACK2)(LPVOID lpParam, LPVOID lpUser);

class CUsbControlDlg : public CDialog
{
// 构造
	HICON m_hIcon;
	enum { IDD = IDD_USBCONTROL_DIALOG };
	DECLARE_MESSAGE_MAP()
	enum TrigMode{TM_FPGA,TM_EXTERN,TM_SOFT};
public:
	CUsbControlDlg(CWnd* pParent = NULL);	
	~CUsbControlDlg();
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnOpenusb();
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnBnClickedBtnVideocapture();
	afx_msg void OnBnClickedBtnStopcapture();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboDriver();
	afx_msg void OnBnClickedRadioDriver();
	afx_msg void OnBnClickedRadioProcType();
	afx_msg void OnBnClickedRadioChangeType();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnReview();
	static unsigned int __stdcall ThreadProcess(void* pParam);
	void ThreadProcessFunction();
	BOOL CloseRbfFile();
	BOOL OpenRbfFile();
	void SendData();
	BOOL OpenDataFile();
	BOOL CloseDataFile();
	BOOL PreTranslateMessage(MSG* pMsg);
	//void _stdcall RawCallBack(LPVOID lpParam,LPVOID lpUser);
	void Split(CString in, CString *out, int &outcnt, CString spliter,int maxn);
private:
	BOOL	m_bUsbOpen;
	CFile*	m_pFileRbf;					//下位机程序文件
	CString m_strRbfFileName;
	BYTE          m_byData[64];
	USB_ORDER     m_sUsbOrder;
	BOOL		  m_bSendData;
	CVideoDlg*    m_pVideoDlg;
	HDC			  m_hDisplayDC;
	HANDLE        m_hThread;
	char*         m_pReadBuff;
	BOOL		  m_bCloseWnd;
	long          m_lBytePerSecond;
	CYUSB_DRIVER  m_CyDriver;			//驱动类型：新、旧
	int			  m_iRdoDriver;
	int			  m_iProcType;
	CBrush*       m_pBrush;	
	CFile*        m_pVideoDataFile;
	BOOL          m_bReview;
	BOOL          m_bSave;
	BOOL          m_Init;
	CString       m_strDataFileName;
	unsigned long lastDataCnt;
	unsigned int lastFrameCnt;
		clientPropStruct prop;

public:
	afx_msg void OnBnClickedBtnCreatebmp();
	CEdit m_Edit_Width;
	afx_msg void OnEnChangeEdit1();
	CEdit m_Edit_Height;
	afx_msg void OnEnChangeEdit2();
	CString m_sEdit_Width;
	CString m_sEdit_Height;
	afx_msg void OnBnClickedButton2();
	//static void saveVideoFun(cv::Mat frame);

	afx_msg void OnBnClickedBtnSnap();
	CComboBox select_channel;
	CButton check_save_file;
	afx_msg void OnCbnSelchangeComboChannel();
	void modifyProp();
	afx_msg void OnBnClickedCheckSave();
	CMACAddrEdit m_eMAC;
	CEdit m_ePACK;
	CEdit m_eINTV;
	CEdit m_eTRIG;
	CEdit m_eEXPO;
	CEdit m_eGAIN;
	CIPAddressCtrl m_eCAMIP;
	CIPAddressCtrl m_ePCIP;
	afx_msg void OnEnChangeEditMac();
	afx_msg void OnBnClickedButtonUdprop();
	afx_msg void OnEnChangeOrder();
	afx_msg void OnCbnSelchangeComboTrig();
	CComboBox m_combo_trig;
	afx_msg void OnBnClickedButtonTrig();
	afx_msg void OnBnClickedButtonCamOrder();
	CNetAddressCtrl m_MACAddr;
	unsigned long long ConverMacAddressStringIntoByte(const char *pszMACAddress, unsigned char* pbyAddress);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CPictureCtrl m_picCtrl;
	void BMPHeader(int lWidth, int lHeight,byte* m_buf,BITMAPINFO* m_bmi);
	CDisplay *m_pDisplay;
	BITMAPINFO* m_bmi;
//	afx_msg void OnEnChangeEditcamsize();
	CEdit m_editcamsize;
	afx_msg void OnBnClickedBtnReaddev();
	afx_msg void OnBnClickedBtnInit();
	CString m_scamsize;
	afx_msg void OnBnClickedBtnClosecon();
	afx_msg void OnBnClickedBtnw();
	CEdit m_eRegAddr;
	CEdit m_eRegData;
	afx_msg void OnBnClickedBtnr();
};
