// FileDB.h: interface for the CFileDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDB_H__7A6D7D58_E09B_42BE_BDC8_D28ABCBF600C__INCLUDED_)
#define AFX_FILEDB_H__7A6D7D58_E09B_42BE_BDC8_D28ABCBF600C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include "filemanage.h"
#include "WordCheckDlg.h"

struct FILEINFO{
	CString strFilecode;
	CString strTxtTitle;
	short nExclusive;
	CString strUrgency;
	BOOL bLock;
	CString strTxtAuthor;
	short nSecret;
	CString strTxtStyle;
	CString strPosBox;
};

#define		FL_NORMAL				0
#define		FL_LOCK					1
#define		FL_URGENCY				2
#define		FL_DELETE				4
#define		FL_TRANSLATE			4
#define		FL_SIGN					4
#define		FL_SECRET				8
#define		FL_HASATTACH			16
#define		FL_EXCLUSIVE			32

// 稿庫權限
#define		LPP_READFILE			1
#define		LPP_MODIFILE			2
#define		LPP_GETFILE				4
#define		LPP_SENDFILE			8
#define		LPP_SENDCOL				16
#define		LPP_SIGNDIRECT			32
#define		LPP_ASSIGNPIC			64


//edit by leon 2012-09-04 
#define GROUP_LIMIT 10

class CDoFileMgr;
class CFileDB  
{
public:
	enum USER_STATUS
	{
		working,  // 正在使用
		banned,	  // 被踢
		expired   // 過期
	};

	CADODatabase *m_pDB;
public:
	CFileDB();
	virtual ~CFileDB();
public:
	BOOL IsLockBySelf(LPCTSTR lpFileCode);
	int  UpdateFilePhotoRelation(CString strFileCode, CString strPicFileID,int nPhotoFlag,int nFileFlag, int nOper, int nRelationID);
	BOOL GetFileCurrUser(LPCTSTR lpszFilecode, CString &strCurrUser );
	void TouchFileCurrUser(LPCTSTR lpszFilecode, LPCTSTR lpszCurruser);
	BOOL InsertPhotoRecord(CCaptionParam *pParm,LPCTSTR lpOper);
	BOOL GetLibFileInfo(const CString& strFileCode, CString &strPlace, CString &strLibName, CString &strTitle, int &nLib, int &nExclusive);
	LONG GetLibPlacePerm(int nGroupId, int nLibId, LPCTSTR lpszPlace);
	BOOL GetConfig(LPCTSTR lpszEntry, CString &strValue);
	USER_STATUS CheckUserStatus(LPCTSTR lpszUser, int nGroup);
	BOOL LoadSynonymns(CADORecordset *pRS, int nCode);
	BOOL LoadSpellingByCase(CADORecordset *pRS, int nCode);
	BOOL LoadSpellingOverAll(CADORecordset *pRS,int nCode);
	BOOL LoadSymbol(CADORecordset *pRS,int nCode);
	BOOL GetCountryCode(CADORecordset *pRS);
	BOOL GetColFileStatus(CString strFileCode, int nColID, CString strColDate, short &nRet);
	BOOL HasFillTransPerm(CADORecordset *rs, int nLibID, int nGroupID);
	BOOL HasTranlatePermity(CADORecordset *rs, int nLibID, int nGroupID);
	void GetLibCopy(int nLibID, LPCTSTR strFileCode,BOOL &bCopy);
	void GetLibSecret(int nLibID, LPCTSTR strFileCode,BOOL &bSecret);
	void GetLibUrgency(int nLibID,LPCTSTR strFileCode,BOOL &bUrgency);
	void GetPaperCopyList(CADORecordset *rs,LPCTSTR strFilecode);
	void ClearFile(LPCTSTR filecode);
	void RestoreFile(LPCTSTR filecode);
	void GetDeletedFile(CADORecordset *rs);
	int GetLinkedDMFI(LPCTSTR lpszFileCode);
	BOOL DeleteFile(LPCTSTR lpFileCode,int &nRet);
	BOOL HasRelationToColumn(CString lpFileCode,int &nRet);// Added by Ronald Saliman on 20120904
	BOOL GetLinkedFilecode(int nDeptFileInfoId, CString& strFileCode);
	BOOL LoadDeptFileInfo(CADORecordset *pRS, LPCTSTR lpszDate, int nLibID, BOOL bHaveDMPerm, BOOL bFilter);
	int GetDMPrivilege(int nGroupId, int nLib);
	int GetDMValidLibIds(int nGroupId, CWordArray &waValid);
	void LinkToDM(int nDeptFileInfoID, LPCTSTR lpszFilecode,LPCTSTR lpOper);
	BOOL CanUnlink(LPCTSTR lpszFileCode, int nGroupId);
	BOOL IsLinkedByDM(LPCTSTR lpszFileCode);
	BOOL GetPicInfo(LPCTSTR lpFileID,short nPos,CString &strInfo);
	BOOL GetRelatePic(CString strFileCode, CADORecordset *pRS);
	BOOL GetFileText(const CString &strFileCode, CString &strText);
	BOOL HasColPerm(int nGroupID,int nColID,TCHAR tcPerm);
	//edit by leon 2012-10-03 for migrating from ANSI to Unicode
	//BOOL UpdateLib(const CString &strFileCode,LPCTSR lpTitle,BOOL bUpdateTitle,double dbWords,int nGroup,const CString &strOper,LPCTSTR lpLastModTime, CString strTextVersion, CString strTextVersion_Mark, CString strModHistory, bool bUpdateModHistory, CString strTextVersion_Comment, CString str_Comment, CString strTextVersion_Style);
	BOOL UpdateLib(const CString &strFileCode,LPWSTR lpTitle,BOOL bUpdateTitle,double dbWords,int nGroup,const CString &strOper,LPCTSTR lpLastModTime, CString strTextVersion, CString strTextVersion_Mark, CString strModHistory, bool bUpdateModHistory, CString strTextVersion_Comment, CString str_Comment, CString strTextVersion_Style, int &RetCode);
	//end by leon 2012-10-03
	
