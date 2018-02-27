/////////////////////////////////////////////////////////////////////////////
//
// MODULE: MACAddrEdit.cpp
//
// AUTHOR: Ralph Perlich <RPSoft@arcor.de>
//
// Copyright (c) 2007. All Rights Reserved.
//
// Date: February 01, 2007
//
// Version 1.10
//
// Updates:
// 
// 7th February 2007 - added context menu handling (copy, paste etc.) (1.10)
//					 - added possibility to change the edit mask (1.10)	  
//					 - added possibility to set a range off valid chars (1.10)
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MACAddrEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMACAddrEdit

CMACAddrEdit::CMACAddrEdit()
{
	m_MACMask		= _T("##:##:##:##:##:##");				// define the actual MAC address mask
	m_bControlDown	= FALSE;
															// initialize char array
	m_arCharList.Add((TCHAR) '0');	m_arCharList.Add((TCHAR) '1');
	m_arCharList.Add((TCHAR) '2');	m_arCharList.Add((TCHAR) '3');
	m_arCharList.Add((TCHAR) '4');	m_arCharList.Add((TCHAR) '5');
	m_arCharList.Add((TCHAR) '6');	m_arCharList.Add((TCHAR) '7');
	m_arCharList.Add((TCHAR) '8');	m_arCharList.Add((TCHAR) '9');

	m_arCharList.Add((TCHAR) 'A');	m_arCharList.Add((TCHAR) 'B');
	m_arCharList.Add((TCHAR) 'C');	m_arCharList.Add((TCHAR) 'D');
	m_arCharList.Add((TCHAR) 'E');	m_arCharList.Add((TCHAR) 'F');
}

/////////////////////////////////////////////////////////////////////////////

CMACAddrEdit::~CMACAddrEdit()
{
	m_arCharList.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMACAddrEdit, CEdit)
	//{{AFX_MSG_MAP(CMACAddrEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMACAddrEdit message handlers

void CMACAddrEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString csMACText;
	int		iPos, iEndPos;
	TCHAR	tcChar;

	GetWindowText(csMACText);								// get the actual text
	GetSel(iPos, iEndPos);									// get the actual selection

	nChar = _totupper(nChar);								// uppercase this char

	if (nChar == VK_BACK)									// we will delete chars
	{
		if (iEndPos == csMACText.GetLength())				// we are at the end position ?
		{
			if ((iPos == iEndPos) && (iPos != 0))			// there is no selection
				iPos--;										// decrement position
			csMACText.Delete(iPos, iEndPos - iPos);			// delete chars		
		}
		else												// in the middle
		{											
			if ((iPos == iEndPos) && (iPos != 0))			// there is no selection
				iPos--;										// decrement position
			for (int i = iPos; i < iEndPos; i++)
			{												// override chars
				if ((tcChar = m_MACMask[i]) == '#')
					csMACText.SetAt(i, (TCHAR) '0');
			}
		}	
				
		SetWindowText(csMACText);							// show the new address
		SetSel(iPos, iPos);									// and set the selection
	}
	else
	if (IsValidEditChar(nChar))								// we will insert valid chars
	{
		if (iPos < m_MACMask.GetLength())					// check max size
		{
			if ((tcChar = m_MACMask[iPos]) != '#')			// mask char is a separator ?
			{
				if (iPos < csMACText.GetLength())			// insert/add the separator
					csMACText.SetAt(iPos, (TCHAR)m_MACMask[iPos]);
				else
					csMACText += (TCHAR)m_MACMask[iPos];
				iPos++;										// increment position !!!
			}

			if ((tcChar = m_MACMask[iPos]) == '#')			// mask char is not a separator	?	
			{
				if (iPos < csMACText.GetLength())			// insert/add the new char
					csMACText.SetAt(iPos, (TCHAR) nChar);
				else
					csMACText += (TCHAR)nChar;
				iPos++;										// increment position
			}
					
			SetWindowText(csMACText);						// now show the new MAC address
			SetSel(iPos, iPos);
		}	
	}
	else													// default: handle only separator chars
	{														
		if ((iPos < m_MACMask.GetLength() &&				// check max size and for separator
			(tcChar = m_MACMask[iPos]) == (TCHAR)nChar))
		{
			if (iPos < csMACText.GetLength())				// insert/add the new char
				csMACText.SetAt(iPos, (TCHAR)tcChar);
			else
				csMACText += (TCHAR)tcChar;	
			iPos++;											// increment position
					
			SetWindowText(csMACText);						// and show the MAC address
			SetSel(iPos, iPos);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CMACAddrEdit::PreSubclassWindow() 
{
	CEdit::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////

void CMACAddrEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL bProcess = TRUE;

	if (nChar == VK_INSERT)
		bProcess = FALSE;									// disable insert function
	else
	if (nChar == VK_CONTROL)
		m_bControlDown = TRUE;								// remember the control state
	else
	if (nChar == VK_DELETE)
	{
		SendMessage(WM_CUT, 0, 0);							// send a message, we will handle this
		bProcess = FALSE;
	}
	else													// handle Context Menu entries
	if ((nChar == 0x58) && m_bControlDown)					// STRG+X
	{
		SendMessage(WM_CUT, 0, 0);							// send a message, we will handle this
		bProcess = FALSE;
	}
	else
	if ((nChar == 0x43) && m_bControlDown)					// STRG+C
	{
		SendMessage(WM_COPY, 0, 0);							// send a message, we will handle this
		bProcess = FALSE;
	}
	else
	if ((nChar == 0x56) && m_bControlDown)					// STRG+V
	{
		SendMessage(WM_PASTE, 0, 0);						// send a message, we will handle this
		bProcess = FALSE;
	}

	if (bProcess)
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);			// do the default work
}

/////////////////////////////////////////////////////////////////////////////

void CMACAddrEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_CONTROL)
		m_bControlDown = FALSE;								// update our control state

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CMACAddrEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	BOOL lRet		= 0;
	BOOL bProcess	= TRUE;

	switch (message)										// handle messages from the context menu
	{
	case WM_CLEAR:											// context menu "Delete"
		bProcess = FALSE;
		ProcessClear();
		break;
	case WM_CUT:
		bProcess = FALSE;
		ProcessClear();
		break;
	case WM_PASTE:
		bProcess = FALSE;
		ProcessPaste();
		break;
	case WM_SETTEXT:
		bProcess = TRUE;
		break;
	default:
		break;
	}

	if (bProcess)
		lRet = CEdit::WindowProc(message, wParam, lParam);

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CMACAddrEdit::IsValidEditChar(TCHAR tcChar)
{
	for (int i = 0; i < m_arCharList.GetSize(); i++)
	{
		if ((TCHAR)m_arCharList.GetAt(i) == tcChar)
			return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CMACAddrEdit::ProcessPaste()
{
	HANDLE	hMem;
	LPTSTR  lptstrCopy; 
	CString csMACText;
	CString csPaste;
	int		iPos, iEndPos;
	TCHAR	tcChar;

	if (!OpenClipboard())
		return FALSE;

#ifdef _UNICODE
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
#else
	if (!IsClipboardFormatAvailable(CF_TEXT))
#endif
	{
		CloseClipboard();
		return FALSE;
	}

#ifdef _UNICODE
	hMem = GetClipboardData(CF_UNICODETEXT);
#else
	hMem = GetClipboardData(CF_TEXT);
#endif

	if (hMem == NULL)
	{
		CloseClipboard();
		return FALSE;
	}

	lptstrCopy = (LPTSTR)GlobalLock(hMem);
	csPaste = lptstrCopy;
	GlobalUnlock(hMem);
	CloseClipboard();
	
	GetWindowText(csMACText);								// get the actual text
	GetSel(iPos, iEndPos);									// get current selection
	
	if (iPos != iEndPos)									// we have a selection
	{
		for (int i = iPos; i < iEndPos; i++)
		{
			if ((i - iPos) > csPaste.GetLength() - 1)		// check length from paste string
				break;										// selection is longer as the paste string

			if ((tcChar = m_MACMask[i]) != '#')				// current pos is a sepatator
			{
				csMACText.SetAt(i, (TCHAR)m_MACMask[i]);	// insert the separator
			}
			else
			{												// current pos is a char pos
				tcChar =  _totupper(csPaste[i - iPos]);
				if (IsValidEditChar(tcChar))
					csMACText.SetAt(i, (TCHAR)tcChar);
				else
					csMACText.SetAt(i, (TCHAR)'0');			// insert default char, if paste char not fit
			}
		}
	}
	else													// we have no selection
	{
		int iMaxLen = m_MACMask.GetLength() - csMACText.GetLength() + iPos;

		for (int i = iPos; i < iMaxLen; i++)
		{
			if ((i - iPos) > csPaste.GetLength() - 1)		// check length from paste string
				break;	

			if ((tcChar = m_MACMask[i]) != '#')				// current pos is a sepatator
			{
				if (i < csMACText.GetLength())
					csMACText.SetAt(i, (TCHAR)m_MACMask[i]);
				else										// insert the separator
					csMACText.Insert(i, (TCHAR)m_MACMask[i]);	
			}
			else											// current pos is a char position
			{				
				tcChar =  _totupper(csPaste[i - iPos]);		// check for valid char
				if (IsValidEditChar(tcChar))
				{
					if (i < csMACText.GetLength())			// insert/add char
						csMACText.SetAt(i, (TCHAR)tcChar);
					else
						csMACText.Insert(i, (TCHAR)tcChar);
				}
				else										// invalid char
				{
					if (i < csMACText.GetLength())			// insert/add default char
						csMACText.SetAt(i, (TCHAR)'0');
					else				
						csMACText.Insert(i, (TCHAR)'0');
				}
			}
		}
		iEndPos += iMaxLen - iPos - 1;						// correct selection values
	}
	
	SetWindowText(csMACText);								// and show the MAC address
	SetSel(iPos, iEndPos);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CMACAddrEdit::ProcessClear()
{
	CString csMACText;
	CString csPaste;
	int		iPos, iEndPos;
	TCHAR	tcChar;

	GetWindowText(csMACText);								// get the actual text
	GetSel(iPos, iEndPos);									// get current selection
	
	if (iPos != iEndPos)									// we have a selection
	{
		if (iEndPos == csMACText.GetLength())				// we are at the end position ?
			csMACText.Delete(iPos, iEndPos - iPos);			// delete chars
		else
		{													// in the middle
			for (int i = iPos; i < iEndPos; i++)
			{
				if ((tcChar = m_MACMask[i]) != '#')			// current pos is a sepatator
					csMACText.SetAt(i, (TCHAR)m_MACMask[i]);// insert the separator
				else
					csMACText.SetAt(i, (TCHAR)'0');			// insert default char
			}
		}
	}

	SetWindowText(csMACText);								// and show the MAC address
	SetSel(iPos, iEndPos);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CMACAddrEdit::SetCurrentMask(CString csNewMask)
{
	ASSERT(csNewMask.GetLength() > 2);						// new mask must have min. 2 chars
	ASSERT(csNewMask.GetLength() < 254);					
	
	if (csNewMask.GetLength() < 2)
		return FALSE;
	if (csNewMask.GetLength() > 254)
		return FALSE;

	if (csNewMask.Find('#', 0) < 0)							// mask must have the '#' char
		return FALSE;

	m_MACMask = csNewMask;									// set new mask
	
	SetWindowText(_T(""));									// clear the edit field

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

CString CMACAddrEdit::GetCurrentMask()
{
	return m_MACMask;										// just return the current mask
}

/////////////////////////////////////////////////////////////////////////////

BOOL CMACAddrEdit::SetValidEditChars(CUIntArray* arChars)
{
	ASSERT(arChars->GetSize() > 0);

	if (arChars->GetSize() == 0)
		return FALSE;

	m_arCharList.RemoveAll();								// first clear the old chars
	m_arCharList.Append(*arChars);							// assign the new chars

	SetWindowText(_T(""));									// clear the edit field

	return (m_arCharList.GetSize() > 0);
}

/////////////////////////////////////////////////////////////////////////////

