
#include "stdafx.h"
#include "filemanage.h"
#include "..\commkits\dostools.h"

CString CSearchParam::GetDateFilter()
{
	CString str,str1;
	str.Empty();
	if(!m_strBeginDate.IsEmpty())
		str.Format(_T(" and txt_date >= '%s 00:00:00'"),m_strBeginDate);
	if(!m_strEndDate.IsEmpty())
	{
		str1.Format(_T(" and txt_date <= '%s 23:59:59' "),m_strEndDate);
		str+=str1;
	}

	
	return str;
}

CString CSearchParam::GetFileCodeFilter()
{
	CString str,str1;
	
	if(!m_strFileCode.IsEmpty())
	{
		//DosToSql(m_strFileCode,str1);
		str1 = m_strFileCode;
		//20081114 Liubc
		//str.Format(_T(" and text_lib.filecode like '%s' "),str1);
		//str.Format(_T(" and text_lib.filecode like N'%%%s%%' "),str1);
		str.Format(_T(" and text_lib.filecode = N'%s' "),str1);
	}
	else
		str.Empty();
	return str;
}

CString CSearchParam::GetUserFilter()
{
//	CString str;
//
//	if(!m_strUser.IsEmpty())
//	{
//		str.Format(_T(" and text_lib.filecode in (select distinct filecode from paper_record ")
//			_T(" where usercode=dbo.f_GetNewsUserCode('%s') %s union select distinct filecode from file_record where usercode=dbo.f_GetNewsUserCode('%s') %s)"),m_strUser, GetRecordDateFilter(), m_strUser, GetRecordDateFilter());
//	}
//	else
//		str.Empty();
//	return str;

	CString str;
	
	if(!m_strUserCode.IsEmpty())
	{
		str.Format(_T(" and text_lib.filecode in (select distinct filecode from paper_record ")
			_T(" where usercode=N'%s' %s union select distinct filecode from file_record where usercode=N'%s' %s)"),m_strUserCode, GetRecordDateFilter(), m_strUserCode, GetRecordDateFilter());
	}
	else
		str.Empty();
	return str;
	

}

CString CSearchParam::GetKeyFilter()
{
	CString str;
	if(!m_strKey.IsEmpty())
	{
		str.Format(_T(" and txt_textcontent like N'%%%s%%' "),m_strKey);
	}
	else
		str.Empty();
	return str;
}

CString CSearchParam::GetLibBaseSQL()
{
	return _T("select distinct text_lib.filecode, txt_format,txt_title , text_lib.txt_words ,  txt_author , txt_style , text_lib.position,  text_lib.txt_state ,  exclusive ,  dbo.f_GetNewsUserName(curruser) username ,")
		_T(" dbo.f_GetLibName(file_place. lib_id) libname , file_place.place_code , '' paper, '' colcode, NULL column_date, urgency_style , secret_style,last_oper")
		_T(" from text_lib  , text_info ,  file_place,trans_permission ")
		_T(" where (text_lib.position ='L' or text_lib.position='T') and text_lib.filecode = text_info.filecode and file_place.filecode=text_lib.filecode")
		+GetLibPermFilter();
}

CString CSearchParam::GetPaperBaseSQL()
{
	return _T("select  distinct text_lib.filecode, txt_format,txt_title , text_lib.txt_words ,  txt_author , txt_style , text_lib.position,  text_lib.txt_state ,")
		_T(" exclusive , dbo.f_GetNewsUserName(curruser) username , '' libname , '' place_code , dbo.f_GetPaperCode(column_id) paper,dbo.f_GetColumnCode(column_id) colcode, column_date ,  urgency_style , secret_style,last_oper")
		_T(" from text_lib  , text_info ,paper_filelist ")
		_T(" where   text_lib.filecode = text_info.filecode "  )
		_T(" and paper_filelist.filecode=text_lib.filecode")
		+GetColPermFilter();
}

CString CSearchParam::GetColPermFilter()
{
	CString str;
	str.Format(L" and ( paper_filelist.column_id in(select distinct column_id from column_permission where group_id=%d and perm_style='R' ) "
		L" or text_lib.filecode in (select distinct filecode from paper_record  where usercode=user_name() %s "
		L" union select distinct filecode from file_record where usercode=user_name() %s ))",
		CGlobal::GetInstance()->m_pEmployee->m_nGroup, GetRecordDateFilter(), GetRecordDateFilter());
	return str;	
}

CString CSearchParam::GetLibPermFilter()
{
	CString str;
	str.Format(_T(" and ( file_place.lib_id =trans_permission.lib_id and file_place.place_code=trans_permission.place and trans_permission.group_id = %d  and trans_permission.perm_style = 'R' ")
		_T(" or  text_lib.filecode in (select distinct filecode from file_record  where usercode=user_name() %s))"),
		CGlobal::GetInstance()->m_pEmployee->m_nGroup, GetRecordDateFilter());

	return str;
}
CString CSearchParam::GetLibFilter()
{
	CString str;
	str.Format(_T(" and file_place.lib_id=%d"),m_nLibID);
	return str;
}

