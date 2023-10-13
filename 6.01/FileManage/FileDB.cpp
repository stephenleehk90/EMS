// FileDB.cpp: implementation of the CFileDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "filemanage.h"
#include "FileDB.h"
#include "dofilemgr.h"
#include "../commkits/dostools.h"
#include "io.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileDB::CFileDB()
{

}

CFileDB::~CFileDB()
{

}
void CFileDB::SetDB(CADODatabase *pDB)
{
	m_pDB=pDB;
}

BOOL CFileDB::LoadLibAll(CADORecordset*pRS)
{
	CADORecordset rs;
	CString strSql;
	strSql="select lib_id , parent_id , lib_style , lib_name from news_lib ";
	if(rs.Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		rs.Clone(*pRS);
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::LoadPlace(int nType,CADORecordset*pRS,int nLibID,int nGroupID)
{
	CString strSql;
	switch(nType)
	{
	case 0://按照權限查詢
		strSql=CADODatabase::PrepareSQL(_T("select distinct place from trans_permission where lib_id=%d and ")
				_T("group_id = %d and (perm_style='R' or perm_style='W' or perm_style='G')"),nLibID,nGroupID);
		break;
	case 1://查詢有權限傳遞的稿庫
		strSql=CADODatabase::PrepareSQL(_T("select distinct place from trans_permission where lib_id=%d and ")
				_T("group_id = %d and perm_style='S'"),nLibID,nGroupID);
		break;
	default:
		CResManage res(g_hInst);
		AfxMessageBox(IDS_NOTSUPORTPARAM);
		return FALSE;
	}
	if(pRS->Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
		return TRUE;
	else
		return FALSE;
}

BOOL CFileDB::LoadLibFile(CADORecordset *pRS,  CDoFileMgr *pMgr)
{
	CString strSql,strDate,strTmp;
	COleDateTime dt;

	dt = COleDateTime::GetCurrentTime();
	dt -= CGlobal::GetInstance()->m_pEntry->tsSpanOfDay;
	dt = COleDateTime(dt.GetYear(),dt.GetMonth(),dt.GetDay(),0,0,0)+CGlobal::GetInstance()->m_pEntry->tsSpanOfDay;

	switch ( pMgr->m_enDateType )
	{
	case CURDAY:
		strTmp = dt.Format(DATETIMEFORMAT);
		strDate.Format(_T(" and txt_date >= '%s'"), strTmp);
		break;
	case THREEDAY:
		strDate.Format(_T(" and txt_date >= dateadd ( day , -3 , '%s')"),dt.Format(DATETIMEFORMAT));
		break;
	case ONEWEEK:
		strDate.Format(_T(" and txt_date >= dateadd ( day , -7 , '%s')"),dt.Format(DATETIMEFORMAT));
		break;
	case ALL:
		strDate="";
		break;
	default:
		return FALSE;
	}

	if ( pMgr->m_enQueryMod == BYPLACE )
	{
		/*
		strSql=CADODatabase::PrepareSQL(_T("select distinct t1.filecode , txt_title ,t1.last_oper,txt_words,dbo.f_GetNewsUserName(lockuser) lockuser ")
			_T(", txt_state , dbo.f_GetNewsUserName(curruser) username , position , urgency_style , copy_style , secret_style, case isnull(tb_rel.filecode, '') when '' then 0 else 1 end as isrelate ")
			_T(", exclusive ,  txt_author ,txt_format , txt_date ,case isnull(ID,0) when 0 then 0 else 1 end has_attach,t2.note ,convert(char(26),last_modtime,121) last_modtime")
			_T(" from text_lib t1, deptfile_info tb_rel,file_place t2,news_user t3,attachs t4")
			_T(" where ( (not (isnull(t1.exclusive,0) = 1 or isnull(t1.secret_style,0) <> 0)) or ((t1.exclusive = 1 or isnull(t1.secret_style,0) <> 0) and (t1.curruser = user_name() or t1.txt_author = '%s' ))) ") 
			_T(" and t1.lockuser*=t3.usercode and t1.filecode *= tb_rel.filecode and txt_state<>'D'and (position='L' or position = 'T') %s and t1.filecode*=t4.filecode and ")
			_T(" t2.filecode=t1.filecode and t2.lib_id=%d and t2.place_code='%s' and t4.del_user is null ")
			_T(" order by txt_date desc ") , CGlobal::GetInstance()->m_pEmployee->m_strUserName,  strDate , pMgr->m_nDepartID,pMgr->m_strPlaceName);
		*/
		strSql = CADODatabase::PrepareSQL(
			_T(" select distinct t1.filecode , txt_title ,t1.last_oper,txt_words,")
			_T(" dbo.f_GetNewsUserName(lockuser) lockuser , txt_state ,")
			_T(" dbo.f_GetNewsUserName(curruser) username , position , urgency_style , copy_style , secret_style,")
			_T(" case isnull(tb_rel.filecode, '') when '' then 0 else 1 end as isrelate,")
			_T(" exclusive ,  txt_author ,txt_format , txt_date ,")
			_T(" case isnull(ID,0) when 0 then 0 else 1 end has_attach,")
			_T(" t2.note ,convert(char(26),last_modtime,121) last_modtime ")
			_T(" from text_lib t1 left join ")
			_T(" deptfile_info tb_rel on t1.filecode = tb_rel.filecode left join ")
			_T(" attachs t4 on  t1.filecode = t4.filecode and t4.del_user is null ")
			_T(" ,file_place t2 ")
			_T(" where ( (not (isnull(t1.exclusive,0) = 1 or isnull(t1.secret_style,0) <> 0)) or ((t1.exclusive = 1 or isnull(t1.secret_style,0) <> 0) and (t1.curruser = user_name() or t1.txt_author = N'%s' )))  ")
			_T(" and txt_state<>'D' ")
			_T(" and (position='L' or position = 'T') %s ")
			_T(" and  t2.filecode=t1.filecode ")
			_T(" and t2.lib_id=%d ")
			_T(" and t2.place_code=N'%s' ")
			
			_T(" order by txt_date desc"),
			CGlobal::GetInstance()->m_pEmployee->m_strUserName,  strDate , pMgr->m_nDepartID,pMgr->m_strPlaceName);
		 
	}
	else
	{
		/*
		strSql=CADODatabase::PrepareSQL(_T("select distinct t1.filecode , txt_title ,t1.last_oper,txt_words ,dbo.f_GetNewsUserName(lockuser) lockuser ")
			_T(", txt_state , dbo.f_GetNewsUserName(curruser) username , position , urgency_style , copy_style , secret_style, case isnull(tb_rel.filecode, '') when '' then 0 else 1 end as isrelate ")
			_T(", exclusive ,  txt_author ,txt_format , txt_date ,case isnull(ID,0) when 0 then 0 else 1 end has_attach,t2.note ,convert(char(26),last_modtime,121) last_modtime ")
			_T(" from text_lib t1, deptfile_info tb_rel,file_place t2,news_user t3,attachs t4")
			_T(" where ( (not (isnull(t1.exclusive,0) = 1 or isnull(t1.secret_style,0) <> 0)) or ((t1.exclusive = 1 or isnull(t1.secret_style,0) <> 0) and (t1.curruser = user_name() or t1.txt_author = '%s' ))) ") 
			_T(" and t1.lockuser*=t3.usercode and t1.filecode *= tb_rel.filecode and txt_state<>'D'and t2.filecode=t1.filecode and (position='L' or position = 'T') %s and t1.filecode*=t4.filecode and ")
			_T(" t1.filecode='%s' and t4.del_user is null"), CGlobal::GetInstance()->m_pEmployee->m_strUserName, pMgr->m_strSearchFile);
		*/
		
		strSql = CADODatabase::PrepareSQL(
			_T(" select distinct t1.filecode , txt_title ,t1.last_oper,txt_words,")
			_T(" dbo.f_GetNewsUserName(lockuser) lockuser , txt_state ,")
			_T(" dbo.f_GetNewsUserName(curruser) username , position , urgency_style , copy_style , secret_style,")
			_T(" case isnull(tb_rel.filecode, '') when '' then 0 else 1 end as isrelate,")
			_T(" exclusive ,  txt_author ,txt_format , txt_date ,")
			_T(" case isnull(ID,0) when 0 then 0 else 1 end has_attach,")
			_T(" t2.note ,convert(char(26),last_modtime,121) last_modtime ")
			_T(" from text_lib t1 left join ")
			_T(" deptfile_info tb_rel on t1.filecode = tb_rel.filecode left join ")
			_T(" attachs t4 on  t1.filecode = t4.filecode and t4.del_user is null ")
			_T(" ,file_place t2 ")
			_T(" where ( (not (isnull(t1.exclusive,0) = 1 or isnull(t1.secret_style,0) <> 0)) or ((t1.exclusive = 1 or isnull(t1.secret_style,0) <> 0) and (t1.curruser = user_name() or t1.txt_author = N'%s' )))  ")
			_T(" and txt_state<>'D' ")
			_T(" and t2.filecode=t1.filecode ")
			_T(" and (position='L' or position = 'T') ")
			_T(" and  t1.filecode=N'%s' "),			
			CGlobal::GetInstance()->m_pEmployee->m_strUserName, pMgr->m_strSearchFile);
			
	}

	//edit by leon 2012-12-20 for crystal report issue
	CGlobal::GetInstance()->m_ReportPara.strSqlLoadData=strSql;
	if(pRS->Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery)){
		CGlobal::GetInstance()->m_ReportPara.bValid=TRUE;
		return TRUE;
	}
	else{
		CGlobal::GetInstance()->m_ReportPara.bValid=FALSE;
		return FALSE;
	}
	//end leon 2012-12-20
}

BOOL CFileDB::GetNewFileCode(CString &strFileCode)
{
	CADOCommand cmd(m_pDB);
	return cmd.GetProcExecRetStr(strFileCode,_T(" GetNewFileName "),_T(""));
}

short CFileDB::InsertLib(CString &strFileName, CString &strSrcFile, double lWord, CString &strTitle, int nGroupID, CString &strOperation,CString &strFMT, CString strTextVersion, CString strTextVersion_Comment, CString str_Comment, CString strTextVersion_Style)
{
	CADOCommand cmd(m_pDB);
	CString strVersionMark;
	for (int i=0; i < strTextVersion.GetLength(); i++) 
		strVersionMark = strVersionMark + L"a";

	CString strModHistory;
	COleDateTime dt = COleDateTime::GetCurrentTime();
	CString strDate; 
	strDate.Format(L"%s",dt.Format(DATETIMEFORMAT));
	strModHistory = CGlobal::GetInstance()->m_pEmployee->m_strUserName + L"於" + strDate;
	return cmd.GetProcExecResult(TRUE,_T("InsertLib"),_T("%n%s%f%n%d%n%n%n%n%n%n%n%n"),strFileName,strSrcFile,lWord,strTitle,nGroupID,strOperation,strFMT, strTextVersion, strVersionMark, strModHistory, strTextVersion_Comment, str_Comment, strTextVersion_Style);
}

short CFileDB::InsertLib(CString &strFileName, CString &strSrcFile, double lWord, CString &strTitle, CString &strSpecify_usercode
						 , int nSpecify_lib_id, CString &strSpecify_placecode, CString &strOperation, CString &strFMT)
{
	CADOCommand cmd(m_pDB);
	return cmd.GetProcExecResult(TRUE,_T("InsertLib_SAddCaption"),_T("%s%s%f%s%s%d%s%s%s")
		, strFileName, strSrcFile, lWord, strTitle, strSpecify_usercode, nSpecify_lib_id, strSpecify_placecode, strOperation, strFMT);
}

BOOL CFileDB::UpdateSimpCapHist(CString strFileName, CString strText)
{
	CADOCommand cmd(m_pDB);
	CString strVersionMark, strVersionComment, strVersionStyle;
	for (int i=0; i < strText.GetLength(); i++) 
	{
		strVersionMark = strVersionMark + L"a";
		strVersionComment = strVersionComment + L"_";
		strVersionStyle = strVersionComment + L"a";
	}
	CADORecordset rs(m_pDB);
	CString strSQL;
	BOOL bRet;
	strSQL=CADODatabase::PrepareSQL(_T("Update text_lib set text_version =N'%s', text_version_mark = N'%s',") 
 		_T("text_version_comment = N'%s', text_version_style = N'%s' where filecode = N'%s'"),strText, strVersionMark, strVersionComment, strVersionStyle, strFileName);
	if(rs.Open(strSQL,CADORecordset::openQuery))
	{
		bRet = TRUE;
		rs.Close();
	}
	else
		bRet = FALSE;
	return bRet;
	CString strModHistory;
}

BOOL CFileDB::IsDefPrivLib(int nLib, CString strPlace,int nGroupID,BOOL &bPriv)
{
	if(strPlace.IsEmpty())
		return FALSE;
	CADOCommand cmd(m_pDB);
	bPriv=cmd.GetProcExecResult(TRUE,_T("IsDefPrivLib"),_T("%d%s%d"),nLib,strPlace,nGroupID);
	CString str;
	str.Format(_T("%d%s%d"),nLib,strPlace,nGroupID);
	return TRUE;
}
BOOL CFileDB::IsDefaultLib(int nLib, CString strPlace,int nGroupID)
{
	if(strPlace.IsEmpty())
		return FALSE;
	CADOCommand cmd(m_pDB);
	short nRet = cmd.GetProcExecResult(TRUE,_T("IsDefaultLib"),_T("%d%s%d"),nLib,strPlace,nGroupID);
	if(nRet==-1)
	{
		CString strErr,strFormat;
		CResManage setRes(g_hInst);
		strFormat.LoadString(IDS_PROCPARAMERR);
		strErr.Format(strFormat,_T("IsDefaultLib"));
		//AfxMessageBox(strErr);
		return FALSE;
	}
	else
		return nRet;
}
short CFileDB::UpdateFileInfo(FILEINFO *pFileInfo)
{
	CADOCommand cmd(m_pDB);
//	return cmd.GetProcExecResult(TRUE,_T("UpdateFileInfo"),_T("%s%s%d%s%d%s%d%s%s"),pFileInfo->strFilecode,
	return cmd.GetProcExecResult(TRUE,_T("UpdateFileInfo"),_T("%n%n%d%s%d%n%d%n%n"),pFileInfo->strFilecode,
		pFileInfo->strTxtTitle,pFileInfo->nExclusive,pFileInfo->strUrgency,
		pFileInfo->bLock,pFileInfo->strTxtAuthor,pFileInfo->nSecret,
		pFileInfo->strTxtStyle,pFileInfo->strPosBox);
	
}

BOOL CFileDB::UpdateText(CString strLocalFile,CString strExt,CString strFileCode)
{
	CString strTxtFile,strFilter;
	//edit by leon 20121114 for bug 1302 to change all file type from FMT to TXT
	strTxtFile = strLocalFile+_T(".Txt");
	//strTxtFile = strLocalFile+_T(".FMT");
	//end leon 20121114
	strLocalFile += _T(".")+strExt;

//	if(_taccess((LPCTSTR)strLocalFile,0)!=0)
//		return FALSE;
	if(_taccess((LPCTSTR)strTxtFile,0)!=0)
		return FALSE;
	CADORecordset rs(m_pDB);
	CString strSQL;
	BOOL bRet;
	strSQL=CADODatabase::PrepareSQL(_T("select * from text_lib where filecode=N'%s'"),strFileCode);
	if(rs.Open(strSQL,CADORecordset::openQuery))
	{

//		if(rs.File2Blob(_T("txt_content"),strLocalFile))
//		if(rs.File2Blob_UTF(_T("txt_content"),strLocalFile))
//			bRet = TRUE;
//		else
//			bRet = FALSE;

		
//		if(bRet&&rs.File2Blob(_T("txt_textcontent"),strTxtFile))
		//if(bRet&&rs.File2Blob_UTF(_T("txt_textcontent"),strTxtFile))
		if(rs.File2Blob_UTF(_T("txt_textcontent"),strTxtFile))
			bRet = TRUE;
		else
			bRet = FALSE;

		rs.Close();
	}
	else
		bRet = FALSE;
	return bRet;
}

BOOL CFileDB::ReadFile(CString strDestPath,CString strExt, CString strFileCode)
{
	strDestPath+="\\"+strFileCode+"."+strExt;
	CADORecordset rs(m_pDB);
	CString strSQL;
	BOOL bRet;
	strSQL=CADODatabase::PrepareSQL(_T("select * from text_lib where filecode=N'%s'"),strFileCode);
	if(rs.Open(strSQL,CADORecordset::openQuery))
	{
//		if(!rs.Blob2File(_T("txt_content"),strDestPath))
		if(!rs.Blob2File_UTF(_T("txt_content"),strDestPath))
			bRet = FALSE;
		else
			bRet = TRUE;
		rs.Close();
	}
	else
		bRet = FALSE;
	return bRet;
}

BOOL CFileDB::InitMissingTxtFile(CString strDestPath, CString strFileCode)
{
	//edit by leon 20121114 for bug 1302 to change all file type from FMT to TXT
	//CString strText = L"", strExt = L"FMT";		
	CString strText = L"", strExt = L"txt";	
	//end leon 20121114
	strDestPath+="\\"+strFileCode+".";
	CADORecordset rs(m_pDB);
	CString strSQL;
	BOOL bOK;
	strSQL=CADODatabase::PrepareSQL(_T("select * from text_lib where filecode=N'%s'"),strFileCode);
	if(rs.Open(strSQL,CADORecordset::openQuery))
	{
		bOK=rs.GetFieldValueContent(_T("txt_textcontent"),strText);
		rs.GetFieldValue(L"txt_format",strExt);
		strDestPath += strExt;
		rs.Close();
	}
	else
	{
		 bOK = FALSE;
		 return bOK;	
	}
	CString strTemp = strText.Left(1);

	if (strTemp == 65279)
		strText = strText.Right(strText.GetLength()-1); 
	
	DWORD 	dwLength = strText.GetLength();

	if (dwLength==0)
	{
		strText = L" ";
		dwLength = 1;
	}

//	if (dwLength>0)
//	{
		TRY
		{					
			CFile fFmt(strDestPath, CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone);

			WORD wSignature = 0xFEFF;// use unicode encoding
			fFmt.Write(&wSignature, 2);//Unicode的文件符?
			
			fFmt.Write(strText, dwLength * 2);
			fFmt.Close();
			bOK = TRUE;
		}
		CATCH(CFileException, pEx)
		{	
			TCHAR sPrompt[512] = {0};
			pEx->GetErrorMessage(sPrompt, 512);
			AfxMessageBox(sPrompt, MB_OK|MB_ICONSTOP);
		}
		END_CATCH
//	}

	return bOK;
}



BOOL CFileDB::IsFileLocked(CString strFileCode, CString &strLockUser)
{
	BOOL bLocked=FALSE;
	CString strSql;
	CADORecordset rs(m_pDB);
//	strSql=CADODatabase::PrepareSQL(_T("select 1 from text_lib where filecode=N'%s' and txt_state='L'") , strFileCode);
	strSql=CADODatabase::PrepareSQL(_T("select lockuser from text_lib where filecode=N'%s' and txt_state='L'") , strFileCode);
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			rs.GetFieldValue(L"lockuser",strLockUser);
			bLocked=TRUE;
		}
		rs.Close();
	}
	return bLocked;
}

