// DoColMgr.cpp: implementation of the CDoColMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "filemanage.h"
#include "DoColMgr.h"
#include <io.h>
#include "..\fileoper\fileopermgr.h"
#include "../commkits/dostools.h"
#include "DoColFrm.h"
#include "listitem.h"

#include "DlgAddToStory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoColMgr::CDoColMgr()
{
	m_nColumnID = -1;
	m_strDate = _T("");
	m_pPathMgr=NULL;
	m_nCurFileState=0;
	m_enFileViewMode = EN_FVCOLFILE;
	rs = new CADORecordset();
}

CDoColMgr::~CDoColMgr()
{
	if (rs)
		delete rs;
	if(m_pPathMgr)
		delete m_pPathMgr;
}
void CDoColMgr::Refresh()
{
	LoadPaperFile(TRUE);
	CFileMgrBase::Refresh();
}
void CDoColMgr::LoadColumn(CComboBox *pComb)
{
	pComb->ResetContent();
	CADORecordset rs;
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(m_pFileDB->LoadColumn(&rs,m_nDepartID,pGlobal->m_pEmployee->m_nGroup))
	{
		int nColID,nRow, nSelIndex = -1;		
		nRow=0;
		CString strPaper,strCol, str_Layout;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,strPaper);
			rs.GetFieldValue(1,strCol);
			rs.GetFieldValue(2,nColID);
			rs.GetFieldValue(3, str_Layout);
			//strCol.Format(L"%s(%s)",strCol,strPaper);
			//strCol+= L"("+strPaper+L")";
			strCol= L"("+str_Layout+L")"+strCol;
			if(pComb->FindStringExact(-1,strCol)==CB_ERR)
			{
				nRow=pComb->AddString(strCol);
				pComb->SetItemData(nRow,nColID);
			}
			if(CGlobal::GetInstance()->strColCode.GetLength()>0)
			{
				if(_wtoi(CGlobal::GetInstance()->strColCode) == nColID)
					nSelIndex = nRow;
			}
			rs.MoveNext();
		}
		m_strPaper = strPaper;
		rs.Close();
		if(nSelIndex == -1 &&CGlobal::GetInstance()->strColCode.GetLength()>0)
		{
			AfxMessageBox(L"沒有相關版面的權限");
			CGlobal::GetInstance()->str_Share = CGlobal::GetInstance()->strColCode = CGlobal::GetInstance()->strLibId = L"";
		}
		if(nSelIndex!= -1)
			pComb->SetCurSel(nSelIndex);
		else
			pComb->SetCurSel(0);
	}

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


