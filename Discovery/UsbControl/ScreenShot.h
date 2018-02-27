#pragma once
class CScreenShot
{
public:
	CScreenShot(void);
	~CScreenShot(void);

public:
	int Open(CDC* pDC,CRect rect);			//pDC:截图区域DC,rect：截图区域大小
	int Close();							//关闭
	int CopyToBmpFile(CString szFilePath);	//截图
	int CopyToOtherDC(CDC *pDC,CRect rect);	//将截取图形copy到目的区域

private:
	void CreateBmpFile();

private:
	CFile*  m_pFile;
	CDC*	m_pDCMem;		//兼容DC
	CBitmap m_btmMem;		//兼容位图
	CDC*    m_pDCScreen;	//屏幕DC
	CRect   m_rectScreen;   //屏幕区域
	BITMAP  m_Bitmap;
	BITMAPINFOHEADER m_btmInfoHd;
	BITMAPFILEHEADER m_btmFileHd;
	char   *m_pDataBuf;		
	int     m_nBufSize;
	BOOL    m_bOpen;
};

