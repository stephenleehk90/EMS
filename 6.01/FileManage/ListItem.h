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
	CString strFileName;		//�Z��W��
	CString strFormat;			//�Z��榡
	CString strTitle;			//���D
	CString strLastOper;		//�̫�ާ@
	double	dbWordCount;		//�r��
	CString strAssignedUser;	//���w�ϥΪ�
	CString strLockUser;		//��w�Τ�
	int		nState;				//���A
	COleDateTime dtUpdateDate;	//�Z���s���
	CString strLastModTime;		//�̫�ק�ɶ�
	BOOL	bLib;				//
	BOOL	bIsRelate;			//�O�_�򳡪����յ�����p
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
	COleDateTime dtLastSendTime;//�̫�ǻ��ɶ�
	CString strSignedFile;		//ñ�o����W
	CDoColItem();
	virtual ~CDoColItem();
};
#endif // !defined(AFX_LISTITEM_H__90A15536_BDEB_41D4_8614_AD2F60B092BC__INCLUDED_)
