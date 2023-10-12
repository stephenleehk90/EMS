
#define MAIN_FRAME_TIMER            1
#define NAME_LEN                    50
#define MAX_FILESPECIAL				30
#define PATH_LENGTH					300//81
#define PATH_LENGTH1				300
#define FILE_LENGTH					300
#define TEMP_STRING_LENGTH          600
#define	CODE_LEN					4
#define MAX_FILE_DISTRIBUTE         30
#define	WINDOW_BUFFER_SIZE  		5000
#define FILESPECIAL_LENGTH          20
#define COMMAND_LEN                 1024
#define	TITLE_LEN						255
#define	MAX_COUNT					5000
#define	MAX_MESSAGE_LENGTH			500
#define MEDIA_AV_PHOTO_MAX  9
#define  DBG_MULTI_THREAD	
//#define DBG_SING_THREAD    // DBG_MULTI_THREAD
#define WIRES_TABLE_NAME_LENGTH 100
#define DBG_FILE_PATH_LENGTH 500
#define SYSTEM_TIME_LENGTH	60
#define MAX_LOG_LENGTH	10  //10 MB
//Edit by Leon Li 2013-02-27 for enhancement: remove dat file and check photo record in memeory
enum TablePhotoType{NoneType = 0,OrgPhoto,SmlPhoto,MidPhoto};
const CString DISKHISTORYADD	  =	_T("C:\\Founder\\temp\\dbgraphic");    // Permanent file address for storing history
const CString DISKFILEADD	  =	_T("C:\\Founder");    // Permanent file address 
const CString DISKLOGADD	  =	_T("C:\\Founder\\log");    // Permanent file address 

class CDBGraphicMain;

typedef struct {
	wchar_t	sign[8];		//
	long	version;		//
	long	size;			//size of this struct
	wchar_t	szCode[32];		//
	long	iType;			//0 is Tele,1 is depart,2 is private,other is not available.
	int		nAgencyID;		//iType = 0 is tele_id ,1 is lib_id,other is not available add by wxy editor5
	//	char	szTeleName[24];
	wchar_t	szTeleName[24];
	wchar_t	szTeleCode[30];
	BOOL bCheckTime;        // If check photo last time or not
	///////////////////////////////source path
	wchar_t	szSourcePath[260];
	wchar_t	szPhotoExtName[32*4];
	short	bAutoBitmap;
	short	bLocalToServer;
	wchar_t	szServerPath[260];
	///////////////////////////////text path
	wchar_t	szTextPath[260];
	wchar_t	szTextExtName[32];
	wchar_t	szCaption[512];			///
	///////////////////////////////
	int		bCurrentDate;
	int		nBefore;
	int		nAfter;
	wchar_t	szReserve[256-sizeof(int)*4];
	//////
	wchar_t	szBeginTime[32];
	wchar_t	szAutoDeleteLo[32];
	long	nKeepDay;
	long	nTime;
	short	bTransfer;
	long	nTitleLine;
	long	nMarkLine;
	wchar_t	szDesPath[260];
	BOOL	bTransMidPhoto;//Âà¤¤¹Ï
	wchar_t	szMidPath[260];
	wchar_t	szSmallPath[260];
	wchar_t	szStartTime[32];

	// Michael 21-12-2011

	BOOL	bXMLByAgency;  
	BOOL	bXMLByFile;  
	BOOL	bXMLOutput;  
	wchar_t szXMLPath[ 260 ];
	BOOL	bUseFolderDate;
	// End Michael 
	int iMediaType;
	/*int nMaxTm;
	int nInterval;
	COleDateTime dtLastImportTime;*/

}DISTFILESTRUCT;
typedef CArray<DISTFILESTRUCT, DISTFILESTRUCT> ADISTFILESTRUCT;
typedef struct
{
	//	char szFile[60];
	//	char szDate[26];
	wchar_t szFile[60];
	wchar_t szDate[26];
	//char szTime[8];
}LOGDISTRIBUTEDALLITEM;

typedef struct  
{
	wchar_t sz_Code[20];
	wchar_t szFile[255];
	wchar_t sz_Date[20];
	wchar_t sz_Time[20];
	wchar_t sz_LastTime[38];
	int err_Count;
	int status;
}ERRORFILE;	

// Edit Stephen 2021-08-25 
typedef struct  
{
	wchar_t sz_Code[20];
	wchar_t szFile[255];
	wchar_t sz_Date[20];
	wchar_t sz_Time[20];
}DUMMYTHUMBNAILFILE;	
// Edit Stephen 2021-08-25 


typedef struct
{
	//	char szCode[ 10 ];
	//	char szDate[ 10 ];
	wchar_t szCode[ 30 ];
	wchar_t szDate[ 30 ];
	long lTotal;
	long lStart;
}LOGDISTRIBUTEDHEAD;

typedef struct
{
	long lTotal;
}LOGDISTRIBUTEDALLHEAD;

typedef struct
{
	wchar_t szFile[NAME_LEN];
	wchar_t szScrFile[ PATH_LENGTH1];
	wchar_t szDstFile[ PATH_LENGTH1];
	wchar_t szNote[ NAME_LEN ];
	long lNumber;
	wchar_t szRealSrcFile[PATH_LENGTH1];
	wchar_t szFileTime[30];
	//long lSize;
}LOGDISTRIBUTEDITEM;

typedef struct  
{
	CString strMediaName;
	CString strFilePath;
	CString strFileTime;
}DISTFILEITEM;
typedef CArray<DISTFILEITEM,DISTFILEITEM> DISTFILEARR;

typedef struct  
{
	CString strAgencyCode;
	int nMediaCount;
	DISTFILEARR MediaDistArr;
}AGENTDISTFILEARR;
//End Leon Li 2013-02-27

typedef struct 
{
	int nAgencyNum;
	CDBGraphicMain* pctrlProgress;
}threadInfo;

typedef struct{
	CString strMessageHeader;
	CString strMessageContent;
}DispalyMessage;

/**Define some dialog message **/
#define MSG_START	10000
#define MESSAGE_UPDATE_UI_STATUS MSG_START
#define MESSAGE_INSERT_LIST_MESSAGE MSG_START+1
