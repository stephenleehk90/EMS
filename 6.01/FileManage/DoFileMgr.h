// DoFileMgr.h: interface for the CDoFileMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOFILEMGR_H__7EA8C93C_C99A_429B_9542_BEF81DB580D6__INCLUDED_)
#define AFX_DOFILEMGR_H__7EA8C93C_C99A_429B_9542_BEF81DB580D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "filemgrbase.h"

enum EN_DATETYPE{CURDAY=0,THREEDAY,ONEWEEK,ALL};
enum EN_QUERYMOD{BYPLACE,BYFILENAME};
class CDoFileMgr   :public CFileMgrBase
{
public:
	CString m_strPlaceName;//稿庫名稱
	EN_DATETYPE m_enDateType;//日期類型
	EN_QUERYMOD m_enQueryMod;//查詢方式
	CString m_strSearchFile;//要查詢的稿件代碼
	
public:
	BOOL OnBeforeOpenFile(FILEPARAM &fileParam);
	void SetTotalCount(int nFileCount, double dblWords);
	void Refresh();
	BOOL SendToCol();
	BOOL CanCurFileModify();
	void GetFilePos(FILEPARAM &fileParam);
	void PrintView(LPCTSTR lpszGroup);
	CADORecordset rs;
	void UnlinkDM(LPCTSTR lpszFileCode);
	void LinkToDM(LPCTSTR lpszFileCode);
	BOOL DeleteFile();
	BOOL SignDirect();
	void Translate();
	virtual void SetColumnIndex();
	BOOL LoadFileInfo(CString &strFileCode , CString &strTitle , double *dbWord, BOOL *bSingle , BOOL *bSecret ,  BOOL *bUrgency, BOOL *bLock ,CString &strAuthor , CString &strStyle, CString &strPostBox, CString &strUser);
	BOOL LoadColUser(CComboBox *pBox, int nColumnID,int nLib);
	BOOL LoadColumn(CListCtrl *pCtrl, int nLib, TCHAR *szPerm);
	BOOL TransFile(CString strFileCode,int nDesLibID,CString strDesPlace, CString strCopyStyle,int nGroupID,CString strUserCode,short nSecret,CString *strNote,CString strOper,int nAction, short &nRet);
	BOOL GetUserCode(CString strUserName,CString &strUserCode);
	BOOL LoadPlace(int nType,int nLibID,CComboBox *pBox);
	BOOL GetLibUser(int nLib,CString strPlce,CComboBox *pBox);
	void GetFile();
	//針對當前操作的稿件定義的權限
	BOOL m_bDelete;			// 能否刪除
	BOOL m_bSendCol;		// 送往版面暫存庫
	BOOL m_bGetFile;		// 能否取稿
	BOOL m_bOwnFile;		// 是否自己的稿件
	BOOL m_bFillTrans;      // 能否送譯
	BOOL m_bReadTrans;      // 能否瀏覽譯稿單
	BOOL m_bTranslate;		// 能否送譯
	BOOL m_bSendFile;		// 能否傳遞
	BOOL m_bSignDirect;		// 能否直接傳遞
	BOOL m_bDefaultLib;		// 是否默認稿庫
	BOOL m_bCanUnlink;		// 是否有權限解除與部門分組菜單的關聯
	BOOL m_bCanlink;		// 是否有權限與部門分組菜單建立關聯
	BOOL m_bLinked;			// 是否已被部門分組菜單關聯

	LONG m_nLibPlacePerm;   // 稿庫權限
	
	void CheckLibFilePermission( ) ;
	virtual BOOL GetCurFileParam(int nItem=-1);
	void SetupDateFilter(CComboBox *pComb);
	BOOL LoadLibFile(BOOL bSelFile=FALSE);
	void SetupPlace(CComboBox*pComb);
	void InsertSubItem(CADORecordset *pRS,CTreeCtrl *pTreeCtrl,HTREEITEM hParent,WORD wParent, int nLibID,HTREEITEM &hSelItem);
	BOOL SetupLibTree(CTreeCtrl*pTreeCtrl , WORD wParent,int nLibID );
	void WriteDefaultSetting();
	void LoadDefaultSetting();
	BOOL GetEditDelPerm();
	void AddToStory();
	CDoFileMgr();
	virtual ~CDoFileMgr();

};

#endif // !defined(AFX_DOFILEMGR_H__7EA8C93C_C99A_429B_9542_BEF81DB580D6__INCLUDED_)
