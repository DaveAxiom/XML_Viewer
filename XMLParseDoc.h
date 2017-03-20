// XMLParseDoc.h : interface of the CXMLParseDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPARSEDOC_H__6C99A36F_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_)
#define AFX_XMLPARSEDOC_H__6C99A36F_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXMLParseDoc : public CDocument
{
protected: // create from serialization only
	CXMLParseDoc();
	DECLARE_DYNCREATE(CXMLParseDoc)

// Attributes
public:

// Operations
public:

	XMLHandler* m_xml_handler;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMLParseDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXMLParseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXMLParseDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMLPARSEDOC_H__6C99A36F_40DB_11D3_AF5A_004F4902BCDD__INCLUDED_)
