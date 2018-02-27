#include "StdAfx.h"
#include "ScreenShot.h"


CScreenShot::CScreenShot(void)
{
	m_btmFileHd.bfType=((WORD)('M'<<8)|'B');
	m_btmFileHd.bfSize=54;
	m_btmFileHd.bfReserved1=0;
	m_btmFileHd.bfReserved2=0;
	m_btmFileHd.bfOffBits=54;

	m_btmInfoHd.biBitCount=24;
	m_btmInfoHd.biClrImportant=0;
	m_btmInfoHd.biClrUsed=0;
	m_btmInfoHd.biCompression=0;
	m_btmInfoHd.biHeight=0;
	m_btmInfoHd.biWidth=0;
	m_btmInfoHd.biPlanes=1;
	m_btmInfoHd.biSize=sizeof(BITMAPINFOHEADER);
	m_btmInfoHd.biSizeImage=0;
	m_btmInfoHd.biXPelsPerMeter=0;
	m_btmInfoHd.biYPelsPerMeter=0;

	m_bOpen=FALSE;
	m_pDataBuf=NULL;
	m_nBufSize=0;
	m_pDCScreen=NULL;
	m_pDCMem=NULL;
	m_pFile=NULL;
}

CScreenShot::~CScreenShot(void)
{
	Close();
}

int CScreenShot::Open( CDC* pDC,CRect rect )
{
	if(pDC->m_hDC==NULL||rect.Width()<=0||rect.Height()<=0)
		return 0;
	m_pDCScreen=pDC;
	m_rectScreen=rect;
	m_pDCMem=new CDC;
	if(m_pDCMem->GetSafeHdc()!=NULL)
	{
		m_pDCMem->DeleteDC();
	}
	m_pDCMem->CreateCompatibleDC(NULL);
	if(m_btmMem.GetSafeHandle()!=NULL)
	{
		m_btmMem.DeleteObject();
	}
	m_btmMem.CreateCompatibleBitmap(pDC,m_rectScreen.Width(),m_rectScreen.Height());
	m_pDCMem->SelectObject(m_btmMem);
	m_pDCMem->StretchBlt(0,0,m_rectScreen.Width(),m_rectScreen.Height(),m_pDCScreen,0,0,m_rectScreen.Width(),m_rectScreen.Height(),SRCCOPY);
	m_btmMem.GetBitmap(&m_Bitmap);
	m_nBufSize=m_Bitmap.bmWidthBytes*m_Bitmap.bmHeight;
	if(m_pDataBuf!=NULL)
	{
		delete[] m_pDataBuf;
		m_pDataBuf=NULL;
	}
	m_pDataBuf=new char[m_nBufSize];
	memset(m_pDataBuf,0,sizeof(char)*m_nBufSize);
	m_btmInfoHd.biBitCount=m_Bitmap.bmBitsPixel;
	m_btmInfoHd.biHeight=m_Bitmap.bmHeight;
	m_btmInfoHd.biWidth=m_Bitmap.bmWidth;
	m_btmInfoHd.biSizeImage=m_nBufSize;
	m_btmFileHd.bfSize+=m_nBufSize;
	GetDIBits(m_pDCMem->m_hDC,m_btmMem,0,m_btmInfoHd.biHeight,m_pDataBuf,(BITMAPINFO*)&m_btmInfoHd,DIB_RGB_COLORS);
	m_bOpen=TRUE;
	return 1;
}

int CScreenShot::Close()
{
	if(m_pDataBuf!=NULL)
	{
		delete[] m_pDataBuf;
		m_pDataBuf=NULL;
	}
	m_bOpen=FALSE;
	if(m_pDCMem!=NULL)
	{
		if(m_pDCMem->GetSafeHdc()!=NULL)
		{
			m_pDCMem->DeleteDC();
		}
		delete m_pDCMem;
		m_pDCMem=NULL;
	}
	if(m_pFile!=NULL)
	{
		if(m_pFile->m_hFile!=CFile::hFileNull)
		{
			m_pFile->Close();
		}
		delete m_pFile;
		m_pFile=NULL;
	}
	if(m_btmMem.GetSafeHandle()!=NULL)
	{
		m_btmMem.DeleteObject();
	}
	return 1;
}

int CScreenShot::CopyToBmpFile( CString szFilePath )
{
	if(szFilePath.GetLength()<=0||!m_bOpen)
		return 0;
	if(m_pFile!=NULL)
	{
		if(m_pFile->m_hFile!=CFile::hFileNull)
		{
			m_pFile->Close();
		}
		delete m_pFile;
		m_pFile=NULL;
	}
	m_pFile=new CFile;
	if(!m_pFile->Open(szFilePath,CFile::modeCreate|CFile::modeWrite))
		return 0;
	CreateBmpFile();
	return 1;
}

void CScreenShot::CreateBmpFile()
{
	m_pFile->Write(&m_btmFileHd,sizeof(BITMAPFILEHEADER));
	m_pFile->Write(&m_btmInfoHd,sizeof(BITMAPINFOHEADER));
	m_pFile->Write(m_pDataBuf,m_nBufSize);
	m_pFile->Close();
}

int CScreenShot::CopyToOtherDC( CDC *pDC,CRect rect )
{
	if(!m_bOpen)
		return 0;
	StretchDIBits(pDC->m_hDC,rect.left,rect.top,rect.Width(),rect.Height(),0,0,m_btmInfoHd.biWidth,m_btmInfoHd.biHeight,
		m_pDataBuf,(BITMAPINFO*)&m_btmInfoHd,DIB_RGB_COLORS,SRCCOPY);
	return 1;
}
