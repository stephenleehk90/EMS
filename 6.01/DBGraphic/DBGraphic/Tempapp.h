//void GetCurrentTime ( char *Time ) ;
#include "io.h"
void ArrangeTitle ( wchar_t *szTitle );
wchar_t *GetTitleWord( LPBYTE hpszBuffer , long lBytesRead , int nTitleLine  , long *plWordNumber );
wchar_t *GetTeleTitleWordW( LPBYTE hpszBuffer , long lBytesRead , int nTitleLine  , long *plWordNumber , long *plDelay );
char *GetTeleTitleWord( LPBYTE hpszBuffer , long lBytesRead , int nTitleLine  , long *plWordNumber , long *plDelay );
char *GetTeleFileTitleWord_Ansi( LPWSTR lpszFile, int nTitleLine, long *plWordNumber); // Michael 30-12-2011
LPBYTE Unix2Dos(LPBYTE szUnix); // Michael 30-12-2011
//BOOL DeleteDirectory1(LPSTR lpszDir , LPSTR lpszFileFormat );
BOOL FindKeyWordL ( wchar_t *szTitle , wchar_t *szKey );

//void GetCurrentDate ( char *Date ) ;
//DWORD TranslateCode( BYTE huge* lpszBuffer , BYTE huge * lpszDstBuffer , DWORD nSize );
void 	ChangeLeftSpaceZero( LPSTR lpszString );
void TransStringToDB( wchar_t *szString );
//BOOL DeleteDirectoryC(LPSTR lpszDir , LPSTR lpszFileFormat );
BOOL FindFileFormat ( wchar_t * szFile , char * szFormat );
void ChangeFileExt( wchar_t *szFile, wchar_t const *szExt);