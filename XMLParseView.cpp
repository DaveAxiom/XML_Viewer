/*
XML Viewer
Copyright 2017 David V. Makray

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// XMLParseView.cpp : implementation of the CXMLParseView class
//

#include "stdafx.h"

#include "CViewScrollView.h"
#include "XMLParse.h"

#include "XMLHandler.h"
#include "XMLParseDoc.h"
#include "XMLParseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXMLParseView

IMPLEMENT_DYNCREATE(CXMLParseView, CCViewScrollView)

BEGIN_MESSAGE_MAP(CXMLParseView, CCViewScrollView)
	//{{AFX_MSG_MAP(CXMLParseView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CCViewScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CCViewScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CCViewScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMLParseView construction/destruction

CXMLParseView::CXMLParseView()
{
	// TODO: add construction code here

}

CXMLParseView::~CXMLParseView()
{
}

BOOL CXMLParseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CCViewScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CXMLParseView drawing

void CXMLParseView::OnDraw(CDC* pDC)
{
	CXMLParseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CFont thin_font;
	thin_font.CreatePointFont(100, "Courier", pDC);
	pDC->SelectObject(&thin_font);

	//Get the text ready.
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int charheight = tm.tmHeight + tm.tmExternalLeading;
	int line_number = 0;
	CString to_view;

	//Get the information ready
	pDoc->m_xml_handler->Display(true);//Begin
	DisplayInfo dis_info;
	dis_info.m_text = "temp";
	
	dis_info = pDoc->m_xml_handler->DisplayNext();
	int indent = 0;
	
	COLORREF default_text_color = pDC->GetTextColor();
	int max_width = 0;

	while(dis_info.m_text != "")
	{
		//Get this information onto the view..
		to_view.Format("%s",dis_info.m_text);

		if (dis_info.m_change_indent != 0)
		{
			indent += dis_info.m_change_indent;
		}

		//Change color here to identify tag or text..
		if (dis_info.m_node_state != ns_tag)
		{
			pDC->SetTextColor(default_text_color);
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,255));
 		}

		max_width = max((pDC->GetOutputTextExtent(to_view)).cx + indent*30, max_width);
		TextOut(pDC, indent*30, line_number++, charheight, to_view);
		
		dis_info = pDoc->m_xml_handler->DisplayNext();
	}
	m_greatest_width = max_width;
	m_font_height = charheight;
	m_amt_total = line_number;
	fix_scrollbars();
	
	pDoc->m_xml_handler->Display(false);//End
	
	//SetScrollSizes( MM_TEXT, CSize(max_width, line_number * charheight));

}

/////////////////////////////////////////////////////////////////////////////
// CXMLParseView printing

BOOL CXMLParseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CXMLParseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CXMLParseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CXMLParseView diagnostics

void CXMLParseView::OnInitialUpdate()
{
	CCViewScrollView::OnInitialUpdate();
}


#ifdef _DEBUG
void CXMLParseView::AssertValid() const
{
	CCViewScrollView::AssertValid();
}

void CXMLParseView::Dump(CDumpContext& dc) const
{
	CCViewScrollView::Dump(dc);
}

CXMLParseDoc* CXMLParseView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXMLParseDoc)));
	return (CXMLParseDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXMLParseView message handlers

