// CViewScrollView.h : interface of the CCViewScrollView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CVIEWSCROLLVIEW_H__E1009E4F_68C0_11D3_AF5A_004F4902BCDD__INCLUDED_)
#define AFX_CVIEWSCROLLVIEW_H__E1009E4F_68C0_11D3_AF5A_004F4902BCDD__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCViewScrollView : public CView
{
protected: // create from serialization only
	CCViewScrollView();
	DECLARE_DYNCREATE(CCViewScrollView)

// Attributes
public:
	//CCViewScrollDoc* GetDocument();

//Why does this need to be public?
public:
	//int m_top_line;
	int m_amt_page;
	int m_amt_total;
	int m_font_height;
	int m_greatest_width;
	int m_client_screen_width;

	void TextOut(CDC*, int, int, int, CString);
	//CSize m_client_max;
	void fix_scrollbars();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCViewScrollView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCViewScrollView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCViewScrollView)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*#ifndef _DEBUG  // debug version in CViewScrollView.cpp
inline CCViewScrollDoc* CCViewScrollView::GetDocument()
   { return (CCViewScrollDoc*)m_pDocument; }
#endif*/

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVIEWSCROLLVIEW_H__E1009E4F_68C0_11D3_AF5A_004F4902BCDD__INCLUDED_)
