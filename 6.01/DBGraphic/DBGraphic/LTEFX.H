/*
   LTEFX.H - effects module header file
   Copyright (c) 1990, 1998 LEAD Technologies, Inc.
   All Rights Reserved.
*/

#ifndef _LTEFX_H_
#define _LTEFX_H_

#include "ltkrn.h"

#define _HEADER_ENTRY_
#include "ltpck.h"

/*
   transition types
*/

enum
{
   EFX_TRANS_SOLID,
   EFX_TRANS_HORZ_LINE,
   EFX_TRANS_VERT_LINE,
   EFX_TRANS_DOWNWARD_DIAG,
   EFX_TRANS_UPWARD_DIAG,
   EFX_TRANS_CROSS,
   EFX_TRANS_DIAG_CROSS,
   EFX_TRANS_GRADIENT_LEFT_TO_RIGHT,
   EFX_TRANS_GRADIENT_RIGHT_TO_LEFT,
   EFX_TRANS_GRADIENT_TOP_TO_BOTTOM,
   EFX_TRANS_GRADIENT_BOTTOM_TO_TOP,
   EFX_TRANS_GRADIENT_OUT_TO_CENTER,
   EFX_TRANS_GRADIENT_CENTER_TO_OUT,
   EFX_TRANS_GRADIENT_DIAG_TL_TO_BR,
   EFX_TRANS_GRADIENT_DIAG_BR_TO_TL,
   EFX_TRANS_GRADIENT_DIAG_TR_TO_BL,
   EFX_TRANS_GRADIENT_DIAG_BL_TO_TR,
   EFX_TRANS_GRADIENT_DIAG_TLBR_TO_CENTER,
   EFX_TRANS_GRADIENT_DIAG_CENTER_TO_TLBR,
   EFX_TRANS_GRADIENT_DIAG_TRBL_TO_CENTER,
   EFX_TRANS_GRADIENT_DIAG_CENTER_TO_TRBL,
   EFX_TRANS_GRADIENT_ELLIPSE_TO_CENTER,
   EFX_TRANS_GRADIENT_ELLIPSE_FROM_CENTER,
   EFX_TRANS_MAX
};

enum
{
   EFX_PATTERN_SOLID,
   EFX_PATTERN_TRANSPARENT,
   EFX_PATTERN_HORZ_LINE,
   EFX_PATTERN_VERT_LINE,
   EFX_PATTERN_DOWNWARD_DIAG,
   EFX_PATTERN_UPWARD_DIAG,
   EFX_PATTERN_CROSS,
   EFX_PATTERN_DIAG_CROSS,
};

enum
{
   EFX_GRADIENT_LEFT_TO_RIGHT,
   EFX_GRADIENT_RIGHT_TO_LEFT,
   EFX_GRADIENT_TOP_TO_BOTTOM,
   EFX_GRADIENT_BOTTOM_TO_TOP,
   EFX_GRADIENT_OUT_TO_CENTER,
   EFX_GRADIENT_CENTER_TO_OUT,
   EFX_GRADIENT_DIAG_TL_TO_BR,
   EFX_GRADIENT_DIAG_BR_TO_TL,
   EFX_GRADIENT_DIAG_TR_TO_BL,
   EFX_GRADIENT_DIAG_BL_TO_TR,
   EFX_GRADIENT_DIAG_TLBR_TO_CENTER,
   EFX_GRADIENT_DIAG_CENTER_TO_TLBR,
   EFX_GRADIENT_DIAG_TRBL_TO_CENTER,
   EFX_GRADIENT_DIAG_CENTER_TO_TRBL,
   EFX_GRADIENT_ELLIPSE_TO_CENTER,
   EFX_GRADIENT_ELLIPSE_FROM_CENTER,
   EFX_GRADIENT_MAX
};

