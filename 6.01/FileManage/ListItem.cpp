// ListItem.cpp: implementation of the CListItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "filemanage.h"
#include "ListItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListItem::CListItem()
{
	strFileName = _T("");
	strFormat = _T("");
	strTitle = _T("");
	strLastOper = _T("");
	dbWordCount = 0.0;
	strAssignedUser = _T("");
	nState = 0;
	strLastModTime = _T("");
	bLib = TRUE;
	bIsRelate = FALSE;
}

CListItem::~CListItem()
{

}
CDoFileItem::CDoFileItem()
{
	strComment = _T("");
	strAuthor = _T("");
}
CDoFileItem::~CDoFileItem()
{

}
CDoColItem::CDoColItem()
{
	strSignedFile = _T("");
}
CDoColItem::~CDoColItem()
{

}