BOOL CFileDB::UpdateTitles(LPCTSTR  szFileCode, LPCTSTR szTitle1, LPCTSTR szTitle2, LPCTSTR szTitle3, LPCTSTR szTitle4,LPCTSTR szModifyTime)
{
	CADOCommand cmd(m_pDB);
//	return cmd.GetProcExecResult(TRUE,_T("UpdateTitles"),_T("%s%s%s%s%s%s"),szFileCode,szTitle1,	
//		szTitle2,szTitle3,szTitle4,szModifyTime);

	CString strTitle1 = L"";  

	return cmd.GetProcExecResult(TRUE,_T("UpdateTitles"),_T("%n%n%n%n%n%n"),szFileCode,strTitle1,	
		strTitle1,strTitle1,strTitle1,szModifyTime);
}

BOOL CFileDB::GetLibFileStatus(CString strFileCode, int nLib, CString strPlace, short &nRet)
{
	CADOCommand cmd(m_pDB);
//	nRet= cmd.GetProcExecResult(TRUE,_T("GetLibFileStatus") , _T("%s%d%s") ,strFileCode,nLib,strPlace);
	nRet= cmd.GetProcExecResult(TRUE,_T("GetLibFileStatus") , _T("%n%d%n") ,strFileCode,nLib,strPlace);
	CResManage setRes(g_hInst);
	switch(nRet)
	{
	case 0:
		return TRUE;
	case 1:
		AfxMessageBox(IDS_FILENOTINLIB);
		break;
	case -1:
		break;
	}
	return FALSE;
}
BOOL CFileDB::GetColFileStatus(CString strFileCode, int nColID, CString strColDate, short &nRet)
{
	CADOCommand cmd(m_pDB);
	nRet= cmd.GetProcExecResult(TRUE,_T("GetColFileStatus") , _T("%s%d%s") ,strFileCode,nColID,strColDate);
	CResManage setRes(g_hInst);
	switch(nRet)
	{
	case 0:
		return TRUE;
	case 1:
		AfxMessageBox(IDS_PLACECHANGE);
		break;
	case -1:
		break;
	}
	return FALSE;
}
BOOL CFileDB::GetDefaultLib(int &nLib, CString &strPlace,int nGroup)
{
	CString strSql;
	CADORecordset rs(m_pDB);
	strSql=CADODatabase::PrepareSQL(_T("select news_group.lib_id,place from user_group , news_group,news_lib  where ")
		_T("usercode=user_name() and user_group.group_id = %d ")
		_T("and news_group.lib_id = news_lib.lib_id " )
		_T("and user_group.group_id = news_group.group_id ") , nGroup );
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		rs.GetFieldValue(0,nLib);
		rs.GetFieldValue(1,strPlace);
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::TransFile(CString strFileCode, int nDesLibID, CString strDesPlace, CString strCopyStyle,
						int nGroupID,CString strUserCode,short nSecret,CString *strNote,CString strOper, int nAction, short &nRet)
{
	CADOCommand cmd(m_pDB);
//	nRet= cmd.GetProcExecResult(TRUE,_T("TransFile") , _T("%s%d%s%s%d%s%d%s%s%d") ,strFileCode,nDesLibID,strDesPlace,strCopyStyle,
//		nGroupID,strUserCode,nSecret,*strNote,strOper,nAction);
	nRet= cmd.GetProcExecResult(TRUE,_T("TransFile") , _T("%n%d%n%s%d%n%d%n%n%d") ,strFileCode,nDesLibID,strDesPlace,strCopyStyle,
		nGroupID,strUserCode,nSecret,*strNote,strOper,nAction);
	CResManage setRes(g_hInst);
	switch (nRet)
	{
		case 1:
			AfxMessageBox( IDS_NOGETFILEPER);
			break;
		case 2:
			AfxMessageBox( IDS_NOSENDFILEPER);
			break;
		case 3:
			AfxMessageBox( IDS_SENDONSELF);
			break;
		case 0:
			return TRUE;
		default:
			break;
	}
	return FALSE;
}

BOOL CFileDB::LoadFilePerm(int nGroupID, CADORecordset *pRS)
{
	CString sql;
	sql.Format(_T("select perm_style from list_permission where group_id=%d"),nGroupID);
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::GetLibUser(int nLib, CString strPlace, CADORecordset *pRS)
{
	CString sql;
	sql=CADODatabase::PrepareSQL(_T("select distinct username from user_group , news_group , news_user where ")
		_T(" user_group.usercode = news_user.usercode and place=N'%s' and user_group.group_id = news_group.group_id and news_group.lib_id = %d") , strPlace , nLib );
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::GetUserCode(CString strUserName, CString &strUserCode)
{
	CString sql;
	CADORecordset rs(m_pDB);
	sql=CADODatabase::PrepareSQL(_T("select usercode from news_user where username=N'%s'") , strUserName);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		if(!rs.IsEof())
			rs.GetFieldValue(0,strUserCode);
		else
			return FALSE;
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

//20081104 Liubc
BOOL CFileDB::GetUserName(const CString &strUserCode, CString &strUserName)
{
	CString sql;
	CADORecordset rs(m_pDB);
	sql=CADODatabase::PrepareSQL(_T("select username from news_user where usercode=N'%s'") , strUserCode);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		if(!rs.IsEof())
			rs.GetFieldValue(0, strUserName);
		else
			return FALSE;
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::LockFile(CString strFileCode)
{
	int nRet;
	CADOCommand cmd(m_pDB);
	nRet= cmd.GetProcExecResult(TRUE,_T("LockFile") , _T("%s") ,strFileCode);
	CResManage setRes(g_hInst);
	if(nRet==0)
		return TRUE;
	return FALSE;
}

BOOL CFileDB::UnlockFile(CString strFileCode,BOOL bSetCurruserNull)
{

	CADOCommand cmd(m_pDB);
	cmd.SetText(_T("UnLockFile"));
	cmd.SetType(adCmdStoredProc);
	cmd.AddParameter(_T(""),adVarChar,adParamInput,strFileCode.GetLength(),strFileCode);
	cmd.AddParameter(_T(""),adSmallInt,adParamInput,sizeof(short),bSetCurruserNull);
	return cmd.Execute();
}

BOOL CFileDB::LoadColumn(CADORecordset *pRS, int nLib,int nGroupID, TCHAR *szPerm)
{
	CString sql;
	sql=CADODatabase::PrepareSQL(_T("select t1.paper_code , t1.column_code from news_column t1, column_permission t2 where lib_id= %d ")
		_T("and t1.column_id = t2.column_id ")
		_T("and group_id = %d and perm_style='%s' order by t1.column_code") , nLib , nGroupID , szPerm );
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CFileDB::LoadColumn(CADORecordset *pRS, int nLib,int nGroupID)
{
	CString sql, strValue;
	if(!GetConfig(_T("ColumnSortMethod"),strValue))
		strValue = L"0";
	sql=CADODatabase::PrepareSQL(_T("select distinct t1.paper_code , t1.column_code,t1.column_id, t1.layout_code from news_column t1, column_permission t2 where lib_id= %d ")
		_T("and t1.column_id= t2.column_id ")
		_T("and group_id = %d and perm_style in ('R','W','G','T','B','F') order by ")/*t1.column_code") */, nLib , nGroupID );
	sql += strValue == L"1"?_T("t1.column_code"):_T("t1.layout_code");
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::LoadColUser(CADORecordset *pRS, int nColumnID, int nLib)
{
	CString sql;
	if(nLib==0)
		sql=CADODatabase::PrepareSQL(_T("select distinct username,usernote from news_user , user_group , column_permission ")
			_T("where user_group.usercode = news_user.usercode and ")
			_T("user_group.group_id = column_permission.group_id ")
			_T("and column_id=%d and perm_style = 'W' "), nColumnID) ;
	else
		sql=CADODatabase::PrepareSQL(_T("select distinct username,usernote from news_user , user_group , column_permission ")
			_T("where news_user.lib_id=%d and user_group.usercode = news_user.usercode and ")
			_T("user_group.group_id = column_permission.group_id ")
			_T("and column_id=%d and perm_style = 'W' " ), nLib , nColumnID ) ;
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::FileHaveSource(LPCTSTR strFileCode)
{
	CString sql;
	sql=CADODatabase::PrepareSQL(_T("select 1 from source_lib ")
			_T(" where src_filecode=N'%s' "),strFileCode);
	CADORecordset rs(m_pDB);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		int nRet=0;
		if(rs.GetRecordCount()>0)
			rs.GetFieldValue(0,nRet);
		rs.Close();
		if(nRet==1)
			return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::IsCopyFile(LPCTSTR strFileCode)
{
	CString sql;
	sql=CADODatabase::PrepareSQL(_T("select copy_style from text_lib where filecode=N'%s'"),strFileCode);
	CADORecordset rs(m_pDB);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		CString strCopy=_T("");
		if(rs.GetRecordCount()>0)
			rs.GetFieldValue(0,strCopy);
		rs.Close();
		if(!strCopy.IsEmpty()&&strCopy.GetAt(0)=='Y')
			return TRUE;
		
	}
	return FALSE;
}

BOOL CFileDB::IsSecret(CString &strFileCode, CString &strUserCode)
{
	CString sql;
	sql=CADODatabase::PrepareSQL(_T("select secret_style from text_lib where filecode=N'%s' and curruser=N'%s'"),strFileCode,strUserCode);
	CADORecordset rs(m_pDB);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		int nSecret=0;
		if(rs.GetRecordCount()>0)
			rs.GetFieldValue(0,nSecret);
		rs.Close();
		if(nSecret==1)
			return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::LoadFileInfo(CString &strFileCode, CString &strTitle, double *dbWord, BOOL *bSingle, BOOL *bSecret, 
	 BOOL *bUrgency, BOOL *bLock, CString &strAuthor, CString &strStyle, CString &strPostBox, CString &strUser)
{
	CString sql;
	sql=CADODatabase::PrepareSQL(_T("select txt_title , txt_words , ")
		_T("exclusive , urgency_style , txt_state , txt_author, ")
		_T("secret_style , txt_style , curruser, ")
		_T("postbox from text_lib t1,text_info t2 where t1.filecode = N'%s' and t1.filecode=t2.filecode "),strFileCode);
	CADORecordset rs(m_pDB);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		int nSecret=0;
		if(rs.GetRecordCount()>0)
		{
			CString str;
			rs.GetFieldValue(0,strTitle);
			rs.GetFieldValue(1,*dbWord);
			rs.GetFieldValue(2,*bSingle);
			rs.GetFieldValue(_T("secret_style"),*bSecret);
			if(*bSecret!=1)
				bSecret=0;
			rs.GetFieldValue(_T("urgency_style"),str);
			if(!str.IsEmpty()&&str.GetAt(0)=='Y')
				*bUrgency = TRUE;
			else
				*bUrgency = FALSE;
			rs.GetFieldValue(_T("txt_state"),str);
			if(!str.IsEmpty()&&str.GetAt(0)=='L')
				*bLock=TRUE;
			else
				*bLock=FALSE;
			rs.GetFieldValue(_T("txt_author"),strAuthor);
			rs.GetFieldValue(_T("curruser"),strUser);
			rs.GetFieldValue(_T("txt_style"),strStyle);
			rs.GetFieldValue(_T("postbox"),strPostBox);
		}
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::LoadColumnPerm(int nGroupID, int nColumnID, CADORecordset *pRS)
{
	CString sql;
	sql.Format(_T("select perm_style from column_permission t1,news_column t2 ")
		_T("where group_id=%d and t1.column_id=%d and t1.column_id=t2.column_id"),nGroupID,nColumnID);
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::ListColumnDate(CADORecordset *pRS, int nColumnID)
{
	CString sql;
	sql.Format(_T("select distinct column_date from paper_filelist ")
		_T("where column_id=%d order by column_date desc"),nColumnID);
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::LoadPaperFile(CADORecordset *pRS, CString &strDate, int nColumnID)
{
	CString sql;
	/*
	sql.Format(_T("select distinct t1.filecode , txt_title , txt_words , txt_state , dbo.f_GetNewsUserName(curruser) curruser ,dbo.f_GetNewsUserName(lockuser) lockuser , position , urgency_style ,t3.last_sendtime,last_oper,convert(char(26),last_modtime,121) last_modtime ")
		_T(", copy_style , secret_style , exclusive , txt_author,txt_format,txt_date,layout_file ")
		_T(",case  when isnull(ID,0)=0 then 0 else 1 end has_attach, case isnull(tb_rel.filecode, '') when '' then 0 else 1 end as isrelate ")
		_T(" from text_lib t1, deptfile_info tb_rel,paper_filelist t2,file_place t3,attachs t4")
		_T(" where ( (not (isnull(t1.exclusive,0) = 1 or isnull(t1.secret_style,0) <> 0)) or ((t1.exclusive = 1 or isnull(t1.secret_style,0) <> 0) and (t1.curruser = user_name() or t1.txt_author = '%s' )))") 
		_T(" and t1.filecode=t2.filecode and t1.filecode *= tb_rel.filecode and txt_state<>'D' and t2.column_id=%d and column_date ='%s' and t1.filecode=t3.filecode and t1.filecode*=t4.filecode and t4.del_user is null"),
		CGlobal::GetInstance()->m_pEmployee->m_strUserName, nColumnID, strDate);
	*/
	sql.Format(_T("select distinct t1.filecode , txt_title , txt_words , txt_state , dbo.f_GetNewsUserName(curruser) curruser ,dbo.f_GetNewsUserName(lockuser) lockuser , position , urgency_style ,t3.last_sendtime,last_oper,convert(char(26),last_modtime,121) last_modtime ")
		_T(", copy_style , secret_style , exclusive , txt_author,txt_format,txt_date,layout_file ")
		_T(",case  when isnull(ID,0)=0 then 0 else 1 end has_attach, case isnull(tb_rel.filecode, '') when '' then 0 else 1 end as isrelate ")
		_T(" from text_lib t1 left join ")
		_T(" deptfile_info tb_rel on t1.filecode = tb_rel.filecode left join ")
		_T(" attachs t4 on t1.filecode=t4.filecode and t4.del_user is null,paper_filelist t2,file_place t3")
		_T(" where ( (not (isnull(t1.exclusive,0) = 1 or isnull(t1.secret_style,0) <> 0)) or ((t1.exclusive = 1 or isnull(t1.secret_style,0) <> 0) and (t1.curruser = user_name() or t1.txt_author = N'%s' )))") 
		_T(" and t1.filecode=t2.filecode and txt_state<>'D' and t2.column_id=%d and column_date ='%s' and t1.filecode=t3.filecode "),
		CGlobal::GetInstance()->m_pEmployee->m_strUserName, nColumnID, strDate);
	

	//edit by leon 2012-12-20 for crystal report issue
	CGlobal::GetInstance()->m_ReportPara.strSqlLoadData=sql;
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		CGlobal::GetInstance()->m_ReportPara.bValid=TRUE;
		return TRUE;
	}
	else {
		CGlobal::GetInstance()->m_ReportPara.bValid=FALSE;
		return FALSE;
	}
	//end leon 2012-12-20
}

BOOL CFileDB::IsFinishComposed(CString &strFileCode)
{
	int nRet;
	CADOCommand cmd(m_pDB);
	nRet= cmd.GetProcExecResult(TRUE,_T("IsFinishComposed") , _T("%s") ,strFileCode);
	if(nRet==0)
		return TRUE;
	return FALSE;	
}

BOOL CFileDB::GetLayoutInfo(const CString &strFileCode, CString &strLayoutCode, COleDateTime &dtLayoutDate, CString &strLayoutFile)
{
	BOOL bRet = FALSE;
	CString sql;
	CADORecordset rs(m_pDB);
	sql.Format(_T("select layout_code,column_date,layout_file from paper_filelist t1,news_column t2 where ")
		_T(" t1.filecode=N'%s'  and t1.column_id=t2.column_id"),strFileCode);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			rs.GetFieldValue(0,strLayoutCode);
			rs.GetFieldValue(1,dtLayoutDate);
			rs.GetFieldValue(2,strLayoutFile);
			bRet = TRUE;
		}
		else
			bRet = FALSE;
		rs.Close();
	}
	return bRet;
}

BOOL CFileDB::GetFileLastModTime(CString &strDate, const CString &strFileCode)
{
	BOOL bRet = FALSE;
	CString sql;
	CADORecordset rs(m_pDB);
	sql.Format(_T("select convert(char(26),last_modtime,121) from text_lib where filecode=N'%s'"),strFileCode);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			CString str;
			rs.GetFieldValue(0,str);
		//	ChangeDBTimeToMyTimeStamp(str,strDate);
			strDate=str;
			bRet = TRUE;
		}
		else
			bRet = FALSE;
		rs.Close();
	}
	return bRet;
}

BOOL CFileDB::IsFileExist(CString &strFileCode,BOOL &bExist)
{
	CString sql;
	CADORecordset rs(m_pDB);
	sql.Format(_T("select 1 from text_lib where filecode=N'%s'"),strFileCode);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			bExist = TRUE;
		}
		else
			bExist = FALSE;
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::RollbackFile(CString &strFileCode, int nGroupID, CString &strOper)
{
	int nRet;
	CADOCommand cmd(m_pDB);
	nRet= cmd.GetProcExecResult(TRUE,_T("RollbackFile") , _T("%s%d%s") ,strFileCode,nGroupID,strOper);
	CResManage setRes(g_hInst);
	if(nRet==0)
	{
		CString strSql;
		CADORecordset rs(m_pDB);
		strSql.Format(L"select 1 from text_lib where filecode = N'%s' and isnull(secret_style,0) <> 0 ",strFileCode );
		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			if(rs.IsEof())
			{
				strSql.Format(L"update text_lib set curruser = null where filecode = N'%s'",strFileCode);
				m_pDB->Execute(strSql);
			}
			rs.Close();
		}
		/*CString strSql;strSql.Format(L"update text_lib set curruser = null where filecode = N'%s'",strFileCode);
		m_pDB->Execute(strSql);*/
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::GetFinalUser(const CString &strFileCode, CString &strUserCode)
{
	CString sql;
	CADORecordset rs(m_pDB);
	BOOL bRet;
	sql.Format(_T("select usercode from file_record where record_id=(select max(record_id) from text_lib t1,file_record t2 where t1.filecode=N'%s' and t2.filecode=t1.filecode )"),strFileCode);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			bRet = TRUE;
			rs.GetFieldValue(0,strUserCode);
		}
		else
			bRet = FALSE;
		rs.Close();
	}
	return bRet;
}

BOOL CFileDB::GetFileRecord(CADORecordset *pRS, CString &strFileCode)
{
	CString sql;
	BOOL bRet;
	sql.Format(_T("(select t1.txt_words , txt_title , username , upper(operation) oper ,finish_time , dbo.f_GetLibName(t1.lib_id) lib_name , t1.place_code ,dbo.f_GetLibName(t1.src_libid) srclib_name ,t1.src_place ,'' paper_code, '' column_code , '' column_date ,t1.note , 0 place ")
		_T("from file_record t1, text_lib t2 , news_user t3  ")
		_T("where t1.filecode=N'%s' and t1.filecode=t2.filecode and t1.usercode=t3.usercode ")
	//	_T("/* and t1.position = 'L'*/ ")
		_T("union ")
		_T("select t4.txt_words,txt_title , username , upper(style) oper,t4.finish_time , '' lib_name, '' place_code ,'','' src_place, paper_code , column_code , column_date ,t4.note,1 place ")
		_T("from text_lib t2 , news_user t3 , paper_record t4 ,news_column t5 ")
		_T("where t2.filecode=N'%s' and t4.usercode=t3.usercode  and t2.filecode=t4.filecode and t5.column_id=t4.column_id) ")
		_T("order by finish_time "),strFileCode,strFileCode);
	if(pRS->Open(sql,CADORecordset::openQuery))
	{
		if(pRS->GetRecordCount()>0)
		{
			bRet = TRUE;
		}
		else
			bRet = FALSE;
	}
	return bRet;
}
BOOL CFileDB::LoadAttach(CADORecordset *pRS,CString &strFileCode)
{
	CString sql;
	BOOL bRet;
	sql.Format(_T("select attach_name,type,attach_size,upload_date,dbo.f_GetNewsUserName(username) username ")
		_T("from attachs where filecode=N'%s' and del_user is null"),strFileCode);
	if(pRS->Open(sql,CADORecordset::openQuery))
	{
		if(pRS->GetRecordCount()>0)
		{
			bRet = TRUE;
		}
		else
			bRet = FALSE;
	}
	return bRet;
}

BOOL CFileDB::GetAttachTotalSize(const CString &strFileCode, int &nSize)
{
	CADORecordset rs(m_pDB);
	CString sql;
	sql.Format(_T("select sum(attach_size) from attachs where filecode=N'%s' and del_user is null"),strFileCode);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		rs.GetFieldValue(0,nSize);
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::ExistAttach(const CString &strFileCode,const CString &strAttachName, CString &strUploadDate)
{
	CADORecordset rs(m_pDB);
	CString sql;
	BOOL bRet=FALSE;
	sql.Format(_T("select upload_date from attachs where filecode=N'%s' and attach_name=N'%s' and del_user is  null"),strFileCode,strAttachName);
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			rs.GetFieldValue(0,strUploadDate,DATEFORMAT);
			bRet=TRUE;
		}
		rs.Close();
	}
	return bRet;
}

BOOL CFileDB::InsertAttach(CAttach *pAttach)
{
	int nRet;
	CADOCommand cmd(m_pDB);

	nRet= cmd.GetProcExecResult(TRUE,_T("insert_attachs") , _T("%n%n%s%d%s%n") ,
		pAttach->strFileCode,pAttach->strName,pAttach->strType,pAttach->nSize,pAttach->strUploadDate,pAttach->strUser);
	if(nRet==0)
		return TRUE;
	return FALSE;	
}

BOOL CFileDB::DeleteAttach(const CString &strFileCode, const CString &strAttachName)
{
	int nRet;
	CADOCommand cmd(m_pDB);
//	nRet= cmd.GetProcExecResult(TRUE,_T("delete_attachs") , _T("%s%s") ,strFileCode,strAttachName);
	nRet= cmd.GetProcExecResult(TRUE,_T("delete_attachs") , _T("%n%n") ,strFileCode,strAttachName);
	if(nRet==0)
		return TRUE;
	return FALSE;	
}

BOOL CFileDB::LoadNewspaper(CComboBox *pComb, int nGroup, TCHAR tcType)
{
	CADORecordset rs(m_pDB);
	CString sql,str;
	BOOL bRet=FALSE;
	//sql.Format(_T("select distinct paper_code from column_permission t1,news_column t2 where")
	//	_T(" t1.column_id=t2.column_id and group_id=%d and perm_style='%c' ") , nGroup , tcType );
	sql.Format(_T("select distinct paper_code from news_column"));
	if(rs.Open(sql,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			while(!rs.IsEof())
			{
				rs.GetFieldValue(0,str);
				pComb->AddString(str);
				rs.MoveNext();
			}			
			bRet=TRUE;
		}
		rs.Close();
	}
	return bRet;
}

BOOL CFileDB::LoadColByPaper(CADORecordset *pRS, const CString &strPaper, int nGroup, TCHAR tcType)
{
	CString sql;
	BOOL bRet;
	sql.Format(_T("select distinct column_code ,t2.column_id from column_permission t1,news_column t2 ")
		_T("where t1.column_id=t2.column_id and group_id=%d and perm_style='%c' and paper_code=N'%s' ") , nGroup , tcType , strPaper);
	if(pRS->Open(sql,CADORecordset::openQuery))
	{
		if(pRS->GetRecordCount()>0)
		{
			bRet = TRUE;
		}
		else
			bRet = FALSE;
	}
	return bRet;	
}

BOOL CFileDB::LoadLibUser(CADORecordset *pRS, int nLibID)
{
	CString sql;
	BOOL bRet = FALSE;;
	sql.Format(_T("select username from news_user where lib_id=%d"),nLibID);
	if(pRS->Open(sql,CADORecordset::openQuery))
	{
		if(pRS->GetRecordCount()>0)
		{
			bRet = TRUE;
		}
	}
	return bRet;	
}

BOOL CFileDB::SearchFile(CADORecordset *pRS, const CString &strSql)
{
	BOOL bRet=FALSE;
	//edit by leon 2012-11-19 for crystal report issue
	if(pRS->Open(strSql,CADORecordset::openQuery))
	{
		if(pRS->GetRecordCount()>0)
		{		
			bRet=TRUE;
		}		
	}
	CGlobal::GetInstance()->m_ReportPara.strSqlLoadData=strSql;
	CGlobal::GetInstance()->m_ReportPara.bValid=bRet;
	//end leon 2012-12-20
	return bRet;
}

//edit by leon 2012-10-03 for migrating from ANSI to Unicode
//BOOL CFileDB::UpdateLib(const CString &strFileCode, LPCSTR lpTitle,BOOL bUpdateTitle,double dbWords, int nGroup, const CString &strOper,LPCTSTR lpLastModTime, CString strTextVersion, CString strTextVersionMark, CString strModHistory, bool bUpdateModHistory, CString strTextVersion_Comment, CString str_Comment, CString strTextVersion_Style)
BOOL CFileDB::UpdateLib(const CString &strFileCode, LPWSTR lpTitle,BOOL bUpdateTitle,double dbWords, int nGroup, const CString &strOper,LPCTSTR lpLastModTime, CString strTextVersion, CString strTextVersionMark, CString strModHistory, bool bUpdateModHistory, CString strTextVersion_Comment, CString str_Comment, CString strTextVersion_Style, int &RetCode)
//end by leon 2012-10-03
{

	if (bUpdateModHistory)
	{
		CString strModHistory_Add;
		COleDateTime dt = COleDateTime::GetCurrentTime();
		CString strDate; 
		strDate.Format(L"%s",dt.Format(DATETIMEFORMAT));
		strModHistory_Add = CGlobal::GetInstance()->m_pEmployee->m_strUserName + L"於" + strDate;
		if (strModHistory.Replace( L"#", L"#") == 14)
			strModHistory = strModHistory.Left(strModHistory.ReverseFind('#')) + L"#" + strModHistory_Add;
		else
			strModHistory = strModHistory + L"#" + strModHistory_Add;
	}
	
	CADOCommand cmd(m_pDB);
	int nRet=cmd.GetProcExecResult(TRUE,_T("UpdateLib"),_T("%n%f%d%n%n%n%d%n%n%n%n%n%n"),strFileCode,dbWords,nGroup,strOper,lpLastModTime,lpTitle,bUpdateTitle, strTextVersion, strTextVersionMark, strModHistory, strTextVersion_Comment, str_Comment, strTextVersion_Style);
//	int nRet=cmd.GetProcExecResult(TRUE,_T("UpdateLib"),_T("%n%f%d%n%n%n%d%n%n"),strFileCode,dbWords,nGroup,strOper,lpLastModTime,lpTitle,bUpdateTitle, strTextVersion, strTextVersionMark);
//	int nRet=cmd.GetProcExecResult(TRUE,_T("UpdateLib"),_T("%n%f%d%n%n%n%d%n%n"),strFileCode,1,1,L"sds",lpLastModTime,lpTitle,TRUE, L"", L"");
	RetCode = nRet;
	return nRet==0;
}

BOOL CFileDB::HasColPerm(int nGroupID, int nColID, TCHAR tcPerm)
{
	CString sql;

	sql.Format(_T("select 1 from column_permission t1,news_column t2 ")
		_T("where group_id=%d and t1.column_id=%d and perm_style='%s' and t1.column_id=t2.column_id"),nGroupID,nColID,tcPerm);
	CADORecordset rs(m_pDB);
	if(rs.Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
			return TRUE;
	}
	return FALSE;
}
BOOL CFileDB::GetFileText(const CString &strFileCode, CString &strText)
{
	strText.Empty();
	CString str;
	str.Format(_T("select txt_textcontent from text_lib  where filecode=N'%s'"),strFileCode);
	
	CADORecordset rs(m_pDB);
	if(rs.Open(str,CADORecordset::openQuery))
	{
		BOOL bOK;
		if(rs.GetRecordCount()>0)
		{
			bOK=rs.GetFieldValueContent(_T("txt_textcontent"),strText);
		}
		rs.Close();
		return bOK;
	}

	return FALSE;
}
BOOL CFileDB::GetRelatePic(CString strFileCode, CADORecordset *pRS)
{
	CString sql;
	if(CGlobal::GetInstance()->m_pShareData->m_bMediaColumnExist != TRUE 
			|| CGlobal::GetInstance()->m_pShareData->m_bSupportMedia != TRUE)
		sql=CADODatabase::PrepareSQL(_T("select photo_path,mid_path,photocode ,'0' photopos from file_photo_relation t1,wires_photo_src t2 where photoflag=0 and t1.filecode=N'%s' and t1.photocode=t2.photo_name")
			_T(" union select photo_path,mid_path,photocode,'1' photopos from file_photo_relation t1,paper_photolist t2,wires_photo_src t3 where photoflag=1 and t1.filecode=N'%s' and t1.photocode=t2.photo_id and t2.photo_name=t3.photo_name") , strFileCode,strFileCode );
	else
		sql=CADODatabase::PrepareSQL(_T("select photo_path,mid_path,photocode ,'0' photopos, media_type, small_path from file_photo_relation t1,wires_photo_src t2 where photoflag=0 and t1.filecode=N'%s' and t1.photocode=t2.photo_name")
			_T(" union select photo_path,mid_path,photocode,'1' photopos, media_type, small_path from file_photo_relation t1,paper_photolist t2,wires_photo_src t3 where photoflag=1 and t1.filecode=N'%s' and t1.photocode=t2.photo_id and t2.photo_name=t3.photo_name") , strFileCode,strFileCode );
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL  IsChineseOnPos(LPCTSTR  pszBuf,int nLen)
{
		int nCount = 0;
		
		for (int j=0; j<nLen; j++)
		{
			if ((BYTE)pszBuf[j] > 0x7f)
				nCount++;
		}

		return (nCount % 2);
}

BOOL CFileDB::GetLastOnColPhoto(const CString &strPhotoName, int &iPhotoID, int &iColID, COleDateTime &odtColDate)
{
	CADORecordset rs(m_pDB);
	CString strSQL;
	BOOL bRet = FALSE;
	strSQL.Format(L"select top 1 photo_id, column_date, column_id "
		L" from paper_photolist where photo_name = N'%s' order by column_date desc"
		, strPhotoName); 
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if(!rs.IsEof())
		{
			rs.GetFieldValue(_T("photo_id"), iPhotoID);
			rs.GetFieldValue(_T("column_date"), odtColDate);
			rs.GetFieldValue(_T("column_id"), iColID);
			bRet = TRUE;
		}
		rs.Close();
	}
	return bRet;
}

BOOL CFileDB::GetPicInfo(LPCTSTR lpFileID, short nPos, CString &strInfo)
{
	strInfo.Empty();
	CString str;
	int nAgencyFlag;
	CADORecordset rs(m_pDB);
	if(nPos==0)//未選用圖片
	{
		str.Format(_T("select tele_code,agency_flag,photo_time,photo_path,photo_title from wires_photo_src where photo_name=N'%s'"),lpFileID);
		if(rs.Open(str,CADORecordset::openQuery))
		{
			if(rs.GetRecordCount()>0)
			{
				CString strDepart,strWires,strTemp;
				CResManage res(g_hInst);
				strDepart.LoadString(IDS_DEPART);
				strWires.LoadString(IDS_WIRES);
				strTemp.LoadString(IDS_IMPORTTIME);
				rs.GetFieldValue(0,str);
				rs.GetFieldValue(1,nAgencyFlag);
				strInfo.Format(_T("%s:%s\n"), nAgencyFlag==0?strWires:strDepart,str);

				rs.GetFieldValue(2,str,DATEFORMAT);
				strInfo += strTemp+_T(":")+str+_T("\n");

				/*rs.GetFieldValue(3,str);
				strTemp.LoadString(IDS_PICSRCPATH);
				strInfo += strTemp+_T(":")+str+_T("\n");*/

				rs.GetFieldValue(4,str);
				strTemp.LoadString(IDS_PICTITLE);
				strInfo += strTemp+_T(":")+str+_T("\n");	
				return TRUE;
			}
		}

	}
	else if(nPos==1)//欄目圖片
	{
		str.Format(_T("select paper_code,column_code,column_date,layoutfile,islayout,photo_title from ")
			_T("paper_photolist t1,news_column t2 where t1.column_id=t2.column_id and photo_id=%s"),lpFileID);
		if(rs.Open(str,CADORecordset::openQuery))
		{
			if(rs.GetRecordCount()>0)
			{
				CString strCol,str1,strTemp;
				CResManage res(g_hInst);
				strCol.LoadString(IDS_COLUMN);
				strTemp.LoadString(IDS_COLUMNDATE);
				rs.GetFieldValue(0,str);
				rs.GetFieldValue(1,str1);
				strInfo.Format(_T("%s:%s(%s)\n"), strCol,str1,str);
				
				rs.GetFieldValue(2,str,DATEFORMAT);
				strInfo += strTemp+_T(":")+str+_T("\n");
				
				/*rs.GetFieldValue(3,str);
				int n;
				rs.GetFieldValue(4,n);
				strTemp.LoadString(IDS_PICSRCPATH);
				str1.LoadString(IDS_NOTSIGN);
				strInfo += strTemp+_T(":")+(n==0?str1:str)+_T("\n");
				*/
				rs.GetFieldValue(5,str);
				strTemp.LoadString(IDS_PICTITLE);
				strInfo += strTemp+_T(":")+str+_T("\n");	
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

BOOL CFileDB::IsLinkedByDM(LPCTSTR lpszFileCode)
{

	CADORecordset rs(m_pDB);

	int nCount = 0;

	CString strSQL;

	strSQL.Format(_T("select count(*) from deptfile_info where filecode=N'%s' "), lpszFileCode); 

	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		rs.GetFieldValue(0, nCount);

		rs.Close();
	}
	return ( nCount > 0 );
}

BOOL CFileDB::CanUnlink(LPCTSTR lpszFileCode, int nGroupId)
{
	CADORecordset rs(m_pDB);
	
	int nCount = 0;
	
	CString strSQL;
	
	strSQL.Format(_T("select count(*) from deptfile_info where filecode=N'%s' and author = user_name()"), lpszFileCode); 
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		rs.GetFieldValue(0, nCount);
		
		rs.Close();
	}
	if ( nCount > 0 )
		return TRUE;

	nCount = 0;

	strSQL.Format(
		L" select count(*) from departgroup_permission A, deptfile_group B, deptfile_info C "
		L" where A.group_id=%d and A.perm_style='M' and A.lib_id=B.lib_id and B.group_id=C.group_id and C.filecode=N'%s' ",
		nGroupId, lpszFileCode);

	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		rs.GetFieldValue(0, nCount);
		
		rs.Close();
	}
	
	return ( nCount > 0 );


}

void CFileDB::LinkToDM(int nDeptFileInfoID, LPCTSTR lpszFilecode,LPCTSTR lpOper)
{
	CADOCommand cmd(m_pDB);
	cmd.GetProcExecResult(
		FALSE,
		_T("UpdateDeptFileCode"), 
		_T("%d%s%s"),
		nDeptFileInfoID, 
		lpszFilecode,
		lpOper);
}

int CFileDB::GetDMValidLibIds(int nGroupId, CWordArray &waValid)
{
	int nResult = 0;
	CADORecordset rs(m_pDB);
	
	CString strSQL;
	strSQL = CADODatabase::PrepareSQL(
		_T("select distinct lib_id from departgroup_permission where group_id=%d and (perm_style ='M' or perm_style = 'B' or perm_style = 'P')"),
		nGroupId );
	
	waValid.RemoveAll();
	
	if (rs.Open(strSQL, CADORecordset::openQuery))
	{	
		while(!rs.IsEOF())
		{
			int nId;
			
			rs.GetFieldValue(0, nId);
			waValid.Add(nId);
			
			rs.MoveNext();
		}
		rs.Close();
		
		nResult = waValid.GetSize();
	}
	return nResult;
}
	


int CFileDB::GetDMPrivilege(int nGroupId, int nLib)
{
	int nResult = 0;
	CADORecordset rs(m_pDB);
	
	CString strSQL;
	strSQL = CADODatabase::PrepareSQL(
		_T("select perm_style from departgroup_permission where group_id = %d and lib_id = %d "), 
		nGroupId, nLib);
	
	
	if (rs.Open(strSQL, CADORecordset::openQuery))
	{	
		while(!rs.IsEOF())
		{
			CString strValue;
			
			rs.GetFieldValue(0, strValue);
			
			if (strValue == _T("B"))
				nResult |= 1;
			else if (strValue == _T("M"))
				nResult |= 2;
			
			rs.MoveNext();
		}
		rs.Close();
		
	}
	return nResult;
}

BOOL CFileDB::LoadDeptFileInfo(CADORecordset *pRS, LPCTSTR lpszDate, int nLibID, BOOL bHaveDMPerm, BOOL bFilter)
{
	BOOL bRet = FALSE;

	CString strSQL;

	if(bHaveDMPerm)
	{
		if (bFilter != FALSE)
		{
			strSQL = CADODatabase::PrepareSQL(
				L" select description,item,dbo.f_GetNewsUserName(t1.author) author ,fileinfo_id,case when filecode is null then 0 else 1 end as Linked from deptfile_info t1,deptfile_group t2 "
				L" where t1.group_id=t2.group_id and "
				L" t2.group_date=convert(char(20),'%s',120) and "
				L" t2.lib_id=%d and "
				L" t1.filecode is null "
				L" order by t2.order_id, t1.order_id ",
				lpszDate, nLibID);
		}
		else
		{
			strSQL = CADODatabase::PrepareSQL(
				L" select description,item,dbo.f_GetNewsUserName(t1.author) author ,fileinfo_id,case when filecode is null then 0 else 1 end as Linked from deptfile_info t1,deptfile_group t2 "
				L" where t1.group_id=t2.group_id and  t2.group_date=convert(char(20),'%s',120) and t2.lib_id=%d "
				L" order by t2.order_id, t1.order_id ",
				lpszDate, nLibID);
		}
		
	}
	else
	{
		if (bFilter != FALSE)
		{
			strSQL = CADODatabase::PrepareSQL(
				L" select description,item,t1.author,fileinfo_id,case when filecode is null then 0 else 1 end as Linked from deptfile_info t1,deptfile_group t2 "
				L" where t1.group_id=t2.group_id and "
				L" t2.group_date=convert(char(20),'%s',120) and "
				L" t2.lib_id=%d and "
				L" t1.filecode is  null "
				L" and t1.author=user_name() "
				L" order by t2.order_id, t1.order_id ",
				lpszDate, nLibID);
			
		}
		else
		{
			strSQL = CADODatabase::PrepareSQL(
				L" select description,item,t1.author,fileinfo_id,case when filecode is null then 0 else 1 end as Linked from deptfile_info t1,deptfile_group t2 "
				L" where t1.group_id=t2.group_id and  t2.group_date=convert(char(20),'%s',120) and t2.lib_id=%d and t1.author=user_name() "
				L" order by t2.order_id, t1.order_id ",
				lpszDate, nLibID);
			
		}
	}
	
	
	if(pRS->Open(m_pDB->GetActiveConnection(), strSQL, CADORecordset::openQuery))
	{
		bRet = TRUE;
	}

	return bRet;
}

BOOL CFileDB::GetLinkedFilecode(int nDeptFileInfoId, CString &strFileCode)
{
	BOOL bRet = FALSE;
	CADORecordset rs(m_pDB);
	
	int nCount = 0;
	
	CString strSQL;
	
	strSQL.Format(_T("select file_code from deptfile_info where fileinfo_id=%d "), nDeptFileInfoId); 
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		rs.GetFieldValue(_T("file_code"), strFileCode);
		
		rs.Close();
		bRet = TRUE;
	}
	return bRet;
	
}

BOOL CFileDB::HasRelationToColumn(CString lpFileCode,int &nRet)// Added by Ronald Saliman on 20120904
{
	nRet = 0;
	/*CADOCommand cmd(m_pDB);
	nRet= cmd.GetProcExecResult(TRUE,_T("HasRelationToColumn") , _T("%s") ,lpFileCode);*/
	CADORecordset rs(m_pDB);
	
	CGlobal *pGL = CGlobal::GetInstance();
	CString strSQL;
	strSQL.Format(	L"Select count(*) as 'count' from file_record "
	L"where operation like N'%s%s' and filecode = N'%s' "
	L"and finish_time = (select max(finish_time) from file_record where filecode = N'%s')",
	L"%關聯到部門%",L"",lpFileCode,lpFileCode); 
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (rs.GetRecordCount() > 0)
			rs.GetFieldValue(0, nRet);
		
		rs.Close();
	}	

	if(nRet==0)
		return TRUE;
	return FALSE;
}//end 20120904

BOOL CFileDB::DeleteFile(LPCTSTR lpFileCode,int &nRet)
{
	CADOCommand cmd(m_pDB);
	nRet= cmd.GetProcExecResult(TRUE,_T("DeleteFile") , _T("%s") ,lpFileCode);
	if(nRet==0)
		return TRUE;
	return FALSE;
}

int CFileDB::GetLinkedDMFI(LPCTSTR lpszFileCode)
{
	int nRet = -1;
	CADORecordset rs(m_pDB);
	
	CString strSQL;
	
	strSQL.Format(_T("select fileinfo_id from deptfile_info where filecode = N'%s' "), lpszFileCode); 
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		rs.GetFieldValue(_T("fileinfo_id"), nRet);
		
		rs.Close();
	}

	return nRet;
	
}

void CFileDB::GetDeletedFile(CADORecordset *rs)
{
	CString sql;
	sql.Format(L"select	t.filecode,t.txt_words,t.txt_title ,t.txt_author,"
		L" rtrim(ltrim(n.lib_name))+'['+rtrim(ltrim(f.place_code))+']' position,"
		L" r.usercode,r.finish_time"
		L" from 	text_lib t left outer join file_place f on t.filecode = f.filecode "
		L" left outer join news_lib n on f.lib_id = n.lib_id"
		L" left outer join file_record r on t.filecode = r.filecode "
		L" and r.record_id in "
		L" (select max(record_id) from file_record where filecode = t.filecode)"
		L" where 	t.txt_state = 'D' and r.usercode = user_name()");
	rs->Open(sql,CADORecordset::openQuery);

}

void CFileDB::RestoreFile(LPCTSTR filecode)
{
	CADOCommand cmd(m_pDB);
	cmd.GetProcExecResult(FALSE,_T("RestoreFile") , _T("%n") ,filecode);
	
}

void CFileDB::ClearFile(LPCTSTR filecode)
{
	CADOCommand cmd(m_pDB);
	cmd.GetProcExecResult(FALSE,_T("ClearFile") , _T("%n") ,filecode);

}

void CFileDB::GetPaperCopyList(CADORecordset *rs,LPCTSTR strFilecode)
{
	CString sql;
	/*
	sql.Format(_T("select  	distinct t.filecode,t.txt_words,t.txt_title,n.paper_code,"
		" n.column_code,p.column_date,nl.lib_name"
		" from 	text_lib t left outer join paper_record p on t.filecode=p.filecode "
		" left outer join news_column  n on p.column_id = n.column_id"
		" left outer join news_lib nl on n.lib_id = nl.lib_id  "
		//" where 	--(t.position='C' or t.position='P') and "
		"  where t.src_filecode in "
		" (select src_filecode from text_lib where filecode = '%s')  "
		),strFilecode);
	*/

	sql.Format(L"select distinct t.filecode,t.txt_words,t.txt_title,n.paper_code,"
		L" n.column_code,p.column_date,nl.lib_name"
		L" from text_lib t left outer join paper_record p on t.filecode=p.filecode "
		L" left outer join news_column  n on p.column_id = n.column_id"
		L" left outer join news_lib nl on n.lib_id = nl.lib_id  "
		L"  where t.filecode = N'%s' or t.src_filecode = N'%s' or t.filecode in "
		L" (select src_filecode from text_lib where filecode = N'%s') "
		,strFilecode, strFilecode, strFilecode);
	
	rs->Open(sql,CADORecordset::openQuery);
}

void CFileDB::GetLibUrgency(int nLibID, LPCTSTR strFileCode,BOOL &bUrgency)
{
	CADORecordset rs(m_pDB);

	CGlobal *pGL = CGlobal::GetInstance();
	
	CString strSQL;
	
	strSQL.Format(L"select * from list_permission where  group_id= %d  "
		            L" and lib_id = %d and perm_style = 9 and "
					 L" (perm_range = 'M' or "
					 L" (perm_range = 'S' and exists(select txt_author from text_lib "
					 //" where filecode = '%s' and dbo.f_GetNewsUserName(user_name()) = txt_author)))  "),pGL->m_pEmployee->m_nGroup,nLibID,strFileCode ); 
					 L" where filecode = N'%s' and N'%s' = txt_author)))  ",pGL->m_pEmployee->m_nGroup,nLibID,strFileCode,pGL->m_pEmployee->m_strUserName ); 


	
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (rs.GetRecordCount() > 0)

			bUrgency = TRUE;
		
		rs.Close();
	}




}

void CFileDB::GetLibSecret(int nLibID, LPCTSTR strFileCode,BOOL &bSecret)
{
	CADORecordset rs(m_pDB);
	
	CGlobal *pGL = CGlobal::GetInstance();
	
	CString strSQL;
	
	strSQL.Format(L"select * from list_permission where  group_id= %d  "
		L" and lib_id = %d and perm_style = 7 and "
		L" (perm_range = 'M' or "
		L" (perm_range = 'S' and exists(select txt_author from text_lib "
		//" where filecode = '%s' and dbo.f_GetNewsUserName(user_name()) = txt_author)))  "),pGL->m_pEmployee->m_nGroup,nLibID,strFileCode ); 
		L" where filecode = N'%s' and N'%s' = txt_author)))  ",pGL->m_pEmployee->m_nGroup,nLibID,strFileCode,  pGL->m_pEmployee->m_strUserName); 
	
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (rs.GetRecordCount() > 0)
			
			bSecret = TRUE;
		
		rs.Close();
	}	
	
}

void CFileDB::GetLibCopy(int nLibID, LPCTSTR strFileCode,BOOL &bCopy)
{
	CADORecordset rs(m_pDB);
	
	CGlobal *pGL = CGlobal::GetInstance();
	
	CString strSQL;
	
	strSQL.Format(L"select * from list_permission where  group_id= %d  "
		L" and lib_id = %d and perm_style = 8 and "
		L" (perm_range = 'M' or "
		L" (perm_range = 'S' and exists(select txt_author from text_lib "
		//" where filecode = '%s' and dbo.f_GetNewsUserName(user_name()) = txt_author)))  "),pGL->m_pEmployee->m_nGroup,nLibID,strFileCode ); 
		L" where filecode = N'%s' and N'%s' = txt_author)))  ",pGL->m_pEmployee->m_nGroup,nLibID,strFileCode,pGL->m_pEmployee->m_strUserName ); 
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (rs.GetRecordCount() > 0)
			
			bCopy = TRUE;
		
		rs.Close();
	}	
	
}
BOOL CFileDB::HasTranlatePermity(CADORecordset *rs, int nLibID, int nGroupID)
{
	CString Sql;
	Sql = CADODatabase::PrepareSQL(_T("select 1 from list_permission where group_id=%d and lib_id=%d and perm_style=4"),nGroupID,nLibID);	
	if(rs->Open(Sql,CADORecordset::openQuery))
	{
		if (rs->GetRecordCount() == 0)
		{
			rs->Close();
            return FALSE;
		}		
		int nPermStyle;
		rs->GetFieldValue(0, nPermStyle);
		rs->Close();
		return nPermStyle==1;
	}
	return FALSE;
}


