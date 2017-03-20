// XMLParse.h : main header file for the XMLPARSE application
//

#if !defined(AFX_XMLPARSE_H__6C99A367_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_)
#define AFX_XMLPARSE_H__6C99A367_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMLParseApp:
// See XMLParse.cpp for the implementation of this class
//

class CXMLParseApp : public CWinApp
{
public:
	CXMLParseApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLParseApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMLParseApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLPARSE_H__6C99A367_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_)
