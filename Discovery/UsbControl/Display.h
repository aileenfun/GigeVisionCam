#pragma once
class CDisplay
{
public:
	CDisplay(void);
	~CDisplay(void);

public:
	void Open(CDC* pDC,CRect rt );
	void Close();
	void Display();
	BOOL SetRect(CRect rt );
	CDC *GetMemDC();

private:
	CDC		*m_pDcDisplay;
	CRect	m_rtDisplay;
	CDC		*m_pMemDC;
	CBitmap *m_pMemBitmap;
	HDC		m_hDc;
};