BOOL CFileDB::HasFillTransPerm(CADORecordset *rs, int nLibID, int nGroupID)
{
	CString Sql;
	Sql = CADODatabase::PrepareSQL(_T("select 1 from list_permission where group_id=%d and lib_id=%d and perm_style=3"),nGroupID,nLibID);	
	if(rs->Open(Sql,CADORecordset::openQuery))
	{
		if (rs->GetRecordCount() == 0)
		{
			rs->Close();
            return FALSE;
		}		
		int nPermStyle;
		rs->GetFieldValue(0, nPermStyle);
		rs->Close();
		return nPermStyle==1;
	}
	return FALSE;
}

BOOL CFileDB::GetCountryCode(CADORecordset *pRS)
{
	CString strSql;
	strSql = CADODatabase::PrepareSQL(_T("select CountryName,CountryCode from tblCountry where Active=-1"));	
	if(pRS->Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		if (pRS->GetRecordCount() == 0)
		{
			pRS->Close();
            return FALSE;
		}		
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::LoadSymbol(CADORecordset *pRS, int nCode)
{
	CString strSql;
	strSql = CADODatabase::PrepareSQL(L"select Orginal,Destination,Destination_2,PairIndicator from tblSymbol "
		L" where  Active=-1 and FK_CountryCode = %d" ,nCode );	
	return pRS->Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery);
}

BOOL CFileDB::LoadSpellingOverAll(CADORecordset *pRS, int nCode)
{
	CString strSql;
	strSql = CADODatabase::PrepareSQL(L"select Orginal,Destination from tblSpellingOverAll "
		L" where  Active=-1 and FK_CountryCode = %d" ,nCode );	
	return pRS->Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery);
}
BOOL CFileDB::LoadSpellingByCase(CADORecordset *pRS, int nCode)
{
	CString strSql;
	strSql = CADODatabase::PrepareSQL(L"select Orginal,Destination from tblSpellingByCase "
		L" where  Active=-1 and FK_CountryCode = %d" ,nCode );	
	return pRS->Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery);
}
BOOL CFileDB::LoadSynonymns(CADORecordset *pRS, int nCode)
{
	CString strSql;
	strSql = CADODatabase::PrepareSQL(L"select Synonymns_name,Name_conversation from tblSynonymns s,tblSynonymns_list l "
		L"where s.Active=-1 and l.Active = -1 and s.FK_CountryCode=%d and l.FK_CountryCode=%d and "
		L"s.Synonymns_ID = l.FK_Synonymns_ID " ,nCode,nCode );	
	return pRS->Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery);
}