/*
   effect types
*/
enum
{
   EFX_EFFECT_NONE,
   EFX_EFFECT_NO_PICTURE,
   EFX_EFFECT_LEFT_TO_RIGHT,
   EFX_EFFECT_RIGHT_TO_LEFT,
   EFX_EFFECT_TOP_TO_BOTTOM,
   EFX_EFFECT_BOTTOM_TO_TOP,
   EFX_EFFECT_DIAG_TL_TO_BR,
   EFX_EFFECT_DIAG_BL_TO_TR,
   EFX_EFFECT_DIAG_TR_TO_BL,
   EFX_EFFECT_DIAG_BR_TO_TL,
   EFX_EFFECT_DBL_DIAG_TL_TO_BR,
   EFX_EFFECT_DBL_DIAG_TR_TO_BL,
   EFX_EFFECT_DIAG_OUT_TL_TO_BR,
   EFX_EFFECT_DIAG_OUT_TR_TO_BL,
   EFX_EFFECT_HORZ_INTERLACE,
   EFX_EFFECT_VERT_INTERLACE,
   EFX_EFFECT_HORZ_DBL_PASS,
   EFX_EFFECT_VERT_DBL_PASS,
   EFX_EFFECT_HORZ_DBL_PASS_2,
   EFX_EFFECT_VERT_DBL_PASS_2,
   EFX_EFFECT_HORZ_BLIND,
   EFX_EFFECT_VERT_BLIND,
   EFX_EFFECT_DBL_BLIND,
   EFX_EFFECT_ZOOM_OUT,
   EFX_EFFECT_ZOOM_IN,
   EFX_EFFECT_SWIRL_IN,
   EFX_EFFECT_SWIRL_OUT,
   EFX_EFFECT_CORNERS_OUT,
   EFX_EFFECT_CHECKERBOARD,
   EFX_EFFECT_KALEIDOSCOPE,
   EFX_EFFECT_DRIP,
   EFX_EFFECT_RANDOM_BARS_LEFT,
   EFX_EFFECT_RANDOM_BARS_RIGHT,
   EFX_EFFECT_RANDOM_BARS_UP,
   EFX_EFFECT_RANDOM_BARS_DOWN,
   EFX_EFFECT_RANDOM_LINES,
   EFX_EFFECT_RANDOM_BLOCK,
   EFX_EFFECT_SPARKLE_LEFT,
   EFX_EFFECT_SPARKLE_RIGHT,
   EFX_EFFECT_SPARKLE_UP,
   EFX_EFFECT_SPARKLE_DOWN,
   EFX_EFFECT_SEMICIRCLE_LR,
   EFX_EFFECT_SEMICIRCLE_RL,
   EFX_EFFECT_SEMICIRCLE_IN,
   EFX_EFFECT_SEMICIRCLE_OUT,
   EFX_EFFECT_DIAG_QUAD,
   EFX_EFFECT_CIRCULAR_QUAD,
   EFX_EFFECT_ROTATE_FROM_LEFT,
   EFX_EFFECT_ROTATE_FROM_RIGHT,
   EFX_EFFECT_ROTATE_FROM_TOP,
   EFX_EFFECT_ROTATE_FROM_BOTTOM,
   EFX_EFFECT_HORZ_CENTER_STRETCH,
   EFX_EFFECT_VERT_CENTER_STRETCH,
   EFX_EFFECT_HORZ_STRETCH_TO_CENTER,
   EFX_EFFECT_VERT_STRETCH_TO_CENTER,
   EFX_EFFECT_BLOCKS_TB,
   EFX_EFFECT_BLOCKS_BT,
   EFX_EFFECT_BLOCKS_LR,
   EFX_EFFECT_BLOCKS_RL,
   EFX_EFFECT_GROWING_BLIND_LR,
   EFX_EFFECT_GROWING_BLIND_RL,
   EFX_EFFECT_GROWING_BLIND_TB,
   EFX_EFFECT_GROWING_BLIND_BT,
   EFX_EFFECT_ROLL_LR,
   EFX_EFFECT_ROLL_RL,
   EFX_EFFECT_ROLL_TB,
   EFX_EFFECT_ROLL_BT,
   EFX_EFFECT_DBL_DIAG_UP,
   EFX_EFFECT_DBL_DIAG_DOWN,
   EFX_EFFECT_DBL_DIAG_LEFT,
   EFX_EFFECT_DBL_DIAG_RIGHT,
   EFX_EFFECT_CLOCKWISE,
   EFX_EFFECT_COUNTER_CLOCKWISE,
   EFX_EFFECT_DBL_CLOCKWISE,
   EFX_EFFECT_DBL_PASS_LEFT_TO_RIGHT,
   EFX_EFFECT_DBL_PASS_RIGHT_TO_LEFT,
   EFX_EFFECT_DBL_PASS_TOP_TO_BOTTOM,
   EFX_EFFECT_DBL_PASS_BOTTOM_TO_TOP,
   EFX_EFFECT_HORZ_WIPE_IN,
   EFX_EFFECT_HORZ_WIPE_OUT,
   EFX_EFFECT_VERT_WIPE_IN,
   EFX_EFFECT_VERT_WIPE_OUT,
   EFX_EFFECT_DBL_PASS_HORZ_WIPE_IN,
   EFX_EFFECT_DBL_PASS_HORZ_WIPE_OUT,
   EFX_EFFECT_DBL_PASS_VERT_WIPE_IN,
   EFX_EFFECT_DBL_PASS_VERT_WIPE_OUT,
   EFX_EFFECT_VERT_DBL_WIPE,
   EFX_EFFECT_HORZ_DBL_WIPE,
   EFX_EFFECT_DBL_WIPE_OUT,
   EFX_EFFECT_DBL_WIPE_IN,
   EFX_EFFECT_EXPLODE,
   EFX_EFFECT_IMPLODE,
   EFX_EFFECT_DBL_PASS_IMPLODE,
   EFX_EFFECT_DBL_PASS_EXPLODE,
   EFX_EFFECT_SLIDE_LEFT,
   EFX_EFFECT_SLIDE_RIGHT,
   EFX_EFFECT_SLIDE_UP,
   EFX_EFFECT_SLIDE_DOWN,
   EFX_EFFECT_DIAG_SLIDE_TL_TO_BR,
   EFX_EFFECT_DIAG_SLIDE_TR_TO_BL,
   EFX_EFFECT_DIAG_SLIDE_BL_TO_TR,
   EFX_EFFECT_DIAG_SLIDE_BR_TO_TL,
   EFX_EFFECT_HORZ_DBL_SLIDE,
   EFX_EFFECT_VERT_DBL_SLIDE,
   EFX_EFFECT_HORZ_INTERLACE_SLIDE,
   EFX_EFFECT_VERT_INTERLACE_SLIDE,
   EFX_EFFECT_PUSH_LEFT,
   EFX_EFFECT_PUSH_RIGHT,
   EFX_EFFECT_PUSH_UP,
   EFX_EFFECT_PUSH_DOWN,
   EFX_EFFECT_VERT_SQUASH,
   EFX_EFFECT_HORZ_SQUASH,
   EFX_EFFECT_VERT_PULL,
   EFX_EFFECT_HORZ_PULL,
   EFX_EFFECT_MAX
};

