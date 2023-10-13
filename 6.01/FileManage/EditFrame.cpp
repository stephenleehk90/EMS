// EditFrame.cpp: implementation of the CEditFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "filemanage.h"
#include "DlgDisplyPicture.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern HACCEL g_hAccel;
/////////////////////////////////////////////////////////////////////////////
// CDoFileFrm

IMPLEMENT_DYNCREATE(CEditFrame, CChildFrame)

CEditFrame::CEditFrame()
{
	m_pDlgDisplyPicture = NULL;	
}

CEditFrame::~CEditFrame()
{
	if(m_pDlgDisplyPicture != NULL)
	{
		if(m_pDlgDisplyPicture->GetSafeHwnd() != NULL)
			m_pDlgDisplyPicture->DestroyWindow();
		delete m_pDlgDisplyPicture;
		m_pDlgDisplyPicture = NULL;
	}
}



BEGIN_MESSAGE_MAP(CEditFrame, CChildFrame)
	//{{AFX_MSG_MAP(CDoFileFrm)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PICTURE, OnCaptionPicture)
	ON_UPDATE_COMMAND_UI(ID_FILE_PICTURE, OnUpdateCaptionPicture)
	ON_WM_DESTROY()

END_MESSAGE_MAP()

int CEditFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CChildFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	
#ifndef _DISEABLECAPTIONPICTRUE_
	if ((!CGlobal::GetInstance()->m_bPrintDirect) &&((CGlobal::GetInstance()->m_enOpenFileType == CAPFILE) 
		|| (CGlobal::GetInstance()->m_enOpenFileType == NETFILE 
					&& CGlobal::GetInstance()->m_arrRelPicFile.GetSize() > 0)))
	{
		if(m_pDlgDisplyPicture == NULL)
		{
			m_pDlgDisplyPicture = new CDlgDisplyPicture;
		}
		if(m_pDlgDisplyPicture == NULL)
			return -1;
	}
