// DoFileMgr.cpp: implementation of the CDoFileMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "filemanage.h"
#include "DoFileMgr.h"
#include "..\fileoper\fileopermgr.h"
#include "../commkits/dostools.h"
#include "filereport.h"
#include "DoFileFrm.h"
#include "listitem.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "LinkToDMDlg.h"
#include "DlgAddToStory.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoFileMgr::CDoFileMgr()
{
	m_strPlaceName = _T("");
	m_enDateType = ALL;
	m_enQueryMod = BYPLACE;	
	m_strSearchFile =  _T("");

	m_bCanUnlink = FALSE; 
	m_bCanlink = FALSE;
	m_bLinked = FALSE; 
	m_enFileViewMode = EN_FVDOFILE;
	m_bDelete = FALSE;
	m_bSendCol = FALSE;
	m_bGetFile = FALSE;
	m_bOwnFile = FALSE;
	m_bFillTrans = FALSE;
	m_bReadTrans = FALSE;
	m_bTranslate = FALSE;
	m_bSendFile = FALSE;
	m_bSignDirect = FALSE;
	m_bDefaultLib = FALSE;
	
	m_nLibPlacePerm = 0;

}

CDoFileMgr::~CDoFileMgr()
{

	
}

void CDoFileMgr::LoadDefaultSetting()
{
	CWinApp *pApp=AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	if(CGlobal::GetInstance()->str_Share.GetLength()>0)
	{
		m_nDepartID = _wtoi(CGlobal::GetInstance()->strLibId);
		m_enDateType = ALL;
		m_enQueryMod = BYPLACE;
		m_strPlaceName = CGlobal::GetInstance()->strPlaceCode;
	}
	else
	{
		m_nDepartID =GetPrivateProfileIntW( L"DoFile" , L"LibID",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
		wchar_t sz_temp[255];
		GetPrivateProfileStringW( L"DoFile" , L"Place",L"",sz_temp,255,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_strPlaceName = sz_temp;
		m_enDateType = (EN_DATETYPE)GetPrivateProfileIntW( L"DoFile" , L"DateType",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
		m_enQueryMod = (EN_QUERYMOD)GetPrivateProfileIntW( L"DoFile" , L"QueryMod",0,((CWinApp*)AfxGetApp())->m_pszProfileName);
	}
	/*m_nDepartID = pApp->GetProfileInt(_T("DoFile"),_T("LibID"),0);
	m_strPlaceName = pApp->GetProfileString(_T("DoFile"),_T("Place"),_T(""));
	m_enDateType = (EN_DATETYPE)pApp->GetProfileInt(_T("DoFile"),_T("DateType"),0);
	m_enQueryMod = (EN_QUERYMOD)pApp->GetProfileInt(_T("DoFile"),_T("QueryMod"),0);*/
	//end 20120927
}

void CDoFileMgr::WriteDefaultSetting()
{
	CWinApp *pApp=AfxGetApp();
	//edit by Ronald 20120927 for migrating from vc6 to 2010
	CString str_temp;
	str_temp.Format(L"%d",m_nDepartID);
	WritePrivateProfileStringW(L"DoFile", L"LibID", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	WritePrivateProfileStringW(L"DoFile", L"Place", m_strPlaceName.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",m_enDateType);
	WritePrivateProfileStringW(L"DoFile", L"DateType", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	str_temp.Format(L"%d",m_enQueryMod);
	WritePrivateProfileStringW(L"DoFile", L"QueryMod", str_temp.AllocSysString(), ((CWinApp*)AfxGetApp())->m_pszProfileName);
	//pApp->WriteProfileInt(_T("DoFile"),_T("LibID"),m_nDepartID);
	//pApp->WriteProfileString(_T("DoFile"),_T("Place"),m_strPlaceName);
	//(EN_DATETYPE)pApp->WriteProfileInt(_T("DoFile"),_T("DateType"),(int)m_enDateType);
	//(EN_QUERYMOD)pApp->WriteProfileInt(_T("DoFile"),_T("QueryMod"),(int)m_enQueryMod);
	//end 20120927
}

BOOL CDoFileMgr::SetupLibTree(CTreeCtrl *pTreeCtrl, WORD wParent,int nLibID)
{
	CADORecordset rs;

	if(m_pFileDB->LoadLibAll(&rs))
	{
		HTREEITEM hSelItem=NULL;
		InsertSubItem(&rs,pTreeCtrl,NULL,0,nLibID,hSelItem);
		if(hSelItem)
			pTreeCtrl->SelectItem(hSelItem);
		rs.Close();
		return TRUE;
	}
	
	return FALSE;
}

void CDoFileMgr::InsertSubItem(CADORecordset *pRS,CTreeCtrl *pTreeCtrl,HTREEITEM hParent, WORD wParent, int nLibID,HTREEITEM &hSelItem)
{
	TV_INSERTSTRUCT		tvstruct;
	HTREEITEM hTreeItem = NULL;
	pRS->MoveFirst();
	int nLibTmpID=0,nParentID,nIsLeaf;
	int nPos;
	CString strLibName;
	while(!pRS->IsEOF())
	{
		pRS->GetFieldValue(0,nLibTmpID);
		pRS->GetFieldValue(1,nParentID);
		pRS->GetFieldValue(2,nIsLeaf);
		if(nParentID==wParent)
		{
			pRS->GetFieldValue(3,strLibName);
			tvstruct.hParent = hParent;
			tvstruct.hInsertAfter = TVI_SORT;
			tvstruct.item.pszText = (LPTSTR)(LPCTSTR)strLibName;
			tvstruct.item.mask = TVIF_TEXT|TVIF_IMAGE| TVIF_SELECTEDIMAGE;
			tvstruct.item.lParam = nLibTmpID ;
			tvstruct.item.iImage = 0 ;
			tvstruct.item.iSelectedImage = 1;
			hTreeItem = pTreeCtrl->InsertItem(&tvstruct);
			pTreeCtrl->SetItemData ( hTreeItem , nLibTmpID );
			if(nLibID==nLibTmpID)hSelItem=hTreeItem;
			nPos = pRS->GetAbsolutePosition();
			if(nIsLeaf!=0)
			{
				InsertSubItem(pRS,pTreeCtrl,hTreeItem,nLibTmpID,nLibID,hSelItem);
				pRS->SetAbsolutePosition(nPos);
			}
		}
		pRS->MoveNext();
	}
	return ;
}

void CDoFileMgr::SetupPlace(CComboBox *pComb)
{
	CADORecordset rs;
	pComb->ResetContent();
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(m_pFileDB->LoadPlace(0,&rs,m_nDepartID,pGlobal->m_pEmployee->m_nGroup))
	{
		CString strPlace;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,strPlace);
			pComb->AddString(strPlace);
			rs.MoveNext();
		}
		rs.Close();
		int nFind=0;
		if(!m_strPlaceName.IsEmpty())
		{
			nFind=pComb->FindString(-1,m_strPlaceName);
			if(nFind==CB_ERR)
				nFind=0;
		}
		if(CB_ERR!=pComb->SetCurSel(nFind))
			pComb->GetLBText(nFind,m_strPlaceName);
		else
			m_strPlaceName=_T("");
	}
	else
		m_strPlaceName=_T("");	

	CSize   sz;
	int     dx = 0;
	CDC*    pDC = pComb->GetDC();
	CString strTemp;
	for (int i = 0; i < pComb->GetCount(); i++)
	{
		pComb->GetLBText(i, strTemp);
		sz = pDC->GetTextExtent(strTemp);
		
		if (sz.cx > dx)
			dx = sz.cx;
	}
	pComb->ReleaseDC(pDC);

	dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);
	
	if (pComb->GetDroppedWidth() < dx)
	{
		pComb->SetDroppedWidth(dx);
	}
}

BOOL CDoFileMgr::LoadLibFile(BOOL bSelFile)
{
	if(!m_pFileListCtrl||m_nDepartID==-1)
	{
		if(m_pFileListCtrl)m_pFileListCtrl->DeleteAllItems();
		return FALSE;
	}

	CString str_Temp;m_pFileDB->GetConfig(L"banxing",str_Temp);
	BOOL bBanxing;
	if(str_Temp == L"F")
		bBanxing = FALSE;
	else
		bBanxing = TRUE;
	//CADORecordset rs;
	int nState=0,nTmp;
	//add by Bob
	//添加稿件總數和總字數
	int nFileCount=0;

	double dblTotalWords=0.0,dblWords;
	CGlobal *pGL=CGlobal::GetInstance();
	BOOL bDefPrivLib;
	//是否私有稿庫
	m_pFileDB->IsDefPrivLib(m_nDepartID,m_strPlaceName,pGL->m_pEmployee->m_nGroup,bDefPrivLib);	
	//是否默認稿庫
	m_bDefaultLib=m_pFileDB->IsDefaultLib(m_nDepartID,m_strPlaceName,pGL->m_pEmployee->m_nGroup);
	int nSelItem=-1;

	// 在此獲取稿庫權限
	m_nLibPlacePerm = m_pFileDB->GetLibPlacePerm(pGL->m_pEmployee->m_nGroup, m_nDepartID, m_strPlaceName);

	//開始初始化稿件列表
	m_pFileListCtrl->LockWindowUpdate();
	m_bInitList = TRUE;
	m_pFileListCtrl->DeleteAllItems();
	if(m_pFileDB->LoadLibFile(&rs,(CDoFileMgr*)this))
	{
		LV_ITEM	lvitem;
		int iItem=0,nExclusive=0;
		CString str,strAuthor;
		CDoFileItem *pItem=NULL;
		while(!rs.IsEof())
		{
			nState=0;
			rs.GetFieldValue(_T("position"),str);
			if(!str.IsEmpty()&&str.GetAt(0)==_T('T'))
				nState ^= FL_TRANSLATE ;
			rs.GetFieldValue(_T("urgency_style"),str);
			if(!str.IsEmpty()&&str.GetAt(0)==_T('Y'))
				nState ^= FL_URGENCY ;
			rs.GetFieldValue(_T("txt_state"),str);
			if (!str.IsEmpty()&& (str.GetAt(0) ==_T('L') || str.GetAt(0) ==_T('O')) )
				nState ^= FL_LOCK ;
			rs.GetFieldValue(_T("has_attach"),nTmp);
			if(nTmp>0)
			{
				nState ^= FL_HASATTACH;
			}
			rs.GetFieldValue(_T("secret_style"),nTmp);
			if(nTmp==1)
				nState ^= FL_SECRET;

			int  iRelateMenu;
			rs.GetFieldValue(_T("isrelate"), iRelateMenu);

			rs.GetFieldValue(_T("username"),str);//當前用戶
			rs.GetFieldValue(_T("txt_words"),dblWords);
			//作者
			rs.GetFieldValue(_T("txt_author"),strAuthor);
			rs.GetFieldValue(_T("exclusive"),nExclusive);
			if(nExclusive==1)
				nState ^= FL_EXCLUSIVE;
			if(nTmp==1||nExclusive==1)
			{
				if(str.CompareNoCase(pGL->m_pEmployee->m_strUserName ) == 0
					||strAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName ) == 0 )
				{
					dblTotalWords +=dblWords;
					nFileCount++;
				}
				else
				{
					rs.MoveNext();
					continue;
				}
					
			}
			else if(!bDefPrivLib||str.IsEmpty()||str.CompareNoCase(pGL->m_pEmployee->m_strUserName ) == 0)
			{
				dblTotalWords +=dblWords;
				nFileCount++;
			}
			
			lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
			lvitem.iItem = iItem ;
			lvitem.iSubItem = 0;
			lvitem.state = LVIS_STATEIMAGEMASK;
			int nTmpState=nState;
			if((nTmpState&FL_HASATTACH)==FL_HASATTACH)
				nTmpState^=FL_HASATTACH;
			if((nTmpState&FL_EXCLUSIVE)==FL_EXCLUSIVE)
				nTmpState^=FL_EXCLUSIVE;
			//if(nTmpState == 0 && iRelateMenu != 1)
			//	nTmpState = FL_UNRELATE;
			/*if ( nTmpState > 12 )
				nTmpState = nTmpState - 8 ;*/
			lvitem.iImage = nTmpState ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			iItem=m_pFileListCtrl->InsertItem(&lvitem);
#ifdef _STANDARD_VERSION_
			if(false)
				;
#else
			if(((nState & FL_HASATTACH) == FL_HASATTACH) && (iRelateMenu != 1))
				m_pFileListCtrl->SetItemState(iItem, INDEXTOSTATEIMAGEMASK(3), LVIS_STATEIMAGEMASK);
			else if(iRelateMenu != 1)
				m_pFileListCtrl->SetItemState(iItem, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK);
#endif
			else if((nState & FL_HASATTACH) == FL_HASATTACH)
				m_pFileListCtrl->SetItemState(iItem, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);
			
			//filecode
			rs.GetFieldValue(_T("filecode"),str);
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iItem ;
			lvitem.iSubItem = DOFILE_FILENAME ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			if((pItem=(CDoFileItem*)m_mapFileItems[str])==NULL)
				pItem = new CDoFileItem;
			pItem->strFileName = str;
			if(bSelFile)
			{
				if(str.CompareNoCase(m_strCurFileCode)==0)
					nSelItem = iItem;
			}

			if(CGlobal::GetInstance()->str_Share.GetLength()>0)
			{
				if(str.CompareNoCase(CGlobal::GetInstance()->str_Share)==0)
					nSelItem = iItem;
			}
			//類型
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iItem ;
			CString strFMT;
			rs.GetFieldValue(_T("txt_format"),strFMT);
			lvitem.iSubItem = DOFILE_FILEFORMAT ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strFMT;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strFormat = strFMT;
			//標題
			rs.GetFieldValue(_T("txt_title"),str);
			lvitem.iSubItem = DOFILE_TITLE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strTitle = str;
			//當前狀態
			rs.GetFieldValue(_T("last_oper"),str);
			lvitem.iSubItem = DOFILE_CURSTATUS ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strLastOper = str;
			//是否關聯
			pItem->bIsRelate = iRelateMenu == 1;
			if(pItem->bIsRelate)
			{
				str.LoadString(IDS_YES);
			}
			else
			{
				str.LoadString(IDS_NO);
			}
			lvitem.iSubItem = DOFILE_HAVERELATE;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);

			//字數
			if(bBanxing==TRUE)
				str.Format(_T("%.1f"),dblWords);
			else
				str.Format(L"%.0f", dblWords);
			lvitem.iSubItem = DOFILE_WORDCOUNT ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->dbWordCount = dblWords;
			//當前用戶			
			rs.GetFieldValue(_T("username"),str);

			lvitem.iSubItem = DOFILE_ASSIGNEDUSER ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strAssignedUser = str;
			//鎖定用戶
			rs.GetFieldValue(_T("lockuser"),str);			
			lvitem.iSubItem = DOFILE_LOCKUSER;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strLockUser = str;
			//留言
			rs.GetFieldValue(_T("note"),str);
			lvitem.iSubItem = DOFILE_COMMENT ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strComment = str;
			//更新日期
			rs.GetFieldValue(_T("txt_date"),pItem->dtUpdateDate);
			str = pItem->dtUpdateDate.Format(DATETIMEFORMAT);
			lvitem.iSubItem = DOFILE_UPDATEDATE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			
			//內部狀態
			/*lvitem.iSubItem = DOFILE_FILESTATE ;
			str.Format(_T("%d"),nState);
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);*/
			pItem->nState = nState;
			//作者
			lvitem.iSubItem = DOFILE_FILEAUTHOR ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strAuthor;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strAuthor = strAuthor;
			//更改時間
			rs.GetFieldValue(_T("last_modtime"),str);
			/*CString str1;
//			ChangeDBTimeToMyTimeStamp(str,str1);
			lvitem.iSubItem = DOFILE_LASTMODTIME ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);*/
			pItem->strLastModTime = str;
			rs.MoveNext();
			iItem++;
			pItem->bLib = TRUE;
			m_mapFileItems[pItem->strFileName] = pItem;
		}
		//rs.Close();
		//結束初始化稿件列表
		m_bInitList = FALSE;
		if(nSelItem!=-1)
		{
			m_pFileListCtrl->SetItemState(nSelItem,LVIS_SELECTED,LVIS_SELECTED);
			m_pFileListCtrl->EnsureVisible(nSelItem,FALSE);
		}
		else
		{
			if(m_iCurFilePos!=-1)
			{
				if(m_iCurFilePos>=m_pFileListCtrl->GetItemCount())
					m_iCurFilePos = m_pFileListCtrl->GetItemCount()-1;
				m_pFileListCtrl->SetItemState(m_iCurFilePos,LVIS_SELECTED,LVIS_SELECTED);
				m_pFileListCtrl->EnsureVisible(m_iCurFilePos,FALSE);
			}
		}
		if(m_pFileListCtrl->iPreviousCol>=0)
		{
			m_pFileListCtrl->SortByColumn(m_pFileListCtrl->iPreviousCol, !m_pFileListCtrl->bPreviousMode);
			m_pFileListCtrl->bPreviousMode = !m_pFileListCtrl->bPreviousMode;
			if(m_strCurFileCode.GetLength()>0)
			{
				for (int i=0;i<m_pFileListCtrl->GetItemCount();i++)
				{
					CString strFileCode = m_pFileListCtrl->GetItemText(i,DOFILE_FILENAME);
					if(m_strCurFileCode == strFileCode)
					{
						m_pFileListCtrl->SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
						m_pFileListCtrl->EnsureVisible(i,FALSE);
						m_iCurFilePos = i;
					}
				}
			}
		}
		SetTotalCount(nFileCount,dblTotalWords);
		m_pFileListCtrl->UnlockWindowUpdate();
		return TRUE;
	}
	//結束初始化稿件列表
	m_pFileListCtrl->UnlockWindowUpdate();
	m_bInitList = FALSE;	
	CGlobal::GetInstance()->str_Share = CGlobal::GetInstance()->strLibId = CGlobal::GetInstance()->strPlaceCode = L"";
	return FALSE;
}

void CDoFileMgr::SetupDateFilter(CComboBox *pComb)
{
	pComb->SetCurSel((int)m_enDateType);
}
BOOL CDoFileMgr::GetCurFileParam(int nItem)
{
	if(!CFileMgrBase::GetCurFileParam(nItem))
		return FALSE;
	//m_strLastModifyTime = m_pFileListCtrl->GetItemTextEx ( m_nItem , DOFILE_UPDATEDATE);
	//m_bExclusive = FL_EXCLUSIVE==(_ttoi(m_pFileListCtrl->GetItemTextEx ( m_nItem , DOFILE_FILESTATE))&FL_EXCLUSIVE);
	m_bExclusive = FL_EXCLUSIVE==(m_pCurItem->nState&FL_EXCLUSIVE);
	m_nState = m_pCurItem->nState;
	m_strAssignedUser = m_pCurItem->strAssignedUser;
	return TRUE;
}

void CDoFileMgr::CheckLibFilePermission(  ) 
{
	/*
	m_bCanUnlink = FALSE;
	m_bCanlink = FALSE;
	m_bLinked = FALSE;

	m_bOwnFile=FALSE;
	m_bGetFile=FALSE;
	m_bSendCol=FALSE;
	m_bEdit=FALSE;
	m_bDelete=FALSE;
	
	if(m_nItem==-1)
	{
		return ;
	}
	CString strFileUser, strFileAuthor, strFileCode;
	strFileUser = m_pFileListCtrl->GetItemTextEx ( m_nItem , DOFILE_ASSIGNEDUSER );
	strFileAuthor = m_pFileListCtrl->GetItemTextEx ( m_nItem , DOFILE_FILEAUTHOR );
	strFileCode = m_pFileListCtrl->GetItemTextEx ( m_nItem, DOFILE_FILENAME);

	CString strStyle;
	strStyle = m_pFileListCtrl->GetItemTextEx(m_nItem,DOFILE_FILESTATE);
	DWORD dwStyle;
	dwStyle = _ttoi(strStyle);

	BOOL bPermArr[6]={1,1,1,1,1,1} ;
	//編輯，取稿，送譯，簽發上欄，刪除

	//編輯
	if ( dwStyle & FL_TRANSLATE || dwStyle & FL_LOCK  )
		bPermArr[0] = FALSE ;

	//取稿
	if ( dwStyle & FL_LOCK || dwStyle & FL_TRANSLATE )	//locked
	{
		bPermArr[1] = FALSE ;
	}
	//傳遞
	if( dwStyle & FL_LOCK || dwStyle & FL_TRANSLATE)
		m_bSendFile = FALSE;
	else
		m_bSendFile = TRUE;
	//送譯
	if ( dwStyle & FL_LOCK || dwStyle & FL_SECRET )
		bPermArr[2] = FALSE ;

	//簽發上欄
	if ( dwStyle & FL_LOCK || dwStyle & FL_TRANSLATE )
		bPermArr[3] = FALSE ;
	
	//刪除
	if ( dwStyle & FL_LOCK || dwStyle & FL_TRANSLATE ) //lock or translate
		bPermArr[4] = FALSE ;

	m_bOwnFile=FALSE;
	m_bGetFile=FALSE;
	m_bSendCol=FALSE;
	m_bEdit=FALSE;
	m_bDelete=FALSE;
	CGlobal *pGL=CGlobal::GetInstance();
	//指定用戶是否是自己
	if(strFileUser.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0)
		m_bOwnFile=TRUE;
	if(m_bDefaultLib)
	{		
		if(m_bOwnFile||strFileUser.IsEmpty())
		{			
			if ( !(dwStyle & FL_TRANSLATE ))
			{
				m_bSendCol=TRUE;
				m_bEdit=TRUE;
				m_bGetFile=TRUE;
				m_bDelete=TRUE;
			}
		}
		else
		{
			m_bSendCol=FALSE;
			m_bEdit=FALSE;
			m_bDelete=FALSE;
		}
	}
	//沒有指定用戶但自己是作者
	else if(m_bOwnFile||(strFileUser.IsEmpty()&&(strFileAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0)))
	{
		m_bOwnFile=TRUE;
		m_bEdit=TRUE;
	}

	m_bEdit = m_bEdit&bPermArr[0];
	if ( bPermArr[1] == TRUE&&!m_bOwnFile) 
	{
		m_bGetFile=TRUE;
	}
	//瀏覽譯稿單權限
	if(bPermArr[2] && pGL->HasTranslatePerm(m_nDepartID))
		m_bReadTrans=TRUE;
	else
		m_bReadTrans=FALSE;
	if (m_strCurFileFMT.CompareNoCase(EDITFMT)!=0)
		m_bReadTrans=FALSE;

	//填寫譯稿單權限
	if(bPermArr[2] && pGL->HasFillTransPerm(m_nDepartID))
		m_bFillTrans=TRUE;
	else
		m_bFillTrans=FALSE;
	if (m_strCurFileFMT.CompareNoCase(EDITFMT)!=0)
		m_bFillTrans=FALSE;

	//上欄權限
	m_bSendCol=bPermArr[3]&&m_bSendCol;
	m_bDelete=bPermArr[4]&&m_bDelete;

	if (m_pFileDB->IsLinkedByDM(strFileCode))
	{
		m_bLinked = TRUE;
		m_bCanUnlink  = m_pFileDB->CanUnlink(strFileCode, pGL->m_pEmployee->m_nGroup);
	}
	else 
	{
		if (m_bOwnFile)
			m_bCanlink = TRUE;
		else
		{
			int nPerm = m_pFileDB->GetDMPrivilege(pGL->m_pEmployee->m_nGroup, m_nDepartID);
			m_bCanlink = ((nPerm & 2) > 0);
		}
	}
	*/

	m_bCanUnlink = FALSE;
	m_bCanlink = FALSE;
	m_bLinked = FALSE;

	m_bOwnFile = FALSE;
	m_bGetFile = FALSE;
	m_bSendCol = FALSE;
	m_bEdit = FALSE;
	m_bDelete = FALSE;

	m_bFillTrans = FALSE;      // 送譯
	m_bReadTrans = FALSE;     // 瀏覽譯稿單
	m_bTranslate = FALSE;		// 送譯
	m_bSendFile = FALSE;		// 傳遞
	m_bSignDirect = FALSE;		// 直接傳遞

	if(m_nItem==-1)
	{
		return ;
	}
	CString strFileUser, strFileAuthor;
	strFileUser = m_pCurItem->strAssignedUser;// m_pFileListCtrl->GetItemTextEx ( m_nItem , DOFILE_ASSIGNEDUSER );
	strFileAuthor = ((CDoFileItem*)m_pCurItem)->strAuthor;//m_pFileListCtrl->GetItemTextEx ( m_nItem , DOFILE_FILEAUTHOR );
	//strFileCode = m_pFileListCtrl->GetItemTextEx ( m_nItem, DOFILE_FILENAME);

//	CString strStyle;
//	strStyle = m_pFileListCtrl->GetItemTextEx(m_nItem,DOFILE_FILESTATE);
	DWORD dwStyle;
//	dwStyle = _ttoi(strStyle);
	dwStyle = m_pCurItem->nState;
	if ( dwStyle & FL_TRANSLATE || dwStyle & FL_LOCK  )
		return;

	CGlobal *pGL=CGlobal::GetInstance();
	//指定用戶是否是自己
	//if(strFileUser.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0)
	if(strFileAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0)
		m_bOwnFile=TRUE;
	
	// 寫稿權限
	/*if(m_bDefaultLib)
	{		
		if(m_bOwnFile||strFileUser.IsEmpty())
			m_bEdit = TRUE;
		
	}
	else*/ if (m_nLibPlacePerm & LPP_MODIFILE)
	{
		m_bEdit = TRUE;
		//if((m_bOwnFile&&strFileUser.IsEmpty())||strFileUser.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0)
		if(!strFileUser.IsEmpty()&&strFileUser.CompareNoCase(pGL->m_pEmployee->m_strUserName)!=0)	
			m_bEdit = FALSE;
	}

	// 取稿
	if (/*!m_bOwnFile && */(m_nLibPlacePerm & LPP_GETFILE))
		m_bGetFile = TRUE;

	// 傳遞
	if (m_nLibPlacePerm & LPP_SENDFILE)
		m_bSendFile = TRUE;
	
	if (m_strCurFileFMT.CompareNoCase(EDITFMT)!=0)
	{
		//瀏覽譯稿單權限
		m_bReadTrans = pGL->HasTranslatePerm(m_nDepartID);
	
		//填寫譯稿單權限
		m_bFillTrans =  pGL->HasFillTransPerm(m_nDepartID);
	}

	//上欄權限
	if (m_nLibPlacePerm & LPP_SENDCOL)
		m_bSendCol = TRUE;

	//直接簽發權限
	if (m_nLibPlacePerm & LPP_SIGNDIRECT)
		m_bSignDirect = TRUE;

	// 刪稿
	if (m_nLibPlacePerm & LPP_MODIFILE)
	{
		BOOL bDel = GetEditDelPerm();
		
		if((m_bOwnFile &&strFileUser.IsEmpty())|| 
			/*(strFileUser.IsEmpty() && //m_bDefaultLib &&//Commented by Ronald Saliman 20120903 (bug fixed item no. 1186)
			(strFileAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0))*/
			(strFileUser.IsEmpty() && bDel && m_bEdit)
		)
		m_bDelete = TRUE;

		if(!strFileUser.IsEmpty()&&strFileUser.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0)	
			m_bDelete = TRUE;
	}

	if (m_pFileDB->IsLinkedByDM(m_strCurFileCode))
	{
		m_bLinked = TRUE;
		if (m_bSendCol || m_bSendFile || m_bSignDirect || m_bEdit)
			m_bCanUnlink  = m_pFileDB->CanUnlink(m_strCurFileCode, pGL->m_pEmployee->m_nGroup);
	}
	else 
	{
		if ((m_bOwnFile) || (strFileAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0))
			m_bCanlink = TRUE;
		else if (m_bSendCol || m_bSendFile || m_bSignDirect || m_bEdit)
		{
			int nPerm = m_pFileDB->GetDMPrivilege(pGL->m_pEmployee->m_nGroup, m_nDepartID);
			m_bCanlink = ((nPerm & 2) > 0);
		}
	}
	
}

void CDoFileMgr::GetFile()
{
	if(m_strCurFileCode.IsEmpty())
		return ;
	FILEPARAM fileParam;
	memset(&fileParam, 0, sizeof(fileParam));
	lstrcpy(fileParam.szFileCode, m_strCurFileCode);
	fileParam.bIsExclusive = m_bExclusive;
	_tcscpy(fileParam.szLastModTime,m_strLastModifyTime);
	fileParam.nDocStyle=0;
	fileParam.bLib = TRUE;
	fileParam.nLibID = m_nDepartID;
	_tcscpy(fileParam.szParam,m_strPlaceName);
	if(!CGlobal::GetInstance()->GetFile(&fileParam))
		return ;
	m_pCurItem->strAssignedUser=CGlobal::GetInstance()->m_pEmployee->m_strUserName;
	m_pFileListCtrl->SetItemTextEx(m_nItem,m_nLastModTimeCol,fileParam.szLastModTime);
	m_pCurItem->strLastModTime = fileParam.szLastModTime;
	m_bEdit = TRUE;
	int nOldLibID=m_nDepartID;
	m_nDepartID = fileParam.nLibID ;
	CString strOldPlace;
	
	strOldPlace=m_strPlaceName;
	m_strPlaceName = fileParam.szFilePlace;
	OpenFile(FALSE);
	m_strPlaceName = strOldPlace;
	m_nDepartID = nOldLibID;
	m_bEdit = FALSE;
	LoadLibFile(TRUE);
}

BOOL CDoFileMgr::GetLibUser(int nLib, CString strPlace, CComboBox *pBox)
{
	CADORecordset rs;
	if(m_pFileDB->GetLibUser(nLib,strPlace,&rs))
	{
		CString str;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,str);
			pBox->AddString(str);
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CDoFileMgr::LoadPlace(int nType, int nLibID, CComboBox *pBox)
{
	CADORecordset rs;
	pBox->ResetContent();
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(m_pFileDB->LoadPlace(1,&rs,nLibID,pGlobal->m_pEmployee->m_nGroup))
	{
		CString strPlace;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,strPlace);
			pBox->AddString(strPlace);
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CDoFileMgr::GetUserCode(CString strUserName, CString &strUserCode)
{
	return m_pFileDB->GetUserCode(strUserName,strUserCode);
}

BOOL CDoFileMgr::TransFile(CString strFileCode, int nDesLibID, CString strDesPlace,
	CString strCopyStyle, int nGroupID, CString strUserCode, short nSecret, CString *strNote, CString strOper, int nAction, short &nRet)
{
	CResManage res(g_hInst);
	CGlobal *pGL= CGlobal::GetInstance();
	if(!pGL->LockFile(strFileCode))
	{
		AfxMessageBox(IDS_LOCKFILEFAIL);
		return FALSE;
	}
	FILEPARAM FileParam;
	memset(&FileParam,0,sizeof(FileParam));
	_tcscpy(FileParam.szFileCode,strFileCode);
	_tcscpy(FileParam.szLastModTime,m_strLastModifyTime);
	FileParam.nDocStyle=0;
	FileParam.bLib = TRUE;
	FileParam.nLibID = m_nDepartID;
	_tcscpy(FileParam.szParam,m_strPlaceName);
	if(!pGL->CanFileModify(&FileParam))
	{
		pGL->UnlockFile(strFileCode,FALSE);
		return FALSE;
	}
	
	BOOL bRet=m_pFileDB->TransFile(strFileCode,nDesLibID,strDesPlace,strCopyStyle,nGroupID,strUserCode,
		nSecret,strNote,strOper,nAction, nRet);
	pGL->UnlockFile(strFileCode,FALSE);
	return bRet;
}


BOOL CDoFileMgr::LoadColumn(CListCtrl *pCtrl, int nLib, TCHAR *szPerm)
{
	pCtrl->DeleteAllItems();
	CADORecordset rs;
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(m_pFileDB->LoadColumn(&rs,nLib,pGlobal->m_pEmployee->m_nGroup,szPerm))
	{
		int nRow;
		nRow = 0;
		CString strPaper,strCol;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,strPaper);
			rs.GetFieldValue(1,strCol);
			LV_ITEM			lvitem;
			
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = nRow ;
			lvitem.iSubItem = 0;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strPaper;
			nRow = pCtrl->InsertItem(&lvitem);
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = nRow;
			lvitem.iSubItem = 1;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strCol;
			pCtrl->SetItem(&lvitem);
			nRow++;
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CDoFileMgr::LoadColUser(CComboBox *pBox, int nColumnID,int nLib)
{
	CADORecordset rs;
	if(m_pFileDB->LoadColUser(&rs,nColumnID,nLib))
	{
		CString strUser,strNote;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,strUser);
			rs.GetFieldValue(1,strNote);
			int n=pBox->AddString(strUser);
			if(strNote.IsEmpty()||strNote==_T(" "))
				pBox->SetItemData(n,0);
			else
				pBox->SetItemData(n,1);
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
}


BOOL CDoFileMgr::LoadFileInfo(CString &strFileCode, CString &strTitle, double *dbWord,  BOOL *bSingle, BOOL *bSecret, BOOL *bUrgency, BOOL *bLock, CString &strAuthor, CString &strStyle, CString &strPostBox, CString &strUser )
{
	return m_pFileDB->LoadFileInfo(strFileCode , strTitle , 
		dbWord, bSingle , bSecret , bUrgency ,bLock ,strAuthor,strStyle,strPostBox, strUser);
}


void CDoFileMgr::SetColumnIndex()
{
	m_nFileCodeCol=DOFILE_FILENAME;
	m_nFileFmtCol=DOFILE_FILEFORMAT;
//	m_nFileStateCol=DOFILE_FILESTATE;
	m_nFileTitleCol=DOFILE_TITLE;
	m_nUpdateDateCol=DOFILE_UPDATEDATE;
//	m_nLastModTimeCol=DOFILE_LASTMODTIME;
	m_nCurrUserCol = DOFILE_ASSIGNEDUSER;
}
void CDoFileMgr::Translate()
{
	if (m_nItem==-1) 
		return;
	CResManage setRes(g_hInst);
	short nRet;
	if(!m_pFileDB->GetLibFileStatus(m_strCurFileCode,m_nDepartID,m_strPlaceName,nRet)||nRet)
	{		
		if(nRet==-1)
			AfxMessageBox(IDS_QUERYLIBFILESTATUSERR);
		return ;
	}
	AfxGetMainWnd()->SendMessage(WM_MANAGE_TRANSLATE,m_bFillTrans,(long)&m_strCurFileCode);
}

BOOL CDoFileMgr::SignDirect()
{
	FILEPARAM fileParam;
	memset(&fileParam, 0, sizeof(fileParam));
	lstrcpy(fileParam.szFileCode, m_strCurFileCode);
	fileParam.bIsExclusive = m_bExclusive;
	fileParam.nState = m_nState;
	lstrcpy(fileParam.szAssignedUser, m_strAssignedUser);
	_tcscpy(fileParam.szLastModTime,m_strLastModifyTime);
	fileParam.nDocStyle=0;
	fileParam.bLib = TRUE;
	fileParam.nLibID = m_nDepartID;
	_tcscpy(fileParam.szParam,m_strPlaceName);
	return CGlobal::GetInstance()->SignFileDirect(fileParam);
}

BOOL CDoFileMgr::DeleteFile()
{
	if(m_nItem==-1)return FALSE;
	int nRet;
	BOOL bRet=FALSE;
	CResManage res(g_hInst);
	//Added by Ronadl Saliman on 20120904
	if(!m_pFileDB->HasRelationToColumn(m_strCurFileCode,nRet))
	{
		AfxMessageBox(L"請先把稿件與部門稿件菜單的關聯移除");
		return 0;
	}
	//end 20120904
	if(m_pFileDB->DeleteFile(m_strCurFileCode,nRet))
	{
		switch(nRet) {
		case 0:
			bRet=TRUE;
			break;
		case 1:
			AfxMessageBox(IDS_NOTENDUSER,MB_OK|MB_ICONSTOP);
			break;
		case 2:
			AfxMessageBox(IDS_HOTFILE,MB_OK|MB_ICONSTOP);
			break;
		default:
			AfxMessageBox(IDS_UNKOWNERR,MB_OK|MB_ICONSTOP);
			break;
		}
		m_nItem = -1;
		m_strCurFileCode = _T("");
	}
	return bRet;
}

void CDoFileMgr::LinkToDM(LPCTSTR lpszFileCode)
{
	if (!m_pFileDB->IsLinkedByDM(lpszFileCode))
	{
		CLinkToDMDlg dlg;
		CResManage res(g_hInst);
		if (dlg.DoModal() == IDOK)
		{
			if (!m_pFileDB->IsLinkedByDM(lpszFileCode))
			{// 關聯到部門分組菜單
				CString str;
				str.LoadString(IDS_LINKTODM);
				m_pFileDB->LinkToDM(dlg.m_nDeptFileInfoId, lpszFileCode,str);
			}
			else
			{//該稿件已經被其他用戶關聯
				AfxMessageBox(IDS_P_LINKED_BY_OTHER, MB_OK | MB_ICONSTOP);
			}
		}
	}
	
}

void CDoFileMgr::UnlinkDM(LPCTSTR lpszFileCode)
{
	int nFileInfoId = m_pFileDB->GetLinkedDMFI(lpszFileCode);

	if (nFileInfoId > -1)
	{
		CString strPrompt,strOper;
		CResManage res(g_hInst);
		strPrompt.Format(IDS_Q_CONFIRM_UNLINK, lpszFileCode);
		strOper.LoadString(IDS_UNLINKTODM);
		if (AfxMessageBox(strPrompt, MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2) == IDOK)
		{
			m_pFileDB->LinkToDM(nFileInfoId, _T(""),strOper);
		}
	}

}

void CDoFileMgr::PrintView(LPCTSTR lpszGroup)
{
	//edit by leon 2012-11-19 for crystal report issue
	/*CFileReport dlg;
	dlg.rs = &rs;
	dlg.m_strPreTitle = m_strDepartName + m_strPlaceName + lpszGroup;
	while (dlg.DoModal() == IDOK);*/
	CGlobal *pGL=CGlobal::GetInstance();
	if (pGL->m_ReportPara.bValid)
	{
		TCHAR path[100];
		CString strFileName,strError;
		GetModuleFileName(AfxGetInstanceHandle(),path,100);
		CString strPath = path;
		int nLength = strPath.ReverseFind(_T('\\'));
		strFileName = strPath.Left(nLength);
		strFileName += DOFILERPT;	
		if( (_taccess(strFileName, 0 )) == -1 )
		{
			strError.Format(IDS_FILENOTEXIST, strFileName);		
			AfxMessageBox(strError);			
			return;
		}
		pGL->m_ReportPara.enType=DOFILEREPORT;
		pGL->m_ReportPara.strFilePath=strFileName;
		pGL->m_ReportPara.strAppTittle=DOFILENAME;
		pGL->m_ReportPara.strTableTittle= m_strDepartName + m_strPlaceName + lpszGroup;

		// Edit Stephen 2013-12-17
		CString strDepartID = "0";
		strDepartID.Format(L"%ld",m_nDepartID); 
		pGL->m_ReportPara.strDomainID = strDepartID;
		pGL->m_ReportPara.strPosition = m_strPlaceName;
		pGL->m_ReportPara.strCurUser = pGL->m_pEmployee->m_strUserName;



		pGL->m_ReportPara.strDutyType = "-9999";
		if (m_enDateType == 0)
			pGL->m_ReportPara.strDutyType = "0";
		else if (m_enDateType == 1)
			pGL->m_ReportPara.strDutyType = "-3";
		else if (m_enDateType == 2)
			pGL->m_ReportPara.strDutyType = "-7";

		COleDateTime dt;

		dt = COleDateTime::GetCurrentTime();
		dt -= pGL->m_pEntry->tsSpanOfDay;
		dt = COleDateTime(dt.GetYear(),dt.GetMonth(),dt.GetDay(),0,0,0)+pGL->m_pEntry->tsSpanOfDay;

		pGL->m_ReportPara.strStartDate = dt.Format(DATETIMEFORMAT);

		// Edit Stephen 2013-12-17

		if(pGL->LoadDLL(MD_CRYSTALREPORT)){
			pGL->DoCommand(MD_CRYSTALREPORT, "DisplayReport");
			//pGL->UnloadDLL(MD_CRYSTALREPORT);
		}
	}
	//end leon 2012-11-19
}


void CDoFileMgr::GetFilePos(FILEPARAM &fileParam)
{
	fileParam.nDocStyle=0;
	fileParam.bLib=TRUE;
	fileParam.nLibID = m_nDepartID;
	_tcscpy(fileParam.szParam,m_strPlaceName);
}

BOOL CDoFileMgr::CanCurFileModify()
{
	FILEPARAM FileParam;
	memset(&FileParam,0,sizeof(FileParam));
	_tcscpy(FileParam.szFileCode,m_strCurFileCode);
	_tcscpy(FileParam.szLastModTime,m_strLastModifyTime);
	FileParam.nDocStyle=0;
	FileParam.bLib = TRUE;
	FileParam.nLibID = m_nDepartID;
	_tcscpy(FileParam.szParam,m_strPlaceName);
	CGlobal *pGL=CGlobal::GetInstance();
	return pGL->CanFileModify(&FileParam);
}

BOOL CDoFileMgr::SendToCol()
{
	FILEPARAM fileParam;
	memset(&fileParam, 0, sizeof(fileParam));
	lstrcpy(fileParam.szFileCode, m_strCurFileCode);
	fileParam.bIsExclusive = m_bExclusive;
	fileParam.nState = m_nState;
	lstrcpy(fileParam.szAssignedUser, m_strAssignedUser);
	_tcscpy(fileParam.szLastModTime,m_strLastModifyTime);
	fileParam.nDocStyle=0;
	fileParam.bLib = TRUE;
	fileParam.nLibID = m_nDepartID;
	_tcscpy(fileParam.szParam,m_strPlaceName);
	return CGlobal::GetInstance()->SendToCol(&fileParam);
}

void CDoFileMgr::Refresh()
{
	if(CGlobal::GetInstance()->str_Share.GetLength()>0)
	{
		
	}
	LoadLibFile(TRUE);
	CFileMgrBase::Refresh();
}

void CDoFileMgr::SetTotalCount(int nFileCount, double dblWords)
{
	if (m_pParentFrm->GetSafeHwnd()!=NULL)
	{
		CString str_Temp;m_pFileDB->GetConfig(L"banxing",str_Temp);
		BOOL bBanxing;
		if(str_Temp == L"F")
			bBanxing = FALSE;
		else
			bBanxing = TRUE;
		CString strTotalCount;
		strTotalCount.Format(L"%d",nFileCount);
		((CDoFileFrm*)m_pParentFrm)->m_dlgTotal.SetDlgItemText(IDC_STATIC_TOTALCOUNT,(LPCTSTR)strTotalCount);
		if(bBanxing==FALSE)
			strTotalCount.Format(L"%.0f",dblWords);
		else
			strTotalCount.Format(L"%.1f",dblWords);
		((CDoFileFrm*)m_pParentFrm)->m_dlgTotal.SetDlgItemText(IDC_STATIC_TOTALWORDS,(LPCTSTR)strTotalCount);
	}
	return;
}
BOOL CDoFileMgr::OnBeforeOpenFile(FILEPARAM &fileParam)
{
	fileParam.bSendCol = m_bSendCol;
	fileParam.bSignDirect = m_bSignDirect;
	return TRUE;
}

BOOL CDoFileMgr::GetEditDelPerm()
{
	CString strId; strId.Format(L"%d", CGlobal::GetInstance()->m_pEmployee->m_nGroup);
	if(m_bDefaultLib&& m_pFileDB->GetDefaultLibDelPerm(strId))
	{
		return TRUE;
	}
	else
		return FALSE;
}

void CDoFileMgr::AddToStory()
{
//
	CDlgAddToStory dlg(m_pFileDB->m_pDB);
	dlg.strStoryName = m_pCurItem->strTitle;
	dlg.strFileName = m_pCurItem->strFileName;
	dlg.m_dt = &m_pCurItem->dtUpdateDate;
	dlg.m_pCurItem = m_pCurItem;
	dlg.m_strDate = dlg.m_dt->Format(DATEFORMAT);
	//

	dlg.m_arrFileName.RemoveAll();

	int nRow=-1;
	int nCount;
	nCount=m_pFileListCtrl->GetSelectedCount();
	for(int i=0;i<nCount;i++)
	{
		nRow=m_pFileListCtrl->GetNextItem(nRow,LVNI_SELECTED);
//		AfxMessageBox(m_pFileListCtrl->GetItemText(nRow,0));
		dlg.m_arrFileName.Add(m_pFileListCtrl->GetItemText(nRow,0));
	}

	dlg.m_b_NewText = false;

	if(dlg.DoModal() != IDOK)
		return;

	AfxGetMainWnd()->SendMessage(WM_DO_PUBLISH, dlg.m_i_storyItemID, 0);

	return;
}