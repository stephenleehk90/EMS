/*
   LTDIS.H - kernel module header file
   Copyright (c) 1990, 1998 LEAD Technologies, Inc.
   All Rights Reserved.
*/

#ifndef _LTDIS_H_
#define _LTDIS_H_

#include "lttyp.h"
#include "lterr.h"

#define _HEADER_ENTRY_
#include "ltpck.h"

/*
   flags for L_CopyToClipboardExt
*/
#define COPY2CB_EMPTY 0x0001    /* Empty the clipboard */
#define COPY2CB_DIB   0x0002    /* Copy DIB data to the clipboard */
#define COPY2CB_DDB   0x0004    /* Copy DDB data to the clipboard */
#define COPY2CB_PALETTE 0x0008  /* Copy palette data to the clipboard */
#define COPY2CB_RGN   0x0010  /* Copy region data to the clipboard */

/*
   flags for L_ConvertColorSpace
*/
#define CCS_RGB                 0x00   /* Color space is RGB */
#define CCS_YUV                 0x01   /* Color space is YUV */
#define CCS_CMYK                0x02   /* Color space is CMYK */
#define CCS_HSV                 0x03   /* Color space is HSV */
#define CCS_HLS                 0x04   /* Color space is HLS */
#define CCS_YIQ                 0x05   /* Color space is YIQ */
#define CCS_CMY                 0x06   /* Color space is CMY */

/*
   Constants for L_PaintDCBuffer
*/

#define PAINTDC_COMPRESSED          -1

/*
   Types for L_HalftoneBitmap
*/
#define HT_PRINT                  0
#define HT_VIEW                   1

/*
   flags for L_UnderlayBitmap
*/
#define UB_TILE                      0x00 /* Tile the underlay. */
#define UB_STRETCH                   0x01 /* Stretch the underlay. */

/*
   flags for L_SetDisplayMode
*/
#define DISPLAYMODE_ORDEREDDITHER    0x00000001
#define DISPLAYMODE_FASTPAINT        0x00000002
#define DISPLAYMODE_INDEXEDPAINT     0x00000004
#define DISPLAYMODE_DITHEREDPAINT    0x00000008
#define DISPLAYMODE_SCALETOGRAY      0x00000010
#define DISPLAYMODE_HALFTONEPRINT    0x00000020
#define DISPLAYMODE_FAVORBLACK       0x00000040
#define DISPLAYMODE_FIXEDPALETTE     0x00000080
#define DISPLAYMODE_NETSCAPEPALETTE  0x00000100
#define DISPLAYMODE_RESETPOSITIONS   0xFFFFFFFF

/*
   playback state constants
*/
#define PLAYSTATE_PRECLEAR    0x0000
#define PLAYSTATE_POSTCLEAR   0x0001
#define PLAYSTATE_PRERENDER   0x0002
#define PLAYSTATE_RENDER      0x0003
#define PLAYSTATE_POSTRENDER  0x0004
#define PLAYSTATE_WAITINPUT   0x0005
#define PLAYSTATE_WAITDELAY   0x0006
#define PLAYSTATE_WAITINPUTDELAY  0x0007
#define PLAYSTATE_PREDISPOSE  0x0008
#define PLAYSTATE_POSTDISPOSE 0x0009
#define PLAYSTATE_END         0x000A

#define PLAYDISPOSE_NONE              0
#define PLAYDISPOSE_LEAVE             1
#define PLAYDISPOSE_RESTOREBACKGROUND 2
#define PLAYDISPOSE_RESTOREPREVIOUS   3

/*
   region processing
*/
#define L_RGN_AND             0
#define L_RGN_SET             1
#define L_RGN_ANDNOTBITMAP    2
#define L_RGN_ANDNOTRGN       3
#define L_RGN_OR              4
#define L_RGN_XOR             5
#define L_RGN_SETNOT          6

#define L_POLY_WINDING        0
#define L_POLY_ALTERNATE      1

#define L_FRAME_MOVING0       0
#define L_FRAME_MOVING1       1
#define L_FRAME_MOVING2       2
#define L_FRAME_MOVING3       3
#define L_FRAME_MOVING4       4
#define L_FRAME_MOVING5       5
#define L_FRAME_MOVING6       6
#define L_FRAME_MOVING7       7

/*
   flags for L_WindowLevel
*/
#define WINDOWLEVEL_PAINT                 0x00
#define WINDOWLEVEL_PAINT_AND_PROCESSING  0x01

typedef struct _RGNXFORM
{
   L_UINT uViewPerspective;
   L_INT nXScalarNum;
   L_INT nXScalarDen;
   L_INT nYScalarNum;
   L_INT nYScalarDen;
   L_INT nXOffset;
   L_INT nYOffset;
} RGNXFORM, L_FAR *pRGNXFORM;

L_INT EXT_FUNCTION L_AppendPlayback(HPLAYBACK hPlayback,
                                     pBITMAPHANDLE pBitmap);

L_BOOL EXT_FUNCTION L_BitmapHasRgn(pBITMAPHANDLE pBitmap);

L_INT EXT_FUNCTION L_CancelPlaybackWait(HPLAYBACK hPlayback);

