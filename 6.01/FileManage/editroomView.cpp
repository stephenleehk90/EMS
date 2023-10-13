// editroomView.cpp : implementation of the CEditroomView class
//

#include "stdafx.h"
#include "resource.h"

#include "filemanage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditroomView

IMPLEMENT_DYNCREATE(CEditroomView, CEditProView)

BEGIN_MESSAGE_MAP(CEditroomView, CEditProView)
	//{{AFX_MSG_MAP(CEditroomView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_READONLY, OnUpdateEditReadonly)
	ON_COMMAND(ID_EDIT_READONLY, OnEditReadonly)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditroomView construction/destruction

CEditroomView::CEditroomView()
{
	// TODO: add construction code here

}

CEditroomView::~CEditroomView()
{
}

BOOL CEditroomView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CEditProView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEditroomView diagnostics

#ifdef _DEBUG
void CEditroomView::AssertValid() const
{
	CEditProView::AssertValid();
}

void CEditroomView::Dump(CDumpContext& dc) const
{
	CEditProView::Dump(dc);
}
#endif //_DEBUG

CEditroomDoc* CEditroomView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditroomDoc)));
	return (CEditroomDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CEditroomView message handlers


void CEditroomView::OnUpdateEditReadonly(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CEditroomDoc *pDoc = (CEditroomDoc*)GetDocument();
	pCmdUI->Enable(pDoc->m_FileParam.bCanEdit&&pDoc->m_FileParam.bEdit);
	pCmdUI->SetCheck(pDoc->m_byteStatus&FS_RDONLY);
}

void CEditroomView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	if(bActivate)
	{
		TRACE("\nTRUE: Actived :%p,  Deactived: %p", pActivateView, pDeactiveView);
		if (pActivateView == pDeactiveView)
		{
			((CFrameWnd*)AfxGetMainWnd())->RecalcLayout();
			CEditroomDoc* pdoc = GetDocument();
			if(!pdoc->m_hDefaultMenu)
			{
				pdoc->m_hDefaultMenu = 
					::LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_EDITROTYPE));
				if (pdoc->m_hDefaultMenu == NULL)
					return;
			}
			// Get the parent window of this view window. The parent window is
			// a CMDIChildWnd-derived class. We can then obtain the MDI parent 
			// frame window using the CMDIChildWnd*. Then, replace the current 
			// menu bar with the new loaded menu resource.
			CMDIFrameWnd* frame = ((CMDIChildWnd *) GetParent()->GetParent())->GetMDIFrame();
			frame->MDISetMenu(CMenu::FromHandle(pdoc->m_hDefaultMenu), NULL);
			frame->DrawMenuBar();
			TRACE("\nDrawMenuBar");
		}
	}
	else
	{
		TRACE("\nFALSE: Actived :%p,  Deactived: %p", pActivateView, pDeactiveView);
		if (pActivateView == pDeactiveView)
			AfxGetMainWnd()->SendMessage(WM_RELOADMENU,0,0);
	}
*/
	Invalidate();
	CEditProView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CEditroomView::OnInitialUpdate() 
{
	CEditProView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

void CEditroomView::OnEditReadonly() 
{
	// TODO: Add your command handler code here
	CEditProView::OnEditReadonly();
/*	if(GetDocument()->m_byteStatus&FS_RDONLY)
		GetDocument()->UnLockFile();
	else
		GetDocument()->OnDoModify();*/
}