CFileDB::USER_STATUS CFileDB::CheckUserStatus(LPCTSTR lpszUser, int nGroup)
{
	USER_STATUS sRet = working;
	CADORecordset rs(m_pDB);
	

	CString strSQL;
	strSQL.Format(
		L" select  perm_begin , perm_end, time_begin "
		L" from news_user t1, user_group t2 "
		L" where t1.usercode=t2.usercode and t1.usercode = N'%s' and group_id = %d",
		lpszUser, 
		nGroup);
	
	if (rs.Open(strSQL,CADORecordset::openQuery))
	{
		COleDateTime tPermBegin, tPermEnd, tBegin;
		if (!rs.IsEof())
		{
			rs.GetFieldValue(_T("perm_begin"), tPermBegin);
			rs.GetFieldValue(_T("perm_end"), tPermEnd);
			rs.GetFieldValue(_T("time_begin"), tBegin);
			
			
			if (tBegin.GetStatus() == COleDateTime::null)
				sRet = banned;
			else if (!IsTimeInRange(tPermBegin.GetHour(), tPermBegin.GetMinute(), tPermEnd.GetHour(), tPermEnd.GetMinute()))
			{
				sRet = expired;
			}
		}
		
	}
	return sRet;
}

BOOL CFileDB::GetConfig(LPCTSTR lpszEntry, CString &strValue)
{
	BOOL bRet = FALSE;
	CString strSQL;
	CADORecordset rs(m_pDB);
	
	strSQL.Format(_T("select value from sys_newsroom where entry = '%s'"), lpszEntry);
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0, strValue);
			bRet = TRUE;
		}
		rs.Close();
	}
	
	return bRet;
	

}

