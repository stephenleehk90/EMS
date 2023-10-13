// DoFileDoc.cpp : implementation file
//

#include "stdafx.h"
#include "filemanage.h"
#include "DoFileDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoFileDoc

IMPLEMENT_DYNCREATE(CDoFileDoc, CDocument)

CDoFileDoc::CDoFileDoc()
{
	m_Mgr.LoadDefaultSetting();
	CGlobal *pGlobal=CGlobal::GetInstance();
	m_Mgr.SetFileDB(&pGlobal->m_FileDB);
}

BOOL CDoFileDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDoFileDoc::~CDoFileDoc()
{
	m_Mgr.WriteDefaultSetting();
}


BEGIN_MESSAGE_MAP(CDoFileDoc, CDocument)
	//{{AFX_MSG_MAP(CDoFileDoc)
	ON_COMMAND(IDT_EDIT, OnEdit)
	ON_UPDATE_COMMAND_UI(IDT_EDIT, OnUpdateEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoFileDoc diagnostics

#ifdef _DEBUG
void CDoFileDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDoFileDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDoFileDoc serialization

void CDoFileDoc::Serialize(CArchive& ar)
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
// CDoFileDoc commands

void CDoFileDoc::OnEdit() 
{
	// TODO: Add your command handler code here
	m_Mgr.OpenFile(FALSE);
}

void CDoFileDoc::OnUpdateEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_Mgr.m_bEdit&&m_Mgr.m_nItem!=-1);
}
