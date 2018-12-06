
// UsbControlDlg.h : 头文件
//

#pragma once
//#include "DataProcess.h"
//#include "DataCapture.h"
#include "VideoDlg.h"
#include "afxwin.h"
#include "..\..\CCHVAPI_Ethernet\CCHVAPI\CCHVAPI.h"
#include <cv.hpp>
#include <opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "MACAddrEdit.h"
#include "afxcmn.h"
#include "atlbase.h"  
#include "atlstr.h"  
#include "comutil.h" 
#include "PictureCtrl.h"
#include "..\..\CCHVAPI_Ethernet\CCHVAPI\Inc\Socket\Socket.h"
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
	afx_msg void OnBnClickedBtnVideocapture();
	afx_msg void OnBnClickedBtnStopcapture();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadioProcType();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL PreTranslateMessage(MSG* pMsg);
	//void _stdcall RawCallBack(LPVOID lpParam,LPVOID lpUser);
	void Split(CString in, CString *out, int &outcnt, CString spliter,int maxn);
private:
	BOOL	m_bUsbOpen;
	CFile*	m_pFileRbf;					//下位机程序文件
	CString m_strRbfFileName;
	BYTE          m_byData[64];
	BOOL		  m_bSendData;
	CVideoDlg*    m_pVideoDlg;
	HDC			  m_hDisplayDC;
	HANDLE        m_hThread;
	char*         m_pReadBuff;
	BOOL		  m_bCloseWnd;
	long          m_lBytePerSecond;
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

	unsigned long lastDataCnt2;
	unsigned int lastFrameCnt2;

	unsigned long lastDataCnt3;
	unsigned int lastFrameCnt3;

	unsigned long lastDataCnt4;
	unsigned int lastFrameCnt4;
	
	GigEclientPropStruct prop;
	map_deviceInfo *devicelist;
	map_camera *cameralist;
public:
	CEdit m_Edit_Width;
	CEdit m_Edit_Height;
	CString m_sEdit_Width;
	CString m_sEdit_Height;
	//static void saveVideoFun(cv::Mat frame);
	CComboBox select_channel;
	CButton check_save_file;
	CMACAddrEdit m_eMAC;
	CEdit m_ePACK;
	CEdit m_eINTV;
	CEdit m_eTRIG;
	CEdit m_eEXPO;
	CEdit m_eGAIN;
	CIPAddressCtrl m_eCAMIP;
	CIPAddressCtrl m_eGATE;
	CIPAddressCtrl m_eSUBNET;
	CListBox listbox;
	CComboBox m_combo_trig;
	CNetAddressCtrl m_MACAddr;
	CPictureCtrl m_picCtrl;
	BITMAPINFO* m_bmi;
	CString m_scamsize;
	CEdit m_editcamsize;
	afx_msg void OnBnClickedButtonSetIP();
	afx_msg void OnCbnSelchangeComboTrig();
	afx_msg void OnBnClickedButtonCamOrder();
	unsigned long long ConverMacAddressStringIntoByte(const char *pszMACAddress, unsigned char* pbyAddress);
	void BMPHeader(int lWidth, int lHeight,byte* m_buf,BITMAPINFO* m_bmi);
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnClosecon();
	afx_msg void OnBnClickedBtnw();
	afx_msg void OnBnClickedBtnr();
	afx_msg void OnLbnSelchangeList1();
	
	afx_msg void OnBnClickedBtnTrig();
	afx_msg void OnCbnSelchangeComboChannel();
	afx_msg void OnBnClickedCheckSave();
	afx_msg void OnBnClickedBtnSnap();
	CButton autogain;
	CButton autoexpo;
	CEdit m_eFreq;
	CEdit m_eRGain;
	CEdit m_eGGain;
	CEdit m_eBGain;
	afx_msg void OnBnClickedCheckGain();
	afx_msg void OnBnClickedCheckExpo();
	afx_msg void OnBnClickedButtonSendgain();
	afx_msg void OnBnClickedButtonSendexpo();
	afx_msg void OnBnClickedBtnWbset();
	afx_msg void OnBnClickedButtonSendgain2();
	CStatic gb_imgctrl;
	CEdit m_eGGain2;
	CEdit m_eRegAddr;
	CEdit m_eRegData;
	afx_msg void OnBnClickedBtnregread();
	afx_msg void OnBnClickedBtnregwrite();
	afx_msg void OnStnClickedStaticText();
	CEdit m_ecamsize;
	afx_msg void OnBnClickedButtonSendcamsize();
	CButton m_cb_roienable;
	CEdit m_eroixstart;
	CEdit m_eroixend;
	CEdit m_eroiystart;
	CEdit m_eroiyend;
	afx_msg void OnBnClickedBtnroiset();
	CButton m_cb_binning;
	CButton m_cb_skip;
	afx_msg void OnBnClickedCheckbinning();
	afx_msg void OnBnClickedCheckskip();
	afx_msg void OnBnClickedCheckroienable();
	CIPAddressCtrl m_ePCIp;
	afx_msg void OnBnClickedButtonForceip();
	CIPAddressCtrl m_ePCIp2;
	CIPAddressCtrl m_eCAMIP2;
	CIPAddressCtrl m_eSUBNET2;
	CIPAddressCtrl m_eGATE2;
	afx_msg void OnBnClickedBtnWbset2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnTrig2();
	afx_msg void OnBnClickedBtnminset();
	afx_msg void OnEnChangeEditGain();
};
