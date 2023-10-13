// ChildFrm.cpp : implementation of the CChildFrame class
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
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_MDIACTIVATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	m_bCreateToolbar = FALSE;
}

CChildFrame::~CChildFrame()
{
	m_ToolBarImageList.DeleteImageList();
	m_ToolBarDisableImageList.DeleteImageList();
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers


void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
#ifdef EMBED_TOOLBAR
	if(!m_bCreateToolbar)return;
	if(bActivate)
		((CFrameWnd*)AfxGetMainWnd())->ShowControlBar(&m_wndToolBar,TRUE,FALSE);
	else
		((CFrameWnd*)AfxGetMainWnd())->ShowControlBar(&m_wndToolBar,FALSE,FALSE);
#endif
}
int CChildFrame::CreateToolBarOnMainFrm(UINT nID,CWnd *pParent)
{
	CWnd *pWnd=NULL;
#ifdef EMBED_TOOLBAR
	pWnd = pParent;
	if(pWnd==NULL)
		pWnd=AfxGetMainWnd();
#else
	pWnd = this;
#endif
	if (!m_wndToolBar.CreateEx(pWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOAT_MULTI|CBRS_SIZE_DYNAMIC   ) ||
		!m_wndToolBar.LoadToolBar(nID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
#ifdef _DISEABLECAPTIONPICTRUE_
		CResManage myRes(g_hInst);
		for(int iIndex  = 0; ; iIndex++)
		{
			if(m_wndToolBar.GetItemID(iIndex) == ID_FILE_PICTURE)
			{
				m_wndToolBar.GetToolBarCtrl().DeleteButton(iIndex);
				m_wndToolBar.GetToolBarCtrl().DeleteButton(iIndex -1);
				break;
			}
		}
#else
		if( (CGlobal::GetInstance()->m_enOpenFileType != CAPFILE) 
			&& (!(CGlobal::GetInstance()->m_enOpenFileType == NETFILE 
						&& CGlobal::GetInstance()->m_arrRelPicFile.GetSize() > 0)) )
		{
			CResManage myRes(g_hInst);
			for(int iIndex  = 0; ; iIndex++)
			{
				if(m_wndToolBar.GetItemID(iIndex) == ID_FILE_PICTURE)
				{
					m_wndToolBar.GetToolBarCtrl().DeleteButton(iIndex);
					m_wndToolBar.GetToolBarCtrl().DeleteButton(iIndex -1);
					break;
				}
			}
		}
#endif		
	m_ToolBarImageList.Create(24,24,ILC_COLOR24|ILC_MASK,0,1);
	m_ToolBarDisableImageList.Create(24,24,ILC_COLOR24|ILC_MASK,0,1);
	
	SetToolBarBitmap();

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

#ifdef EMBED_TOOLBAR
	AfxGetMainWnd()->SendMessage(WM_DOCKTOOLBARLEFTOF,(UINT)&m_wndToolBar);
#endif
	m_wndToolBar.SetOwner(pWnd);
	m_bCreateToolbar = TRUE;
	return 0;
}

void CChildFrame::OnDestroy() 
{

	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
//	AfxGetMainWnd()->SendMessage(WM_RELOADMENU,0,0);
	
}

BOOL CChildFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	BOOL bRet=CMDIChildWnd::DestroyWindow();

	return bRet;
}

void CChildFrame::SetToolBarBitmap()
{
	
}