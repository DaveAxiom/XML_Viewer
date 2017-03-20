// CViewScrollView.cpp : implementation of the CCViewScrollView class
//

#include "stdafx.h"
#include "CViewScrollView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCViewScrollView

IMPLEMENT_DYNCREATE(CCViewScrollView, CView)

BEGIN_MESSAGE_MAP(CCViewScrollView, CView)
	//{{AFX_MSG_MAP(CCViewScrollView)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCViewScrollView construction/destruction

CCViewScrollView::CCViewScrollView()
{
	m_amt_page = 0;
	m_amt_total = 0;
	m_font_height = 0;
	m_greatest_width = 0;
	m_client_screen_width = 0;

}

CCViewScrollView::~CCViewScrollView()
{
}

BOOL CCViewScrollView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCViewScrollView drawing

void CCViewScrollView::OnDraw(CDC* pDC)
{
	CFont thin_font;
	thin_font.CreatePointFont(100, "Courier", pDC);
	pDC->SelectObject(&thin_font);

	//Get the text ready.
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int charheight = tm.tmHeight + tm.tmExternalLeading;
	int line_number = 0;
	CString to_view;
	
	to_view.Format("The position of the vertical scroll bar: %d", GetScrollPos(SB_VERT));

	int number_of_lines = 20;
	m_greatest_width = 0;
	for (int loop = 0; loop < number_of_lines; loop++)
	{
		CString number;
		number.Format("%d ", loop);

		CSize text_dimensions = pDC->GetTextExtent(number + to_view);
		m_greatest_width = max(m_greatest_width, 0/*x offset*/ + text_dimensions.cx);

		TextOut(pDC, 0, line_number++, charheight, number + to_view);
	}

	//If area size doesn't change then this doesn't need to be called.
	m_amt_total = number_of_lines;
	m_font_height = charheight;
	fix_scrollbars();
}

/////////////////////////////////////////////////////////////////////////////
// CCViewScrollView printing

BOOL CCViewScrollView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCViewScrollView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCViewScrollView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCViewScrollView diagnostics

#ifdef _DEBUG
void CCViewScrollView::AssertValid() const
{
	CView::AssertValid();
}

void CCViewScrollView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCViewScrollView message handlers

BOOL CCViewScrollView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	int old_y, y;
	old_y = GetScrollPos(SB_VERT);
	y = old_y;

	switch(LOBYTE(nScrollCode))
	{
	case SB_TOP: //Scroll to top.
		{
			y = 0;
			break;
		}
	case SB_BOTTOM:
		{
			y = INT_MAX;
			break;
		}

	case SB_LINEDOWN: //Scroll one line down.
		{
			y++;
			break;
		}

	case SB_LINEUP: //Scroll one line up.
		{
			y--;
			break;
		}

	case SB_PAGEDOWN:	//Scroll one page down.
		{
			y += m_amt_page;
			break;
		}

	case SB_PAGEUP:		//Scroll one page up.
		{
			y -= m_amt_page;
			break;
		}

	case SB_THUMBTRACK:	//Drag scroll box to specified position. The current position is provided in nPos.
		{
			y = nPos;
			break;
		}
	}

	int old_x, x;
	old_x = GetScrollPos(SB_HORZ);
	x = old_x;

	switch(HIBYTE(nScrollCode))
	{
	case SB_TOP: //Scroll to top.
		{
			x = 0;
			break;
		}
	case SB_BOTTOM:
		{
			x = INT_MAX;
			break;
		}

	case SB_LINEDOWN: //Scroll one line down.
		{
			x++;
			break;
		}

	case SB_LINEUP: //Scroll one line up.
		{
			x--;
			break;
		}

	case SB_PAGEDOWN:	//Scroll one page down.
		{
			x += m_client_screen_width;
			break;
		}

	case SB_PAGEUP:		//Scroll one page up.
		{
			x -= m_client_screen_width;
			break;
		}

	case SB_THUMBTRACK:	//Drag scroll box to specified position. The current position is provided in nPos.
		{
			x = nPos;
			break;
		}
	}


	bool bresult;

	//Take care of the necessary scrolling.
	if (OnScrollBy(CSize(x - old_x, y - old_y), bDoScroll))
	{
		bresult = true;
	}
	else
	{
		bresult = false;
	}
	
	if (bresult && bDoScroll)
	{
		Invalidate(true);
	}

	return(false);
}

void CCViewScrollView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
	{
		//Unknown effect. Research this later.
		return;
	}

	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
	{
		//This isn't the correct scroll bar..
		return;
	}

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
}

void CCViewScrollView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
	{
		//Unknown effect. Research this later.
		return;
	}

	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
	{
		//This isn't the correct scroll bar..
		return;
	}
	OnScroll(MAKEWORD(-1, nSBCode), nPos);
}


BOOL CCViewScrollView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	int y2, y = GetScrollPos(SB_VERT);
	y2 = y;
	if (sizeScroll.cy > 0)
	{
		y2 = min(GetScrollLimit(SB_VERT), y2 + sizeScroll.cy);
	}
	else
	{
		y2 = max(0, y2 + sizeScroll.cy);
	}

	int x2, x = GetScrollPos(SB_HORZ);
	x2 = x;
	if (sizeScroll.cx > 0)
	{
		x2 = min(GetScrollLimit(SB_HORZ), x2 + sizeScroll.cx);
	}
	else
	{
		x2 = max(0, x2 + sizeScroll.cx);
	}


	if (y == y2 && x == x2)
	{
		//There were no changes.
		return(false);
	}

	SetScrollPos(SB_VERT, y2);
	SetScrollPos(SB_HORZ, x2);

	return(true);
}

void CCViewScrollView::TextOut(CDC* pDC, int x, int line_number, int charheight, CString to_view)
{
	if (GetScrollPos(SB_VERT) <= line_number && GetScrollPos(SB_VERT) + m_amt_page + 1 > line_number)
	{
		pDC->TextOut(x - GetScrollPos(SB_HORZ),  (line_number - GetScrollPos(SB_VERT)) * charheight, to_view);
	}

}

void CCViewScrollView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	//EnableScrollBar(SB_BOTH, ESB_ENABLE_BOTH);	
	SetScrollPos(SB_HORZ, 0);
	SetScrollPos(SB_VERT, 0);
	
	fix_scrollbars();
}

void CCViewScrollView::fix_scrollbars()
{
	//EnableScrollBarCtrl(SB_BOTH, true);
	//EnableScrollBarCtrl(SB_VERT, true);

	RECT screen_rect;
	GetClientRect(&screen_rect);//Should use one that counts scroll bars.

	//How many lines can fit in one screen?

	if (m_font_height == 0 || screen_rect.bottom == 0)
	{
		m_amt_page = 0;
	}
	else
	{
		m_amt_page = screen_rect.bottom / m_font_height;
	}
	
	SCROLLINFO info;
	info.fMask = SIF_PAGE | SIF_RANGE;
	info.nMin = 0;
	info.nMax = m_amt_total;
	info.nPage = m_amt_page;
	if (!SetScrollInfo(SB_VERT, &info, true))
	{
		SetScrollRange(SB_VERT, info.nMin, info.nMax, true);
	}
	
	//This m_client_screen_width, is only used for horz scrolling..
	m_client_screen_width = screen_rect.right;

	info.nMin = 0;
	info.nMax = m_greatest_width;
	info.nPage = screen_rect.right;
	if (!SetScrollInfo(SB_HORZ, &info, true))
	{
		SetScrollRange(SB_HORZ, info.nMin, info.nMax, true);
	}
}
