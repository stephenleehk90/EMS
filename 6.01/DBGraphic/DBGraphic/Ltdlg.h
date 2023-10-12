/*
   LTDLG.H - LEAD Common Dialog module header file
   Copyright (c) 1991-1998 by LEAD Technologies, Inc.
   All Rights Reserved.
*/

#ifndef _LTDLG_H_
#define _LTDLG_H_

#include "ltkrn.h"
#include "ltimg.h"
#include "ltfil.h"

#define _HEADER_ENTRY_
#include "ltpck.h"
#include <stdlib.h>
#include <commdlg.h>

/* callback typedefs */
typedef L_VOID (pEXT_CALLBACK LTCOMMDLGHELPCB) (L_UINT32);

/* structure typedefs */
typedef struct _FILEOPENPARM
{
   L_CHAR        szFileName[_MAX_PATH];/* filename                                   */
   pBITMAPHANDLE pBitmap;              /* Bitmap to fill in if AutoProcess           */
   pBITMAPHANDLE pThumbnail;           /* Bitmap to fill in with Thumbnail           */
   pFILEINFO     pFileInfo;            /* FILEINFO structure to fill                 */
   L_INT         nPageNumber;          /* Page Number to load                        */
   L_INT         nPasses;              /* Passes to load from Progressive image file */
   L_BOOL        bLoadCompressed;      /* Load 1-bit image as compressed data?       */
   L_BOOL        bLoadRotated;         /* Load image without rotating data?          */
   L_BOOL        bPreviewEnabled;      /* TRUE to have Preview enabled               */
} FILEOPENPARM, L_FAR* LPFILEOPENPARM;

typedef struct _FILESAVEPARM
{
   L_CHAR        szFileName[_MAX_PATH]; /* filename */
   pBITMAPHANDLE pBitmap;              /* Bitmap to save */
   L_INT         nBitsPerPixel;        /* bits per pixel to save */
   L_INT         nFormat;              /* file format to save */
   L_INT         nQFactor;             /* QFactor value for save */
   L_BOOL        bSaveMulti;           /* save as multi-page file? */
   L_BOOL        bSaveInterlaced;      /* save as interlaced GIF? */
   L_BOOL        bSaveProgressive;     /* save as progressive image? */
   L_INT         nPasses;              /* # of progressive passes */
   L_BOOL        bSaveWithStamp;       /* save with stamp preview? */
   L_INT         nStampBits;           /* stamp bits per pixel */
   L_INT         nStampWidth;          /* stamp width */
   L_INT         nStampHeight;         /* stamp height */
} FILESAVEPARM, L_FAR* LPFILESAVEPARM;

/* flags for the Help Callbacks for L_Dlg* function */
#define DLG_HELP_FILEOPEN     1
#define DLG_HELP_FILEOPEN95   2
#define DLG_HELP_FILESAVE     3
#define DLG_HELP_FILESAVE95   4
#define DLG_HELP_COLORRES     5
#define DLG_HELP_GETANGLE     6
#define DLG_HELP_GETSIZE      7
#define DLG_HELP_GETFILTER    8
#define DLG_HELP_GETCHANGE    9
#define DLG_HELP_GETRANGE     10
#define DLG_HELP_GETGAMMA     11
#define DLG_HELP_GETNOISE     12
#define DLG_HELP_GETEMBOSS    13
#define DLG_HELP_WINDOWLEVEL  14

/* flags for L_DlgFileOpen() */
#define DLG_FO_AUTOPROCESS    0x00000001   /* Dlg should autoprocess image and fill the pBitmap */
#define DLG_FO_PROGRESSIVE    0x00000002   /* FileOpen Dlg should have Prog. File options */
#define DLG_FO_MULTIPAGE      0x00000004   /* FileOpen Dlg should have Multipage options */
#define DLG_FO_LOADROTATED    0x00000008   /* FileOpen Dlg should have Load Rotated option */
#define DLG_FO_LOADCOMPRESSED 0x00000010   /* FileOpen Dlg should have Load 1-bit compressed option */
#define DLG_FO_FILEINFO       0x00000020   /* FileOpen Dlg should have FileInfo button */
#define DLG_FO_SHOWPREVIEW    0x00000040   /* FileOpen Dlg should have ThumbNail and should create the */
                                           /* preview if none is stored in the file */
