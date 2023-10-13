// DoColMgr.h: interface for the CDoColMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCOLMGR_H__4EBFFB7C_0DE5_44B8_BFEF_4656A2886CF2__INCLUDED_)
#define AFX_DOCOLMGR_H__4EBFFB7C_0DE5_44B8_BFEF_4656A2886CF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\fzado\pathmgr.h"
#include "filemgrbase.h"
#include "DoColTreeView.h"

struct CColPerm
{
	BOOL m_bNeedRefresh;
	BOOL m_bEditCol;//編輯
	BOOL m_bChangeCol;//調版
	BOOL m_bSign;//簽發
	BOOL m_bRollBack;//回調簽發
	BOOL m_bBack;//退稿
};
class CDoColMgr :public CFileMgrBase
{
public:
	CDoColTreeView * treeView;
	BOOL OnBeforeOpenFile(FILEPARAM &fileParam);
	BOOL CanCurFileModify();
	BOOL RollbackFile(CString &strMsg);
	virtual BOOL GetCurFileParam(int nItem=-1);
	void LoadColumn(CComboBox *pComb);
	CDoColMgr();
	virtual ~CDoColMgr();
public:
	BOOL GetCurFileParam(int nItem, FILEPARAM &fileParam);
	void SetTotalCount(int nFileCount, double dblWords);
	void GetFilePos(FILEPARAM &fileParam);
	CADORecordset * rs;
	virtual void Refresh();
	virtual void SetColumnIndex();
	CString m_strCol;
	CString m_strPaper;
	void GetPaperCol(CString &strSource,CString &strPaper,CString &strCol);
	BOOL TransPaper();
	BOOL CanRollBack();
	BOOL CanModify();
	virtual BOOL CanManageAttach();
	BOOL CanBack();
	BOOL CanSign();
	BOOL CanTransPaper();
	BOOL IsFileLocked(CString &strFileCode);
	void SignFile();
	void BackFile();
	BOOL ListColumnDate(CComboBox *pBox);
	void LoadPaperFile(BOOL bSelFile=FALSE);
	CString m_strDate;//當前選擇的日期
	BOOL LoadColPerm();
	CColPerm m_ColPerm;
	int m_nColumnID;
	CPathMgr *m_pPathMgr;
	EN_ConnectType m_enConnType;
	int iPreviousPos;
	CString iPrevioustxtId;
	void AddToStory();
};

#endif // !defined(AFX_DOCOLMGR_H__4EBFFB7C_0DE5_44B8_BFEF_4656A2886CF2__INCLUDED_)
