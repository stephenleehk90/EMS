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
	CString m_strPlaceName;//�Z�w�W��
	EN_DATETYPE m_enDateType;//�������
	EN_QUERYMOD m_enQueryMod;//�d�ߤ覡
	CString m_strSearchFile;//�n�d�ߪ��Z��N�X
	
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
	//�w���e�ާ@���Z��w�q���v��
	BOOL m_bDelete;			// ��_�R��
	BOOL m_bSendCol;		// �e�������Ȧs�w
	BOOL m_bGetFile;		// ��_���Z
	BOOL m_bOwnFile;		// �O�_�ۤv���Z��
	BOOL m_bFillTrans;      // ��_�eĶ
	BOOL m_bReadTrans;      // ��_�s��Ķ�Z��
	BOOL m_bTranslate;		// ��_�eĶ
	BOOL m_bSendFile;		// ��_�ǻ�
	BOOL m_bSignDirect;		// ��_�����ǻ�
	BOOL m_bDefaultLib;		// �O�_�q�{�Z�w
	BOOL m_bCanUnlink;		// �O�_���v���Ѱ��P�������յ�檺���p
	BOOL m_bCanlink;		// �O�_���v���P�������յ��إ����p
	BOOL m_bLinked;			// �O�_�w�Q�������յ�����p

	LONG m_nLibPlacePerm;   // �Z�w�v��
	
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
