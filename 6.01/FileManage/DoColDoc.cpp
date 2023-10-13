// DoColDoc.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DoColDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoColDoc

IMPLEMENT_DYNCREATE(CDoColDoc, CDocument)

CDoColDoc::CDoColDoc()
{
	CGlobal *pGlobal=CGlobal::GetInstance();
	m_Mgr.SetFileDB(&pGlobal->m_FileDB);
}

BOOL CDoColDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDoColDoc::~CDoColDoc()
{
}


BEGIN_MESSAGE_MAP(CDoColDoc, CDocument)
	//{{AFX_MSG_MAP(CDoColDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoColDoc diagnostics

#ifdef _DEBUG
void CDoColDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDoColDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDoColDoc serialization

void CDoColDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDoColDoc commands