#endif

	m_bCaptionPicShowed = FALSE;
	CreateToolBarOnMainFrm(IDR_EDITTOOLBAR);
	// TODO: Add your specialized creation code here
	CreateModifyToolbar();
	
	return 0;
}
BOOL CEditFrame::PreTranslateMessage(MSG* pMsg) 
{
	if(g_hAccel!=NULL)
		
		if(::TranslateAccelerator(m_hWnd,g_hAccel,pMsg))
			
			return TRUE;
		
		return CChildFrame::PreTranslateMessage(pMsg);		
}
BOOL MergeMenu(CMenu * pMenuDestination, const CMenu * pMenuAdd, bool bTopLevel =FALSE);
BOOL MergeMenu(CMenu * pMenuDestination, const CMenu * pMenuAdd, bool bTopLevel /*=false*/)
{
    // Abstract:
    //      Merges two menus.
    //
    // Parameters:
    //      pMenuDestination    - [in, retval] destination menu handle
    //      pMenuAdd            - [in] menu to merge
    //      bTopLevel           - [in] indicator for special top level behavior
    //
    // Return value:
    //      <false> in case of error.
    //
    // Comments:
    //      This function calles itself recursivley. If bTopLevel is set to true,
    //      we append popups at top level or we insert before <Window> or <Help>.

    // get the number menu items in the menus
    int iMenuAddItemCount = pMenuAdd->GetMenuItemCount();
    int iMenuDestItemCount = pMenuDestination->GetMenuItemCount();
    
    // if there are no items return
    if( iMenuAddItemCount == 0 )
        return true;
    
    // if we are not at top level and the destination menu is not empty
    // -> we append a seperator
   // if( !bTopLevel && iMenuDestItemCount > 0 )
      //  pMenuDestination->AppendMenu(MF_SEPARATOR);
    
    // iterate through the top level of <PMENUADD>
    for( int iLoop = 0; iLoop < iMenuAddItemCount; iLoop++ )
    {
        // get the menu string from the add menu
        CString sMenuAddString;
        pMenuAdd->GetMenuString( iLoop, sMenuAddString, MF_BYPOSITION );
        
        // try to get the submenu of the current menu item
        CMenu* pSubMenu = pMenuAdd->GetSubMenu(iLoop);
        
        // check if we have a sub menu
        if (!pSubMenu)
        {
            // normal menu item
            // read the source and append at the destination
            UINT nState = pMenuAdd->GetMenuState( iLoop, MF_BYPOSITION );
            UINT nItemID = pMenuAdd->GetMenuItemID( iLoop );
            
            if( pMenuDestination->AppendMenu( nState, nItemID, sMenuAddString ))
            {
                // menu item added, don't forget to correct the item count
                iMenuDestItemCount++;
            }
            else
            {
                TRACE( L"MergeMenu: AppendMenu failed!\n" );
                return false;
            }
        }
        else
        {
            // create or insert a new popup menu item
            
            // default insert pos is like ap
            int iInsertPosDefault = -1;
            
            // if we are at top level merge into existing popups rather than
            // creating new ones
            if( bTopLevel )
            {
                ASSERT( sMenuAddString != "&?" && sMenuAddString != "?" );
                CString sAdd( sMenuAddString );
                //sAdd.Remove('&');  // for comparison of menu items supress '&'
                bool bAdded = false;

                // try to find existing popup
                for( int iLoop1 = 0; iLoop1 < iMenuDestItemCount; iLoop1++ )
                {
                    // get the menu string from the destination menu
                    CString sDest;
                    pMenuDestination->GetMenuString( iLoop1, sDest, MF_BYPOSITION );
                   // sDest.Remove( '&' ); // for a better compare (s.a.)
                    
                    if( sAdd == sDest )
                    {
                        // we got a hit -> merge the two popups
                        // try to get the submenu of the desired destination menu item
                        CMenu* pSubMenuDest = pMenuDestination->GetSubMenu( iLoop1 );
                        
                        if( pSubMenuDest )
                        {
                            // merge the popup recursivly and continue with outer for loop
                            if( !MergeMenu( pSubMenuDest, pSubMenu ))
                                return false;
                            
                            bAdded = true;
                            break;
                        }
                    }

                    // alternativ insert before <Window> or <Help>
                    if( iInsertPosDefault == -1 && ( sDest == "工具(&O)" || sDest == "?" || sDest == "Help" ))
                        iInsertPosDefault = iLoop1;

                }
                
                if( bAdded )
                {
                    // menu added, so go on with loop over pMenuAdd's top level
                    continue;
                }
            }

            // if the top level search did not find a position append the menu
            if( iInsertPosDefault == -1 )
                iInsertPosDefault = pMenuDestination->GetMenuItemCount();
            
            // create a new popup and insert before <Window> or <Help>
            CMenu NewPopupMenu;
            if( !NewPopupMenu.CreatePopupMenu() )
            {
                TRACE( L"MergeMenu: CreatePopupMenu failed!\n" );
                return false;
            }
            
            // merge the new popup recursivly
            if( !MergeMenu( &NewPopupMenu, pSubMenu ))
                return false;
            
            // insert the new popup menu into the destination menu
            HMENU hNewMenu = NewPopupMenu.GetSafeHmenu();

            if( pMenuDestination->InsertMenu( iInsertPosDefault,
                MF_BYPOSITION | MF_POPUP | MF_ENABLED, 
                (UINT)hNewMenu, sMenuAddString ))
            {
                // don't forget to correct the item count
                iMenuDestItemCount++;
            }
            else
            {
                TRACE( L"MergeMenu: InsertMenu failed!\n" );
                return false;
            }

            // don't destroy the new menu       
            NewPopupMenu.Detach();
        } 
    } 
    
    return true;
}
BOOL RemoveMenu(CMenu * pMenuDestination, const CMenu * pMenuDel, bool bTopLevel =false);
BOOL RemoveMenu(CMenu * pMenuDestination, const CMenu * pMenuDel, bool bTopLevel /*=false*/)
{
    // Abstract:
    //      Merges two menus.
    //
    // Parameters:
    //      pMenuDestination    - [in, retval] destination menu handle
    //      pMenuAdd            - [in] menu to merge
    //      bTopLevel           - [in] indicator for special top level behavior
    //
    // Return value:
    //      <false> in case of error.
    //
    // Comments:
    //      This function calles itself recursivley. If bTopLevel is set to true,
    //      we append popups at top level or we insert before <Window> or <Help>.

    // get the number menu items in the menus
    int iMenuDelItemCount = pMenuDel->GetMenuItemCount();
    int iMenuDestItemCount = pMenuDestination->GetMenuItemCount();
    
    // if there are no items return
    if( iMenuDelItemCount == 0 )
        return true;
    
    // if we are not at top level and the destination menu is not empty
    // -> we delete the seperator
    //if( !bTopLevel && iMenuDestItemCount > 0 )
     //   pMenuDestination->RemoveMenu(iMenuDestItemCount,MF_BYPOSITION);
    
    // iterate through the top level of <PMENUADD>
    for( int iLoop = 0; iLoop < iMenuDelItemCount; iLoop++ )
    {
        // get the menu string from the add menu
        CString sMenuAddString;
        pMenuDel->GetMenuString( iLoop, sMenuAddString, MF_BYPOSITION );
        
        // try to get the submenu of the current menu item
        CMenu* pSubMenu = pMenuDel->GetSubMenu(iLoop);
        
        // check if we have a sub menu
        if (!pSubMenu)
        {
            // normal menu item
            UINT nItemID = pMenuDel->GetMenuItemID( iLoop );
            
            if( pMenuDestination->RemoveMenu( nItemID, MF_BYCOMMAND ))
            {
                // menu item deleted, don't forget to correct the item count
                iMenuDestItemCount--;
            }
            else
            {
                TRACE( L"MergeMenu: AppendMenu failed!\n" );
                return false;
            }
        }
        else
        {
            // create or insert a new popup menu item
            
            // default insert pos is like ap
            int iInsertPosDefault = -1;
            
            // if we are at top level merge into existing popups rather than
            // creating new ones
            if( bTopLevel )
            {
                ASSERT( sMenuAddString != "&?" && sMenuAddString != "?" );
                CString sAdd( sMenuAddString );
                //sAdd.Remove('&');  // for comparison of menu items supress '&'
                bool bAdded = false;

                // try to find existing popup
                for( int iLoop1 = 0; iLoop1 < iMenuDestItemCount; iLoop1++ )
                {
                    // get the menu string from the destination menu
                    CString sDest;
                    pMenuDestination->GetMenuString( iLoop1, sDest, MF_BYPOSITION );
                   // sDest.Remove( '&' ); // for a better compare (s.a.)
                    
                    if( sAdd == sDest )
                    {
                        // we got a hit -> merge the two popups
                        // try to get the submenu of the desired destination menu item
                        CMenu* pSubMenuDest = pMenuDestination->GetSubMenu( iLoop1 );
                        
                        if( pSubMenuDest )
                        {
                            // merge the popup recursivly and continue with outer for loop
                            if( !RemoveMenu( pSubMenuDest, pSubMenu ))
                                return false;
                            
                            if(pSubMenuDest->GetMenuItemCount()==0)
								pMenuDestination->RemoveMenu(iLoop1,MF_BYPOSITION);
                            break;
                        }
                    }

                }
                
            }
        } 
    } 
    
    return true;
}
void CEditFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CChildFrame::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	// TODO: Add your message handler code here
	if (bActivate)
	{
#ifndef _DISEABLECAPTIONPICTRUE_
		CGlobal *pGlobal = CGlobal::GetInstance();
		if(m_pDlgDisplyPicture != NULL && (pGlobal->m_enOpenFileType == CAPFILE 
			|| (pGlobal->m_enOpenFileType == NETFILE 
					&& pGlobal->m_arrRelPicFile.GetSize() > 0)) )
		{
			if(m_pDlgDisplyPicture->GetSafeHwnd() == NULL)
			{
				CResManage myRes(g_hInst);
				m_pDlgDisplyPicture->Create(IDD_DLGPICTURE, this);
				RECT rcFrame;
				this->GetActiveView()->GetWindowRect(&rcFrame);
				m_pDlgDisplyPicture->SetFrameRect(rcFrame);
				if(pGlobal->m_enOpenFileType == CAPFILE)
					m_pDlgDisplyPicture->SetPictureFile(pGlobal->m_strCaptionPicFile);
				else if(pGlobal->m_enOpenFileType == NETFILE)
					m_pDlgDisplyPicture->SetPictureFile(pGlobal->m_arrRelPicFile);
				m_pDlgDisplyPicture->ShowCaptionPicture();
			}
			m_pDlgDisplyPicture->ShowWindow(SW_NORMAL);
			this->SetFocus();
			m_bCaptionPicShowed = TRUE;
		}
#endif
		
		CEditroomDoc* pDoc = (CEditroomDoc*)GetActiveDocument();

		if(!pDoc->m_hDefaultMenu)
		{
			pDoc->m_hDefaultMenu = 
				::LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_EDITROTYPE));
			if (pDoc->m_hDefaultMenu == NULL)
				return;
		}
		
		CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
		pFrame->RecalcLayout();
		//pFrame->MDISetMenu(CMenu::FromHandle(pDoc->m_hDefaultMenu), NULL);
		CMenu *pMainMenu=pFrame->GetMenu();
		CMenu *pEditMenu=CMenu::FromHandle(pDoc->m_hDefaultMenu);
		if (pMainMenu && pEditMenu)
			MergeMenu(pMainMenu,pEditMenu,TRUE);
		pFrame->DrawMenuBar();
	}
	else
	{
#ifndef _DISEABLECAPTIONPICTRUE_

		//if(CGlobal::GetInstance()->m_enOpenFileType == CAPFILE)
		if(m_pDlgDisplyPicture->GetSafeHwnd() != NULL)
		{
			m_pDlgDisplyPicture->ShowWindow(SW_HIDE);
			m_bCaptionPicShowed = FALSE;
		}
#endif
		//AfxGetMainWnd()->SendMessage(WM_RELOADMENU,0,0);
		CEditroomDoc* pDoc = (CEditroomDoc*)GetActiveDocument();
		
		if(!pDoc->m_hDefaultMenu)
		{
			pDoc->m_hDefaultMenu = 
				::LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_EDITROTYPE));
			if (pDoc->m_hDefaultMenu == NULL)
				return;
		}
		
		CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
		pFrame->RecalcLayout();
		//pFrame->MDISetMenu(CMenu::FromHandle(pDoc->m_hDefaultMenu), NULL);
		CMenu *pMainMenu=pFrame->GetMenu();
		CMenu *pEditMenu=CMenu::FromHandle(pDoc->m_hDefaultMenu);
		if (pMainMenu && pEditMenu)
			RemoveMenu(pMainMenu,pEditMenu,TRUE);
		pFrame->DrawMenuBar();
	}
}