BOOL CDoColMgr::LoadColPerm()
{
	CADORecordset rs;
	ZeroMemory(&m_ColPerm,sizeof(CColPerm));
	if(m_pFileDB->LoadColumnPerm(CGlobal::GetInstance()->m_pEmployee->m_nGroup,
		m_nColumnID,&rs))
	{
		while(!rs.IsEof())
		{
			CString strPerm;
			rs.GetFieldValue(0,strPerm);
			if(!strPerm.IsEmpty())
			{
				switch(strPerm.GetAt(0))
				{
				  case 'W': //編輯
					  m_ColPerm.m_bEditCol=TRUE;;
					  break;
				  case 'T': //調版
					  m_ColPerm.m_bChangeCol=TRUE;
					  break;
				  case 'F'://簽發
					  m_ColPerm.m_bSign=TRUE;
					  break;
				  case 'B'://回調簽發
					  m_ColPerm.m_bBack=TRUE;
					  break;
				  case 'G'://退稿
					  m_ColPerm.m_bRollBack=TRUE;
					  break;
				  default:
					  break;
				}
			}
			rs.MoveNext();
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

void CDoColMgr::LoadPaperFile(BOOL bSelFile)
{
	m_pFileListCtrl->LockWindowUpdate();
	if(!m_pFileListCtrl)return;
	m_pFileListCtrl->DeleteAllItems();

	CString str_Temp;m_pFileDB->GetConfig(L"banxing",str_Temp);
	BOOL bBanxing;
	if(str_Temp == L"F")
		bBanxing = FALSE;
	else
		bBanxing = TRUE;
	
	int nFileCount=0;
	
	m_bInitList = TRUE;
	int nSelItem=-1;
	if(m_pFileDB->LoadPaperFile(rs,m_strDate,m_nColumnID))
	{
		LV_ITEM	lvitem;
		int iItem=0;
		CString str,strAuthor,strCurrUser;
		int nState,nTmp,nExclusive;
		double dblWords=0.0,dblTotalWords=0.0;
		CGlobal *pGL=CGlobal::GetInstance();
		CDoColItem *pItem=NULL;
		while(!rs->IsEof())
		{
			nState=0;
			rs->GetFieldValue(_T("position"),str);
			if(!str.IsEmpty()&&str.GetAt(0)=='P')
				nState ^= FL_SIGN ;
			rs->GetFieldValue(_T("urgency_style"),str);
			if(!str.IsEmpty()&&str.GetAt(0)=='Y')
				nState ^= FL_URGENCY ;
			rs->GetFieldValue(_T("txt_state"),str);
			if (!str.IsEmpty()&& str.GetAt(0) =='L' )
				nState ^= FL_LOCK ;
			rs->GetFieldValue(_T("has_attach"),nTmp);
			//rs->GetFieldValue(_T("del_user"),str);
			//if(nTmp>0&&str.IsEmpty())
			if(nTmp>0)
				nState ^= FL_HASATTACH;
			rs->GetFieldValue(_T("exclusive"),nExclusive);
			if(nExclusive==1)
				nState ^= FL_EXCLUSIVE;
			int iRelateMenu;
			rs->GetFieldValue(_T("isrelate"), iRelateMenu);

			rs->GetFieldValue(_T("secret_style"),nTmp);

			rs->GetFieldValue(_T("curruser"),strCurrUser);
			rs->GetFieldValue(_T("txt_author"),strAuthor);
			rs->GetFieldValue(_T("txt_words"),dblWords);
			if(nTmp==1||nExclusive==1)
			{
				if(strCurrUser.CompareNoCase(pGL->m_pEmployee->m_strUserName ) == 0
					||strAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName ) == 0 )
				{
					if(nTmp==1&&((nState&FL_SIGN) != FL_SIGN))
						nState^=FL_SECRET;
					{
						dblTotalWords +=dblWords;
						nFileCount++;
					}
				}
				else
				{
					rs->MoveNext();
					continue;
				}
				
			}
			else
			{
				dblTotalWords +=dblWords;
				nFileCount++;
			}

			lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
			lvitem.iItem = iItem ;
			lvitem.iSubItem = 0;
			lvitem.state = LVIS_STATEIMAGEMASK ;
			int nTmpState=nState;
			if((nTmpState&FL_HASATTACH)==FL_HASATTACH)
				nTmpState^=FL_HASATTACH;
			if((nTmpState&FL_EXCLUSIVE)==FL_EXCLUSIVE)
				nTmpState^=FL_EXCLUSIVE;
			if ( nTmpState > 12 )
				nTmpState = nTmpState - 8 ;
			//if(nTmpState == 0 && iRelateMenu != 1)
			//	lvitem.iImage = FL_UNRELATE;
				lvitem.iImage = nTmpState ;
			if ( (nTmpState > 3 && nTmpState < 8)) //pap
			{
				//lvitem.iImage = 13 ; //sign
				lvitem.iImage = 16 ; //sign
			}
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			iItem=m_pFileListCtrl->InsertItem(&lvitem);
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iItem ;
			lvitem.iSubItem = DOCOL_FILENAME ;
			rs->GetFieldValue(_T("filecode"),str);
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			m_pFileListCtrl->SetItemEx(&lvitem);
			
			lvitem.iSubItem = DOCOL_FILEAUTHOR ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strAuthor ;
			m_pFileListCtrl->SetItemEx(&lvitem);
			if((pItem=(CDoColItem*)m_mapFileItems[str])==NULL)
				pItem = new CDoColItem;
			pItem->strFileName = str;
			//if((nState&FL_HASATTACH)==FL_HASATTACH)
			//	m_pFileListCtrl->SetItemState(iItem,INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK);
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
			rs->GetFieldValue(_T("txt_format"),strFMT);
			lvitem.iSubItem = DOCOL_FILEFORMAT ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strFMT;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strFormat = strFMT;
			//標題
			rs->GetFieldValue(_T("txt_title"),str);
			lvitem.iSubItem = DOCOL_TITLE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strTitle = str;
			//當前狀態
			rs->GetFieldValue(_T("last_oper"),str);
			lvitem.iSubItem = DOCOL_CURSTATUS ;
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
			lvitem.iSubItem = DOCOL_HAVERELATE;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			//字數
			if(bBanxing==TRUE)
				str.Format(_T("%.1f"),dblWords);
			else
				str.Format(L"%.0f", dblWords);
			lvitem.iSubItem = DOCOL_WORDCOUNT ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->dbWordCount = dblWords;
			//最後傳遞時間
			COleDateTime dt;
			rs->GetFieldValue(_T("last_sendtime"),pItem->dtLastSendTime);
			str=pItem->dtLastSendTime.Format(DATETIMEFORMAT);
			lvitem.iSubItem = DOCOL_TRANSDATE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			
			//內部狀態
			/*lvitem.iSubItem = DOCOL_FILESTATE ;
			str.Format(_T("%d"),nState);
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);*/
			pItem->nState = nState;
			//簽發後文件名
			rs->GetFieldValue(_T("layout_file"),str);
			lvitem.iSubItem = DOCOL_SIGNEDFILE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strSignedFile = str;
			//更新日期
			rs->GetFieldValue(_T("txt_date"),pItem->dtUpdateDate);
			str = pItem->dtUpdateDate.Format(DATETIMEFORMAT);
			lvitem.iSubItem = DOCOL_UPDATEDATE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			
			//更改時間
			rs->GetFieldValue(_T("last_modtime"),str);
			/*CString str1;
//			ChangeDBTimeToMyTimeStamp(str,str1);
			lvitem.iSubItem = DOCOL_LASTMODTIME ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);*/
			pItem->strLastModTime = str;
			//當前用戶
			lvitem.iSubItem = DOCOL_ASSIGNEDUSER ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strCurrUser;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strAssignedUser = strCurrUser;
			//鎖定用戶
			rs->GetFieldValue(_T("lockuser"),str);
			lvitem.iSubItem = DOCOL_LOCKUSER ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_pFileListCtrl->SetItemEx(&lvitem);
			pItem->strLockUser = str;
			iItem++;
			pItem->bLib = FALSE;
			m_mapFileItems[pItem->strFileName] = pItem;
			rs->MoveNext();
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
			}
		}
		if(m_pFileListCtrl->iPreviousCol>=0)
		{
			m_pFileListCtrl->SortByColumn(m_pFileListCtrl->iPreviousCol, !m_pFileListCtrl->bPreviousMode);
			m_pFileListCtrl->bPreviousMode = !m_pFileListCtrl->bPreviousMode;
		}
		SetTotalCount(nFileCount,dblTotalWords);
		m_pFileListCtrl->UnlockWindowUpdate();
		return ;
	}
	//結束初始化稿件列表
	m_pFileListCtrl->UnlockWindowUpdate();
	m_bInitList = FALSE;	
	CGlobal::GetInstance()->str_Share = CGlobal::GetInstance()->strColCode = CGlobal::GetInstance()->strLibId = L"";
	return ;
}

BOOL CDoColMgr::ListColumnDate(CComboBox *pBox)
{
	pBox->ResetContent();
	CADORecordset rs;
	CGlobal *pGlobal=CGlobal::GetInstance();
	if(m_pFileDB->ListColumnDate(&rs,m_nColumnID))
	{
		COleDateTime dt;
		CString strDate;
		while(!rs.IsEof())
		{
			rs.GetFieldValue(0,dt);
			strDate = dt.Format(DATEFORMAT);
			pBox->AddString(strDate);
			rs.MoveNext();
		}
		pBox->SetCurSel(0);
		rs.Close();
		return TRUE;
	}
	return FALSE;
}
BOOL CDoColMgr::GetCurFileParam(int nItem)
{
	if(!CFileMgrBase::GetCurFileParam(nItem))
		return FALSE;
	//m_strLastModifyTime = m_pFileListCtrl->GetItemTextEx ( m_nItem , DOCOL_UPDATEDATE);
	m_bEdit = m_ColPerm.m_bEditCol&&CanModify();
	//m_bExclusive = FL_EXCLUSIVE==(_ttoi(m_pFileListCtrl->GetItemTextEx ( m_nItem , DOCOL_FILESTATE))&FL_EXCLUSIVE);
	m_bExclusive = FL_EXCLUSIVE==(m_pCurItem->nState&FL_EXCLUSIVE);
	return TRUE;
}
BOOL CDoColMgr::GetCurFileParam(int nItem, FILEPARAM &fileParam)
{
	if(!CFileMgrBase::GetCurFileParam(nItem))
		return FALSE;
	m_bEdit = m_ColPerm.m_bEditCol&&CanModify();
	m_bExclusive = FL_EXCLUSIVE==(m_pCurItem->nState&FL_EXCLUSIVE);
	
	memset(&fileParam,0,sizeof(FILEPARAM));
	GetFilePos(fileParam);
	_tcscpy(fileParam.szFileCode,m_strCurFileCode);
	fileParam.bLib = m_enFileViewMode==EN_FVDOFILE;
	fileParam.bCanEdit = m_bEdit;
	fileParam.bEdit = FALSE;
	fileParam.dWordcount = m_pCurItem->dbWordCount;
	_tcscpy(fileParam.szLastModTime,m_strLastModifyTime);
	return TRUE;
}
void CDoColMgr::BackFile()
{
	if(!m_pPathMgr)
	{
		m_pPathMgr = new CPathMgr;
		m_pPathMgr->SetDB(m_pFileDB->m_pDB);
	}
	CString strPathName;
	
	int nListItem =-1;
	/*::SetLastError(IDS_ERRSERVERNAME);
	DWORD dw=::GetLastError();
	CString str;
	str.LoadString(dw);
	*/
	CString strLayCode,strLayFile;
	COleDateTime dt;
	while((nListItem = m_pFileListCtrl->GetNextItem ( nListItem , LVNI_SELECTED ))!=-1)
	{
		FILEPARAM fileParam;
		GetCurFileParam(nListItem, fileParam);
		if(!CanBack())
			continue;
		CGlobal::GetInstance()->BackFile(&fileParam);
		
	}
}

void CDoColMgr::SignFile()
{
	CString strLayCode,strLayFile,strPathName;
	COleDateTime dt;
	int nListItem=-1;
	dt = COleDateTime::GetCurrentTime();
	dt-=COleDateTimeSpan(1,0,0,0)+CGlobal::GetInstance()->m_pEntry->tsSpanOfDay;
	COleDateTimeEx dtSel(m_strDate,DATEFORMAT);
	if(dtSel<dt)
	{
		CResManage res(g_hInst);
		AfxMessageBox(IDS_DATEEXPIRED);
		return;
	}
	while((nListItem = m_pFileListCtrl->GetNextItem ( nListItem , LVNI_SELECTED ))!=-1)
	{
		FILEPARAM fileParam;
		GetCurFileParam(nListItem, fileParam);
		if(!CanSign())
			continue;
		CGlobal::GetInstance()->SignFile(fileParam);
	}
}

BOOL CDoColMgr::IsFileLocked(CString &strFileCode)
{
	CString strLockUser = L"";
	return m_pFileDB->IsFileLocked(strFileCode, strLockUser);
}

BOOL CDoColMgr::CanTransPaper()
{
	return m_ColPerm.m_bChangeCol&&(m_nItem!=-1)&&!(m_nCurFileState&FL_LOCK)&&!(m_nCurFileState&FL_SIGN);
}

BOOL CDoColMgr::CanSign()
{
	return !(m_nCurFileState&FL_LOCK)&&!(m_nCurFileState&FL_SIGN)&&m_ColPerm.m_bSign&&(m_nItem!=-1);
}

BOOL CDoColMgr::CanBack()
{
	return m_ColPerm.m_bBack&&(m_nCurFileState&FL_SIGN)&&(m_nItem!=-1);
}

BOOL CDoColMgr::CanManageAttach()
{
	return m_ColPerm.m_bEditCol&&!(m_nCurFileState&FL_SIGN)&&(m_nItem!=-1)&&m_pFileListCtrl->GetSelectedCount()==1;
}

BOOL CDoColMgr::CanModify()
{
	return m_ColPerm.m_bEditCol&&!(m_nCurFileState&FL_LOCK)&&!(m_nCurFileState&FL_SIGN)&&(m_nItem!=-1)&&m_pFileListCtrl->GetSelectedCount()==1;
	/*BOOL bCheckCurrUser = FALSE;
	if(m_pCurItem!=NULL)
	{
		if(m_pCurItem->strAssignedUser.GetLength()==0)
			bCheckCurrUser = TRUE;
		else
		{
			CString strName = CGlobal::GetInstance()->m_pEmployee->m_strUserName;
			bCheckCurrUser = m_pCurItem->strAssignedUser == strName?TRUE:FALSE;
		}
	}
	return m_ColPerm.m_bEditCol&&!(m_nCurFileState&FL_LOCK)&&!(m_nCurFileState&FL_SIGN)&&(m_nItem!=-1)&&m_pFileListCtrl->GetSelectedCount()==1&&bCheckCurrUser;*/
}

BOOL CDoColMgr::CanRollBack()
{
	return m_ColPerm.m_bRollBack&&!(m_nCurFileState&FL_LOCK)&&!(m_nCurFileState&FL_SIGN)&&(m_nItem!=-1);
}

BOOL CDoColMgr::TransPaper()
{
	CString strFileCode,strTitle;
	BOOL bRet;
	int nListItem=-1;
	while((nListItem = m_pFileListCtrl->GetNextItem ( nListItem , LVNI_SELECTED ))!=-1)
	{
		GetCurFileParam(nListItem);
		strTitle = m_pCurItem->strTitle;//m_pFileListCtrl->GetItemTextEx(nListItem,DOCOL_TITLE);
		if(!CanTransPaper())
			continue;
		CGlobal *pGL = CGlobal::GetInstance();
		if(!pGL->LockFile(m_strCurFileCode))
		{
			CGlobal::GetInstance()->GetFileOperMgr()->EndTransPaper();
			AfxMessageBox(IDS_LOCKFILEFAIL);
			break;
		}
		if(!CanCurFileModify())
		{
			CGlobal::GetInstance()->GetFileOperMgr()->EndTransPaper();
			pGL->UnlockFile(m_strCurFileCode,FALSE);
			return FALSE;
		}
		bRet=CGlobal::GetInstance()->GetFileOperMgr()->TransPaper(m_strCurFileCode,strTitle,m_strPaper,m_strCol,m_strDate,pGL->HasCopyPerm(),pGL->HasSecretPerm(),m_bExclusive);
		pGL->UnlockFile(m_strCurFileCode,!bRet);
	}
	CGlobal::GetInstance()->GetFileOperMgr()->EndTransPaper();
	LoadPaperFile(FALSE);	
	return bRet;
}

void CDoColMgr::GetPaperCol(CString &strSource,CString &strPaper, CString &strCol)
{
//	strPaper.Empty();
	strCol.Empty();
	if(strSource.IsEmpty())
		return;
	int n=strSource.Find(_T("("),0);
	int n1=strSource.Find(_T(")"),n);
	/*if(n!=CB_ERR&&n1!=CB_ERR)
	{
		strPaper = strSource.Mid(n+1,n1-n-1);
		strCol = strSource.Left(n);
	}*/
	strCol = strSource;
}


BOOL CDoColMgr::RollbackFile(CString &strMsg)
{
	CGlobal *pGL=CGlobal::GetInstance();
	int nListItem=-1;
	
	
	while((nListItem = m_pFileListCtrl->GetNextItem ( nListItem , LVNI_SELECTED ))!=-1)
	{
		FILEPARAM fileParam;
		GetCurFileParam(nListItem, fileParam);
		if(!CanRollBack())
			continue;
		pGL->RollbackFile(fileParam, strMsg);

	}

	return TRUE;
}
void CDoColMgr::SetColumnIndex()
{
	m_nFileCodeCol=DOCOL_FILENAME;
	m_nFileFmtCol=DOCOL_FILEFORMAT;
//	m_nFileStateCol=DOCOL_FILESTATE;
	m_nFileTitleCol=DOCOL_TITLE;
	m_nUpdateDateCol=DOCOL_UPDATEDATE;
//	m_nLastModTimeCol=DOCOL_LASTMODTIME;
	m_nCurrUserCol = DOCOL_ASSIGNEDUSER;
}


void CDoColMgr::GetFilePos(FILEPARAM &fileParam)
{
	fileParam.nDocStyle=0;
	fileParam.bLib=FALSE;
	fileParam.nColID = m_nColumnID;
	fileParam.nLibID = m_nDepartID;
	_tcscpy(fileParam.szParam,m_strDate);
}
BOOL CDoColMgr::CanCurFileModify()
{
	FILEPARAM FileParam;
	memset(&FileParam,0,sizeof(FileParam));
	_tcscpy(FileParam.szFileCode,m_strCurFileCode);
	_tcscpy(FileParam.szLastModTime,m_strLastModifyTime);
	FileParam.nDocStyle=0;
	FileParam.bLib = FALSE;
	FileParam.nColID = m_nColumnID;
	_tcscpy(FileParam.szParam,m_strDate);
	CGlobal *pGL=CGlobal::GetInstance();
	return pGL->CanFileModify(&FileParam);
}

void CDoColMgr::SetTotalCount(int nFileCount, double dblWords)
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
		((CDoColFrm*)m_pParentFrm)->m_dlgTotal.SetDlgItemText(IDC_STATIC_TOTALCOUNT,(LPCTSTR)strTotalCount);
		if(bBanxing==FALSE)
			strTotalCount.Format(L"%.0f",dblWords);
		else
			strTotalCount.Format(L"%.1f",dblWords);
		((CDoColFrm*)m_pParentFrm)->m_dlgTotal.SetDlgItemText(IDC_STATIC_TOTALWORDS,(LPCTSTR)strTotalCount);
	}
	return;
}
BOOL CDoColMgr::OnBeforeOpenFile(FILEPARAM &fileParam)
{
	fileParam.bBack = CanRollBack();
	fileParam.bSign = CanSign();
	return TRUE;
}

void CDoColMgr::AddToStory()
{
//
	CDlgAddToStory dlg(m_pFileDB->m_pDB);
	dlg.strStoryName = m_pCurItem->strTitle;
	dlg.strFileName = m_pCurItem->strFileName;
	COleDateTimeEx dt_sel(m_strDate,DATEFORMAT);
	dlg.m_dt = &dt_sel;
	dlg.m_pCurItem = m_pCurItem;
	dlg.m_strDate = m_strDate;
	
	dlg.m_arrFileName.RemoveAll();

	int nRow=-1;
	int nCount;
	nCount=m_pFileListCtrl->GetSelectedCount();
	for(int i=0;i<nCount;i++)
	{
		nRow=m_pFileListCtrl->GetNextItem(nRow,LVNI_SELECTED);
	//	AfxMessageBox(m_pFileListCtrl->GetItemText(nRow,0));
		dlg.m_arrFileName.Add(m_pFileListCtrl->GetItemText(nRow,0));
	}
	
	dlg.m_b_NewText = false;
	
	if(dlg.DoModal() != IDOK)
		return;

	AfxGetMainWnd()->SendMessage(WM_DO_PUBLISH, dlg.m_i_storyItemID, 0);

	return;
}