LONG CFileDB::GetLibPlacePerm(int nGroupId, int nLibId, LPCTSTR lpszPlace)
{
	LONG nRet = 0;
	CString strSQL, strValue;
	CADORecordset rs(m_pDB);

	strSQL = CADODatabase::PrepareSQL(
		L" select perm_style from trans_permission "
		L" where lib_id=%d and group_id = %d and place = N'%s'", 
		nLibId, nGroupId, lpszPlace);

	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		while (!rs.IsEof())
		{
			rs.GetFieldValue(0, strValue);
			
			strValue.TrimLeft();
			strValue.TrimRight();

			if (strValue == _T("R"))
				nRet |= LPP_READFILE;
			else if (strValue == _T("W"))
				nRet |= LPP_MODIFILE;
			else if (strValue == _T("S"))
				nRet |= LPP_SENDFILE;
			else if (strValue == _T("G"))
				nRet |= LPP_GETFILE;
			else if (strValue == _T("C"))
				nRet |= LPP_SIGNDIRECT;
			else if (strValue == _T("U"))
				nRet |= LPP_SENDCOL;
			else if (strValue == _T("N"))
				nRet |= LPP_ASSIGNPIC;

			
			rs.MoveNext();
		}
		rs.Close();
	}

	return nRet;

}

