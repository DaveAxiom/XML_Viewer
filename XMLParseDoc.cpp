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

// XMLParseDoc.cpp : implementation of the CXMLParseDoc class
//

#include "stdafx.h"
#include "XMLParse.h"

#include "XMLHandler.h"
#include "XMLParseDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CXMLParseDoc

IMPLEMENT_DYNCREATE(CXMLParseDoc, CDocument)

BEGIN_MESSAGE_MAP(CXMLParseDoc, CDocument)
	//{{AFX_MSG_MAP(CXMLParseDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMLParseDoc construction/destruction

CXMLParseDoc::CXMLParseDoc()
{
	m_xml_handler = new XMLHandler();

}

CXMLParseDoc::~CXMLParseDoc()
{
	delete m_xml_handler;
}

BOOL CXMLParseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CXMLParseDoc serialization

void CXMLParseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		/*The purpose of this program is to load and parse.*/
		return;
	}
	else
	{
		//Load away.. Read the file in now...
		CString read_in;

		bool read_success = true;
		while (read_success)
		{

			if (ar.ReadString(read_in))
			{
				read_success = true;
			}
			else
			{
				read_success = false;
			}

			m_xml_handler->add_xml_text(read_in);
			read_in.Empty();
		}

	}
}

/////////////////////////////////////////////////////////////////////////////
// CXMLParseDoc diagnostics

#ifdef _DEBUG
void CXMLParseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CXMLParseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXMLParseDoc commands