#define EFFECT_NONE                 EFX_EFFECT_NONE
#define EFFECT_WIPE_DOWN            EFX_EFFECT_TOP_TO_BOTTOM
#define EFFECT_WIPE_UP              EFX_EFFECT_BOTTOM_TO_TOP
#define EFFECT_WIPE_RIGHT           EFX_EFFECT_LEFT_TO_RIGHT
#define EFFECT_WIPE_LEFT            EFX_EFFECT_RIGHT_TO_LEFT
#define EFFECT_RANDOM_PIXEL         EFX_EFFECT_RANDOM_BLOCK           // close match - use the grain size
#define EFFECT_RANDOM_SMALLBOX      EFX_EFFECT_RANDOM_BLOCK           // close match - use the grain size
#define EFFECT_RANDOM_MEDIUMBOX     EFX_EFFECT_RANDOM_BLOCK           // close match - use the grain size
#define EFFECT_RANDOM_LARGEBOX      EFX_EFFECT_RANDOM_BLOCK           // close match - use the grain size
#define EFFECT_RANDOM_HLINE         EFX_EFFECT_RANDOM_LINES           // close match - use the grain size
#define EFFECT_RANDOM_VLINE         EFX_EFFECT_RANDOM_LINES           // close match - use the grain size
#define EFFECT_VMIDDLE              EFX_EFFECT_HORZ_WIPE_OUT
#define EFFECT_HMIDDLE              EFX_EFFECT_VERT_WIPE_OUT
#define EFFECT_HINTERLACE           EFX_EFFECT_HORZ_BLIND             // close match
#define EFFECT_VINTERLACE           EFX_EFFECT_VERT_BLIND             // close match
#define EFFECT_HINTERLACE_OUTWARD   EFX_EFFECT_DBL_PASS_HORZ_WIPE_OUT // close match
#define EFFECT_VINTERLACE_OUTWARD   EFX_EFFECT_DBL_PASS_VERT_WIPE_OUT // close match
#define EFFECT_OUTTOIN              EFX_EFFECT_IMPLODE
#define EFFECT_INTOOUT              EFX_EFFECT_EXPLODE
#define EFFECT_OUTTOIN_INTERLACE    EFX_EFFECT_DBL_PASS_IMPLODE       // close match
#define EFFECT_INTOOUT_INTERLACE    EFX_EFFECT_DBL_PASS_EXPLODE       // close match
#define EFFECT_SPIRAL_IN            EFX_EFFECT_SWIRL_IN
#define EFFECT_SPIRAL_OUT           EFX_EFFECT_SWIRL_OUT
#define EFFECT_HMIDDLE_INWARD       EFX_EFFECT_VERT_WIPE_IN
#define EFFECT_VMIDDLE_INWARD       EFX_EFFECT_HORZ_WIPE_IN
#define EFFECT_VINTERLACE_INWARD    EFX_EFFECT_DBL_PASS_HORZ_WIPE_IN  // close match
#define EFFECT_HINTERLACE_INWARD    EFX_EFFECT_DBL_PASS_VERT_WIPE_IN  // close match
#define EFFECT_VINTERLACE_RIGHT     EFX_EFFECT_VERT_BLIND             // close match
#define EFFECT_VINTERLACE_LEFT      EFX_EFFECT_VERT_BLIND             // close match
#define EFFECT_HINTERLACE_UP        EFX_EFFECT_HORZ_BLIND             // close match
#define EFFECT_HINTERLACE_DOWN      EFX_EFFECT_HORZ_BLIND             // close match
#define EFFECT_LAST                 (EFX_EFFECT_MAX - 1)