BOOL CFileDB::GetLibFileInfo(const CString &strFileCode, CString &strPlace, CString &strLibName, CString &strTitle, int &nLib, int &nExclusive)
{
	BOOL bRet = FALSE;
	
	CString strSQL;
	CADORecordset rs(m_pDB);
	
	strSQL = CADODatabase::PrepareSQL(
		L" select exclusive,place_code,t2.lib_id, lib_name,txt_title from text_lib t1,file_place t2, news_lib t3 "
		L" where t1.filecode=t2.filecode and t1.filecode=N'%s' and t2.lib_id = t3.lib_id ",
		strFileCode);
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0, nExclusive);
			rs.GetFieldValue(1, strPlace);
			rs.GetFieldValue(2, nLib);
			rs.GetFieldValue(3, strLibName);
			rs.GetFieldValue(4, strTitle);
			bRet = TRUE;
		}
		rs.Close();
	}
	
	return bRet;
	
}

BOOL CFileDB::InsertPhotoRecord(CCaptionParam *pParm,LPCTSTR lpOper)
{
	CADOCommand cmd(m_pDB);
	return cmd.GetProcExecResult(TRUE,_T("InsertPhotoRecord") , _T("%d%s%d%s%s") ,pParm->bOnCol,pParm->strPicID,
		pParm->nColID,pParm->strColDate,lpOper)==0;
	
}

void CFileDB::TouchFileCurrUser(LPCTSTR lpszFilecode, LPCTSTR lpszCurruser)
{
	CADOCommand cmd(m_pDB);
	cmd.GetProcExecResult(FALSE,_T("TouchFileCurrUser") , _T("%s%s") ,lpszFilecode, lpszCurruser);
}

BOOL CFileDB::GetFileCurrUser(LPCTSTR lpszFilecode, CString &strCurrUser)
{
	BOOL bRet = FALSE;
	
	CString strSQL;
	CADORecordset rs(m_pDB);
	
	strSQL = CADODatabase::PrepareSQL(
		L" select isnull(curruser,'') from text_lib "
		L" where filecode=N'%s' ",
		lpszFilecode);
	
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0, strCurrUser);

			bRet = TRUE;
		}
		rs.Close();
	}
	
	return bRet;
}

int CFileDB::UpdateFilePhotoRelation(CString strFileCode, CString strPicFileID,int nPhotoFlag,int nFileFlag, int nOper, int nRelationID)
{
	CADOCommand cmd(m_pDB);
	return cmd.GetProcExecResult(TRUE,_T("UpdateFilePhotoRelation"),_T("%s%s%d%d%d%d"),strFileCode,strPicFileID,nOper,nRelationID,nPhotoFlag,nFileFlag);
}

BOOL CFileDB::IsLockBySelf(LPCTSTR lpFileCode)
{
	CADORecordset rs(m_pDB);
	BOOL bRet =FALSE;
	CString strSql;
	strSql.Format(_T("select 1 from text_lib where filecode=N'%s' and lockuser=user_name() and txt_state='L' "),lpFileCode);
	rs.GetQueryResult(_T("BOOL"),&bRet,strSql);
	return bRet;
}

BOOL CFileDB::ReadText_Version(CString strFileCode, CString &strVersion, CString &strVersion_Mark, CString &strLastUser, CString &strModHistory, CString &strVersion_Comment, CString &str_Comment, CString &strVersion_Style)
{
	CString strSql;
	CADORecordset rs(m_pDB);
//	strSql=CADODatabase::PrepareSQL(_T("select text_version, text_version_mark, last_moduser from text_lib where filecode=N'%s'"), strFileCode );
	strSql=CADODatabase::PrepareSQL(_T("select text_version, text_version_mark, last_moduser, text_mod_hist, text_version_comment, text_comment, text_version_style from text_lib where filecode=N'%s'"), strFileCode );
	if(rs.Open(strSql,CADORecordset::openQuery) && !rs.IsEOF())
	{
		rs.GetFieldValueContent(0,strVersion);
		rs.GetFieldValue(1,strVersion_Mark);
		rs.GetFieldValue(2,strLastUser);
		rs.GetFieldValue(3,strModHistory);
		rs.GetFieldValue(4,strVersion_Comment);
		rs.GetFieldValue(5,str_Comment);
		rs.GetFieldValue(6,strVersion_Style);

	strVersion.Replace(L"\r\n",L"\r"); 	
		
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::UpdateText_Version(CString strFileCode, CString strVersion, CString strVersion_Mark)
{
	CADOCommand cmd(m_pDB);
//	return cmd.GetProcExecResult(TRUE,_T("UpdateFileVersion"),_T("%n%n%n"),strFileCode,strVersion, strVersion_Mark);
	return cmd.GetProcExecResult(TRUE,_T("UpdateFileVersion"),_T("%n"),strFileCode);
}


//edit by leon 2012-09-04 for word bank
int CFileDB::LoadWordInfo(ARRAY_WORDINFO *arrWordList,CString strLibGroup){
	CString strSql,strName,strLastName,tmp;
	int nTotalCount=0;
	CADORecordset rs(m_pDB);	
	strSql=CADODatabase::PrepareSQL(_T("select Word_Name from word_list where Dic_ID in (select Dic_ID from word_lib where Dic_Name in %s ) order by Word_Name "),strLibGroup);
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		while(!rs.IsEof())
		{		
			rs.GetFieldValue(0,strName);
			BOOL bFind = FALSE;
			for (int i = arrWordList->GetCount()-1; i>=0; i--)
			{
				if (arrWordList->GetAt(i).strName == strName)
				{
					bFind = TRUE;
					break;
				}
			}
			if(bFind)
			{
				rs.MoveNext();
				continue;
			}
			WORDITEM wItem;
			wItem.strName=strName;
			arrWordList->Add(wItem);		
			rs.MoveNext();
			nTotalCount++;
		}	
		rs.Close();
	}
	return nTotalCount;
}

void CFileDB::LoadDictionary(CString szName,CListBox * cList,CString strLibGroup){
	CString strSql,strName;
	CADORecordset rs(m_pDB);
	cList->ResetContent();
	strSql=CADODatabase::PrepareSQL(L"select Word_Name, min(a.Group_ID), max(CONVERT(int, Is_default)) AS Is_default"
		L" from word_list a inner join word_lib b on a.Dic_ID=b.Dic_ID and b.Dic_Name in %s"
		L" where Group_ID in (select Group_ID from word_list where Word_Name=N'%s' and Is_default <> 1)"
		L" and a.Dic_ID in (select Dic_ID from word_list where Word_Name=N'%s') and Word_Name !=N'%s'"
		/*L" and (Group_ID not in (select Group_ID from word_list where not exists (select Word_Name"
		L" where Group_ID in (select Group_ID from word_list where Word_Name=N'%s' and Is_default <> 1)"
		L" and Is_default <> 1))	or not exists (select Word_Name"
		L" where Group_ID in (select Group_ID from word_list where Word_Name=N'%s' and Is_default <> 1)"
		L" and Is_default <> 1))"*/
		L" Group by Word_Name",strLibGroup,szName,szName,szName,szName,szName);

	strSql=CADODatabase::PrepareSQL(L"select Word_Name, Group_ID,CONVERT(int,Is_default) AS Is_default from word_list "
		L"where Dic_ID in (select Dic_ID from word_lib where Dic_Name in %s) "
		L"and Group_ID in (select Group_ID from word_list where Word_Name=N'%s') order by Is_default desc", strLibGroup,szName);
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		int index = 0;
		CList <int,int> defGroup;
		while(!rs.IsEof())
		{		
			rs.GetFieldValueContent(0,strName);
			int iFind = cList->FindString(0, strName);
			int iGroupID;
			if(iFind >= 0 || strName == szName)
			{
				rs.GetFieldValue(2,strName);
				if (_ttoi(strName))
				{
					rs.GetFieldValue(1,iGroupID);
					defGroup.AddTail(iGroupID);
					rs.MoveNext();
					continue;
				}
				rs.MoveNext();
				continue;
			}
			rs.GetFieldValue(1,iGroupID);
			if (defGroup.Find(iGroupID) != NULL)
			{
				rs.MoveNext();
				continue;
			}
			cList->AddString(strName);
			rs.GetFieldValue(2,strName);
			if(_ttoi(strName))
				cList->SetItemData(index,iGroupID);
			else
				cList->SetItemData(index,-1);
			rs.MoveNext();
			index++;
		}	 
		rs.Close();
	}
}