#define DLG_FO_SHOWSTAMP      0x00000080   /* FileOpen Dlg should have ThumbNail and should only load */
                                           /* the preview stamp if one is stored in the file */
#define DLG_FO_95STYLE        0x00000100   /* FileOpen Dlg should have Windows95 Explorer style look */

/* flags for L_DlgFileSave() */
#define DLG_FS_AUTOPROCESS    0x00000001   /* Dlg should autoprocess image and fill the pBitmap */
#define DLG_FS_PROGRESSIVE    0x00000002   /* FileSave Dlg should have Prog. File options */
#define DLG_FS_MULTIPAGE      0x00000004   /* FileSave Dlg should have Multipage options */
#define DLG_FS_STAMP          0x00000008   /* FileSave Dlg should have save with stamp options */
#define DLG_FS_QFACTOR        0x00000010   /* FileSave Dlg should have QFactor option */
#define DLG_FS_95STYLE        0x00000020   /* FileSave Dlg should have Windows95 Explorer style look */
/* file format flags for L_DlgFileSave() */
#define DLG_FS_LEAD           0x00000002
#define DLG_FS_LEAD1BIT       0x00000004
#define DLG_FS_JFIF           0x00000008
#define DLG_FS_CALS           0x00000010
#define DLG_FS_PNG            0x00000020
#define DLG_FS_DICOM          0x00000040
#define DLG_FS_FAX            0x00000080
#define DLG_FS_EPS            0x00000100
#define DLG_FS_EXIF           0x00000200
#define DLG_FS_FPX            0x00000400
#define DLG_FS_IMG            0x00000800
#define DLG_FS_GIF            0x00001000
#define DLG_FS_IOCA           0x00001000
#define DLG_FS_PCT            0x00004000
#define DLG_FS_MAC            0x00008000
#define DLG_FS_AWD            0x00010000
#define DLG_FS_MSP            0x00020000
#define DLG_FS_OS2BMP         0x00040000
#define DLG_FS_PCX            0x00080000
#define DLG_FS_PSD            0x00100000
#define DLG_FS_RAS            0x00200000
#define DLG_FS_TGA            0x00400000
#define DLG_FS_TIFF           0x00800000
#define DLG_FS_BMP            0x01000000
#define DLG_FS_WFX            0x02000000
#define DLG_FS_WMF            0x04000000
#define DLG_FS_WPG            0x08000000
#define DLG_FS_ICO            0x10000000
#define DLG_FS_CUR            0x20000000
#define DLG_FS_ALL            0xFFFFFFFF  /* all file formats */

/* flags for Image processing dialog functions */
#define DLG_IMG_AUTOPROCESS     0x80000000   /* Dlg should autoprocess image and fill the pBitmap */
#define DLG_IMG_SHOWPREVIEW     0x40000000   /* ImageProcess Dlg should preview of image */
#define DLG_IMG_HIDEBACKCOLOR   0x20000000   /* GetAngle Dlg should hide the Background Color button */

#define DLG_IMG_RESIZE          0x00000001   /* resize dialog */
#define DLG_IMG_RESAMPLE        0x00000002   /* resample dialog */
#define DLG_IMG_ROTATE          0x00000003   /* rotate dialog */
#define DLG_IMG_SHEAR           0x00000004   /* shear dialog */

