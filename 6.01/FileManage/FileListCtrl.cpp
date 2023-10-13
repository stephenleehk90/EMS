// FileListCtrl.cpp: implementation of the CFileListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "filemanage.h"
#include "FileListCtrl.h"
#include "resource.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileListCtrl::CFileListCtrl()
{
	m_nUserCol = 5;
}

CFileListCtrl::~CFileListCtrl()
{

}
const int OFFSET_FIRST =	2;
const int OFFSET_OTHER =	6;

void CFileListCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	UINT uiFlags=ILD_TRANSPARENT;
	CImageList* pImageList;
	int nItem=lpDrawItemStruct->itemID;
	BOOL bFocus=(GetFocus()==this);
	//COLORREF clrTextSave, clrBkSave;
	COLORREF clrBkSave;
	COLORREF clrImage=m_clrBkgnd;
	static _TCHAR szBuff[MAX_PATH];
	TCHAR szFlow[30];
	LPCTSTR pszText;
	
// get item data
	COLORREF nClr;
	::SendMessage(m_hParent,WM_GETLISTCTRLTEXTCLR,(WPARAM)&nClr,0);
	LV_ITEM lvi;
	lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem=nItem;
	lvi.iSubItem=0;
	lvi.pszText=szBuff;
	lvi.cchTextMax=sizeof(szBuff);
	lvi.stateMask=0xFFFF;		// get all state flags
	GetItem(&lvi);

	BOOL bSelected=(bFocus || (GetStyle() & LVS_SHOWSELALWAYS)) && lvi.state & LVIS_SELECTED;
	bSelected=bSelected || (lvi.state & LVIS_DROPHILITED);

// set colors if item is selected

	CRect rcAllLabels;
	GetItemRect(nItem,rcAllLabels,LVIR_BOUNDS);
	CRect rcLabel;
	GetItemRect(nItem,rcLabel,LVIR_LABEL);
	rcAllLabels.left=rcLabel.left;
	if(m_bClientWidthSel && rcAllLabels.right<m_cxClient)
		rcAllLabels.right=m_cxClient;

	BOOL bNormal=FALSE;
	LV_ITEM lvi1;
	lvi1.mask=LVIF_TEXT ;
	lvi1.iItem=nItem;
	lvi1.iSubItem=3;
	lvi1.pszText=szFlow;
	lvi1.cchTextMax=sizeof(szFlow);	
	GetItem(&lvi1);

	CString strUser;
	strUser = GetItemText(nItem,m_nUserCol);
	/*if(m_bWarnColor&&lvi.iImage==m_nWarnImage)
	{
			pDC->SetTextColor(RGB(255,0,255));
//				clrTextSave=m_clrText;
	}
	else */
	{
		if(lvi.iImage == 2)//«æ½Z
			pDC->SetTextColor(RGB(255,0,0));
		else 
		{
			CString strSend;
			strSend.LoadString(IDS_SENDFILE);
			if(strSend.CompareNoCase(szFlow)==0&&!bSelected)
			{
				CGlobal *pGlobal=CGlobal::GetInstance();
				if((strUser.Compare(pGlobal->m_pEmployee->m_strUserCode)==0))
					pDC->SetTextColor(RGB(0,0,255));
				else
					pDC->SetTextColor(RGB(0,128,0));
			}
			else
			{
				pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
				bNormal = TRUE;
			}
		}
	}
	if(bSelected)
	{
		if(bNormal)
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		clrBkSave=pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(rcAllLabels,&CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
	}
	else
	{
		if(m_nTextBkShowMode==1)
			pDC->FillRect(rcAllLabels,&CBrush(m_clrTextBk));
		else if(m_nTextBkShowMode==2)
		{
			if(nItem%2==0)
				pDC->FillRect(rcAllLabels,&CBrush(m_clrTextBkEven));
			else
				pDC->FillRect(rcAllLabels,&CBrush(m_clrTextBkOdd));
		}
	}

// set color and mask for the icon

	if(lvi.state & LVIS_CUT)
	{
		clrImage=m_clrBkgnd;
		uiFlags|=ILD_BLEND50;
	}
	else if(bSelected)
	{
		clrImage=::GetSysColor(COLOR_HIGHLIGHT);
		uiFlags|=ILD_BLEND50;
	}

// draw state icon

	UINT nStateImageMask=lvi.state & LVIS_STATEIMAGEMASK;
	if(nStateImageMask)
	{
		int nImage=(nStateImageMask>>12)-1;
		pImageList=GetImageList(LVSIL_STATE);
		if(pImageList)
			pImageList->Draw(pDC,nImage,CPoint(rcItem.left,rcItem.top),ILD_TRANSPARENT);
	}

// draw normal and overlay icon

	CRect rcIcon;
	GetItemRect(nItem,rcIcon,LVIR_ICON);

	pImageList=GetImageList(LVSIL_SMALL);
	if(pImageList)
	{
		UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
		if(rcItem.left<rcItem.right-1)
			ImageList_DrawEx(pImageList->m_hImageList,lvi.iImage,pDC->m_hDC,rcIcon.left,rcIcon.top,16,16,m_clrBkgnd,clrImage,uiFlags | nOvlImageMask);
	}
	
// draw item label

	GetItemRect(nItem,rcItem,LVIR_LABEL);
	//rcItem.right -=m_cxStateImageOffset;//comment by wxy

	pszText=MakeShortString(pDC,szBuff,rcItem.right-rcItem.left,2*OFFSET_FIRST);

	rcLabel=rcItem;
	rcLabel.left+=OFFSET_FIRST;
	rcLabel.right-=OFFSET_FIRST;

	if (!((rcItem.right-rcItem.left ==0) /*&& m_bMakeShort == FALSE*/))
		pDC->DrawText(pszText,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

// draw labels for extra columns

	LV_COLUMN lvc;
	lvc.mask=LVCF_FMT | LVCF_WIDTH;

	for(int nColumn=1; GetColumn(nColumn,&lvc); nColumn++)
	{
		rcItem.left=rcItem.right;
		rcItem.right+=lvc.cx;

		int nRetLen=GetItemText(nItem,nColumn,szBuff,sizeof(szBuff));
		if(nRetLen==0) continue;

		pszText=MakeShortString(pDC,szBuff,rcItem.right-rcItem.left,2*OFFSET_OTHER);

		UINT nJustify=DT_LEFT;

		if(pszText==szBuff)
		{
			switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_RIGHT:
				nJustify=DT_RIGHT;
				break;
			case LVCFMT_CENTER:
				nJustify=DT_CENTER;
				break;
			default:
				break;
			}
		}

		rcLabel=rcItem;
		rcLabel.left+=OFFSET_OTHER;
		rcLabel.right-=OFFSET_OTHER;

		if (!((rcItem.right-rcItem.left ==0) /*&& m_bMakeShort == FALSE*/))
			pDC->DrawText(pszText,-1,rcLabel,nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
	}

// draw focus rectangle if item has focus

	if(lvi.state & LVIS_FOCUSED && bFocus)
		pDC->DrawFocusRect(rcAllLabels);

// set original colors if item was selected

	if(bSelected)
	{
	 //   pDC->SetTextColor(clrTextSave);
		pDC->SetBkColor(clrBkSave);
	}
}