	BOOL SearchFile(CADORecordset *pRS,const CString &strSql);
	BOOL LoadLibUser(CADORecordset *pRS,int nLibID);
	BOOL LoadColByPaper(CADORecordset *pRS,const CString &strPaper,int nGroup,TCHAR tcType);
	BOOL LoadNewspaper(CComboBox *pComb,int nGroup,TCHAR tcType);
	BOOL DeleteAttach(const CString & strFileCode, const CString & strAttachName);
	BOOL InsertAttach(CAttach *pAttach);
	BOOL ExistAttach(const CString &strFileCode,const CString &strAttachName,CString &strUploadDate);
	BOOL GetAttachTotalSize(const CString &strFileCode, int &nSize);

	BOOL LoadAttach(CADORecordset *pRS,CString &strFileCode);
	BOOL GetFileRecord(CADORecordset *pRS,CString &strFileCode);
	BOOL GetFinalUser(const CString &strFileCode,CString &strUserCode);
	BOOL RollbackFile(CString &strFileCode,int nGroupID,CString &strOper);
	BOOL IsFileExist(CString &strFileCode,BOOL &bExist);
	BOOL GetFileLastModTime(CString &strDate,const CString &strFileCode);
	BOOL GetLayoutInfo(const CString &strFileCode,CString &strLayoutCode,COleDateTime &dtLayoutDate,CString &strLayoutFile);
	BOOL IsFinishComposed(CString &strFileCode);
	BOOL LoadPaperFile(CADORecordset *pRS,CString &strDate,int nColumnID);
	BOOL ListColumnDate(CADORecordset *pRS,int nColumnID);
	BOOL LoadColumnPerm(int nGroupID,int nColumnID,CADORecordset *pRS);
	BOOL LoadColumn(CADORecordset *pRS, int nLib,int nGroupID);
	BOOL LoadFileInfo(CString &strFileCode , CString &strTitle , double *dbWord, BOOL *bSingle , BOOL *bSecret , BOOL *bUrgency , BOOL *bLock ,CString &strAuthor , CString &strStyle, CString &strPostBox, CString &strUser);
	BOOL IsSecret(CString &strFileCOde,CString &strUserCode);
	BOOL IsCopyFile(LPCTSTR strFileCode);
	BOOL FileHaveSource(LPCTSTR strFileCode);
	BOOL LoadColUser(CADORecordset *pRS,int nColumnID,int nLib);
	BOOL LoadColumn(CADORecordset *pRS,int nLib,int nGroupID,TCHAR *szPerm);
	BOOL UnlockFile(CString strFileCode,BOOL bSetCurruserNull=TRUE);
	BOOL LockFile(CString strFileCode);
	BOOL GetUserCode(CString strUserName,CString &strUserCode);
	//20081104 Liubc
	BOOL GetUserName(const CString &strUserCode, CString &strUserName);
	BOOL GetLibUser(int nLib, CString strPlce,CADORecordset *pRS);
	BOOL LoadFilePerm(int nGroupID,CADORecordset *pRS);
	BOOL TransFile(CString strFileCode,int nDesLibID,CString strDesPlace, CString strCopyStyle,int nGroupID,CString strUserCode,short nSecret,CString *strNote,CString strOper,int nAction,short &nRet);
	BOOL GetDefaultLib(int &nLib,CString &strPlace,int nGroup);
	BOOL GetLibFileStatus(CString strFileCode,int nLib,CString strPlace,short &nRet);
	BOOL UpdateTitles(LPCTSTR  szFileCode, LPCTSTR szTitle1, LPCTSTR szTitle2, LPCTSTR szTitle3, LPCTSTR szTitle4,LPCTSTR szModifyTime);
	BOOL IsDefaultLib(int nLib, CString strPlace,int nGroupID);
	BOOL IsFileLocked(CString strFileCode, CString &strLockUser);
	BOOL ReadFile(CString strDestPath,CString strExt,CString strFileCode);
	BOOL InitMissingTxtFile(CString strDestPath, CString strFileCode);	
	