#define DLG_IMG_GRADIENT        0x00000001   /* gradient dialog */
#define DLG_IMG_LAPLACIAN       0x00000002   /* laplacian dialog */
#define DLG_IMG_SOBEL           0x00000004   /* sobel dialog */
#define DLG_IMG_PREWITT         0x00000008   /* prewitt dialog */
#define DLG_IMG_SHIFTDIFF       0x00000010   /* shiftdiff dialog */
#define DLG_IMG_LINESEG         0x00000020   /* lineseg dialog */
#define DLG_IMG_EROSION         0x00000040   /* erosion dialog */
#define DLG_IMG_DILATION        0x00000080   /* dilation dialog */

#define DLG_IMG_SHARPEN         0x00000001   /* sharpen dialog */
#define DLG_IMG_BRIGHTNESS      0x00000002   /* brightness dialog */
#define DLG_IMG_CONTRAST        0x00000004   /* contrast dialog */
#define DLG_IMG_SATURATION      0x00000008   /* saturation dialog */
#define DLG_IMG_HISTOCONTRAST   0x00000010   /* histo contrast dialog */
#define DLG_IMG_HUE             0x00000020   /* hue dialog */
#define DLG_IMG_HALFTONE        0x00000040   /* halftone dialog */
#define DLG_IMG_POSTERIZE       0x00000080   /* posterize dialog */
#define DLG_IMG_MOSAIC          0x00000100   /* mosaic dialog */
#define DLG_IMG_AVERAGE         0x00000200   /* average dialog */
#define DLG_IMG_MEDIAN          0x00000400   /* median dialog */
#define DLG_IMG_OILIFY          0x00000800   /* oilify dialog */
#define DLG_IMG_MIN             0x00001000   /* min filter dialog */
#define DLG_IMG_MAX             0x00002000   /* max filter dialog */
#define DLG_IMG_SOLARIZE        0x00004000   /* solarize dialog */

#define DLG_IMG_ADDNOISE        0x0000001C   /* add noise dialog */
#define DLG_IMG_GAMMACORRECT    0x0000001D   /* gamma correct dialog */
#define DLG_IMG_EMBOSS          0x0000001E   /* emboss filter dialog */
#define DLG_IMG_INTENSITYDETECT 0x0000001F   /* emboss filter dialog */

/* flags for L_DlgColorRes() */
#define DLG_CR_SHOWPREVIEW      0x00000001   /* shows preview of image */
#define DLG_CR_AUTOPROCESS      0x00000002   /* process the image on OK */

#define DLG_CR_1BIT             0x00000004   /* 1-bit is an option */
#define DLG_CR_2BIT             0x00000008   /* 2-bit is an option */
#define DLG_CR_3BIT             0x00000010   /* 3-bit is an option */
#define DLG_CR_4BIT             0x00000020   /* 4-bit is an option */
#define DLG_CR_5BIT             0x00000040   /* 5-bit is an option */
#define DLG_CR_6BIT             0x00000080   /* 6-bit is an option */
#define DLG_CR_7BIT             0x00000100   /* 7-bit is an option */
#define DLG_CR_8BIT             0x00000200   /* 8-bit is an option */
#define DLG_CR_12BIT            0x00000400   /* 12-bit is an option */
#define DLG_CR_16BIT            0x00000800   /* 16-bit is an option */
#define DLG_CR_24BIT            0x00001000   /* 24-bit is an option */
#define DLG_CR_32BIT            0x00002000   /* 32-bit is an option */
#define DLG_CR_BITALL           0x00003FFC   /* All BitsPerPixel are options */

#define DLG_CR_DITHER_NONE      0x00004000   /* no dithering is an option */
#define DLG_CR_DITHER_FLOYD     0x00008000   /* floyd dithering is an option */
#define DLG_CR_DITHER_STUCKI    0x00010000   /* stucki dithering is an option */
#define DLG_CR_DITHER_BURKES    0x00020000   /* burkes dithering is an option */
#define DLG_CR_DITHER_SIERRA    0x00040000   /* sierra dithering is an option */
#define DLG_CR_DITHER_STEVENSON 0x00080000   /* stevenson dithering is an option */
#define DLG_CR_DITHER_JARVIS    0x00100000   /* jarvis dithering is an option */
#define DLG_CR_DITHER_ORDERED   0x00200000   /* ordered dithering is an option */
#define DLG_CR_DITHER_ALL       0x003FC000   /* All dithering methods are options */

