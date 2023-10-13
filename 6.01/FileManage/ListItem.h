// ListItem.h: interface for the CListItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTITEM_H__90A15536_BDEB_41D4_8614_AD2F60B092BC__INCLUDED_)
#define AFX_LISTITEM_H__90A15536_BDEB_41D4_8614_AD2F60B092BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CListItem  :public CObject
{
public:
	CString strFileName;		//稿件名稱
	CString strFormat;			//稿件格式
	CString strTitle;			//標題
	CString strLastOper;		//最後操作
	double	dbWordCount;		//字數
	CString strAssignedUser;	//指定使用者
	CString strLockUser;		//鎖定用戶
	int		nState;				//狀態
	COleDateTime dtUpdateDate;	//稿件更新日期
	CString strLastModTime;		//最後修改時間
	BOOL	bLib;				//
	BOOL	bIsRelate;			//是否跟部門分組菜單關聯
	CListItem();
	virtual ~CListItem();

};
class CDoFileItem :public CListItem
{
public:
	CString strComment;
	CString strAuthor;
	CDoFileItem();
	virtual ~CDoFileItem();
};
class CDoColItem :public CListItem
{
public:
	COleDateTime dtLastSendTime;//最後傳遞時間
	CString strSignedFile;		//簽發後文件名
	CDoColItem();
	virtual ~CDoColItem();
};
#endif // !defined(AFX_LISTITEM_H__90A15536_BDEB_41D4_8614_AD2F60B092BC__INCLUDED_)