	BOOL UpdateText(CString strLocalFile,CString strExt,CString strFileCode);
	short UpdateFileInfo(FILEINFO *pFileInfo);
	BOOL IsDefPrivLib(int nLib,CString strPlace,int nGroupID,BOOL &bPriv);
	short InsertLib(CString &strFileName , CString &strSrcFile,double lWord , CString &strTitle ,int nGroupID , CString &strOperation,CString &strFMT, CString strTextVersion, CString strTextVersion_Comment, CString str_Comment, CString strTextVersion_Style);
	short InsertLib(CString &strFileName, CString &strSrcFile, double lWord, CString &strTitle, CString &strSpecify_usercode
		, int nSpecify_lib_id, CString &strSpecify_placecode, CString &strOperation, CString &strFMT);

	BOOL GetNewFileCode(CString &strFileCode);
	BOOL LoadLibFile(CADORecordset *pRS,  CDoFileMgr *pMgr);
	BOOL LoadPlace(int nType,CADORecordset*pRS,int nLibID,int nGroupID);
	BOOL LoadLibAll(CADORecordset *pRS);
	BOOL GetLastOnColPhoto(const CString &strPhotoName, int &iPhotoID, int &iColID, COleDateTime &odtColDate);
	void SetDB(CADODatabase *pDB);

	BOOL ReadText_Version(CString strFileCode, CString &strVersion, CString &strVersion_Mark, CString &strLastUser, CString &strModHistory, CString &strVersion_Comment, CString &str_Comment, CString &strVersion_Style);
	BOOL UpdateText_Version(CString strFileCode, CString strVersion, CString strVersion_Mark);