#define EFX_FRAME_INNER_INSET  0x0000
#define EFX_FRAME_INNER_RAISED 0x0001
#define EFX_FRAME_INNER_MASK   0x000F
#define EFX_FRAME_OUTER_INSET  0x0000
#define EFX_FRAME_OUTER_RAISED 0x0010
#define EFX_FRAME_OUTER_MASK   0x00F0
#define EFX_FRAME_ADJUST_RECT  0x0100

#define EFX_TEXT_NORMAL        0x0000
#define EFX_TEXT_INSETLIGHT    0x0001
#define EFX_TEXT_INSETHEAVY    0x0002
#define EFX_TEXT_RAISEDLIGHT   0x0003
#define EFX_TEXT_RAISEDHEAVY   0x0004
#define EFX_TEXT_DROPSHADOW    0x0005
#define EFX_TEXT_BLOCKSHADOW   0x0006
#define EFX_TEXT_OUTLINEBLOCK  0x0007
#define EFX_TEXT_STYLEMASK     0x000F
#define EFX_TEXT_MAX           8

#define EFX_TEXT_LEFT          0x0000
#define EFX_TEXT_HCENTER       0x0010
#define EFX_TEXT_RIGHT         0x0020
#define EFX_TEXT_HMASK         0x00F0

#define EFX_TEXT_TOP           0x0000
#define EFX_TEXT_VCENTER       0x0100
#define EFX_TEXT_BOTTOM        0x0200
#define EFX_TEXT_VMASK         0x0F00

#define EFX_TEXT_WORDWRAP      0x1000

