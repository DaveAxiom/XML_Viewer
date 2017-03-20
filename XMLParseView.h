// XMLParseView.h : interface of the CXMLParseView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPARSEVIEW_H__6C99A371_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_)
#define AFX_XMLPARSEVIEW_H__6C99A371_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXMLParseView : public CCViewScrollView
{
protected: // create from serialization only
	CXMLParseView();
	DECLARE_DYNCREATE(CXMLParseView)

// Attributes
public:
	CXMLParseDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLParseView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXMLParseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXMLParseView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in XMLParseView.cpp
inline CXMLParseDoc* CXMLParseView::GetDocument()
   { return (CXMLParseDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLPARSEVIEW_H__6C99A371_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_)
