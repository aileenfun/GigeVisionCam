#pragma once
class CScreenShot
{
public:
	CScreenShot(void);
	~CScreenShot(void);

public:
	int Open(CDC* pDC,CRect rect);			//pDC:��ͼ����DC,rect����ͼ�����С
	int Close();							//�ر�
	int CopyToBmpFile(CString szFilePath);	//��ͼ
	int CopyToOtherDC(CDC *pDC,CRect rect);	//����ȡͼ��copy��Ŀ������

private:
	void CreateBmpFile();

private:
	CFile*  m_pFile;
	CDC*	m_pDCMem;		//����DC
	CBitmap m_btmMem;		//����λͼ
	CDC*    m_pDCScreen;	//��ĻDC
	CRect   m_rectScreen;   //��Ļ����
	BITMAP  m_Bitmap;
	BITMAPINFOHEADER m_btmInfoHd;
	BITMAPFILEHEADER m_btmFileHd;
	char   *m_pDataBuf;		
	int     m_nBufSize;
	BOOL    m_bOpen;
};