void CFileDB::LoadWordLib(CListBox * cList){
	CString strSql,strName;
	CADORecordset rs(m_pDB);
	int isSelect=0,nCount=0,nSelect;
	cList->ResetContent();
	strSql=CADODatabase::PrepareSQL(_T("select Dic_Name from word_lib"));
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		while(!rs.IsEof())
		{		
			rs.GetFieldValue(0,strName);
			//rs.GetFieldValue(1,isSelect);
			cList->AddString(strName);
			if (isSelect==1)
			{
				nSelect=nCount;
			}
			nCount++;
			rs.MoveNext();
		}	
		//cList->SetSel(nSelect);
		rs.Close();
	}	
}

BOOL CFileDB::GetLayoutData(CADORecordset *pRS)
{
	CString sql;
	sql=CADODatabase::PrepareSQL(_T("select layout_name, layout_code from news_column "));
	if(pRS->Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::LoadLibByUserGroup(CADORecordset*pRS)
{
	CADORecordset rs;
	CString strSql;
	CGlobal *pGlobal=CGlobal::GetInstance();
	strSql.Format(L"select distinct news_lib.lib_id , parent_id , lib_style , lib_name from news_lib "
		L"inner join trans_permission on news_lib.lib_id = trans_permission.lib_id "
		L"where trans_permission.perm_style = N'S' and group_id = %d "
		L" union "
		L"select lib_id , parent_id , lib_style , lib_name from news_lib "
		L"where lib_id in (select parent_id from news_lib inner join trans_permission on "
		L"news_lib.lib_id = trans_permission.lib_id where trans_permission.perm_style = N'S' and group_id = %d)", 
		pGlobal->m_pEmployee->m_nGroup, pGlobal->m_pEmployee->m_nGroup);
	if(rs.Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		rs.Clone(*pRS);
		rs.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::UpdateHist(CString &strFileName)
{
	CADOCommand cmd(m_pDB);
	CADORecordset rs(m_pDB);
	CString sql, strText=L"";
	
	return cmd.GetProcExecResult(TRUE,_T("UpdateHist"),_T("%n"),strFileName);
}

BOOL CFileDB::LoadTextHist(CString strFileName, CString &txt_content, int iVersion)
{
	CADORecordset rs(m_pDB);
	CString strSql;
	BOOL bRet = FALSE;
	strSql.Format(L"Select txt_content from text_hist where filecode= N'%s' and version = %d ", strFileName, iVersion);
	if(rs.Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0, txt_content);
			bRet = TRUE;
		}
		rs.Close();
	}
	return bRet;
}

CString CFileDB::GetLibOrColName(BOOL bLib, int id)
{
	CString strName;
	CADORecordset rs(m_pDB);
	CString strSql;
	BOOL bRet = FALSE;
	if(bLib==TRUE)
		strSql.Format(L"Select * from news_lib where lib_id = %d",id );
	else
		strSql.Format(L"Select * from news_column where lib_id = %d",id );
	if(rs.Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			if(bLib == TRUE)
			{
				rs.GetFieldValue(L"lib_name", strName);
			}else
			{
				rs.GetFieldValue(L"column_code", strName);
			}
			bRet = TRUE;
		}
		rs.Close();
	}
	return strName;
}

CString CFileDB::GetAgencyNameByCode(CString strWiresCode)
{
	CString strName;
	CADORecordset rs(m_pDB);
	CString strSql;
	BOOL bRet = FALSE;
	strSql.Format(L"Select * from news_agency where tele_code = N'%s'",strWiresCode );
	if(rs.Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(L"tele_name", strName);
			bRet = TRUE;
		}
		rs.Close();
	}
	return strName;
}

int CFileDB::GetFileHasModCount(CString strFileCode)
{
	int count = 0;
	CADORecordset rs(m_pDB);
	CString strSql;
	BOOL bRet = FALSE;
	strSql.Format(L"select count(*) from file_record where filecode = '%s' "
		L"and operation = N'修改'" ,strFileCode );
	if(rs.Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0, count);
			bRet = TRUE;
		}
		rs.Close();
	}
	return count;
}

BOOL CFileDB::GetDefaultLibDelPerm(CString strGroupId)
{
	CADORecordset rs(m_pDB);
	CString strSql, str;
	BOOL bRet = FALSE;
	if(!CheckColumnExists(L"CanDel", L"news_group"))
		return bRet;
	strSql.Format(L"select CanDel from news_group where group_id = %s" ,strGroupId );
	if(rs.Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0, str);
			if(str==L"D")
				bRet = TRUE;
		}
		rs.Close();
	}
	return bRet;
}

BOOL CFileDB::CheckColumnExists(CString strColName, CString strTableName)
{
	BOOL bRet = FALSE;
	CADORecordset rs(m_pDB);
	CString strSQL;strSQL.Format(L"if exists(select * from sys.columns "
			L" where Name = N'%s' and Object_ID = Object_ID(N'%s')) "
			L"begin "
			L"	select 1 "
			L"end "
			L"else "
			L"	select 0", strColName, strTableName);
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0,bRet);
		}
	}
	rs.Close();
	return bRet;
}

CString CFileDB::GetFileLastOper(CString strFileCode)
{
	CADORecordset rs(m_pDB);
	CString strSql, str = L"";
	BOOL bRet = FALSE;
		strSql.Format(L"select top 1 operation from file_record where filecode = '%s' "
			L"order by finish_time desc " ,strFileCode );
	if(rs.Open(m_pDB->GetActiveConnection(),strSql,CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0, str);
			if(str==L"D")
				bRet = TRUE;
		}
		rs.Close();
	}
	return str;
}

CString CFileDB::GetPDFAttachedFile(CString str_AgencyCode, CString str_WiresCode)
{
	CString strSql, strRet = L"";
	CADORecordset rs(m_pDB);
	if(CheckTableExists(L"wires_attachs")==FALSE)
		return strRet;
	strSql=CADODatabase::PrepareSQL(L"select attach_src, attach_dest from wires_attachs where tele_name = N'%s' "
		L"and tele_code = N'%s'", str_WiresCode, str_AgencyCode);
	if(rs.Open(strSql, CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
		rs.GetFieldValue(0,strRet);
			if(_waccess(strRet, 0)== -1)
			{
				rs.GetFieldValue(1,strRet);
				if(_waccess(strRet, 0)== -1)
					strRet = L"Not Found";
			}
		}
	}
	return strRet;
}

BOOL CFileDB::CheckTableExists(CString strTableName)
{
	BOOL bRet = FALSE;
	CADORecordset rs(m_pDB);
	CString strSQL;strSQL.Format(L"IF EXISTS (SELECT 1 "
            L"FROM INFORMATION_SCHEMA.TABLES "
		    L"WHERE TABLE_TYPE='BASE TABLE' "
            L"AND TABLE_NAME='%s') "
			L"begin "
			L"	select 1 "
			L"end "
			L"else "
			L"	select 0", strTableName);
	if(rs.Open(strSQL, CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0,bRet);
		}
	}
	rs.Close();
	return bRet;
}

BOOL CFileDB::UpdateWiresFileFlag(const CString &strWiresCode, const CString &strWiresFile, TCHAR tcFlag)
{
	CString sql;
	sql.Format(_T("update %s set tele_flag='%c' where tele_name='%s'"),strWiresCode,tcFlag,strWiresFile);
	return m_pDB->Execute(sql);
}

BOOL CFileDB::GetTextData(CADORecordset *pRS)
{
	CString strSql,strFileCode;
	strSql.Format(L"select distinct text_lib.filecode, CAST(txt_textcontent AS nvarchar(max)) txt_textcontent, CAST(text_version AS nvarchar(max)) text_version, "
		L" CAST(text_version_mark AS nvarchar(max)) text_version_mark, CAST(text_mod_hist AS nvarchar(max)) text_mod_hist, txt_title, "
		L" txt_author, last_modtime, text_lib.position, dbo.f_GetNewsUserName(curruser) username, "
		L" (select MIN(finish_time) from file_record where file_record.filecode=text_lib.filecode) createtime, "
		L" dbo.f_GetLibName(file_place.lib_id) libname, file_place.place_code, '' paper, '' colcode, NULL column_date "
		L" from text_lib, text_info,  file_place"
		L" where (text_lib.position ='L' or text_lib.position='T') and text_lib.filecode = text_info.filecode and file_place.filecode=text_lib.filecode  and txt_state<>'D' "
		L" union select distinct text_lib.filecode, CAST(txt_textcontent AS nvarchar(max)) txt_textcontent, CAST(text_version AS nvarchar(max)) text_version, "
		L" CAST(text_version_mark AS nvarchar(max)) text_version_mark, CAST(text_mod_hist AS nvarchar(max)) text_mod_hist, txt_title, "
		L" txt_author, last_modtime, text_lib.position, dbo.f_GetNewsUserName(curruser) username, "
		L" (select MIN(finish_time) from file_record where file_record.filecode=text_lib.filecode) createtime, "
		L" '' libname, '' place_code, dbo.f_GetPaperCode(column_id) paper, dbo.f_GetColumnCode(column_id) colcode, column_date "
		L" from text_lib, text_info, paper_filelist "
		L" where text_lib.filecode = text_info.filecode and paper_filelist.filecode = text_lib.filecode  and txt_state<>'D' Order by text_lib.filecode");
	if(pRS->Open(strSql,CADORecordset::openQuery))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::CheckAttachment(CString strFileCode)
{
	CString strSql;
	BOOL nRet = FALSE;
	int count = 0;
	CADORecordset rs(m_pDB);
	strSql.Format(L"select count(*) from attachs where filecode = N'%s'", strFileCode);

	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		if (!rs.IsEof())
		{
			rs.GetFieldValue(0,count);
		}
		if(count > 0)
			nRet = TRUE;
	}
	return nRet;
}


int CFileDB::GetValidLibIds(int nGroupId, TCHAR cPerm, CWordArray &waValid)
{
	int nResult = 0;
	CADORecordset rs(m_pDB);
	
	CString strSQL;
// 	strSQL = CADODatabase::PrepareSQL(
// 		_T("select distinct lib_id from news_lib t1, depart_permission t2 where t1.lib_name=t2.lib_name and group_id=%d and perm_style ='%c'"),
// 		nGroupId , cPerm);
//	edit by leon 2012-08-10 for use id instead of name for permission
 
	strSQL = CADODatabase::PrepareSQL(
		_T("select distinct t1.lib_id from news_lib t1, trans_permission t2 where t1.lib_id=t2.lib_id and group_id=%d and perm_style ='%c'"),
		nGroupId , cPerm);
	waValid.RemoveAll();	
	if (rs.Open(strSQL, CADORecordset::openQuery))
	{	
		while(!rs.IsEOF())
		{
			int nId;
			
			rs.GetFieldValue(0, nId);
			waValid.Add(nId);
			TRACE1("ID = %X\r\n",nId);
			
			rs.MoveNext();
		}
		rs.Close();
		
		nResult = waValid.GetSize();
	}
	return nResult;
}