#define DLG_CR_PAL_FIXED      0x00400000   /* fixed palette is an option */
#define DLG_CR_PAL_OPTIMIZED  0x00800000   /* fixed palette is an option */
#define DLG_CR_PAL_IDENTITY   0x01000000   /* identity palette is an option */
#define DLG_CR_PAL_NETSCAPE   0x02000000   /* netscape palette is an option */
#define DLG_CR_PAL_ALL        0x03C00000   /* All palette options are options */

#define DLG_CR_ORDER          0x04000000   /* color order is an option */


/* flags for L_DlgWindowLevel() */
#define DLG_WL_SHOWPREVIEW      0x00000001   /* shows preview of image */
#define DLG_WL_AUTOPROCESS      0x00000002   /* process the image on OK */


/* external prototypes */
L_INT EXT_FUNCTION L_DlgFileOpen( HWND hWnd,LPOPENFILENAME pOpenFileName,LPFILEOPENPARM pFileOpenParm,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgGetAngle( HWND hWnd,pBITMAPHANDLE pBitmap,L_INT32 L_FAR* pnAngle,L_INT L_FAR* pbFlag,COLORREF L_FAR* pcrBack,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgGetSize( HWND hWnd,pBITMAPHANDLE pBitmap,L_INT L_FAR* pnWidth,L_INT L_FAR* pnHeight,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgGetFilter( HWND hWnd,pBITMAPHANDLE pBitmap,LPSPATIALFLT L_FAR* ppSpatialFlt,LPBINARYFLT L_FAR* ppBinaryFlt,L_BOOL L_FAR* pbSpatialFilter, L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgGetChange( HWND hWnd,pBITMAPHANDLE pBitmap,L_INT L_FAR* pnChange,L_UINT32 L_FAR* puProcess,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgGetGamma( HWND hWnd,pBITMAPHANDLE pBitmap,L_UINT L_FAR* puGamma,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgGetNoise( HWND hWnd,pBITMAPHANDLE pBitmap,L_UINT L_FAR* puRange,L_UINT L_FAR* puChannel,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgGetEmboss( HWND hWnd,pBITMAPHANDLE pBitmap,L_UINT L_FAR* puDirection,L_UINT L_FAR* puDepth,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgGetRange( HWND hWnd,pBITMAPHANDLE pBitmap,L_UINT L_FAR* puLow,L_UINT L_FAR* puHigh,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgColorRes( HWND hWnd,pBITMAPHANDLE pBitmap,L_INT L_FAR* pnBits,L_UINT32 L_FAR* puColorResFlags,L_UINT32 uFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgFileSave( HWND hWnd,LPOPENFILENAME pOpenFileName,LPFILESAVEPARM pFileSaveParm,L_UINT32 uFlags,L_UINT32 uFormatFlags,LTCOMMDLGHELPCB pfnCallback );
L_INT EXT_FUNCTION L_DlgWindowLevel( HWND hWnd, pBITMAPHANDLE pBitmap, L_RGBQUAD L_HUGE* pLUT, L_UINT32 uLUTLength, L_INT nLowBit, L_INT nHighBit, L_UINT L_FAR* pnLow, L_UINT L_FAR* pnHigh,
                                     COLORREF L_FAR* pcrStart, COLORREF L_FAR* pcrEnd, L_UINT32 uFlags, LTCOMMDLGHELPCB pfnCallback );

#undef _HEADER_ENTRY_
#include "ltpck.h"

#endif                          /* _LTDLG_H_ */