enum
{
   EFX_SHAPE_RECT,
   EFX_SHAPE_SQUARE,
   EFX_SHAPE_OVAL,
   EFX_SHAPE_CIRCLE,
   EFX_SHAPE_ROUNDEDRECT,
   EFX_SHAPE_ROUNDEDSQUARE,
   EFX_SHAPE_OCTAGON,
   EFX_SHAPE_STAR,
   EFX_SHAPE_TRIANGLEEQ,
   EFX_SHAPE_TRIANGLERIGHT,
   EFX_SHAPE_TRIANGLEUP,
   EFX_SHAPE_TRIANGLELEFT,
   EFX_SHAPE_TRIANGLEDOWN,
   EFX_SHAPE_DIAMOND,
   EFX_SHAPE_PENTAGON,
   EFX_SHAPE_HEXAGON,
   EFX_SHAPE_LINE,
   EFX_SHAPE_REGION,
   EFX_SHAPE_MAX
};

enum
{
   EFX_BORDERSTYLE_TRANSPARENT,
   EFX_BORDERSTYLE_SOLID,
   EFX_BORDERSTYLE_DASH,
   EFX_BORDERSTYLE_DOT,
   EFX_BORDERSTYLE_DASHDOT,
   EFX_BORDERSTYLE_DASHDOTDOT,
};

enum
{
   EFX_FILLSTYLE_SOLID,
   EFX_FILLSTYLE_TRANSPARENT,
   EFX_FILLSTYLE_HORIZONTAL,
   EFX_FILLSTYLE_VERTICAL,
   EFX_FILLSTYLE_FDIAGONAL,
   EFX_FILLSTYLE_BDIAGONAL,
   EFX_FILLSTYLE_CROSS,
   EFX_FILLSTYLE_DIAGCROSS,
};

enum
{
   EFX_BACKSTYLE_TRANSPARENT,
   EFX_BACKSTYLE_OPAQUE,
   EFX_BACKSTYLE_TRANSLUCENT,
   EFX_BACKSTYLE_TILEDIMAGE,
   EFX_BACKSTYLE_TRANSLUCENTTILEDIMAGE,
};

enum {
   EFX_INNERSTYLE_NONE,
   EFX_INNERSTYLE_INSET,
   EFX_INNERSTYLE_RAISED,
};

enum {
   EFX_OUTERSTYLE_NONE,
   EFX_OUTERSTYLE_INSET,
   EFX_OUTERSTYLE_RAISED,
};

L_INT EXT_FUNCTION L_EfxPaintTransition (HDC hDC,
                                     L_UINT uTransition,
                                     COLORREF crBack,
                                     COLORREF crFore,
                                     L_UINT uSteps,
                                     RECT L_FAR *pDest,
                                     L_UINT uEffect,
                                     L_UINT uGrain,
                                     L_UINT uDelay,
                                     L_UINT uPass,
                                     L_UINT uMaxPass,
                                     L_BOOL fTransparency,
                                     COLORREF crTransparency,
                                     L_UINT uWandWidth,
                                     COLORREF crWand,
                                     L_UINT32 uROP);

L_INT EXT_FUNCTION L_EfxPaintBitmap (HDC hDC,
                                     pBITMAPHANDLE pBitmap,
                                     RECT L_FAR *pSrc,
                                     RECT L_FAR *pSrcClip,
                                     RECT L_FAR *pDest,
                                     RECT L_FAR *pDestClip,
                                     L_UINT uEffect,
                                     L_UINT uGrain,
                                     L_UINT uDelay,
                                     L_UINT uPass,
                                     L_UINT uMaxPass,
                                     L_BOOL fTransparency,
                                     COLORREF crTransparency,
                                     L_UINT uWandWidth,
                                     COLORREF crWand,
                                     L_UINT32 uROP);

L_INT EXT_FUNCTION L_EfxDrawFrame(HDC hDC,
                                  RECT L_FAR *pRect,
                                  L_UINT uFlags,
                                  L_UINT uFrameWidth,
                                  COLORREF crFrame,
                                  L_UINT uInnerWidth,
                                  COLORREF crInner1,
                                  COLORREF crInner2,
                                  L_UINT uOuterWidth,
                                  COLORREF crOuter1,
                                  COLORREF crOuter2);