BOOL CFileDB::GetColFileDateAndColIDWithID(CString strFileID, CString &strColId, COleDateTime &dt, CString &strLibId)
{
	CADORecordset rs(m_pDB);
	BOOL nRet = FALSE;
	CString strSQL;
	strSQL = CADODatabase::PrepareSQL(L"select * from paper_filelist a inner join news_column b on a.column_id = b.column_id "
		L" where filecode = N'%s'", strFileID);
	if (rs.Open(strSQL, CADORecordset::openQuery))
	{
		rs.GetFieldValue(L"column_id", strColId);
		rs.GetFieldValue(L"column_date", dt);
		rs.GetFieldValue(L"lib_id", strLibId);
		nRet = TRUE;
	}
	rs.Close();
	return nRet;
}

BOOL CFileDB::GetFileDateAndLibIDWithID(CString strFileID, CString &strLibId, CString &strPlaceCode)
{
	CADORecordset rs(m_pDB);
	BOOL nRet = FALSE;
	CString strSQL;
	strSQL = CADODatabase::PrepareSQL(L"select * from text_lib a inner join file_place b on a.filecode = b.filecode "
		L" where a.filecode = N'%s'", strFileID);
	if (rs.Open(strSQL, CADORecordset::openQuery))
	{
		rs.GetFieldValue(L"lib_id", strLibId);
		rs.GetFieldValue(L"place_code", strPlaceCode);
		nRet = TRUE;
	}
	rs.Close();
	return nRet;
}

int CFileDB::GetLibIdStoryCodeFromStoryID(int i_StoryID, CString& strCode)
{
	CString str;
	CADORecordset rs(m_pDB);
	str=CADODatabase::PrepareSQL(_T(" select lib_id, story_code from t_story where story_id = %d"), i_StoryID);
	int nID = -1;
	if(rs.Open(str,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			rs.GetFieldValue(0,nID);
			rs.GetFieldValue(1,strCode);
		}
	}
	return nID;
}

BOOL CFileDB::GetLibCodeFromLibId(int nLibID, CString & strLibCode)
{
	CString strSql;
	BOOL bRet = FALSE;
	strSql=CADODatabase::PrepareSQL(_T("select lib_code from v_tolibrary_get_lib_code ")
		_T("where lib_id = %d "), nLibID);
	CADORecordset rs(m_pDB);
	if (rs.Open(strSql, CADORecordset::openQuery))
	{	
		if(rs.GetRecordCount()>0)
		{	
			rs.GetFieldValue(0,strLibCode);
			bRet = TRUE;
		}
		rs.Close();
	}
	return bRet;
}

int CFileDB::InsertStoryItem(int nStoryID, LPCTSTR szType, LPCTSTR szPath, LPCTSTR szCaption, LPCTSTR szTitle, LPCTSTR szDesc, 
	double dHeight, int nHeightUnit, double dWidth, int nWidthUnit,  LPCTSTR szLength)
{
	
	CADOCommand cmd(m_pDB);
	return cmd.GetProcExecResult(TRUE,_T("InsertStoryItem"),_T("%d%s%s%s%s%s%f%d%f%d%s"),nStoryID, szType, szPath, szCaption, szTitle, 
		szDesc, dHeight, nHeightUnit, dWidth, nWidthUnit, szLength);

	
}

BOOL CFileDB::InsertStoryItemTag(CString strFileName, int nStoryID, int nPublishTypeID, int iStoryItemID, CString strNewFileCode)
{
	
	CString strSQL = m_pDB->PrepareSQL(L"exec InsertStoryItemTagText '%s',%d,%d,%d,'%s'", 
		strFileName, nStoryID, nPublishTypeID, iStoryItemID, strNewFileCode);
//	CString strSQL = m_pDB->PrepareSQL(L"exec InsertStoryItemTag 93,5,4");
	return m_pDB->Execute(strSQL);

/*	if (b_result)
		AfxMessageBox(L"OK");
	else
		AfxMessageBox(L"Fail");

	return b_result;*/
//	CADOCommand cmd(m_pDB);
//	return cmd.GetProcExecResult(TRUE,_T("InsertStoryItemTag"),_T("%d%d%d"),nPhotoID, nStoryID, nPublishTypeID);
	
}


void CFileDB::GetStoryItemPath(CString& strSIRootPath, CString& strSIVersionPath, CString& strSIPhotoPath, CString& strSIVideoPath, 
		CString& strSISubTitlePath, CString& strSIAudioPath, CString& strSIScreenCapPath, CString& strSITextPath, CString& strSIXMLPath,
		int nLibID)
{
	CString str;
	CADORecordset rs(m_pDB);
	str=CADODatabase::PrepareSQL(_T(" select root_path, version_path, photo_path, video_path, audio_path, text_path, screenCap_path, xml_path, subtitle_path from t_story_item_path where lib_id=%d"), nLibID);

	if(rs.Open(str,CADORecordset::openQuery))
	{
		if(rs.GetRecordCount()>0)
		{
			rs.GetFieldValue(0,str);
			strSIRootPath = str;
			rs.GetFieldValue(1,str);
			strSIVersionPath = str;
			rs.GetFieldValue(2,str);
			strSIPhotoPath = str;
			rs.GetFieldValue(3,str);
			strSIVideoPath = str;
			rs.GetFieldValue(4,str);
			strSIAudioPath = str;
			rs.GetFieldValue(5,str);
			strSITextPath = str;
			rs.GetFieldValue(6,str);
			strSIScreenCapPath = str;
			rs.GetFieldValue(7,str);
			strSIXMLPath = str;
			rs.GetFieldValue(8,str);
			strSISubTitlePath = str;
		}
	}
	
}

BOOL CFileDB::IsStoryItemTagExist(CString strFileCode, int i_StoryID)
{
	CString strSql;
	BOOL bRet = FALSE;
	strSql=CADODatabase::PrepareSQL(_T("select filecode from text_story_tag  where flag =0 and story_id =%d and parent_filecode = '%s' "), i_StoryID, strFileCode);
	CADORecordset rs(m_pDB);
	if (rs.Open(strSql, CADORecordset::openQuery))
	{	
		if(rs.GetRecordCount()>0)
		{	
			bRet = TRUE;
		}
		rs.Close();
	}
	return bRet;
}

BOOL CFileDB::GetStoryList(CADORecordset &pRS, CString strDate, int iLibID)
{
	CString str;

	if (iLibID > 0)
		str.Format(L"select * from t_story where Status not in (N'D', N'P', N'E') and lib_id = %d and story_date = N'%s'", iLibID, strDate);
	else
		str.Format(L"select * from t_story where Status not in (N'D', N'P', N'E') and story_date = N'%s'", strDate);
	
	CADORecordset rs(m_pDB);
	if(rs.Open(str,CADORecordset::openQuery))
	{
		rs.Clone(pRS);
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::GetStoryInfo(CString strStoryCode, CString &strPath, CString &strVerPath, CString &strStoryID)
{
	CString str, strTempPath, strTempVerPath, strDeptCode, strDate;
	COleDateTime dt;
	if(strStoryCode == L"")
		str.Format(L"select * from t_story a inner join t_story_item_path b on a.lib_id = b.lib_id "
			L"inner join news_lib c on a.lib_id = c.lib_id "
			L"where a.story_id = %s", strStoryID);
	else
		str.Format(L"select * from t_story a inner join t_story_item_path b on a.lib_id = b.lib_id "
			L"inner join news_lib c on a.lib_id = c.lib_id "
			L"where a.story_code = N'%s'", strStoryCode);
	CADORecordset rs(m_pDB);
	if(rs.Open(str,CADORecordset::openQuery))
	{
		rs.GetFieldValue(L"story_id", strStoryID);
		rs.GetFieldValue(L"root_path", strTempPath);
		rs.GetFieldValue(L"version_path", strTempVerPath);
		rs.GetFieldValue(L"story_date", dt);
		rs.GetFieldValue(L"lib_code",strDeptCode);
		if(strStoryCode == L"")
			rs.GetFieldValue(L"story_code",strStoryCode);
		strDate.Format(L"%04d%02d%02d", dt.GetYear(), dt.GetMonth(), dt.GetDay());
		strPath = strTempPath + L"\\"+strDate+L"\\"+strDeptCode+"\\"+strStoryCode+L"\\Text\\";
		strVerPath = strTempPath + L"\\"+strDate+L"\\"+strDeptCode+"\\"+strStoryCode+L"\\Text\\"+strTempVerPath+L"\\";
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::InsertStoryItem(CString strStoryID, CString strPath, CString strVerPath, CString strFileName)
{
	CADOCommand cmd(m_pDB);
	CADORecordset rs(m_pDB);
	CString strSql, strText=L"";
	
	int iItemID = cmd.GetProcExecResult(TRUE,_T("InsertStoryItem"),_T("%d%s%s%s%s%s%d%d%d%d%s"),_wtoi(strStoryID), L"T", strPath, L"", L"",L"", 0,0,0,0,0);
	strSql = m_pDB->PrepareSQL(L"exec InsertStoryItemVersion %d,%d,%d,N'%s',N'%s'", iItemID, 1, 1, strVerPath, strVerPath);
	m_pDB->Execute(strSql);
	
	strSql.Format(L"update t_story_item set editroom_id = N'%s' where story_item_id = %d", strFileName, iItemID);
	m_pDB->Execute(strSql);
	return true;
}


int CFileDB::InsertStory(CString strStoryCode, CString strStoryName, COleDateTime *dt, int iCategoryID, int iLibID)
{
	CADOCommand cmd(m_pDB);
	CString strDT;
	strDT.Format(L"%04d-%02d-%02d", dt->GetYear(), dt->GetMonth(), dt->GetDay());
	int iStoryID = cmd.GetProcExecResult(TRUE,_T("InsertStory"),_T("%s%s%s%d%d"),strStoryCode, strStoryName, strDT, iCategoryID, iLibID);
	return iStoryID;
}

BOOL CFileDB::GetNewLibList(CADORecordset &rs, int iGroupId)
{
	CADORecordset rss(m_pDB);
	CString strSQL, strRet = L"";
	strSQL = CADODatabase::PrepareSQL(L" select * from news_lib where lib_id in "
		L"(select lib_id from publish_permission where group_id =%d and perm_style = 'AI')", iGroupId);
	if (rss.Open(strSQL, CADORecordset::openQuery))
	{
		rss.Clone(rs);
	}else
		return false;
	rss.Close();
	return true;
}

BOOL CFileDB::GetStoryCategoryList(CADORecordset &rs)
{
	CADORecordset rss(m_pDB);
	CString strSQL, strRet = L"";
	strSQL = CADODatabase::PrepareSQL(L"select * from t_story_category where flag = 0");
	if (rss.Open(strSQL, CADORecordset::openQuery))
	{
		rss.Clone(rs);
	}else
		return false;
	rss.Close();
	return true;
}

BOOL CFileDB::IsStoryCodeDuplicate(CString strStoryCode)
{
	CADORecordset rss(m_pDB);
	CString strSQL, strRet = L"";
	int iRet = 0;
	strSQL = CADODatabase::PrepareSQL(L"select count(*) from t_story where story_code = N'%s'", strStoryCode);
	if (rss.Open(strSQL, CADORecordset::openQuery))
	{
		rss.GetFieldValue(0, iRet);
	}
	return iRet;
}

BOOL CFileDB::GetStoryCode(CString &strRet)
{
	CADOCommand adoCmd(m_pDB);
	return adoCmd.GetProcExecRetStr(strRet, _T("GetStoryCode"), L"");
}

BOOL CFileDB::CheckLibStoryPathExist(int iLibID)
{
	CString sql;
	BOOL nRet = FALSE;
	sql.Format(_T("select * from t_story_item_path where lib_id = %d"),iLibID);
	CADORecordset rs(m_pDB);
	if(rs.Open(m_pDB->GetActiveConnection(),sql,CADORecordset::openQuery))
	{
		CString str;
		for(int i=2;i<10;i++)
		{
			rs.GetFieldValue(i, str);
			if(str.GetLength()<=0)
			{
				nRet = FALSE;
				break;
			}
			nRet = TRUE;
		}

	}
	return nRet;
}

BOOL CFileDB::GetRecordFromSQL(CString strSql, CADORecordset &rs)
{
	CADORecordset pRS(m_pDB);
	if(pRS.Open(strSql, CADORecordset::openQuery))
	{
		pRS.Clone(rs);
		return TRUE;
	}
	return FALSE;
}

BOOL CFileDB::InsertStoryCategoryRelation(int nStoryID, CString strCategoryID)
{
	CString strSQL = m_pDB->PrepareSQL(L"exec InsertStoryCategoryRelation %d, '%s'", nStoryID ,strCategoryID);
	return m_pDB->Execute(strSQL);
}