	//edit by leon 2012-09-04 for word bank
	int LoadWordInfo(ARRAY_WORDINFO * arrWordList,CString strLibGroup);
	void LoadDictionary(CString szName,CListBox * cList,CString strLibGroup);
	void LoadWordLib(CListBox * cList);

	BOOL GetLayoutData(CADORecordset *pRS);
	BOOL LoadLibByUserGroup(CADORecordset*pRS);

	BOOL UpdateHist(CString &strFileName);
	BOOL LoadTextHist(CString strFileName, CString &strText, int iVersion);
	BOOL UpdateSimpCapHist(CString strFileName, CString strText);
	CString GetLibOrColName(BOOL bLib, int id);
	CString GetAgencyNameByCode(CString strWiresCode);
	int GetFileHasModCount(CString strFileCode);
	BOOL GetDefaultLibDelPerm(CString strGroupId);
	BOOL CheckColumnExists(CString strColName, CString strTableName);
	CString GetFileLastOper(CString strFileCode);
	CString GetPDFAttachedFile(CString str_AgencyCode, CString str_WiresCode);
	BOOL CheckTableExists(CString strTableName);
	BOOL UpdateWiresFileFlag(const CString &strWiresCode, const CString &strWiresFile, TCHAR tcFlag);
	
	BOOL GetTextData(CADORecordset *pRS);
	BOOL CFileDB::CheckAttachment(CString strFileCode);
	int GetValidLibIds(int nGroupId, TCHAR cPerm, CWordArray &waValid);
	BOOL GetColFileDateAndColIDWithID(CString strFileID, CString &strColId, COleDateTime &dt, CString &strLibId);
	BOOL CFileDB::GetFileDateAndLibIDWithID(CString strFileID, CString &strLibId, CString &strPlaceCode);
	int GetLibIdStoryCodeFromStoryID(int i_StoryID, CString& strCode);
	BOOL GetLibCodeFromLibId(int nLibID, CString & strLibCode);
	
	int InsertStoryItem(int nStoryID, LPCTSTR szType, LPCTSTR szPath, LPCTSTR szCaption, LPCTSTR szTitle, LPCTSTR szDesc, 
	double dHeight, int nHeightUnit, double dWidth, int nWidthUnit,  LPCTSTR szLength);

	BOOL InsertStoryItemTag(CString strFileName, int nStoryID, int nPublishTypeID, int iStoryItemID, CString strNewFileCode);

	void GetStoryItemPath(CString& strSIRootPath, CString& strSIVersionPath, CString& strSIPhotoPath, CString& strSIVideoPath, 
		CString& strSISubTitlePath, CString& strSIAudioPath, CString& strSIScreenCapPath, CString& strSITextPath, CString& strSIXMLPath,
		int nLibID);

	BOOL IsStoryItemTagExist(CString strFileCode, int i_StoryID);
	BOOL GetStoryCategoryList(CADORecordset &rs);
	BOOL GetNewLibList(CADORecordset &rs, int iGroupId);
	BOOL GetStoryList(CADORecordset &pRS, CString strDate, int iLibID);
	BOOL GetStoryInfo(CString strStoryCode, CString &strPath, CString &strVerPath, CString &strStoryID);
	BOOL InsertStoryItem(CString strStoryID, CString strPath, CString strVerPath, CString strFileName);
	int InsertStory(CString strStoryCode, CString strStoryName, COleDateTime *dt, int iCategoryID, int iLibID);
	BOOL IsStoryCodeDuplicate(CString strStoryCode);
	BOOL GetStoryCode(CString &strRet);
	BOOL CheckLibStoryPathExist(int iLibID);
	BOOL GetRecordFromSQL(CString strSql, CADORecordset &rs);
	BOOL InsertStoryCategoryRelation(int nStoryID, CString strCategoryID);


};

#endif // !defined(AFX_FILEDB_H__7A6D7D58_E09B_42BE_BDC8_D28ABCBF600C__INCLUDED_)