void CEditFrame::SetToolBarBitmap()
{
	CResManage myRes(g_hInst);
	
	CBitmap bitmap;
	//提交稿件
	bitmap.LoadBitmap(IDB_BITMAP_SUBMIT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//復原鍵入
	bitmap.LoadBitmap(IDB_BITMAP_UNDO);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//重復鍵入
	bitmap.LoadBitmap(IDB_BITMAP_REDO);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//修改
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//剪下
	bitmap.LoadBitmap(IDB_BITMAP_CUT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//複製
	bitmap.LoadBitmap(IDB_BITMAP_COPY);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//貼上
	bitmap.LoadBitmap(IDB_BITMAP_PASTE);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//預覽列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINTPREVIEW);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//字詞檢查
	bitmap.LoadBitmap(IDB_BITMAP_PRINT);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();

	//顯示圖片
	bitmap.LoadBitmap(IDB_BITMAP_CAPTIONPIC);
	m_ToolBarImageList.Add(&bitmap,ARTMASKCOLOR);
	bitmap.DeleteObject();
	
	//Now, set the disabled toolbar

	//提交稿件
	bitmap.LoadBitmap(IDB_BITMAP_SUBMIT_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//復原鍵入
	bitmap.LoadBitmap(IDB_BITMAP_UNDO_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//重復鍵入
	bitmap.LoadBitmap(IDB_BITMAP_REDO_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//修改
	bitmap.LoadBitmap(IDB_BITMAP_MODIFY_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//剪下
	bitmap.LoadBitmap(IDB_BITMAP_CUT_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//複製
	bitmap.LoadBitmap(IDB_BITMAP_COPY_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//貼上
	bitmap.LoadBitmap(IDB_BITMAP_PASTE_DISABLE);
//	bitmap.LoadBitmap(IDB_BITMAP_CHECK_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//預覽列印
	bitmap.LoadBitmap(IDB_BITMAP_PRINTPREVIEW_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	
	//字詞檢查
	bitmap.LoadBitmap(IDB_BITMAP_PASTE_DISABLE);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();
	

	//顯示圖片
	bitmap.LoadBitmap(IDB_BITMAP_CAPTIONPIC);
	m_ToolBarDisableImageList.Add(&bitmap,ARTDISABLEDMASKCOLOR);
	bitmap.DeleteObject();


	CImageList *pImagelist=m_wndToolBar.GetToolBarCtrl().GetImageList();
	if(pImagelist)
		pImagelist->DeleteImageList();
	pImagelist=m_wndToolBar.GetToolBarCtrl().GetDisabledImageList();
	if(pImagelist)
		pImagelist->DeleteImageList();

	m_wndToolBar.GetToolBarCtrl().SetButtonSize(TOOLBARBUTTONSIZE);
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(TOOLBARBITMAPSIZE);
	
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarImageList);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarDisableImageList);
}

void CEditFrame::OnDestroy() 
{

	CChildFrame::OnDestroy();
}

void CEditFrame::OnCaptionPicture()
{
	m_bCaptionPicShowed = !m_bCaptionPicShowed;
	if(m_bCaptionPicShowed)
	{
		if(m_pDlgDisplyPicture->GetSafeHwnd() == NULL)
		{
			CResManage myRes(g_hInst);
			m_pDlgDisplyPicture->Create(IDD_DLGPICTURE, this);
			RECT rcFrame;
			this->GetActiveView()->GetWindowRect(&rcFrame);
			m_pDlgDisplyPicture->SetFrameRect(rcFrame);
			m_pDlgDisplyPicture->ShowCaptionPicture();
		}
		this->SetFocus();
		m_pDlgDisplyPicture->ShowWindow(SW_NORMAL);
	}
	else
	{
		if(m_pDlgDisplyPicture->GetSafeHwnd() != NULL)
			m_pDlgDisplyPicture->ShowWindow(SW_HIDE);
	}
}

void CEditFrame::OnUpdateCaptionPicture(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bCaptionPicShowed);
}