CString CSearchParam::GetPaperFilter()
{
	CString str;
	str.Format(_T(" and paper_filelist.column_id=%d"), m_nColumnID);
	return str;
}


void CSearchParam::GetSQL(CString &strSQL)
{
	switch(m_iType) {
	case -1:   // 
		strSQL.Format(_T("(%s %s %s %s %s union %s %s %s %s %s)"),
			GetLibBaseSQL(),GetFileCodeFilter(),GetUserFilter(),
			GetKeyFilter(),GetDateFilter(),GetPaperBaseSQL(),
			GetFileCodeFilter(),GetUserFilter(),GetKeyFilter(),GetDateFilter());
		break;
	case 0:
		strSQL.Format(_T("%s %s %s %s %s %s"),
			GetLibBaseSQL(),GetFileCodeFilter(),GetUserFilter(),
			GetKeyFilter(),GetDateFilter(),GetLibFilter());
		break;
	case 1:
		strSQL.Format(_T("%s %s %s %s %s %s"),
			GetPaperBaseSQL(),GetFileCodeFilter(),GetUserFilter(),
			GetKeyFilter(),GetDateFilter(),GetPaperFilter());
		break;
	default:
		ASSERT(FALSE);
	}
	return ;
}
BOOL CSearchParam::SearchFile(CListCtrlEx *pCtrl,CADORecordset *rs,CMapStringToOb *pMap)
{
	CString strSql,strTmp,usrName;
	GetSQL(strSql);
	CFile file;
#ifdef _DEBUG
	file.Open(L"..\\log.txt",CFile::modeCreate|CFile::modeReadWrite);	
	file.Write(strSql,strSql.GetLength()*2);
	file.Close();
#endif
	CGlobal *pGL=CGlobal::GetInstance();
	usrName=pGL->m_pEmployee->m_strUserName;
	strSql = _T("select * from (" + strSql + ") as result_set ");
	//strTmp.Format(_T( " where (exclusive =0 and secret_style =0)  or (username = 'lee') or (txt_author= 'lee')"),usrName,usrName);	
	//strSql = strSql +strTmp;
	CString str_Temp;pGL->m_FileDB.GetConfig(L"banxing",str_Temp);
	BOOL bBanxing;
	if(str_Temp == L"F")
		bBanxing = FALSE;
	else
		bBanxing = TRUE;
	if(pGL->m_FileDB.SearchFile(rs,strSql))
	{

		
		LV_ITEM	lvitem;
		int iItem=0,nState,nTmp, nExclusive;
		double dblWords=0.0;
		CString str,str1,str2,str3,strUser, strAuthor;
		m_nTotalOnPaper=0;
		m_dblWordsOnPaper =0.0;
		CListItem *pItem=NULL;
		while(!rs->IsEof())
		{
			nState=0;
			
			rs->GetFieldValue(_T("txt_author"), strAuthor);
			rs->GetFieldValue(_T("exclusive"), nExclusive);
			rs->GetFieldValue(_T("username"), strUser);

			rs->GetFieldValue(_T("urgency_style"),str);
			if(!str.IsEmpty()&&str.GetAt(0)=='Y')
				nState ^= FL_URGENCY ;
			rs->GetFieldValue(_T("txt_state"),str);
			if (!str.IsEmpty()&& str.GetAt(0) =='L' )
				nState ^= FL_LOCK ;
			if (!str.IsEmpty()&& str.GetAt(0) =='D' )
				nState ^= FL_DELETE ;
			rs->GetFieldValue(_T("secret_style"),nTmp);
			if(nTmp==1)
				nState ^= FL_SECRET;
			
			rs->GetFieldValue(_T("txt_words"),dblWords);
			// 加密, 僅允許作者和指定使用者看見
			if (nTmp==1&&
				!(strUser.CompareNoCase(pGL->m_pEmployee->m_strUserName ) == 0  || 
				strAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName)==0	)
			  )
			{
				rs->MoveNext();
				continue;	
			}

			// 獨家, 僅允許作者和指定使用者看見
			if(nExclusive == 1 &&
				!(strUser.CompareNoCase(pGL->m_pEmployee->m_strUserName ) == 0  || 
				strAuthor.CompareNoCase(pGL->m_pEmployee->m_strUserName) ==0
				)
				)
			{	
				rs->MoveNext();
				continue;	
			}
			lvitem.mask = LVIF_TEXT|LVIF_IMAGE;
			lvitem.iItem = iItem ;
			lvitem.iSubItem = 0;
			if ( nState > 11 )
				nState = 8 ;
			lvitem.iImage = nState ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str ;
			iItem=pCtrl->InsertItem(&lvitem);
			if(iItem==-1)break;
			//filecode
			rs->GetFieldValue(_T("filecode"),str);
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iItem ;
			lvitem.iSubItem = FSEARCH_FILENAME ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			if((pItem=(CListItem*)(*pMap)[str])==NULL)
				pItem = new CListItem;
			pItem->strFileName = str;
			//類型
			rs->GetFieldValue(_T("txt_format"),str);
			lvitem.iSubItem = FSEARCH_FILEFMT ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			pItem->strFormat = str;
			//標題
			rs->GetFieldValue(_T("txt_title"),str);
			lvitem.iSubItem = FSEARCH_FILETITLE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			pItem->strTitle = str;
			//字數
			if(bBanxing==TRUE)
				str.Format(_T("%.1f"),dblWords);
			else
				str.Format(L"%.0f", dblWords);
			lvitem.iSubItem = FSEARCH_FILEWORDS ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			pItem->dbWordCount = dblWords;
			//當前狀態
			rs->GetFieldValue(_T("last_oper"),str);
			if((nState&FL_DELETE)==FL_DELETE)
			{
				str.LoadString(IDS_DELETEDFILE);
			}
			
			lvitem.iSubItem = FSEARCH_CURSTATUS ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			pItem->strLastOper = str;
			//作者
			rs->GetFieldValue(_T("txt_author"),str);
			lvitem.iSubItem = FSEARCH_FILEAUTHOR ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			
			//style
			rs->GetFieldValue(_T("txt_style"),str);
			lvitem.iSubItem = FSEARCH_FILESTYLE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			//位置
			COleDateTime dt;
			rs->GetFieldValue(_T("position"),str);
			if(!str.IsEmpty())
			{
				TCHAR tcPos=str.GetAt(0);
				switch(tcPos) {
				case 'L':
					str3.LoadString(IDS_LIBRARY);
					rs->GetFieldValue(_T("libname"),str1);
					rs->GetFieldValue(_T("place_code"),str2);
					str.Format(_T("%s (%s %s)"),str3,str1,str2);
					pItem->bLib = TRUE;
					break;
				case 'T':
					str3.LoadString(IDS_TRANSLIB);
					rs->GetFieldValue(_T("libname"),str1);
					rs->GetFieldValue(_T("place_code"),str2);
					str.Format(_T("%s (%s %s)"),str3,str1,str2);
					pItem->bLib = TRUE;
					break;
				case 'C':
					str3.LoadString(IDS_COLUMN);
					rs->GetFieldValue(_T("paper"),str1);
					rs->GetFieldValue(_T("colcode"),str2);
					
					rs->GetFieldValue(_T("column_date"),dt);
					str.Format(_T("%s (%s %s %02d-%02d)"),str3,str1,str2,dt.GetMonth(),dt.GetDay());
					pItem->bLib = FALSE;
					break;
				case 'P':
					str3.LoadString(IDS_SIGNED);
					rs->GetFieldValue(_T("paper"),str1);
					rs->GetFieldValue(_T("colcode"),str2);
					rs->GetFieldValue(_T("column_date"),dt);
					str.Format(_T("%s (%s %s %02d-%02d)"),str3,str1,str2,dt.GetMonth(),dt.GetDay());
					m_nTotalOnPaper++;
					m_dblWordsOnPaper += dblWords;
					pItem->bLib = FALSE;
					break;
				default:
					str.Empty();
				}
			}
			lvitem.iSubItem = FSEARCH_CURPOSITION;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			//獨家
			rs->GetFieldValue(_T("exclusive"),nTmp);
			if(nTmp==1)
				str.LoadString(IDS_YES);
			else
				str.LoadString(IDS_NO);
			lvitem.iSubItem = FSEARCH_SINGLE ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);
			//有圖
			/*rs.GetFieldValue(_T("hasphoto"),str);
			lvitem.iSubItem = FSEARCH_HASPHOTO ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			pCtrl->SetItemEx(&lvitem);*/
			//當前用戶
			lvitem.iSubItem = FSEARCH_CURUSER ;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strUser;
			pCtrl->SetItemEx(&lvitem);
			pItem->strAssignedUser = str;
			rs->MoveNext();
			iItem++;
			(*pMap)[pItem->strFileName] = pItem;
		}
		//rs->Close();
		return TRUE;
	}
	return FALSE;
}


CString CSearchParam::GetRecordDateFilter()
{
	CString str,str1;
	str.Empty();
	if(!m_strBeginDate.IsEmpty())
		str.Format(_T(" and finish_time >= '%s 00:00:00'"),m_strBeginDate);
	if(!m_strEndDate.IsEmpty())
	{
		str1.Format(_T(" and finish_time <= '%s 23:59:59' "),m_strEndDate);
		str+=str1;
	}
	
	
	return str;
}