L_INT EXT_FUNCTION L_ChangeFromDDB (HDC hDC,
                                     pBITMAPHANDLE pBitmap,
                                     HBITMAP hBitmap,
                                     HPALETTE hPalette);

HBITMAP EXT_FUNCTION L_ChangeToDDB (HDC hDC,
                                     pBITMAPHANDLE pBitmap);

L_INT EXT_FUNCTION L_ClearPlaybackUpdateRect(HPLAYBACK hPlayback);

L_BOOL EXT_FUNCTION L_ClipboardReady (L_VOID);

L_INT EXT_FUNCTION L_ConvertColorSpace (L_UCHAR L_FAR * pBufferSrc,
                                         L_UCHAR L_FAR * pBufferDst,
                                         L_INT nWidth,
                                         L_INT nFormatSrc,
                                         L_INT nFormatDst);

L_INT EXT_FUNCTION L_ConvertFromDDB (HDC hDC,
                                      pBITMAPHANDLE pBitmap,
                                      HBITMAP hBitmap,
                                      HPALETTE hPalette);

HBITMAP EXT_FUNCTION L_ConvertToDDB (HDC hDC,
                                      pBITMAPHANDLE pBitmap);

L_BOOL EXT_FUNCTION L_CopyFromClipboard (HWND hWnd,
                                          pBITMAPHANDLE pBitmap);

L_BOOL EXT_FUNCTION L_CopyToClipboard (HWND hWnd,
                                        pBITMAPHANDLE pBitmap);

L_INT EXT_FUNCTION L_CopyToClipboardExt (HWND hWnd,
                                          pBITMAPHANDLE pBitmap,
                                          L_UINT uFlags);

HPALETTE EXT_FUNCTION L_CreatePaintPalette (HDC hDC,
                                             pBITMAPHANDLE pBitmap);

L_INT EXT_FUNCTION L_CreateMaskFromBitmapRgn(pBITMAPHANDLE pBitmap,
                                     pBITMAPHANDLE pMask);

L_INT EXT_FUNCTION L_CreatePlayback(pHPLAYBACK phPlayback,
                                    pBITMAPHANDLE pBitmap,
                                    HBITMAPLIST hList);

L_INT EXT_FUNCTION L_DestroyPlayback(HPLAYBACK hPlayback,
                                     pHBITMAPLIST phList);

L_INT EXT_FUNCTION L_FrameBitmapRgn(HDC hDC,
                                       pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       L_UINT uType);

L_INT EXT_FUNCTION L_FreeBitmapRgn(pBITMAPHANDLE pBitmap);

L_INT EXT_FUNCTION L_GetBitmapRgnArea(pBITMAPHANDLE pBitmap,
                                       L_UINT32 L_FAR *puArea);

L_INT EXT_FUNCTION L_GetBitmapRgnBounds(pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       RECT L_FAR *pRect);

L_INT EXT_FUNCTION L_GetBitmapRgnHandle(pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       HRGN L_FAR *phRgn);

L_UINT32 EXT_FUNCTION L_GetDisplayMode (L_VOID);


L_INT EXT_FUNCTION L_GetPaintContrast(L_VOID);

L_UINT EXT_FUNCTION L_GetPaintGamma(L_VOID);

L_INT EXT_FUNCTION L_GetPaintIntensity(L_VOID);

L_INT EXT_FUNCTION L_GetPlaybackDelay(HPLAYBACK hPlayback,
                                      L_UINT32 L_FAR *puDelay);

L_INT EXT_FUNCTION L_GetPlaybackIndex(HPLAYBACK hPlayback,
                                      L_INT L_FAR *pnIndex);

L_INT EXT_FUNCTION L_GetPlaybackState(HPLAYBACK hPlayback,
                                      L_UINT L_FAR *puState);

L_INT EXT_FUNCTION L_GetPlaybackUpdateRect(HPLAYBACK hPlayback,
                                      LPRECT prcUpdate,
                                      L_BOOL fClear);

L_INT EXT_FUNCTION L_HalfToneBitmap (pBITMAPHANDLE pBitmap,
                                      L_UINT uType);

L_INT EXT_FUNCTION L_HalfToneBitmapExt (pBITMAPHANDLE pBitmap,
                                         L_UINT uType,
                                         L_INT32 nAngle);

L_BOOL EXT_FUNCTION L_IsPtInBitmapRgn(pBITMAPHANDLE pBitmap,
                                       L_INT nRow,
                                       L_INT nCol);

L_INT EXT_FUNCTION L_OffsetBitmapRgn(pBITMAPHANDLE pBitmap,
                                       L_INT nRowOffset,
                                       L_INT nColOffset);

L_INT EXT_FUNCTION L_PaintDC (HDC hDC,
                               pBITMAPHANDLE pBitmap,
                               LPRECT pSrc,
                               LPRECT pClipSrc,
                               LPRECT pDst,
                               LPRECT pClipDst,
                               L_UINT32 uROP3);