L_INT EXT_FUNCTION L_EfxGradientFillRect(HDC hDC, RECT L_FAR *pRect,
                                         L_UINT uStyle, COLORREF crStart,
                                         COLORREF crEnd, L_UINT uSteps);

L_INT EXT_FUNCTION L_EfxPatternFillRect(HDC hDC, RECT L_FAR *pRect,
                                         L_UINT uStyle, COLORREF crBack,
                                         COLORREF crFore);

L_INT EXT_FUNCTION L_EfxDraw3dText(HDC hDC,
                                 LPSTR pszText,
                                 RECT L_FAR *pRect,
                                 L_UINT uFlags,
                                 L_INT nXDepth, L_INT nYDepth,
                                 COLORREF crText,
                                 COLORREF crShadow,
                                 COLORREF crHilite,
                                 HFONT hFont,
                                 HDC hdcFore);

L_INT EXT_FUNCTION L_EfxDrawRotated3dText(HDC hDC,
                                 LPSTR pszText,
                                 RECT L_FAR *pRect,
                                 L_INT nAngle,
                                 L_UINT uFlags,
                                 L_INT nXDepth, L_INT nYDepth,
                                 COLORREF crText,
                                 COLORREF crShadow,
                                 COLORREF crHilite,
                                 HFONT hFont,
                                 HDC hdcFore);

L_INT EXT_FUNCTION L_EfxDraw3dShape(HDC hDC,
                                 L_UINT uShape,
                                 RECT L_FAR *pRect,
                                 COLORREF crBack,
                                 HDC hdcBack,
                                 RECT L_FAR *prcBack,
                                 L_UINT uBackStyle,
                                 COLORREF crFill,
                                 L_UINT uFillStyle,
                                 COLORREF crBorder,
                                 L_UINT uBorderStyle,
                                 L_UINT uBorderWidth,
                                 COLORREF crInnerHilite,
                                 COLORREF crInnerShadow,
                                 L_UINT uInnerStyle,
                                 L_UINT uInnerWidth,
                                 COLORREF crOuterHilite,
                                 COLORREF crOuterShadow,
                                 L_UINT uOuterStyle,
                                 L_UINT uOuterWidth,
                                 L_INT nShadowX,
                                 L_INT nShadowY,
                                 COLORREF crShadow,
                                 HRGN hRgn);

L_INT EXT_FUNCTION L_EfxEffectBlt(HDC hdcDest,
                                  L_INT nXDest,
                                  L_INT nYDest,
                                  L_INT nWidth,
                                  L_INT nHeight,
                                  HDC hdcSrc,
                                  L_INT nXSrc,
                                  L_INT nYSrc,
                                  L_UINT uEffect,
                                  L_UINT uGrain,
                                  L_UINT uDelay,
                                  L_UINT uPass,
                                  L_UINT uMaxPass,
                                  L_BOOL fTransparency,
                                  COLORREF crTransparency,
                                  L_UINT uWandWidth,
                                  COLORREF crWand,
                                  L_UINT32 uROP);


L_INT EXT_FUNCTION L_PaintDCEffect (HDC hDC,
                                     pBITMAPHANDLE pBitmap,
                                     LPRECT pSrc,
                                     LPRECT pClipSrc,
                                     LPRECT pDst,
                                     LPRECT pClipDst,
                                     L_UINT32 uROP3,
                                     L_UINT uEffect);


L_INT EXT_FUNCTION L_PaintRgnDCEffect (HDC hDC,
                                       pBITMAPHANDLE pBitmap,
                                       LPRECT pSrc,
                                       LPRECT pClipSrc,
                                       LPRECT pDst,
                                       LPRECT pClipDst,
                                       L_UINT32 uROP3,
                                       L_UINT uEffect);

#undef _HEADER_ENTRY_
#include "ltpck.h"

#endif                          /* _LTEFX_H_ */

