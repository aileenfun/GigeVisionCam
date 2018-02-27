#include "StdAfx.h"
#include "DisPlay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CDisplay::CDisplay(void)
{
	m_rtDisplay.SetRect(0,0,0,0);
	m_pMemDC = NULL;
	m_pMemBitmap = NULL;
	m_pDcDisplay = NULL;
	m_hDc = NULL;
}

CDisplay::~CDisplay(void)
{
	Close();
}

void CDisplay::Open( CDC* pDC,CRect rt )
{	
	m_pDcDisplay=pDC;
	SetRect(rt);
}

void CDisplay::Close()
{
	if (m_pMemBitmap != NULL)
	{
		if(m_pMemBitmap->GetSafeHandle()!=NULL)
		{
			m_pMemBitmap->DeleteObject();
		}
		delete m_pMemBitmap;
		m_pMemBitmap = NULL;
	}
	if(m_pMemDC != NULL)
	{
		if(m_pMemDC->GetSafeHdc()!=NULL)
		{
			m_pMemDC->DeleteDC();
		}
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}

CDC * CDisplay::GetMemDC()
{
	return m_pMemDC;
}

BOOL CDisplay::SetRect( CRect rt )
{
	if (m_pMemDC == NULL)
		m_pMemDC = new CDC();
	if (m_pMemBitmap == NULL)
		m_pMemBitmap = new CBitmap;

	if(m_pMemBitmap->GetSafeHandle() != NULL)
		m_pMemBitmap->DeleteObject();

	if(m_pMemDC->GetSafeHdc()!=NULL)
		m_pMemDC->DeleteDC();

	m_rtDisplay.CopyRect(&rt);
	m_pMemDC->CreateCompatibleDC(NULL);	
	m_pMemBitmap->CreateCompatibleBitmap(m_pDcDisplay, m_rtDisplay.Width(), m_rtDisplay.Height());
	m_pMemDC->SelectObject(m_pMemBitmap);
	return TRUE;
}

void CDisplay::Display()
{
	if(m_pDcDisplay == NULL || m_pDcDisplay->GetSafeHdc()==NULL)
		return ;
	m_pDcDisplay->BitBlt(m_rtDisplay.left, m_rtDisplay.top, m_rtDisplay.Width(), m_rtDisplay.Height(), m_pMemDC, 0, 0, SRCCOPY);
}