L_INT EXT_FUNCTION L_PaintDCBuffer (HDC hDC,
                                     pBITMAPHANDLE pBitmap,
                                     LPRECT pSrc,
                                     LPRECT pClipSrc,
                                     LPRECT pDst,
                                     LPRECT pClipDst,
                                     L_UINT32 uROP3,
                                     L_UCHAR L_FAR * pBuffer,
                                     L_INT nRow,
                                     L_INT nCount);

L_INT EXT_FUNCTION L_PaintRgnDC (HDC hDC,
                                       pBITMAPHANDLE pBitmap,
                                       LPRECT pSrc,
                                       LPRECT pClipSrc,
                                       LPRECT pDst,
                                       LPRECT pClipDst,
                                       L_UINT32 uROP3);

L_INT EXT_FUNCTION L_PaintRgnDCBuffer (HDC hDC,
                                       pBITMAPHANDLE pBitmap,
                                       LPRECT pSrc,
                                       LPRECT pClipSrc,
                                       LPRECT pDst,
                                       LPRECT pClipDst,
                                       L_UINT32 uROP3,
                                       L_UCHAR L_FAR * pBuffer,
                                       L_INT nRow,
                                       L_INT nCount);

HDC EXT_FUNCTION L_PrintBitmap (pBITMAPHANDLE pBitmap,
                                 L_INT nX,
                                 L_INT nY,
                                 L_INT nWidth,
                                 L_INT nHeight,
                                 L_BOOL fEndDoc);

HDC EXT_FUNCTION L_PrintBitmapExt (HDC hDC,
                                    pBITMAPHANDLE pBitmap,
                                    L_INT nX,
                                    L_INT nY,
                                    L_INT nWidth,
                                    L_INT nHeight,
                                    L_BOOL fEndDoc);

HDC EXT_FUNCTION L_PrintBitmapFast (HDC hDC,
                                     pBITMAPHANDLE pBitmap,
                                     L_INT nX,
                                     L_INT nY,
                                     L_INT nWidth,
                                     L_INT nHeight,
                                     L_BOOL fEndDoc);

L_INT EXT_FUNCTION L_ProcessPlayback(HPLAYBACK hPlayback,
                                     L_UINT L_FAR *puState);

L_INT EXT_FUNCTION L_ScreenCaptureBitmap (HDC hDC,
                                           pBITMAPHANDLE pBitmap,
                                           LPRECT pRect);

L_INT EXT_FUNCTION L_SetBitmapRgnColor(pBITMAPHANDLE pBitmap,
                                       COLORREF crColor,
                                       L_UINT uCombineMode);

L_INT EXT_FUNCTION L_SetBitmapRgnEllipse(pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       RECT L_FAR *pRect,
                                       L_UINT uCombineMode);

L_INT EXT_FUNCTION L_SetBitmapRgnFromMask(pBITMAPHANDLE pBitmap,
                                    pRGNXFORM pXForm,
                                    pBITMAPHANDLE pMask,
                                    L_UINT uCombineMode);

L_INT EXT_FUNCTION L_SetBitmapRgnHandle(pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       HRGN hRgn,
                                       L_UINT uCombineMode);

L_INT EXT_FUNCTION L_SetBitmapRgnPolygon(pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       POINT L_FAR *pPoints,
                                       L_UINT uPoints,
                                       L_UINT uFillMode,
                                       L_UINT uCombineMode);

L_INT EXT_FUNCTION L_SetBitmapRgnRect(pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       RECT L_FAR *pRect,
                                       L_UINT uCombineMode);

L_INT EXT_FUNCTION L_SetBitmapRgnRoundRect(pBITMAPHANDLE pBitmap,
                                       pRGNXFORM pXForm,
                                       RECT L_FAR *pRect,
                                       L_INT nWidthEllipse,
                                       L_INT nHeightEllipse,
                                       L_UINT uCombineMode);

L_UINT32 EXT_FUNCTION L_SetDisplayMode (L_UINT32 uFlagPos,
                                         L_UINT32 uFlagSet);

L_INT EXT_FUNCTION L_SetPaintContrast(L_INT nValue);

L_INT EXT_FUNCTION L_SetPaintGamma(L_UINT uValue);

L_INT EXT_FUNCTION L_SetPaintIntensity(L_INT nValue);

L_INT EXT_FUNCTION L_SetPlaybackIndex(HPLAYBACK hPlayback,
                                      L_INT nIndex);

L_INT EXT_FUNCTION L_UnderlayBitmap (pBITMAPHANDLE pBitmapDst,
                                      pBITMAPHANDLE pUnderlay,
                                      L_UINT uFlags);

L_INT EXT_FUNCTION L_ValidatePlaybackLines(HPLAYBACK hPlayback,
                                           L_INT nRow,
                                           L_INT nLines);

L_INT EXT_FUNCTION L_WindowLevel(pBITMAPHANDLE pBitmap,
                                 L_INT nLowBit,
                                 L_INT nHighBit,
                                 L_RGBQUAD L_HUGE*pLUT,
                                 L_UINT32 ulLUTLength,
                                 L_UINT   uFlags);

#undef _HEADER_ENTRY_
#include "ltpck.h"

#endif                          /* _LTDIS_H_ */
