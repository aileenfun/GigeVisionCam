/////////////////////////////////////////////////////////////////////////////
//
// MODULE: MACAddrEdit.h
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

#if !defined(AFX_MACADDREDIT_H__F7E792A4_ABB8_4F8D_AA52_DEA14D0C249C__INCLUDED_)
#define AFX_MACADDREDIT_H__F7E792A4_ABB8_4F8D_AA52_DEA14D0C249C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MACAddrEdit.h : header file
//

#include <afxcoll.h>

/////////////////////////////////////////////////////////////////////////////
// CMACAddrEdit window

class CMACAddrEdit : public CEdit
{
// Construction
public:
	CMACAddrEdit();

// Attributes
public:

// Operations
public:
	BOOL	SetCurrentMask(CString csNewMask);
	CString GetCurrentMask();
	BOOL	SetValidEditChars(CUIntArray* arChars);

protected:
	BOOL ProcessPaste();
	BOOL ProcessClear();
	BOOL IsValidEditChar(TCHAR tcChar);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMACAddrEdit)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMACAddrEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMACAddrEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	CString		m_MACMask;									// holds the MAC mask
	CUIntArray	m_arCharList;								// holds all valid edit chars
	BOOL		m_bControlDown;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACADDREDIT_H__F7E792A4_ABB8_4F8D_AA52_DEA14D0C249C__INCLUDED_)
