#ifndef _L_OCX_H
#define _L_OCX_H

// Lead definition constants
// enumeration constants
#include "ltkey.h"
#include "Ltlck.h"

// ActiveX error codes
#include "L_OcxErr.h"

#define UNLOCKSUPPORT(leadocx) \
{ \
   (leadocx).UnlockSupport(L_SUPPORT_DOCUMENT                           , L_KEY_DOCUMENT                          ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR                                , L_KEY_OCR                               ); \
   (leadocx).UnlockSupport(L_SUPPORT_MEDICAL                            , L_KEY_MEDICAL                           ); \
   (leadocx).UnlockSupport(L_SUPPORT_MEDICAL_NET                        , L_KEY_MEDICAL_NET                       ); \
   (leadocx).UnlockSupport(L_SUPPORT_VECTOR                             , L_KEY_VECTOR                            ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_1D                        , L_KEY_BARCODES_1D                       ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_2D_READ                   , L_KEY_BARCODES_2D_READ                  ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_2D_WRITE                  , L_KEY_BARCODES_2D_WRITE                 ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_PDF_READ                  , L_KEY_BARCODES_PDF_READ                 ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_PDF_WRITE                 , L_KEY_BARCODES_PDF_WRITE                ); \
   (leadocx).UnlockSupport(L_SUPPORT_PDF                                , L_KEY_PDF                               ); \
   (leadocx).UnlockSupport(L_SUPPORT_J2K                                , L_KEY_J2K                               ); \
   (leadocx).UnlockSupport(L_SUPPORT_CMW                                , L_KEY_CMW                               ); \
   (leadocx).UnlockSupport(L_SUPPORT_DICOM                              , L_KEY_DICOM                             ); \
   (leadocx).UnlockSupport(L_SUPPORT_EXTGRAY                            , L_KEY_EXTGRAY                           ); \
   (leadocx).UnlockSupport(L_SUPPORT_BITONAL                            , L_KEY_BITONAL                           ); \
   (leadocx).UnlockSupport(L_SUPPORT_PDF_SAVE                           , L_KEY_PDF_SAVE                          ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_PDF_OUTPUT                     , L_KEY_OCR_PDF_OUTPUT                    ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_DATAMATRIX_READ           , L_KEY_BARCODES_DATAMATRIX_READ          ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_DATAMATRIX_WRITE          , L_KEY_BARCODES_DATAMATRIX_WRITE         ); \
   (leadocx).UnlockSupport(L_SUPPORT_LTPRO                              , L_KEY_LTPRO                             ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_PROFESSIONAL_ASIAN             , L_KEY_OCR_PROFESSIONAL_ASIAN            ); \
   (leadocx).UnlockSupport(L_SUPPORT_ICR                                , L_KEY_ICR                               ); \
   (leadocx).UnlockSupport(L_SUPPORT_OMR                                , L_KEY_OMR                               ); \
   (leadocx).UnlockSupport(L_SUPPORT_ABC                                , L_KEY_ABC                               ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_QR_READ                   , L_KEY_BARCODES_QR_READ                  ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_QR_WRITE                  , L_KEY_BARCODES_QR_WRITE                 ); \
   (leadocx).UnlockSupport(L_SUPPORT_BARCODES_1D_SILVER                 , L_KEY_BARCODES_1D_SILVER                ); \
   (leadocx).UnlockSupport(L_SUPPORT_MOBILE                             , L_KEY_MOBILE                            ); \
   (leadocx).UnlockSupport(L_SUPPORT_JBIG2                              , L_KEY_JBIG2                             ); \
   (leadocx).UnlockSupport(L_SUPPORT_ABIC_READ                          , L_KEY_ABIC_READ                         ); \
   (leadocx).UnlockSupport(L_SUPPORT_ABIC_SAVE                          , L_KEY_ABIC_SAVE                         ); \
   (leadocx).UnlockSupport(L_SUPPORT_PDF_READ                           , L_KEY_PDF_READ                          ); \
   (leadocx).UnlockSupport(L_SUPPORT_PDF_ADVANCED                       , L_KEY_PDF_ADVANCED                      ); \
   (leadocx).UnlockSupport(L_SUPPORT_NITF                               , L_KEY_NITF                              ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_ARABIC                         , L_KEY_OCR_ARABIC                        ); \
   (leadocx).UnlockSupport(L_SUPPORT_JPIP                               , L_KEY_JPIP                              ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_PLUS                           , L_KEY_OCR_PLUS                          ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_PLUS_PDF_OUTPUT                , L_KEY_OCR_PLUS_PDF_OUTPUT               ); \
   (leadocx).UnlockSupport(L_SUPPORT_ICR_PLUS                           , L_KEY_ICR_PLUS                          ); \
   (leadocx).UnlockSupport(L_SUPPORT_OMR_PLUS                           , L_KEY_OMR_PLUS                          ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_PLUS_PDF_LEAD_OUTPUT           , L_KEY_OCR_PLUS_PDF_LEAD_OUTPUT          ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_PROFESSIONAL                   , L_KEY_OCR_PROFESSIONAL                  ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_PROFESSIONAL_PDF_OUTPUT        , L_KEY_OCR_PROFESSIONAL_PDF_OUTPUT       ); \
   (leadocx).UnlockSupport(L_SUPPORT_ICR_PROFESSIONAL                   , L_KEY_ICR_PROFESSIONAL                  ); \
   (leadocx).UnlockSupport(L_SUPPORT_OMR_PROFESSIONAL                   , L_KEY_OMR_PROFESSIONAL                  ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_PROFESSIONAL_PDF_LEAD_OUTPUT   , L_KEY_OCR_PROFESSIONAL_PDF_LEAD_OUTPUT  ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_ADVANTAGE                      , L_KEY_OCR_ADVANTAGE                     ); \
   (leadocx).UnlockSupport(L_SUPPORT_OCR_ADVANTAGE_PDF_LEAD_OUTPUT      , L_KEY_OCR_ADVANTAGE_PDF_LEAD_OUTPUT     ); \
   (leadocx).UnlockSupport(L_SUPPORT_FORMS                              , L_KEY_FORMS                             ); \
}

#if !defined(USE_DLG_COM)
   enum AnimationDisposalConstants {
      ANIMATIONDISPOSAL_NONE,
      ANIMATIONDISPOSAL_LEAVE,
      ANIMATIONDISPOSAL_RESTOREBACKGROUND,
      ANIMATIONDISPOSAL_RESTOREPREVIOUS,
   };

   enum ViewPerspectiveConstants {
      TOP_LEFT        = 1,
      BOTTOM_LEFT     = 4,

      TOP_RIGHT       = 2,
      BOTTOM_LEFT180  = 2,
      BOTTOM_RIGHT    = 3,
      TOP_LEFT180     = 3,

      RIGHT_TOP       = 6,
      TOP_LEFT90      = 6,
      LEFT_BOTTOM     = 8,
      TOP_LEFT270     = 8,

      LEFT_TOP        = 5,
      BOTTOM_LEFT90   = 5,
      RIGHT_BOTTOM    = 7,
      BOTTOM_LEFT270  = 7,
   };

   enum ColorResPaletteConstants {
      CRP_FIXEDPALETTE = 0x0001,
      CRP_OPTIMIZEDPALETTE = 0x0002,
      CRP_BYTEORDERBGR = 0x0004,
      CRP_BYTEORDERRGB = 0x0000,
      CRP_IDENTITYPALETTE = 0x0008,
      CRP_USERPALETTE = 0x0010,
      CRP_NETSCAPEPALETTE = 0x0040,
      CRP_BYTEORDERGRAY = 0x0080,
   };

   enum ColorResDitherConstants {
      CRD_NODITHERING = 0x0000,
      CRD_FLOYDSTEINDITHERING = 0x0001,
      CRD_STUCKIDITHERING = 0x0002,
      CRD_BURKESDITHERING = 0x0003,
      CRD_SIERRADITHERING = 0x0004,
      CRD_STEVENSONARCEDITHERING = 0x0005,
      CRD_JARVISDITHERING = 0x0006,
      CRD_ORDEREDDITHERING = 0x0007,
      CRD_CLUSTEREDDITHERING = 0x0008,
   };

   enum SpatialFilterConstants {
      FLT_EMBOSS_N = 0,
      FLT_EMBOSS_NE = 1,
      FLT_EMBOSS_E = 2,
      FLT_EMBOSS_SE = 3,
      FLT_EMBOSS_S = 4,
      FLT_EMBOSS_SW = 5,
      FLT_EMBOSS_W = 6,
      FLT_EMBOSS_NW = 7,

      FLT_GRADIENT_N = 8,
      FLT_GRADIENT_NE = 9,
      FLT_GRADIENT_E = 10,
      FLT_GRADIENT_SE = 11,
      FLT_GRADIENT_S = 12,
      FLT_GRADIENT_SW = 13,
      FLT_GRADIENT_W = 14,
      FLT_GRADIENT_NW = 15,

      FLT_LAPLACE_1 = 16,
      FLT_LAPLACE_2 = 17,
      FLT_LAPLACE_3 = 18,
      FLT_LAPLACE_DIAG = 19,
      FLT_LAPLACE_HORZ = 20,
      FLT_LAPLACE_VERT = 21,

      FLT_SOBEL_HORZ = 22,
      FLT_SOBEL_VERT = 23,

      FLT_PREWITT_HORZ = 24,
      FLT_PREWITT_VERT = 25,

      FLT_SHIFTDIFF_DIAG = 26,
      FLT_SHIFTDIFF_HORZ = 27,
      FLT_SHIFTDIFF_VERT = 28,

      FLT_LINESEG_HORZ = 29,
      FLT_LINESEG_VERT = 30,
      FLT_LINESEG_LTOR = 31,
      FLT_LINESEG_RTOL = 32
   };

   enum HalftoneConstants {
      HALFTONE_PRINT = 0,
      HALFTONE_VIEW = 1,
      HALFTONE_RECT    = 2,
      HALFTONE_CIRC    = 3,
      HALFTONE_ELLIPS  = 4,
      HALFTONE_RAND    = 5,
      HALFTONE_LINEAR  = 6
   };

   enum QFactorConstants {
      QFACTOR_LEAD_0 = 0,
      QFACTOR_LEAD_1 = 1,
      QFACTOR_PQ1 = -1,
      QFACTOR_PQ2 = -2,
      QFACTOR_QFS = -3,
      QFACTOR_QMS = -4,
      QFACTOR_QS = -5,
      QFACTOR_SQS = -6,
      QFACTOR_SQT = -7,
      QFACTOR_MCQ = -8,
      QFACTOR_MC = -9,
      QFACTOR_ABCQ_LOSSLESS = 0,
      QFACTOR_ABCQ_VIRTUALLOSSLESS = 1,
      QFACTOR_ABCQ_REMOVEBORDER = 2,
      QFACTOR_ABCQ_ENHANCE = 3,
      QFACTOR_ABCQ_MODIFIED1 = 4,
      QFACTOR_ABCQ_MODIFIED2 = 5
   };

   enum BinaryFilterConstants {
      BFLT_EROSION_OMNI = 0,
      BFLT_EROSION_HORZ = 1,
      BFLT_EROSION_VERT = 2,
      BFLT_EROSION_DIAG = 3,
      BFLT_DILATION_OMNI = 4,
      BFLT_DILATION_HORZ = 5,
      BFLT_DILATION_VERT = 6,
      BFLT_DILATION_DIAG = 7
   };

   enum ColorSeparationConstants {
      COLORSEP_RGB   = 0x00,
      COLORSEP_CMYK  = 0x01,
      COLORSEP_HSV   = 0x02,
      COLORSEP_HLS   = 0x03,
      COLORSEP_CMY   = 0x04,
      COLORSEP_YUV   = 0x05,
      COLORSEP_XYZ   = 0x06,
      COLORSEP_LAB   = 0x07,
      COLORSEP_YCrCb = 0x08,
      COLORSEP_SCT   = 0x09
   };

   enum ChannelConstants {
      CHANNEL_MASTER = 0,
      CHANNEL_RED = 1,
      CHANNEL_GREEN = 2,
      CHANNEL_BLUE = 3
   };

   enum CombineConstants {
      CB_SRC_NOP = 0x0000,
      CB_SRC_NOT = 0x0001,
      CB_SRC_0 = 0x0002,
      CB_SRC_1 = 0x0003,

      CB_DST_NOP = 0x0000,
      CB_DST_NOT = 0x0010,
      CB_DST_0 = 0x0020,
      CB_DST_1 = 0x0030,

      CB_OP_AND = 0x0000,
      CB_OP_OR = 0x0100,
      CB_OP_XOR = 0x0200,
      CB_OP_ADD = 0x0300,
      CB_OP_SUBSRC = 0x0400,
      CB_OP_SUBDST = 0x0500,
      CB_OP_MUL = 0x0600,
      CB_OP_DIVSRC = 0x0700,
      CB_OP_DIVDST = 0x0800,
      CB_OP_AVG = 0x0900,
      CB_OP_MIN = 0x0A00,
      CB_OP_MAX = 0x0B00,

      CB_RES_NOP = 0x0000,
      CB_RES_NOT = 0x1000,
      CB_RES_0 = 0x2000,
      CB_RES_1 = 0x3000,

      CB_SRC_MASTER = 0x00000000,
      CB_SRC_RED    = 0x00010000,
      CB_SRC_GREEN  = 0x00020000,
      CB_SRC_BLUE   = 0x00030000,

      CB_DST_MASTER = 0x00000000,
      CB_DST_RED    = 0x00100000,
      CB_DST_GREEN  = 0x00200000,
      CB_DST_BLUE   = 0x00300000,

      CB_RES_MASTER = 0x00000000,
      CB_RES_RED    = 0x01000000,
      CB_RES_GREEN  = 0x02000000,
      CB_RES_BLUE   = 0x03000000,
      CB_OP_ABSDIF  = 0x00000C00
   };

   enum  CombineBitmapWarpConstants {
      CBW_BILINEAR   =	0x0001,
      CBW_RESERVED1  =	0x0002,
      CBW_RESERVED2  =	0x0004,
      CBW_RESERVED3  =	0x0008,
      CBW_RESERVED4  =	0x0010,
      CBW_RESERVED5  =	0x0020,
      CBW_RESERVED6  =	0x0040,
      CBW_RESERVED7  =	0x0080,
      CBW_RESERVED8  =	0x0100,
      CBW_RESERVED9  =	0x0200
   };

   enum EmbossConstants {
      EMBOSS_N = 0,
      EMBOSS_NE = 1,
      EMBOSS_E = 2,
      EMBOSS_SE = 3,
      EMBOSS_S = 4,
      EMBOSS_SW = 5,
      EMBOSS_W = 6,
      EMBOSS_NW = 7
   };

   enum CopyConstants {
      COPY_EMPTY = 0x0001,
      COPY_DIB = 0x0002,
      COPY_DDB = 0x0004,
      COPY_PALETTE = 0x0008,
      COPY_RGN = 0x0010
   };

   enum UnderlayConstants {
      UNDERLAY_TILE = 0,
      UNDERLAY_STRETCH = 1
   };

   enum FileConstants {
      FILE_PCX = 1,
      FILE_GIF = 2,
      FILE_TIF = 3,
      FILE_TGA = 4,
      FILE_CMP = 5,
      FILE_BMP = 6,
      FROM_BUFFER = 7,
      FILE_BITMAP = 9,
      FILE_JPEG = 10,
      FILE_TIF_JPEG = 11,
      FILE_BIN = 12,
      FILE_HANDLE = 13,
      FILE_OS2 = 14,
      FILE_WMF = 15,
      FILE_EPS = 16,
      FILE_TIFLZW = 17,
      FILE_LEAD = 20,
      FILE_JPEG_411 = 21,
      FILE_TIF_JPEG_411 = 22,
      FILE_JPEG_422 = 23,
      FILE_TIF_JPEG_422 = 24,
      FILE_CCITT = 25,
      FILE_LEAD1BIT = 26,
      FILE_CCITT_GROUP3_1DIM = 27,
      FILE_CCITT_GROUP3_2DIM = 28,
      FILE_CCITT_GROUP4 = 29,
      FILE_LEAD_NOLOSS = 30,
      FILE_ABC = 32,
      FILE_CALS = 50,
      FILE_MAC = 51,
      FILE_IMG = 52,
      FILE_MSP = 53,
      FILE_WPG = 54,
      FILE_RAS = 55,
      FILE_PCT = 56,
      FILE_PCD = 57,
      FILE_DXF = 58,
      FILE_AVI = 59,
      FILE_WAV = 60,
      FILE_FLI = 61,
      FILE_CGM = 62,
      FILE_EPSTIFF = 63,
      FILE_EPSWMF = 64,
      FILE_CMPNOLOSS = 65,
      FILE_FAX_G3_1D = 66,
      FILE_FAX_G3_2D = 67,
      FILE_FAX_G4 = 68,
      FILE_WFX_G3_1D = 69,
      FILE_WFX_G4 = 70,
      FILE_ICA_G3_1D = 71,
      FILE_ICA_G3_2D = 72,
      FILE_ICA_G4 = 73,
      FILE_OS2_2 = 74,
      FILE_PNG = 75,
      FILE_PSD = 76,
      FILE_FPX = 80,
      FILE_RAWICA_G3_1D = 77,
      FILE_RAWICA_G3_2D = 78,
      FILE_RAWICA_G4 = 79,
      FILE_FPX_SINGLE_COLOR = 81,
      FILE_FPX_JPEG = 82,
      FILE_FPX_JPEG_QFACTOR = 83,
      FILE_BMP_RLE = 84,
      FILE_TIF_CMYK = 85,
      FILE_TIFLZW_CMYK = 86,
      FILE_TIF_PACKBITS = 87,
      FILE_TIF_PACKBITS_CMYK = 88,
      FILE_DICOM_GRAY = 89,
      FILE_DICOM_COLOR = 90,
      FILE_WIN_ICO = 91,
      FILE_WIN_CUR = 92,
      FILE_TIF_YCC = 93,
      FILE_TIFLZW_YCC = 94,
      FILE_TIF_PACKBITS_YCC = 95,
      FILE_EXIF = 96,
      FILE_EXIF_YCC = 97,
      FILE_EXIF_JPEG = 98,
      FILE_AWD = 99,
      FILE_FASTEST = 100,
      FILE_EXIF_JPEG_411 = 101,
      FILE_PBM_ASCII = 102,
      FILE_PBM_BINARY = 103,
      FILE_PGM_ASCII = 104,
      FILE_PGM_BINARY = 105,
      FILE_PPM_ASCII = 106,
      FILE_PPM_BINARY = 107,
      FILE_CUT = 108,
      FILE_XPM = 109,
      FILE_XBM = 110,
      FILE_IFF_ILBM = 111,
      FILE_IFF_CAT = 112,
      FILE_XWD = 113,
      FILE_CLP = 114,
      FILE_JBIG = 115,
      FILE_EMF = 116,
      FILE_ICA_IBM_MMR = 117,
      FILE_RAWICA_IBM_MMR = 118,
      FILE_ANI = 119,
      FILE_ANI_RLE = 120,
      FILE_LASERDATA = 121,
      FILE_INTERGRAPH_RLE = 122,
      FILE_INTERGRAPH_VECTOR = 123,
      FILE_DWG = 124,
      FILE_DICOM_RLE_GRAY = 125,
      FILE_DICOM_RLE_COLOR = 126,
      FILE_DICOM_JPEG_GRAY = 127,
      FILE_DICOM_JPEG_COLOR = 128,
      FILE_CALS4 = 129,
      FILE_CALS2 = 130,
      FILE_CALS3 = 131,
      FILE_XWD10 = 132,
      FILE_XWD11 = 133,
      FILE_FLC = 134,
      FILE_KDC = 135,
      FILE_DRW = 136,
      FILE_PLT = 137,
      FILE_TIF_CMP = 138,
      FILE_TIF_JBIG = 139,
      FILE_TIF_DXF = 140,
      FILE_TIF_UNKNOWN = 141,
      FILE_SGI = 142,
      FILE_SGI_RLE = 143,
      FILE_VECTOR_DUMP = 144,
      FILE_DWF = 145,
      FILE_RAS_PDF          = 146,
      FILE_RAS_PDF_G3_1D    = 147,
      FILE_RAS_PDF_G3_2D    = 148,
      FILE_RAS_PDF_G4       = 149,
      FILE_RAS_PDF_JPEG     = 150,
      FILE_RAS_PDF_JPEG_422 = 151,
      FILE_RAS_PDF_JPEG_411 = 152,
      FILE_RAW = 153,

      FILE_RASTER_DUMP = 154,
      FILE_TIFF_CUSTOM = 155,

      FILE_RAW_RGB = 156,
      FILE_RAW_RLE4 = 157,
      FILE_RAW_RLE8 = 158,
      FILE_RAW_BITFIELDS = 159,
      FILE_RAW_PACKBITS = 160,
      FILE_RAW_JPEG = 161,
      FILE_FAX_G3_1D_NOEOL = 162,
      FILE_RAW_CCITT = 162,

      FILE_JP2 = 163,
      FILE_J2K = 164,
      FILE_CMW = 165,

      FILE_TIF_J2K = 166,
      FILE_TIF_CMW = 167,
      FILE_MRC = 168,
      FILE_GERBER = 169,

      FILE_WBMP               = 170,
      FILE_JPEG_LAB           = 171,
      FILE_JPEG_LAB_411      = 172,
      FILE_JPEG_LAB_422      = 173,
      FILE_TIF_ABC           = 180,
      FILE_AMI_PRO_20             = 200,
      FILE_AMI_PRO_30             = 201,
      FILE_ASCII_SMART            = 202,
      FILE_ASCII_STANDARD         = 203,
      FILE_ASCII_STANDARD_DOS     = 204,
      FILE_ASCII_STRIPPED         = 205,
      FILE_DBASE_IV_10            = 206,
      FILE_DCA_RFT                = 207,
      FILE_DCA_RFT_DW_5           = 208,
      FILE_EXCEL_MAC              = 209,
      FILE_EXCEL_30               = 210,
      FILE_EXCEL_40               = 211,
      FILE_EXCEL_50               = 212,
      FILE_EXCEL_OFFICE97         = 213,
      FILE_FRAMEMAKER             = 214,
      FILE_HTML_20                = 215,
      FILE_HTML_EDITOR_20         = 216,
      FILE_HTML_NETSCAPE_20       = 217,
      FILE_INTERLEAF              = 218,
      FILE_LOTUS123               = 219,
      FILE_LOTUS_WORD_PRO         = 220,
      FILE_MULTIMATE_ADV_II       = 221,
      FILE_POSTSCRIPT             = 222,
      FILE_PROFESSIONAL_WRITE_20  = 223,
      FILE_PROFESSIONAL_WRITE_22  = 224,
      FILE_QUATTRA_PRO            = 225,
      FILE_RTF                    = 226,
      FILE_RTF_MAC                = 227,
      FILE_RTF_WORD_60            = 228,
      FILE_WINDOWS_WRITE          = 229,
      FILE_WORD_WINDOWS_2X        = 230,
      FILE_WORD_WINDOWS_60        = 231,
      FILE_WORD_OFFICE97          = 232,
      FILE_WORDPERFECT_DOS_42     = 233,
      FILE_WORDPERFECT_WINDOWS    = 234,
      FILE_WORDPERFECT_WINDOWS_60 = 235,
      FILE_WORDPERFECT_WINDOWS_61 = 236,
      FILE_WORDPERFECT_WINDOWS_7X = 237,
      FILE_WORDSTAR_WINDOWS_1X    = 238,
      FILE_WORKS                  = 239,
      FILE_XDOC                   = 240,
      FILE_MOV                    = 241,
      FILE_MIDI                   = 242,
      FILE_MPEG1                  = 243,
      FILE_AU                     = 244,
      FILE_AIFF                   = 245,
      FILE_MPEG2                  = 246,
      FILE_SVG                    = 247,
      FILE_NITF                   = 248,
      FILE_PTOCA                  = 249,
      FILE_SCT                    = 250,
      FILE_PCL                    = 251,
      FILE_AFP                    = 252,
      FILE_ICA_UNCOMPRESSED       = 253,
      FILE_RAWICA_UNCOMPRESSED    = 254,
      FILE_SHP                    = 255,
      FILE_SMP                    = 256,
      FILE_SMP_G3_1D              = 257,
      FILE_SMP_G3_2D              = 258,
      FILE_SMP_G4                 = 259,
                                       
      FILE_VWPG                   = 260,
      FILE_CMX                    = 261,
                                       
      FILE_TGA_RLE                = 262,

      FILE_KDC_120                = 263,
      FILE_KDC_40                 = 264,
      FILE_KDC_50                 = 265,
      FILE_DCS                    = 266,
                                       
      FILE_PSP                    = 267,
      FILE_PSP_RLE                = 268,
                                       
      FILE_TIFX_JBIG              = 269,
      FILE_TIFX_JBIG_T43          = 270,
      FILE_TIFX_JBIG_T43_ITULAB   = 271,
      FILE_TIFX_JBIG_T43_GS       = 272,
      FILE_TIFX_FAX_G4            = 273,
      FILE_TIFX_FAX_G3_1D         = 274,
      FILE_TIFX_FAX_G3_2D         = 275,
      FILE_TIFX_JPEG              = 276,

      FILE_ECW                    = 277,
      FILE_RAS_RLE                = 288,
      FILE_SVG_EMBED_IMAGES       = 289,
      FILE_DXF_R13                = 290,
      FILE_CLP_RLE                = 291,
      FILE_DCR                    = 292,

      FILE_DICOM_J2K_GRAY         = 293,
      FILE_DICOM_J2K_COLOR        = 294,

      FILE_FIT                    = 295,
      FILE_CRW                    = 296,
      FILE_DWF_TEXT_AS_POLYLINE   = 297,
      FILE_CIN                    = 298,
      FILE_PCL_TEXT_AS_POLYLINE   = 299,
      FILE_EPSPOSTSCRIPT          = 300,
      FILE_INTERGRAPH_CCITT_G4    = 301,

      FILE_SFF                    = 302,
      FILE_IFF_ILBM_UNCOMPRESSED  = 303,
      FILE_IFF_CAT_UNCOMPRESSED   = 304,
      FILE_RTF_RASTER             = 305,
      FILE_SID                    = 306,
      FILE_WMZ                    = 307,
      FILE_DJVU                   = 308,
      FILE_AFPICA_G3_1D           = 309,
      FILE_AFPICA_G3_2D           = 310,
      FILE_AFPICA_G4              = 311,
      FILE_AFPICA_UNCOMPRESSED    = 312,
      FILE_AFPICA_IBM_MMR         = 313,

      FILE_LEAD_MRC               = 314,
      FILE_TIF_LEAD_MRC           = 315,

      FILE_TXT                    = 316,

      FILE_PDF_LEAD_MRC           = 317,

      FILE_HDP                    = 318,
      FILE_HDP_GRAY               = 319,
      FILE_HDP_CMYK               = 320,

      FILE_PNG_ICO                = 321,
      FILE_XPS                    = 322,
      FILE_JPX                    = 323,

      FILE_XPS_JPEG               = 324,
      FILE_XPS_JPEG_422           = 325,
      FILE_XPS_JPEG_411           = 326,

      FILE_MNG                    = 327,
      FILE_MNG_GRAY               = 329,
      FILE_MNG_JNG                = 330,
      FILE_MNG_JNG_411            = 331,
      FILE_MNG_JNG_422            = 332,

      FILE_RAS_PDF_CMYK           = 333,
      FILE_RAS_PDF_LZW_CMYK       = 334,

      FILE_MIF                    = 335,
   };

   enum GetFileInfoFlags {
      FILEINFO_TOTALPAGES        = 0x00000001,  /* Update InfoTotalPages */
   };

   enum ResizeConstants {
      RESIZE_NORMAL = 0x0000,
      RESIZE_FAVORBLACK = 0x0001,
      RESIZE_RESAMPLE = 0x0002,
      RESIZE_FAVORBLACK_OR_RESAMPLE = 0x0003,
      RESIZE_BICUBIC = 0x0004,
      RESIZE_FAVORBLACK_OR_BICUBIC = 0x0005,
   };

   enum PasteConstants {
      PASTE_ISREADY = 0x4000
   };

   enum PCDResConstants {
      PCDRES_BASE_OVER_64,
      PCDRES_BASE_OVER_16,
      PCDRES_BASE_OVER_4,
      PCDRES_BASE,
      PCDRES_4BASE,
      PCDRES_16BASE
   };

   enum IsGrayscaleConstants {
      GRAY_NO              = 0,
      GRAY_ORDEREDNORMAL   = 1,
      GRAY_ORDEREDREVERSE  = 2,
      GRAY_NOTORDERED      = 3,
   };

   enum RgnCombineModeConstants {
      L_RGN_AND,
      L_RGN_SET,
      L_RGN_ANDNOTBITMAP,
      L_RGN_ANDNOTRGN,
      L_RGN_OR,
      L_RGN_XOR,
      L_RGN_SETNOT,
   };

   enum PolygonFillModeConstants {  /* FillMode in SetRgnPolygon */
      L_POLY_WINDING,
      L_POLY_ALTERNATE
   };

   enum LoadInfoFlagConstants {
      LOADINFO_TOPLEFT         = 0x00000001,
      LOADINFO_ORDERRGB        = 0x00000002,
      LOADINFO_WHITEONBLACK    = 0x00000004,
      LOADINFO_LSB             = 0x00000008,
      LOADINFO_TOPLEFT90       = 0x00000010,
      LOADINFO_TOPLEFT270      = 0x00000020,
      LOADINFO_REVERSE         = 0x00000040,

      LOADINFO_TOPLEFT180      = 0x00000080,
      LOADINFO_BOTTOMLEFT90    = 0x00000100,
      LOADINFO_BOTTOMLEFT180   = 0x00000200,
      LOADINFO_BOTTOMLEFT270   = 0x00000400,
      LOADINFO_PAD4            = 0x00000800,

      LOADINFO_PALETTE         = 0x00001000,

      LOADINFO_BITFIELDS       = 0x00002000,
      LOADINFO_ORDERGRAY       = 0x00004000,
      LOADINFO_MOTOROLAORDER   = 0x00008000,
      LOADINFO_ORDERROMM       = 0x00010000,
      LOADINFO_SIGNED          = 0x00020000,
   };

   enum ProgressivePassesConstants {
      PROGRESSIVEPASSES_WHEN_MEANINGFUL = -1,
      PROGRESSIVEPASSES_ALWAYS = -2,
      PROGRESSIVEPASSES_ONCE = 0,
   };

   enum CommentConstants {
      CMNT_SZARTIST                                    = 0,     // Person who created image
      CMNT_SZCOPYRIGHT                                 = 1,     // Copyright notice
      CMNT_SZDATETIME                                  = 2,     // "YYYY:MM:DD HH:MM:SS" format
      CMNT_SZDESC                                      = 3,     // Description of image
      CMNT_SZHOSTCOMP                                  = 4,     // Computer/OP System in use
      CMNT_SZMAKE                                      = 5,     // Manufacturer of Equip. used generate the image
      CMNT_SZMODEL                                     = 6,     // Model Name/Number of Equipment
      CMNT_SZNAMEOFDOC                                 = 7,     // Doc name image was scanned from
      CMNT_SZNAMEOFPAGE                                = 8,     // Page name image was scanned from
      CMNT_SZSOFTWARE                                  = 9,     // Name & Version of Software Package used to gen the image
      CMNT_SZPATIENTNAME                               = 10,    // Patient name (DICOM)
      CMNT_SZPATIENTID                                 = 11,    // Patient ID (DICOM)
      CMNT_SZPATIENTBIRTHDATE                          = 12,    // Patient birthdate (DICOM)
      CMNT_SZPATIENTSEX                                = 13,    // Patient sex (DICOM)
      CMNT_SZSTUDYINSTANCE                             = 14,    // Study instance ID (DICOM)
      CMNT_SZSTUDYDATE                                 = 15,    // Study date (DICOM)
      CMNT_SZSTUDYTIME                                 = 16,    // Study time (DICOM)
      CMNT_SZSTUDYREFERRINGPHYSICIAN                   = 17,    // Referring physician (DICOM)
      CMNT_SZSERIESMODALITY                            = 18,    // Series modality (DICOM)
      CMNT_SZSERIESID                                  = 19,    // Series ID (DICOM)
      CMNT_SZSERIESNUMBER                              = 20,    // Series number (DICOM)

      // Exif 1.0 and 1.1 comments
      CMNT_EXIFVERSION                                 = 21,    // Exif version
      CMNT_SZDATETIMEORIGINAL                          = 22,    // Date and time the original image image is captured (Exif)
      CMNT_SZDATETIMEDIGITIZED                         = 23,    // Date and time the file is generated (Exif)
      CMNT_SHUTTERSPEEDVALUE                           = 24,    // Shutter speed (Exif)
      CMNT_APERTURE                                    = 25,    // Aperture value (Exif)
      CMNT_BRIGHTNESS                                  = 26,    // Brightness value (Exif)
      CMNT_EXPOSUREBIAS                                = 27,    // Exposure bias (Exif)
      CMNT_MAXAPERTURE                                 = 28,    // Minimum lens f-number (Exif)
      CMNT_SUBJECTDISTANCE                             = 29,    // Distance from lens to subject (m) (Exif)
      CMNT_METERINGMODE                                = 30,    // Photometry mode (Exif)
      CMNT_LIGHTSOURCE                                 = 31,    // Light source (Exif)
      CMNT_FLASH                                       = 32,    // Flash On/Off (Exif)
      CMNT_FOCALLENGTH                                 = 33,    // Focal length (Exif)
      CMNT_EXPOSURETIME                                = 34,    // Exposure  (Exif)
      CMNT_FNUMBER                                     = 35,    // F-numnber (Exif)
      CMNT_MAKERNOTE                                   = 36,    // Maker note (Exif)
      CMNT_USERCOMMENT                                 = 37,    // User comment (Exif)
      CMNT_SZSUBSECTIME                                = 38,    // Date Time subsec (Exif)
      CMNT_SZSUBSECTIMEORIGINAL                        = 39,    // Date Time original subsec (Exif)
      CMNT_SZSUBSECTIMEDIGITIZED                       = 40,    // Date Time digitized subsec (Exif)

      // Exif 2.0 comments - comments introduced since Exif 1.1
      CMNT_SUPPORTEDFLASHPIXVERSION                    = 158,   // Supported FlashPix version (Exif)
      CMNT_COLORSPACE                                  = 159,   // Color space (Exif)
      CMNT_EXPOSUREPROGRAM                             = 160,   // Exposure program (Exif)
      CMNT_SZSPECTRALSENSITIVITY                       = 161,   // Spectral sensitivity (Exif)
      CMNT_ISOSPEEDRATINGS                             = 162,   // ISO speed ratings (Exif)
      CMNT_OPTOELECTRICCOEFFICIENT                     = 163,   // Optoelectric coefficient (Exif)
      CMNT_SZRELATEDSOUNDFILE                          = 164,   // Related audio file (Exif)
      CMNT_FLASHENERGY                                 = 165,   // Flash energy (Exif)
      CMNT_SPATIALFREQUENCYRESPONSE                    = 166,   // Spatial frequency response (Exif)
      CMNT_FOCALPLANEXRESOLUTION                       = 167,   // Focal plane X Resolution (Exif)
      CMNT_FOCALPLANEYRESOLUTION                       = 168,   // Focal plane Y Resolution (Exif)
      CMNT_FOCALPLANERESOLUTIONUNIT                    = 245,   // Focal plane Resolution Unit (Exif)
      CMNT_SUBJECTLOCATION                             = 169,   // Subject location (Exif)
      CMNT_EXPOSUREINDEX                               = 170,   // Exposure index (Exif)
      CMNT_SENSINGMETHOD                               = 171,   // Sensing method (Exif)
      CMNT_FILESOURCE                                  = 172,   // File source (Exif)
      CMNT_SCENETYPE                                   = 173,   // Scene type (Exif)
      CMNT_CFAPATTERN                                  = 174,   // CFA Pattern (Exif)

      // Exif 2.2 comments - comments introduced since Exif 2.0
      CMNT_SUBJECTAREA                                 = 227,
      CMNT_CUSTOMRENDERED                              = 228,
      CMNT_EXPOSUREMODE                                = 229,
      CMNT_WHITEBALANCE                                = 230,
      CMNT_DIGITALZOOMRATIO                            = 231,
      CMNT_FOCALLENGTHIN35MMFILM                       = 232,
      CMNT_SCENECAPTURETYPE                            = 233,
      CMNT_GAINCONTROL                                 = 234,
      CMNT_CONTRAST                                    = 235,
      CMNT_SATURATION                                  = 236,
      CMNT_SHARPNESS                                   = 237,
      CMNT_DEVICESETTINGDESCRIPTION                    = 238,
      CMNT_SUBJECTDISTANCERANGE                        = 239,
      CMNT_SZIMAGEUNIQUEID                             = 240,

      /* Exif 2.21 comments - comments introduced since Exif 2.2 */
      CMNT_GAMMA                                       = 246,

      // Exif 1.1 GPS comments
      CMNT_GPSVERSIONID                                = 41,
      CMNT_GPSLATITUDEREF                              = 42,
      CMNT_GPSLATITUDE                                 = 43,
      CMNT_GPSLONGITUDEREF                             = 44,
      CMNT_GPSLONGITUDE                                = 45,
      CMNT_GPSALTITUDEREF                              = 46,
      CMNT_GPSALTITUDE                                 = 47,
      CMNT_GPSTIMESTAMP                                = 48,
      CMNT_GPSSATELLITES                               = 49,
      CMNT_GPSSTATUS                                   = 50,
      CMNT_GPSMEASUREMODE                              = 51,
      CMNT_GPSDOP                                      = 52,
      CMNT_GPSSPEEDREF                                 = 53,
      CMNT_GPSSPEED                                    = 54,
      CMNT_GPSTRACKREF                                 = 55,
      CMNT_GPSTRACK                                    = 56,
      CMNT_GPSIMGDIRECTIONREF                          = 57,
      CMNT_GPSIMGDIRECTION                             = 58,
      CMNT_GPSMAPDATUM                                 = 59,
      CMNT_GPSDESTLATITUDEREF                          = 60,
      CMNT_GPSDESTLATITUDE                             = 61,
      CMNT_GPSDESTLONGITUDEREF                         = 62,
      CMNT_GPSDESTLONGITUDE                            = 63,
      CMNT_GPSDESTBEARINGREF                           = 64,
      CMNT_GPSDESTBEARING                              = 65,
      CMNT_GPSDESTDISTANCEREF                          = 66,
      CMNT_GPSDESTDISTANCE                             = 67,

      // Exif 2.2 comments - comments introduced since Exif, 2.0
      CMNT_GPSPROCESSINGMETHOD                         = 241,
      CMNT_GPSAREAINFORMATION                          = 242,
      CMNT_GPSDATESTAMP                                = 243,
      CMNT_GPSDIFFERENTIAL                             = 244,

      CMNT_FPXSUMMARYINFORMATION                       = 0x8001,
      CMNT_FPXTITLE                                    = 68,
      CMNT_FPXSUBJECT                                  = 69,
      CMNT_FPXAUTHOR                                   = 70,
      CMNT_FPXKEYWORDS                                 = 71,
      CMNT_FPXCOMMENTS                                 = 72,
      CMNT_FPXOLETEMPLATE                              = 73,
      CMNT_FPXLASTAUTHOR                               = 74,
      CMNT_FPXREVNUMBER                                = 75,
      CMNT_FPXEDITTIME                                 = 76,
      CMNT_FPXLASTPRINTED                              = 77,
      CMNT_FPXCREATEDTM                                = 78,
      CMNT_FPXLASTSAVEDTM                              = 79,
      CMNT_FPXPAGECOUNT                                = 80,
      CMNT_FPXWORDCOUNT                                = 81,
      CMNT_FPXCHARCOUNT                                = 82,
      CMNT_FPXTHUMBNAIL                                = 83,
      CMNT_FPXAPPNAME                                  = 84,
      CMNT_FPXSECURITY                                 = 85,
      CMNT_FPXSUMMARYINFORMATION1                      = CMNT_FPXTITLE,
      CMNT_FPXSUMMARYINFORMATION2                      = CMNT_FPXSECURITY,

      CMNT_FPXFILESOURCEGROUP                          = 0x8002,
      CMNT_FPXFILESOURCE                               = 86,
      CMNT_FPXSCENETYPE                                = 87,
      CMNT_FPXCREATIONPATH                             = 88,
      CMNT_FPXNAMEMANRELEASE                           = 89,
      CMNT_FPXUSERDEFINEDID                            = 90,
      CMNT_FPXORIGINALSHARPNESSAPPROXIMATION           = 91,
      CMNT_FPXFILESOURCEGROUP1                         = CMNT_FPXFILESOURCE,
      CMNT_FPXFILESOURCEGROUP2                         = CMNT_FPXORIGINALSHARPNESSAPPROXIMATION,

      CMNT_FPXINTELLECTUALPROPERTYGROUP                = 0x8004,
      CMNT_FPXCOPYRIGHT                                = 92,
      CMNT_FPXLEGALBROKERFORORIGIMAGE                  = 93,
      CMNT_FPXLEGALBROKERFORDIGITALIMAGE               = 94,
      CMNT_FPXAUTHORSHIP                               = 95,
      CMNT_FPXINTELLECTUALPROPNOTES                    = 96,
      CMNT_FPXINTELLECTUALPROPERTYGROUP1               = CMNT_FPXCOPYRIGHT,
      CMNT_FPXINTELLECTUALPROPERTYGROUP2               = CMNT_FPXINTELLECTUALPROPNOTES,

      CMNT_FPXCONTENTDESCRIPTIONGROUP                  = 0x8008,
      CMNT_FPXTESTTARGETINTHEIMAGE                     = 97,
      CMNT_FPXGROUPCAPTION                             = 98,
      CMNT_FPXCAPTIONTEXT                              = 99,
      CMNT_FPXPEOPLEINTHEIMAGE                         = 100,
      CMNT_FPXTHINGSINIMAGE                            = 101,
      CMNT_FPXDATEOFORIGINALIMAGE                      = 102,
      CMNT_FPXEVENTSINTHEIMAGE                         = 103,
      CMNT_FPXPLACESINTHE                              = 104,
      CMNT_FPXCONTENTDESCRIPTIONNOTES                  = 105,
      CMNT_FPXCONTENTDESCRIPTIONGROUP1                 = CMNT_FPXTESTTARGETINTHEIMAGE,
      CMNT_FPXCONTENTDESCRIPTIONGROUP2                 = CMNT_FPXCONTENTDESCRIPTIONNOTES,

      CMNT_FPXCAMERAINFORMATIONGROUP                   = 0x8010,
      CMNT_FPXCAMERAMANUFACTURERNAME                   = 106,
      CMNT_FPXCAMERAMODELNAME                          = 107,
      CMNT_FPXCAMERASERIALNUMBER                       = 108,
      CMNT_FPXCAMERAINFORMATIONGROUP1                  = CMNT_FPXCAMERAMANUFACTURERNAME,
      CMNT_FPXCAMERAINFORMATIONGROUP2                  = CMNT_FPXCAMERASERIALNUMBER,

      CMNT_FPXPERPICTURECAMERASETTINGSGROUP            = 0x8020,
      CMNT_FPXCAPTUREDATE                              = 109,
      CMNT_FPXEXPOSURETIME                             = 110,
      CMNT_FPXFNUMBER                                  = 111,
      CMNT_FPXEXPOSUREPROGRAM                          = 112,
      CMNT_FPXBRIGHTNESSVALUE                          = 113,
      CMNT_FPXEXPOSUREBIASVALUE                        = 114,
      CMNT_FPXSUBJECTDISTANCE                          = 115,
      CMNT_FPXMETERINGMODE                             = 116,
      CMNT_FPXSCENEILLUMINANT                          = 117,
      CMNT_FPXFOCALLENGTH                              = 118,
      CMNT_FPXMAXIMUMAPERATUREVALUE                    = 119,
      CMNT_FPXFLASH                                    = 120,
      CMNT_FPXFLASHENERGY                              = 121,
      CMNT_FPXFLASHRETURN                              = 122,
      CMNT_FPXBACKLIGHT                                = 123,
      CMNT_FPXSUBJECTLOCATION                          = 124,
      CMNT_FPXEXPOSUREINDEX                            = 125,
      CMNT_FPXSPECIALEFFECTSOPTICALFILTER              = 126,
      CMNT_FPXPERPICTURENOTES                          = 127,
      CMNT_FPXPERPICTURECAMERASETTINGSGROUP1           = CMNT_FPXCAPTUREDATE,
      CMNT_FPXPERPICTURECAMERASETTINGSGROUP2           = CMNT_FPXPERPICTURENOTES,

      CMNT_FPXDIGITALCAMERACHARACTERIZATIONGROUP       = 0x8040,
      CMNT_FPXSENSINGMETHOD                            = 128,
      CMNT_FPXFOCALPLANEXRESOLUTION                    = 129,
      CMNT_FPXFOCALPLANEYRESOLUTION                    = 130,
      CMNT_FPXFOCALPLANERESOLUTIONUNIT                 = 131,
      CMNT_FPXSPACIALFREQUENCY                         = 132,
      CMNT_FPXCFAPATTERN                               = 133,
      CMNT_FPXSPECTRALSENSITIVITY                      = 134,
      CMNT_FPXISOSPEEDRATINGS                          = 135,
      CMNT_FPXOECF                                     = 136,
      CMNT_FPXDIGITALCAMERACHARACTERIZATIONGROUP1      = CMNT_FPXSENSINGMETHOD,
      CMNT_FPXDIGITALCAMERACHARACTERIZATIONGROUP2      = CMNT_FPXOECF,

      CMNT_FPXFILMDESCRIPTIONGROUP                     = 0x8080,
      CMNT_FPXFILMBRAND                                = 137,
      CMNT_FPXFILMCATEGORY                             = 138,
      CMNT_FPXFILMSIZEX                                = 139,
      CMNT_FPXFILMSIZEY                                = 140,
      CMNT_FPXFILMSIZEUNIT                             = 141,
      CMNT_FPXFILMROLLNUMBER                           = 142,
      CMNT_FPXFILMFRAMENUMBER                          = 143,
      CMNT_FPXFILMDESCRIPTIONGROUP1                    = CMNT_FPXFILMBRAND,
      CMNT_FPXFILMDESCRIPTIONGROUP2                    = CMNT_FPXFILMFRAMENUMBER,

      CMNT_FPXORIGINALDOCUMENTSCANDESCRIPTIONGROUP     = 0x8100,
      CMNT_FPXORIGINALSCANNEDIMAGESIZE                 = 144,
      CMNT_FPXORIGINALDOCUMENTSIZE                     = 145,
      CMNT_FPXORIGINALMEDIUM                           = 146,
      CMNT_FPXTYPEOFREFLECTIONORIGINAL                 = 147,
      CMNT_FPXORIGINALDOCUMENTSCANDESCRIPTIONGROUP1    = CMNT_FPXORIGINALSCANNEDIMAGESIZE,
      CMNT_FPXORIGINALDOCUMENTSCANDESCRIPTIONGROUP2    = CMNT_FPXTYPEOFREFLECTIONORIGINAL,

      CMNT_FPXSCANDEVICEPROPERTYGROUP                  = 0x8200,
      CMNT_FPXSCANNERMANUFACTURERNAME                  = 148,
      CMNT_FPXSCANNERMODELNAME                         = 149,
      CMNT_FPXSCANNERSERIALNUMBER                      = 150,
      CMNT_FPXSCANSOFTWARE                             = 151,
      CMNT_FPXSCANSOFTWAREREVISIONDATE                 = 152,
      CMNT_FPXSERVICEBUREAUORGNAME                     = 153,
      CMNT_FPXSCANOPERATORID                           = 154,
      CMNT_FPXSCANDATE                                 = 155,
      CMNT_FPXLASTMODIFIEDDATE                         = 156,
      CMNT_FPXSCANNERPIXELSIZE                         = 157,
      CMNT_FPXSCANDEVICEPROPERTYGROUP1                 = CMNT_FPXSCANNERMANUFACTURERNAME,
      CMNT_FPXSCANDEVICEPROPERTYGROUP2                 = CMNT_FPXSCANNERPIXELSIZE,

      CMNT_SZTITLE                                     = 175,   // Title or caption for image
      CMNT_SZDISCLAIMER                                = 176,   // Legal Disclaimer
      CMNT_SZWARNING                                   = 177,   // Warning of nature of content
      CMNT_MISC                                        = 178,   // Miscellaneous comment
      CMNT_J2K_BINARY                                  = 179,   // Jpeg 2000 binary comment
      CMNT_J2K_LATIN                                   = 180,   // Jpeg 2000 latin comment

      // IPTC comments
      IPTC_SEPARATOR                                   = 1,     // '\001' is a separator used for repeatable comments

      CMNT_IPTC_FIRST                                  = 181,   // The first IPTC comment
      CMNT_IPTC_VERSION                                = 181,   // The version of IPTC comments (read-only)
      CMNT_IPTC_OBJECTTYPEREFERENCE                    = 182,
      CMNT_IPTC_OBJECTATTRIBUTEREFERENCE               = 183,
      CMNT_IPTC_OBJECTNAME                             = 184,
      CMNT_IPTC_EDITSTATUS                             = 185,
      CMNT_IPTC_EDITORIALUPDATE                        = 186,
      CMNT_IPTC_URGENCY                                = 187,
      CMNT_IPTC_SUBJECTREFERENCE                       = 188,
      CMNT_IPTC_CATEGORY                               = 189,
      CMNT_IPTC_SUPPLEMENTALCATEGORY                   = 190,
      CMNT_IPTC_FIXTUREIDENTIFIER                      = 191,
      CMNT_IPTC_KEYWORDS                               = 192,
      CMNT_IPTC_CONTENTLOCATIONCODE                    = 193,
      CMNT_IPTC_CONTENTLOCATIONNAME                    = 194,
      CMNT_IPTC_RELEASEDATE                            = 195,
      CMNT_IPTC_RELEASETIME                            = 196,
      CMNT_IPTC_EXPIRATIONDATE                         = 197,
      CMNT_IPTC_EXPIRATIONTIME                         = 198,
      CMNT_IPTC_SPECIALINSTRUCTIONS                    = 199,
      CMNT_IPTC_ACTIONADVISED                          = 200,
      CMNT_IPTC_REFERENCESERVICE                       = 201,
      CMNT_IPTC_REFERENCEDATE                          = 202,
      CMNT_IPTC_REFERENCENUMBER                        = 203,
      CMNT_IPTC_DATECREATED                            = 204,
      CMNT_IPTC_TIMECREATED                            = 205,
      CMNT_IPTC_DIGITALCREATIONDATE                    = 206,
      CMNT_IPTC_DIGITALCREATIONTIME                    = 207,
      CMNT_IPTC_ORIGINATINGPROGRAM                     = 208,
      CMNT_IPTC_PROGRAMVERSION                         = 209,
      CMNT_IPTC_OBJECTCYCLE                            = 210,
      CMNT_IPTC_BYLINE                                 = 211,
      CMNT_IPTC_BYLINETITLE                            = 212,
      CMNT_IPTC_CITY                                   = 213,
      CMNT_IPTC_SUBLOCATION                            = 214,
      CMNT_IPTC_PROVINCE_STATE                         = 215,
      CMNT_IPTC_PRIMARYLOCATIONCODE                    = 216,
      CMNT_IPTC_PRIMARYLOCATIONNAME                    = 217,
      CMNT_IPTC_ORIGINALTRANSMISSIONREFERENCE          = 218,
      CMNT_IPTC_HEADLINE                               = 219,
      CMNT_IPTC_CREDIT                                 = 220,
      CMNT_IPTC_SOURCE                                 = 221,
      CMNT_IPTC_COPYRIGHT                              = 222,
      CMNT_IPTC_CONTACT                                = 223,
      CMNT_IPTC_CAPTION                                = 224,
      CMNT_IPTC_AUTHOR                                 = 225,
      CMNT_IPTC_LANGUAGEIDENTIFIER                     = 226,
      CMNT_IPTC_LAST                                   = 226,   // The last IPTC comment

      CMNT_LAST                                        = 247,   // Last defined number for comments

      CMNT_ALL                                         = 0xFFFF,
   };

#ifndef SAVE_OVERWRITE
   #define SAVE_OVERWRITE 0
   #define SAVE_APPEND    1
   #define SAVE_REPLACE   2
   #define SAVE_INSERT    3
#endif
       
   enum  PicturizeConstants{
      PICTURIZE_LOADALL = 0x0001,
      PICTURIZE_RESIZE = 0x0002,
      PICTURIZE_RESAMPLE = 0x0004,
      PICTURIZE_ONCE = 0x0010,
      PICTURIZE_EQUALUSAGE = 0x0020,
      PICTURIZE_BESTFIT = 0x0040,
   };

   enum  PixelConstants{
      COLORREF_GRAY16 = 0x04000000L,
   };

#ifndef CONTOUR_VALUES
#define CONTOUR_VALUES
   enum  CountourFilterOptions{
      CONTOUR_THIN      = 0,
      CONTOUR_LINK_BLACKWHITE = 1,
      CONTOUR_LINK_GRAY    = 2,
      CONTOUR_LINK_COLOR   = 3,
      CONTOUR_APPROX_COLOR = 4,
   };
#endif

   enum  RotateConstants{
      ROTATE_RESIZE = 0x0001,
      ROTATE_RESAMPLE = 0x0002,
      ROTATE_BICUBIC = 0x0004,
   };

   enum  TagTypeConstants{
      TAG_BYTE = 1,
      TAG_ASCII = 2,
      TAG_SHORT = 3,
      TAG_LONG = 4,
      TAG_RATIONAL = 5,
      TAG_SBYTE = 6,
      TAG_UNDEFINED = 7,
      TAG_SSHORT = 8,
      TAG_SLONG = 9,
      TAG_SRATIONAL = 10,
      TAG_FLOAT = 11,
      TAG_DOUBLE = 12,
   };

#ifndef DOCCLEAN_CONSTANTS
#define DOCCLEAN_CONSTANTS
      enum BumpNickConstants{
         SMOOTH_BUMP = 0,
         SMOOTH_NICK = 1,
         SMOOTH_NONE = 2,
      };

      enum  DocImagingReturnConstants{
         SUCCESS_REMOVE          = 1,
         SUCCESS_NOREMOVE        = 2,
         LINEREMOVE_HORIZONTAL   = 1,
         LINEREMOVE_VERTICAL     = 2,
         SUCCESS_INVERT          = 1,
         SUCCESS_NOINVERT        = 2,
      };

      enum  SmoothConstants{
         SMOOTH_HORIZONTAL_ELEMENT  = 0,
         SMOOTH_VERTICAL_ELEMENT    = 1,
      };

      enum BorderConstants{
         BORDER_LEFT       = 1,
         BORDER_RIGHT      = 2,
         BORDER_TOP        = 4,
         BORDER_BOTTOM     = 8,
         BORDER_ALL        = 0xF,
         BORDER_DELTA_MAX  = 0xFFFFFFFF,
      };

      enum HolePunchConstants{
         HOLEPUNCH_LEFT    = 1,
         HOLEPUNCH_RIGHT   = 2,
         HOLEPUNCH_TOP     = 3,
         HOLEPUNCH_BOTTOM  = 4,
      } ;

      enum DocImagingFlags{
         FLAG_USE_DPI           = 0x00000001,
         FLAG_SINGLE_REGION     = 0x00000002,
         FLAG_LEAD_REGION       = 0x00000004,
         FLAG_CALLBACK_REGION   = 0x00000008,
         FLAG_IMAGE_UNCHANGED   = 0x00000010,
         FLAG_USE_SIZE          = 0x00000020,
         FLAG_USE_COUNT         = 0x00000040,
         FLAG_USE_LOCATION      = 0x00000080,
         FLAG_FAVOR_LONG        = 0x00000100,
         FLAG_REMOVE_ENTIRE     = 0x00000200,
         FLAG_USE_GAP           = 0x00000400,
         FLAG_USE_VARIANCE      = 0x00000800,
         FLAG_USE_DIAGONALS     = 0x00001000,
      } ;

      enum SmoothFlags{
         SMOOTH_SINGLE_REGION   = 0x00000002, //FLAG_SINGLE_REGION,
         SMOOTH_LEAD_REGION     = 0x00000004, //FLAG_LEAD_REGION,
         SMOOTH_IMAGE_UNCHANGED = 0x00000010, //FLAG_IMAGE_UNCHANGED,
         SMOOTH_FAVOR_LONG      = 0x00000100, //FLAG_FAVOR_LONG,
         SMOOTH_ALLFLAGS        = 0x00000116, //SMOOTH_SINGLE_REGION|SMOOTH_LEAD_REGION|SMOOTH_IMAGE_UNCHANGED|SMOOTH_FAVOR_LONG,
      } ;

      enum LineRemoveFlags{
         LINE_USE_DPI           = 0x00000001, //FLAG_USE_DPI,            
         LINE_SINGLE_REGION     = 0x00000002, //FLAG_SINGLE_REGION,
         LINE_LEAD_REGION       = 0x00000004, //FLAG_LEAD_REGION,
         LINE_CALLBACK_REGION   = 0x00000008, //FLAG_CALLBACK_REGION, 
         LINE_IMAGE_UNCHANGED   = 0x00000010, //FLAG_IMAGE_UNCHANGED,
         LINE_REMOVE_ENTIRE     = 0x00000200, //FLAG_REMOVE_ENTIRE,
         LINE_USE_GAP           = 0x00000400, //FLAG_USE_GAP,
         LINE_USE_VARIANCE      = 0x00000800, //FLAG_USE_VARIANCE,
         LINE_ALLFLAGS          = 0x00000E1F, //LINE_USE_DPI|LINE_SINGLE_REGION|LINE_LEAD_REGION|LINE_CALLBACK_REGION|LINE_REMOVE_ENTIRE|LINE_USE_GAP|LINE_USE_VARIANCE,
      } ;

      enum BorderRemoveFlags{
         BORDER_LEAD_REGION      = 0x00000004, //FLAG_LEAD_REGION,
         BORDER_CALLBACK_REGION  = 0x00000008, //FLAG_CALLBACK_REGION, 
         BORDER_SINGLE_REGION    = 0x00000002, //FLAG_SINGLE_REGION,
         BORDER_IMAGE_UNCHANGED  = 0x00000010, //FLAG_IMAGE_UNCHANGED,
         BORDER_USE_VARIANCE     = 0x00000800, //FLAG_USE_VARIANCE,
         BORDER_ALLFLAGS         = 0x0000081E, //BORDER_LEAD_REGION|BORDER_CALLBACK_REGION|BORDER_SINGLE_REGION|BORDER_IMAGE_UNCHANGED|BORDER_USE_VARIANCE,
      } ;

      enum InvertedTextFlags{
         INVERTEDTEXT_USE_DPI         = 0x00000001, //FLAG_USE_DPI,            
         INVERTEDTEXT_SINGLE_REGION   = 0x00000002, //FLAG_SINGLE_REGION,
         INVERTEDTEXT_LEAD_REGION     = 0x00000004, //FLAG_LEAD_REGION,
         INVERTEDTEXT_CALLBACK_REGION = 0x00000008, //FLAG_CALLBACK_REGION, 
         INVERTEDTEXT_IMAGE_UNCHANGED = 0x00000010, //FLAG_IMAGE_UNCHANGED,
         INVERTEDTEXT_USE_DIAGONALS   = 0x00001000, //FLAG_USE_DIAGONALS,            
         INVERTEDTEXT_ALLFLAGS        = 0x0000101F, //INVERTEDTEXT_USE_DPI|INVERTEDTEXT_SINGLE_REGION|INVERTEDTEXT_LEAD_REGION|INVERTEDTEXT_CALLBACK_REGION|INVERTEDTEXT_IMAGE_UNCHANGED|INVERTEDTEXT_USE_DIAGONALS,
      };

      enum DotRemoveFlags{
         DOT_USE_DPI            = 0x00000001, //FLAG_USE_DPI,            
         DOT_SINGLE_REGION      = 0x00000002, //FLAG_SINGLE_REGION,
         DOT_LEAD_REGION        = 0x00000004, //FLAG_LEAD_REGION,
         DOT_CALLBACK_REGION    = 0x00000008, //FLAG_CALLBACK_REGION, 
         DOT_IMAGE_UNCHANGED    = 0x00000010, //FLAG_IMAGE_UNCHANGED,
         DOT_USE_SIZE           = 0x00000020, //FLAG_USE_SIZE,
         DOT_USE_DIAGONALS      = 0x00001000, //FLAG_USE_DIAGONALS,            
         DOT_ALLFLAGS           = 0x0000103F, //DOT_USE_DPI|DOT_SINGLE_REGION|DOT_LEAD_REGION|DOT_CALLBACK_REGION|DOT_IMAGE_UNCHANGED|DOT_USE_SIZE|DOT_USE_DIAGONALS,
      };

      enum HolePunchRemoveFlags{
         HOLEPUNCH_USE_DPI          = 0x00000001, //FLAG_USE_DPI,            
         HOLEPUNCH_SINGLE_REGION    = 0x00000002, //FLAG_SINGLE_REGION,
         HOLEPUNCH_LEAD_REGION      = 0x00000004, //FLAG_LEAD_REGION,
         HOLEPUNCH_CALLBACK_REGION  = 0x00000008, //FLAG_CALLBACK_REGION, 
         HOLEPUNCH_IMAGE_UNCHANGED  = 0x00000010, //FLAG_IMAGE_UNCHANGED,
         HOLEPUNCH_USE_SIZE         = 0x00000020, //FLAG_USE_SIZE,
         HOLEPUNCH_USE_COUNT        = 0x00000040, //FLAG_USE_COUNT,     
         HOLEPUNCH_USE_LOCATION     = 0x00000080, //FLAG_USE_LOCATION,  
         HOLEPUNCH_ALLFLAGS         = 0x000000FF, //HOLEPUNCH_USE_DPI|HOLEPUNCH_SINGLE_REGION|HOLEPUNCH_LEAD_REGION|HOLEPUNCH_CALLBACK_REGION|HOLEPUNCH_IMAGE_UNCHANGED|HOLEPUNCH_USE_SIZE|HOLEPUNCH_USE_COUNT|HOLEPUNCH_USE_LOCATION,
      };
#endif

#ifndef J2K_CONSTANTS
#define J2K_CONSTANTS
      // JPEG 2000 Limitations imposed by the standard
      enum J2KLimitConstants
      {
         J2K_MAX_COMPONENTS_NUM = 3,
         J2K_MAX_DECOMP_LEVEL = 20,
      };

      // Ways of controlling the compression
      enum J2KCompressionControl
      {
         J2K_COMP_LOSSLESS,     // lossless compression
         J2K_COMP_RATIO,        // use fCompressionRatio
         J2K_COMP_TARGETSIZE,   // use uTargetFileSize
         J2K_COMP_QFACTOR,      // use qFactor
      };

      // Progressions Order
      enum J2KProgressionFlags
      {
         J2K_LAYER_RESOLUTION_COMPONENT_POSITION = 0,
         J2K_RESOLUTION_LAYER_COMPONENT_POSITION = 1,
         J2K_RESOLUTION_POSITION_COMPONENT_LAYER = 2,
         J2K_POSITION_COMPONENT_RESOLUTION_LAYER = 3,
         J2K_COMPONENT_POSITION_RESOLUTION_LAYER = 4,
      };

      // Ways of how to set the Region Of Interest 
      typedef enum
      {
         J2K_USELEAD_REGION,
         J2K_USEOPTION_RECT
      } J2KRegionOfInterest;

#endif

      enum  CurveTypeConstants{
         CURVE_BEZIER   = 0,
         CURVE_STANDAR  = 1
      };

      enum  CurveCloseConstants{
         CURVE_NO_CLOSE      = 0,   //Process only included points -- For n points draw (n-3) segments
         CURVE_CLOSE         = 1,   //For n points draw (n-1) segments
         CURVE_PARTIAL_CLOSE = 2    //Repeat first three points -- For n points draw (n) segments
      };

      enum  ColorSpaceConstants{
         RGB_SPACE   = 0x0001,
         YUV_SPACE   = 0x0002,
         GRAY_SPACE  = 0x0004,
      };

      enum  SwapColorsConstants{
         SWAP_RG    = 0x0001,
         SWAP_RB    = 0x0002,
         SWAP_GB    = 0x0004,
         SWAP_RGBR  = 0x0008,
         SWAP_RBGR  = 0x0010,
      };

      enum  ResizeRgnConstants{
         RGN_EXPAND   = 0x0001,
         RGN_CONTRACT = 0x0002,
      };

      enum  FadedMaskConstants{
         FADE_OUTER                   = 0x0001,
         FADE_DUMPFILL                = 0x0002,
         FADE_FILLCOLOR_START         = 0x0004,
         FADE_WHITETOBLACK            = 0x0008,
         FADE_NOTRANSPARENCY          = 0x0010,
         FADE_TRANSPARENCY_DUMPCOLOR  = 0x0020,
      };

      enum  AddBitmapsConstants{
         BC_AVG             = 0x0001,
         BC_ADD             = 0x0002,
      };

      enum  AntiAliasConstants{
         ANTIALIAS_1       = 0x0000,
         ANTIALIAS_2       = 0x0001,
         ANTIALIAS_3       = 0x0002,
         ANTIALIAS_DIAG    = 0x0003,
         ANTIALIAS_HORZ    = 0x0004,
         ANTIALIAS_VERT    = 0x0005,
      };

      enum  EdgeDetectorConstants{
         EDG_SOBEL_VERT    = 0x0101,
         EDG_SOBEL_HORZ    = 0x0102,
         EDG_SOBEL_ALL     = 0x0103,

         EDG_PREWITT_VERT  = 0x0201,
         EDG_PREWITT_HORZ  = 0x0202,
         EDG_PREWITT_ALL   = 0x0203,

         EDG_LAPLACE_1     = 0x0301,
         EDG_LAPLACE_2     = 0x0302,
         EDG_LAPLACE_3     = 0x0303,
         EDG_LAPLACE_DIAG  = 0x0304,
         EDG_LAPLACE_HORZ  = 0x0305,
         EDG_LAPLACE_VERT  = 0x0306,

         EDG_GRADIENT_N    = 0x0401,
         EDG_GRADIENT_NE   = 0x0402,
         EDG_GRADIENT_E    = 0x0403,
         EDG_GRADIENT_SE   = 0x0404,
         EDG_GRADIENT_S    = 0x0405,
         EDG_GRADIENT_SW   = 0x0406,
         EDG_GRADIENT_W    = 0x0407,
         EDG_GRADIENT_NW   = 0x0408,
      };

      enum  GetLookupTableFlags{
         GL_EXP         = 0x0000,
         GL_LN          = 0x0001,
         GL_LINEAR      = 0x0002,
      };   

      enum  AddBorderLocationConstants{
         BORDER_INSIDE     = 0x0000,
         BORDER_OUTSIDE    = 0x0001,
      };

      enum  AddBorderStyleConstants{
         BORDER_STYLE_TRANSPARENT = 0x0000,
         BORDER_STYLE_OPAQUE      = 0x0001,
         BORDER_STYLE_GRADIENT    = 0x0002,
         BORDER_STYLE_TILES       = 0x0003,
      };

      enum  AddBorderEffectStyleConstants{
         BORDER_EFFECT_CURVEDIN   = 0x0001,
         BORDER_EFFECT_CURVEDOUT  = 0x0002,
         BORDER_EFFECT_RAISED     = 0x0003,
      };

      enum  AddBorderGradientStyleConstants{
         BORDER_GRAD_INSIDEOUT    = 0x0000,
         BORDER_GRAD_CENTEROUT    = 0x0001,
         BORDER_GRAD_CORNEROUT    = 0x0002,
      };

      enum  AddBorderShadowDirectionConstants{
         BORDER_SHADOW_LEFT       = 0x0000,
         BORDER_SHADOW_RIGHT      = 0x0001,
         BORDER_SHADOW_SURROUND   = 0x0002,
      };

      enum  AddFrameLocationConstants{
         FRAME_INSIDE  = 0,
         FRAME_OUTSIDE = 1,
      };

      enum  AddFrameQualityConstants{
         FRAME_QUALITY_LOW  = 0,
         FRAME_QUALITY_HIGH = 1,
      };

#endif /* USE_DLG_COM */

   enum AnnUserModeConstants {
      ANNUSERMODE_NONE,
      ANNUSERMODE_DESIGN,
      ANNUSERMODE_RUN,
   } ;

   enum AnnToolConstants {
      ANNTOOL_SELECT          = 0,
      ANNTOOL_LINE            = 1,
      ANNTOOL_RECT            = 2,
      ANNTOOL_ELLIPSE         = 3,
      ANNTOOL_POLYLINE        = 4,
      ANNTOOL_POLYGON         = 5,
      ANNTOOL_POINTER         = 6,
      ANNTOOL_FREEHAND        = 7,
      ANNTOOL_HILITE          = 8,
      ANNTOOL_REDACT          = 9,
      ANNTOOL_TEXT            = 10,
      ANNTOOL_NOTE            = 11,
      ANNTOOL_STAMP           = 12,
      ANNTOOL_BUTTON          = 13,
      ANNTOOL_HOTSPOT         = 14,
      ANNTOOL_AUDIO           = 15,
      ANNTOOL_RULER           = 16,
      ANNTOOL_CROSSPRODUCT    = 17,
      ANNTOOL_POINT           = 18,
      ANNTOOL_PROTRACTOR      = 19,
      ANNTOOL_VIDEO           = 20,
      ANNTOOL_PUSHPIN         = 21,
      ANNTOOL_FREEHANDHOTSPOT = 22,
      ANNTOOL_CURVE           = 23,
      ANNTOOL_CURVECLOSED     = 24,
      ANNTOOL_ENCRYPT         = 25,
      ANNTOOL_TEXTPOINTER     = 26,
      ANNTOOL_POLYRULER       = 27,
      ANNTOOL_RTF             = 28,
      ANNTOOL_STAMP_FIRST     = 29,
      ANNTOOL_STAMP_APPROVED  = 29,
      ANNTOOL_STAMP_ASSIGNED  = 30,
      ANNTOOL_STAMP_CHECKED   = 31,
      ANNTOOL_STAMP_CLATPRV   = 32,
      ANNTOOL_STAMP_COPY      = 33,
      ANNTOOL_STAMP_DRAFT     = 34,
      ANNTOOL_STAMP_EXTENDED  = 35,
      ANNTOOL_STAMP_FAX       = 36,
      ANNTOOL_STAMP_FAXED     = 37,
      ANNTOOL_STAMP_IMPORTANT = 38,
      ANNTOOL_STAMP_INVOICE   = 39,
      ANNTOOL_STAMP_NOTICE    = 40,
      ANNTOOL_STAMP_OFFICIAL  = 41,
      ANNTOOL_STAMP_ONFILE    = 42,
      ANNTOOL_STAMP_PAID      = 43,
      ANNTOOL_STAMP_PASSED    = 44,
      ANNTOOL_STAMP_PENDING   = 45,
      ANNTOOL_STAMP_PROCESSED = 46,
      ANNTOOL_STAMP_RECEIVED  = 47,
      ANNTOOL_STAMP_REJECTED  = 48,
      ANNTOOL_STAMP_RELEASE   = 49,
      ANNTOOL_STAMP_SENT      = 50,
      ANNTOOL_STAMP_SHIPPED   = 51,
      ANNTOOL_STAMP_TOPSECRET = 52,
      ANNTOOL_STAMP_URGENT    = 53,
      ANNTOOL_STAMP_VOID      = 54,
      ANNTOOL_STAMP_LAST      = 54,
      ANNTOOL_USER_FIRST      = 0x0100, // first user defined tool
      ANNTOOL_USER_LAST       = 0x0200, // last allowed user tools
   };

   enum AnnObjectConstants {
      ANNOBJECT_ALL = -1,
      ANNOBJECT_CONTAINER,
      ANNOBJECT_POINTER,
      ANNOBJECT_AUDIO,
      ANNOBJECT_BUTTON,
      ANNOBJECT_ELLIPSE,
      ANNOBJECT_FREEHAND,
      ANNOBJECT_HILITE,
      ANNOBJECT_HOTSPOT,
      ANNOBJECT_LINE,
      ANNOBJECT_NOTE,
      ANNOBJECT_POLYGON,
      ANNOBJECT_POLYLINE,
      ANNOBJECT_RECT,
      ANNOBJECT_REDACT,
      ANNOBJECT_STAMP,
      ANNOBJECT_TEXT,
      ANNOBJECT_AUTOMATION,
      ANNOBJECT_RULER,
      ANNOBJECT_CROSSPRODUCT,
      ANNOBJECT_POINT,
      ANNOBJECT_PROTRACTOR,
      ANNOBJECT_VIDEO,
      ANNOBJECT_PUSHPIN,
      ANNOBJECT_FREEHANDHOTSPOT,
      ANNOBJECT_CURVE,
      ANNOBJECT_CURVECLOSED,
      ANNOBJECT_ENCRYPT,
      ANNOBJECT_TEXTPOINTER,
      ANNOBJECT_POLYRULER,
      ANNOBJECT_RTF,
   };

   enum AnnFormatConstants {
      ANNFMT_NATIVE,
      ANNFMT_WMF,
      ANNFMT_TIFFTAG,
      ANNFMT_WANGTAG,
      ANNFMT_ENCODED,/* Encoded native format (better security, smaller file size) */
      ANNFMT_XML,
   };

   enum AnnAutoTextConstants {
      /*
         automation menu text
      */
      ANNAUTOTEXT_MENU_UNDO,
      ANNAUTOTEXT_MENU_CUT,
      ANNAUTOTEXT_MENU_COPY,
      ANNAUTOTEXT_MENU_PASTE,
      ANNAUTOTEXT_MENU_DELETE,
      ANNAUTOTEXT_MENU_SELECTALL,
      ANNAUTOTEXT_MENU_BRINGTOFRONT,
      ANNAUTOTEXT_MENU_SENDTOBACK,
      ANNAUTOTEXT_MENU_DEFAULTPROPERTIES,
      ANNAUTOTEXT_MENU_SELECTEDPROPERTIES,
      ANNAUTOTEXT_MENU_CONTAINERPROPERTIES,
      ANNAUTOTEXT_MENU_POINTERPROPERTIES,
      ANNAUTOTEXT_MENU_AUDIOPROPERTIES,
      ANNAUTOTEXT_MENU_BUTTONPROPERTIES,
      ANNAUTOTEXT_MENU_ELLIPSEPROPERTIES,
      ANNAUTOTEXT_MENU_FREEHANDPROPERTIES,
      ANNAUTOTEXT_MENU_HILITEPROPERTIES,
      ANNAUTOTEXT_MENU_HOTSPOTPROPERTIES,
      ANNAUTOTEXT_MENU_LINEPROPERTIES,
      ANNAUTOTEXT_MENU_NOTEPROPERTIES,
      ANNAUTOTEXT_MENU_POLYGONPROPERTIES,
      ANNAUTOTEXT_MENU_POLYLINEPROPERTIES,
      ANNAUTOTEXT_MENU_RECTANGLEPROPERTIES,
      ANNAUTOTEXT_MENU_REDACTPROPERTIES,
      ANNAUTOTEXT_MENU_STAMPPROPERTIES,
      ANNAUTOTEXT_MENU_TEXTPROPERTIES,
      ANNAUTOTEXT_MENU_AUTOMATIONPROPERTIES,
      ANNAUTOTEXT_MENU_RULERPROPERTIES,
      ANNAUTOTEXT_MENU_CROSSPRODUCTPROPERTIES,
      ANNAUTOTEXT_MENU_POINTPROPERTIES,
      ANNAUTOTEXT_MENU_PROTRACTORPROPERTIES,
      ANNAUTOTEXT_MENU_VIDEOPROPERTIES,
      ANNAUTOTEXT_MENU_PUSHPINPROPERTIES,
      ANNAUTOTEXT_MENU_FREEHANDHOSPOTPROPERTIES,
      ANNAUTOTEXT_MENU_CURVEPROPERTIES,
      ANNAUTOTEXT_MENU_CURVECLOSEDPROPERTIES,
      ANNAUTOTEXT_MENU_ENCRYPTPROPERTIES,
      ANNAUTOTEXT_MENU_TEXTPOINTERPROPERTIES,
      ANNAUTOTEXT_MENU_POLYRULERPROPERTIES,
      ANNAUTOTEXT_MENU_RTFPROPERTIES,


      ANNAUTOTEXT_MENU_LINE,
      ANNAUTOTEXT_MENU_FILL,
      ANNAUTOTEXT_MENU_FOREGROUNDCOLOR,
      ANNAUTOTEXT_MENU_BACKGROUNDCOLOR,
      ANNAUTOTEXT_MENU_FONT,
      ANNAUTOTEXT_MENU_BITMAP,
      ANNAUTOTEXT_MENU_TEXT,
      ANNAUTOTEXT_MENU_FILE,
      ANNAUTOTEXT_MENU_ROP2,
      ANNAUTOTEXT_MENU_TRANSPARENT,
      ANNAUTOTEXT_MENU_LOCK,
      ANNAUTOTEXT_MENU_UNLOCK,
      ANNAUTOTEXT_MENU_LOCKALL,
      ANNAUTOTEXT_MENU_UNLOCKALL,
      ANNAUTOTEXT_MENU_HYPERLINK,
      ANNAUTOTEXT_MENU_NOTEBACKGROUNDCOLOR,
      ANNAUTOTEXT_MENU_REDACTBACKGROUNDCOLOR,
      ANNAUTOTEXT_MENU_HILITEBACKGROUNDCOLOR,
      ANNAUTOTEXT_MENU_HOTSPOTMETAFILE,
      ANNAUTOTEXT_MENU_ENCRYPTMETAFILE,
      ANNAUTOTEXT_MENU_DECRYPTMETAFILE,
      ANNAUTOTEXT_MENU_RULER,

      ANNAUTOTEXT_MENU_BUTTONFONT,
      ANNAUTOTEXT_MENU_GROUP,
      ANNAUTOTEXT_MENU_UNGROUP,
      ANNAUTOTEXT_MENU_NODES,
      ANNAUTOTEXT_MENU_PROTRACTOR,
      ANNAUTOTEXT_MENU_NAME,
      ANNAUTOTEXT_MENU_ENCRYPT,
      ANNAUTOTEXT_MENU_POINT, 

      ANNAUTOTEXT_MENU_METAFILE,
      ANNAUTOTEXT_MENU_SECONDARYMETAFILE,
      ANNAUTOTEXT_MENU_SECONDARYFILE,
      ANNAUTOTEXT_MENU_TRANSPARENTCOLOR,
      ANNAUTOTEXT_MENU_DELETENODE,
      ANNAUTOTEXT_MENU_ADDNODE,
      ANNAUTOTEXT_MENU_CAPTURE,

      ANNAUTOTEXT_MENU_FIXED,

      /*
         toolbar rubber stamps menu text
      */
      ANNAUTOTEXT_MENU_STAMP_APPROVED,
      ANNAUTOTEXT_MENU_STAMP_ASSIGNED,
      ANNAUTOTEXT_MENU_STAMP_CHECKED,
      ANNAUTOTEXT_MENU_STAMP_CLATPRV,
      ANNAUTOTEXT_MENU_STAMP_COPY,
      ANNAUTOTEXT_MENU_STAMP_DRAFT,
      ANNAUTOTEXT_MENU_STAMP_EXTENDED,
      ANNAUTOTEXT_MENU_STAMP_FAX,
      ANNAUTOTEXT_MENU_STAMP_FAXED,
      ANNAUTOTEXT_MENU_STAMP_IMPORTANT,
      ANNAUTOTEXT_MENU_STAMP_INVOICE,
      ANNAUTOTEXT_MENU_STAMP_NOTICE,
      ANNAUTOTEXT_MENU_STAMP_OFFICIAL,
      ANNAUTOTEXT_MENU_STAMP_ONFILE,
      ANNAUTOTEXT_MENU_STAMP_PAID,
      ANNAUTOTEXT_MENU_STAMP_PASSED,
      ANNAUTOTEXT_MENU_STAMP_PENDING,
      ANNAUTOTEXT_MENU_STAMP_PROCESSED,
      ANNAUTOTEXT_MENU_STAMP_RECEIVED,
      ANNAUTOTEXT_MENU_STAMP_REJECTED,
      ANNAUTOTEXT_MENU_STAMP_RELEASE,
      ANNAUTOTEXT_MENU_STAMP_SENT,
      ANNAUTOTEXT_MENU_STAMP_SHIPPED,
      ANNAUTOTEXT_MENU_STAMP_TOPSECRET,
      ANNAUTOTEXT_MENU_STAMP_URGENT,
      ANNAUTOTEXT_MENU_STAMP_VOID,

      /*
         automation line dialog text
      */
      ANNAUTOTEXT_LINE_CAPTION,
      ANNAUTOTEXT_LINE_WIDTH,
      ANNAUTOTEXT_LINE_STYLE,
      ANNAUTOTEXT_LINE_OK,
      ANNAUTOTEXT_LINE_CANCEL,
      ANNAUTOTEXT_LINE_APPLY,

      ANNAUTOTEXT_LINE_SOLID,
      ANNAUTOTEXT_LINE_DASH,
      ANNAUTOTEXT_LINE_DOT,
      ANNAUTOTEXT_LINE_DASHDOT,
      ANNAUTOTEXT_LINE_DASHDOTDOT,
      ANNAUTOTEXT_LINE_NULL,

      /*
         automation fill dialog text
      */
      ANNAUTOTEXT_FILL_CAPTION,
      ANNAUTOTEXT_FILL_MODE,
      ANNAUTOTEXT_FILL_PATTERNS,
      ANNAUTOTEXT_FILL_POLYGON,
      ANNAUTOTEXT_FILL_OK,
      ANNAUTOTEXT_FILL_CANCEL,
      ANNAUTOTEXT_FILL_APPLY,

      ANNAUTOTEXT_FILL_TRANSPARENT,
      ANNAUTOTEXT_FILL_TRANSLUCENT,
      ANNAUTOTEXT_FILL_OPAQUE,

      ANNAUTOTEXT_FILL_SOLID,
      ANNAUTOTEXT_FILL_BACKWARDDIAGONAL,
      ANNAUTOTEXT_FILL_CROSS,
      ANNAUTOTEXT_FILL_DIAGONALCROSS,
      ANNAUTOTEXT_FILL_FORWARDDIAGONAL,
      ANNAUTOTEXT_FILL_HORIZONTAL,
      ANNAUTOTEXT_FILL_VERTICAL,

      ANNAUTOTEXT_FILL_WINDING,
      ANNAUTOTEXT_FILL_ALTERNATE,

      /*
         automation text dialog text
      */
      ANNAUTOTEXT_TEXT_CAPTION,
      ANNAUTOTEXT_TEXT_IMPORT,
      ANNAUTOTEXT_TEXT_FILE_CAPTION,   // "Text File", 0, 
      ANNAUTOTEXT_TEXT_FILE_FILTER,    // "Text files (*.txt)|*.txt|All Files (*.*)|*.*|", 0,
      ANNAUTOTEXT_TEXT_INSERT_TOKEN,
      ANNAUTOTEXT_TEXT_ALIGN,
      ANNAUTOTEXT_TEXT_ALIGN_LEFT,
      ANNAUTOTEXT_TEXT_ALIGN_CENTER,
      ANNAUTOTEXT_TEXT_ALIGN_RIGHT,

      ANNAUTOTEXT_TEXT_ROTATION, 
      ANNAUTOTEXT_TEXT_ROTATE_0,  
      ANNAUTOTEXT_TEXT_ROTATE_90, 
      ANNAUTOTEXT_TEXT_ROTATE_180,
      ANNAUTOTEXT_TEXT_ROTATE_270,

      ANNAUTOTEXT_TEXT_FIXED_POINTER,
      ANNAUTOTEXT_TEXT_EXPAND_TOKENS,
      ANNAUTOTEXT_TEXT_OK,
      ANNAUTOTEXT_TEXT_CANCEL,
      ANNAUTOTEXT_TEXT_APPLY,

      /*
         automation audio file dialog text
      */
      ANNAUTOTEXT_AUDIOFILE_CAPTION,
      ANNAUTOTEXT_AUDIOFILE_FILTER,

      /*
         automation stamp file dialog text
      */
      ANNAUTOTEXT_STAMPFILE_CAPTION,
      ANNAUTOTEXT_STAMPFILE_FILTER,

      /*
         automation hotspot file dialog text
      */

      /*
         automation point file dialog text
      */
      ANNAUTOTEXT_POINTFILE_CAPTION,
      ANNAUTOTEXT_POINTFILE_FILTER,

      /*
         automation audio file dialog text
      */
      ANNAUTOTEXT_VIDEOFILE_CAPTION,
      ANNAUTOTEXT_VIDEOFILE_FILTER,

      /*
         automation pushpin file dialog text
      */
      ANNAUTOTEXT_PUSHPINFILE_CAPTION,
      ANNAUTOTEXT_PUSHPINFILE_FILTER,
      ANNAUTOTEXT_PUSHPINSECONDARYFILE_CAPTION,
      ANNAUTOTEXT_PUSHPINSECONDARYFILE_FILTER,

      /*
         automation default item text
      */
      ANNAUTOTEXT_ITEM_DEFAULTTEXT,

      /*
         automation ROP2 dialog text
      */
      ANNAUTOTEXT_ROP2_CAPTION,
      ANNAUTOTEXT_ROP2_TEXT,
      ANNAUTOTEXT_ROP2_OK,
      ANNAUTOTEXT_ROP2_CANCEL,
      ANNAUTOTEXT_ROP2_APPLY,
      ANNAUTOTEXT_ROP2_COPY,
      ANNAUTOTEXT_ROP2_AND,
      ANNAUTOTEXT_ROP2_XOR,

      /*
         automation Lock dialog text
      */
      ANNAUTOTEXT_LOCK_CAPTION,
      ANNAUTOTEXT_LOCK_TEXT,
      ANNAUTOTEXT_LOCK_OK,
      ANNAUTOTEXT_LOCK_CANCEL,

      /*
         automation Unlock dialog text
      */
      ANNAUTOTEXT_UNLOCK_CAPTION,
      ANNAUTOTEXT_UNLOCK_TEXT,
      ANNAUTOTEXT_UNLOCK_OK,
      ANNAUTOTEXT_UNLOCK_CANCEL,

      /*
         automation Ruler dialog text
      */
      ANNAUTOTEXT_RULER_CAPTION,
      ANNAUTOTEXT_RULER_UNITS,
      ANNAUTOTEXT_RULER_ABBREV,
      ANNAUTOTEXT_RULER_PRECISION,
      ANNAUTOTEXT_RULER_LENGTH,
      ANNAUTOTEXT_RULER_OK,
      ANNAUTOTEXT_RULER_CANCEL,
      ANNAUTOTEXT_RULER_APPLY,

      ANNAUTOTEXT_RULER_INCHES,
      ANNAUTOTEXT_RULER_FEET,
      ANNAUTOTEXT_RULER_YARDS,
      ANNAUTOTEXT_RULER_MICROMETERS,
      ANNAUTOTEXT_RULER_MILLIMETERS,
      ANNAUTOTEXT_RULER_CENTIMETERS,
      ANNAUTOTEXT_RULER_METERS,
      ANNAUTOTEXT_RULER_TWIPS,
      ANNAUTOTEXT_RULER_POINTS,
      ANNAUTOTEXT_RULER_PIXELS,
      ANNAUTOTEXT_RULER_SMART_METRIC,
      ANNAUTOTEXT_RULER_SMART_ENGLISH,

      ANNAUTOTEXT_RULER_SHOWLENGTH,
      ANNAUTOTEXT_RULER_SHOWGAUGE,
      ANNAUTOTEXT_RULER_SHOWTICMARKS,

      /*
         automation Nodes dialog text
      */
      ANNAUTOTEXT_NODES_CAPTION,
      ANNAUTOTEXT_NODES_DISPLAYNODES,
      ANNAUTOTEXT_NODES_GAP,
      ANNAUTOTEXT_NODES_OK,
      ANNAUTOTEXT_NODES_CANCEL,
      ANNAUTOTEXT_NODES_APPLY,

      /*
         automation Protractor dialog text
      */
      ANNAUTOTEXT_PROTRACTOR_CAPTION,
      ANNAUTOTEXT_PROTRACTOR_ACUTE,
      ANNAUTOTEXT_PROTRACTOR_UNITS,
      ANNAUTOTEXT_PROTRACTOR_ABBREV,
      ANNAUTOTEXT_PROTRACTOR_PRECISION,
      ANNAUTOTEXT_PROTRACTOR_ARCRADIUS,
      ANNAUTOTEXT_PROTRACTOR_OK,
      ANNAUTOTEXT_PROTRACTOR_CANCEL,
      ANNAUTOTEXT_PROTRACTOR_APPLY,

      ANNAUTOTEXT_PROTRACTOR_DEGREES,
      ANNAUTOTEXT_PROTRACTOR_RADIANS,

      /*
      automation Name dialog text
      */
      ANNAUTOTEXT_NAME_CAPTION,
      ANNAUTOTEXT_NAME_SHOW,
      ANNAUTOTEXT_NAME_LOCATION,
      ANNAUTOTEXT_NAME_OFFSET,
      ANNAUTOTEXT_NAME_OFFSET_X,
      ANNAUTOTEXT_NAME_OFFSET_Y,
      ANNAUTOTEXT_NAME_RESTRICT,
      ANNAUTOTEXT_NAME_FONT,
      ANNAUTOTEXT_NAME_SAMPLE,
      ANNAUTOTEXT_NAME_SAMPLE_TEXT,
      ANNAUTOTEXT_NAME_COLORS,
      ANNAUTOTEXT_NAME_FOREGROUNDCOLOR,
      ANNAUTOTEXT_NAME_BACKGROUNDCOLOR,
      ANNAUTOTEXT_NAME_TRANSPARENT,
      ANNAUTOTEXT_NAME_OK,
      ANNAUTOTEXT_NAME_CANCEL,
      ANNAUTOTEXT_NAME_APPLY,

      /*
         automation default item text
      */
      ANNAUTOTEXT_ITEM_DEFAULTNAME,

      /*
         automation Play Video dialog text
      */
      ANNAUTOTEXT_PLAYVIDEO_CAPTION,
      ANNAUTOTEXT_PLAYVIDEO_BALANCE,
      ANNAUTOTEXT_PLAYVIDEO_VOLUME,
      ANNAUTOTEXT_PLAYVIDEO_SPEED,

      ANNAUTOTEXT_PLAYVIDEO_TIP_PLAY,
      ANNAUTOTEXT_PLAYVIDEO_TIP_PAUSE,
      ANNAUTOTEXT_PLAYVIDEO_TIP_STOP,
      ANNAUTOTEXT_PLAYVIDEO_TIP_REWIND,
      ANNAUTOTEXT_PLAYVIDEO_TIP_FORWARD,
      ANNAUTOTEXT_PLAYVIDEO_TIP_PREVMARK,
      ANNAUTOTEXT_PLAYVIDEO_TIP_NEXTMARK,
      ANNAUTOTEXT_PLAYVIDEO_TIP_NEXTFRAME,
      ANNAUTOTEXT_PLAYVIDEO_TIP_PREVFRAME,
      ANNAUTOTEXT_PLAYVIDEO_TIP_STARTSEL,
      ANNAUTOTEXT_PLAYVIDEO_TIP_ENDSEL,

      /*
         automation stamp metafile dialog text
      */
      ANNAUTOTEXT_STAMPMETAFILE_CAPTION,
      ANNAUTOTEXT_STAMPMETAFILE_FILTER,

      /*
         automation hotspot metafile dialog text
      */
      ANNAUTOTEXT_HOTSPOTMETAFILE_CAPTION,
      ANNAUTOTEXT_HOTSPOTMETAFILE_FILTER,

      /*
         automation point dialog text
      */
      ANNAUTOTEXT_POINT_CAPTION,
      ANNAUTOTEXT_POINT_USE_BITMAP,
      ANNAUTOTEXT_POINT_NO_BITMAP,
      ANNAUTOTEXT_POINT_BITMAP,
      ANNAUTOTEXT_POINT_TRANSPARENT_BITMAP,
      ANNAUTOTEXT_POINT_RADIUS,
      ANNAUTOTEXT_POINT_BORDER_COLOR,
      ANNAUTOTEXT_POINT_FILL_COLOR,
      ANNAUTOTEXT_POINT_TRANSPARENT_FILL,
      ANNAUTOTEXT_POINT_FIXED_SIZE,

      /* 
      automation encrypt metafile dialog text
      */
      ANNAUTOTEXT_ENCRYPTMETAFILE_CAPTION,
      ANNAUTOTEXT_ENCRYPTMETAFILE_FILTER,
      ANNAUTOTEXT_DECRYPTMETAFILE_CAPTION,
      ANNAUTOTEXT_DECRYPTMETAFILE_FILTER,

      /*
         automation encrypt dialog text
      */
      ANNAUTOTEXT_ENCRYPT_CAPTION,
      ANNAUTOTEXT_ENCRYPT_KEY,
      ANNAUTOTEXT_ENCRYPT_TYPE,
      ANNAUTOTEXT_ENCRYPT_RADIO_ENCRYPTOR,
      ANNAUTOTEXT_ENCRYPT_RADIO_DECRYPTOR,
      ANNAUTOTEXT_ENCRYPT_LOCATION,
      ANNAUTOTEXT_ENCRYPT_LOCATION_LEFT,
      ANNAUTOTEXT_ENCRYPT_LOCATION_TOP,
      ANNAUTOTEXT_ENCRYPT_LOCATION_RIGHT,
      ANNAUTOTEXT_ENCRYPT_LOCATION_BOTTOM,
      ANNAUTOTEXT_ENCRYPT_OK,
      ANNAUTOTEXT_ENCRYPT_CANCEL,

      /*
         automation fixed dialog text
      */
      ANNAUTOTEXT_FIXED_CAPTION,
      ANNAUTOTEXT_FIXED_DETAILED_DESCRIPTION,
      ANNAUTOTEXT_FIXED_FIXED,
      ANNAUTOTEXT_FIXED_FIXED_DESCRIPTION,
      ANNAUTOTEXT_FIXED_ADJUST,
      ANNAUTOTEXT_FIXED_ADJUST_DESCRIPTION,
      ANNAUTOTEXT_FIXED_OK,
      ANNAUTOTEXT_FIXED_CANCEL,
      ANNAUTOTEXT_FIXED_APPLY,

      /*
         toolbar buttons tooltip text
      */
      ANNAUTOTEXT_TOOLTIP_SELECT,
      ANNAUTOTEXT_TOOLTIP_LINE,
      ANNAUTOTEXT_TOOLTIP_RECT,
      ANNAUTOTEXT_TOOLTIP_ELLIPSE,
      ANNAUTOTEXT_TOOLTIP_POLYLINE,
      ANNAUTOTEXT_TOOLTIP_POLYGON,
      ANNAUTOTEXT_TOOLTIP_POINTER,
      ANNAUTOTEXT_TOOLTIP_FREEHAND,
      ANNAUTOTEXT_TOOLTIP_HILITE,
      ANNAUTOTEXT_TOOLTIP_REDACT,
      ANNAUTOTEXT_TOOLTIP_TEXT,
      ANNAUTOTEXT_TOOLTIP_NOTE,
      ANNAUTOTEXT_TOOLTIP_STAMP,
      ANNAUTOTEXT_TOOLTIP_RUBBERSTAMP,
      ANNAUTOTEXT_TOOLTIP_HOTSPOT,
      ANNAUTOTEXT_TOOLTIP_FREEHANDHOTSPOT,
      ANNAUTOTEXT_TOOLTIP_AUDIO,
      ANNAUTOTEXT_TOOLTIP_BUTTON,
      ANNAUTOTEXT_TOOLTIP_RULER,
      ANNAUTOTEXT_TOOLTIP_CROSSPRODUCT,
      ANNAUTOTEXT_TOOLTIP_POINT,
      ANNAUTOTEXT_TOOLTIP_PROTRACTOR,
      ANNAUTOTEXT_TOOLTIP_VIDEO,
      ANNAUTOTEXT_TOOLTIP_PUSHPIN,
      ANNAUTOTEXT_TOOLTIP_CURVE,
      ANNAUTOTEXT_TOOLTIP_CURVECLOSED,
      ANNAUTOTEXT_TOOLTIP_ENCRYPT,
      ANNAUTOTEXT_TOOLTIP_TEXTPOINTER,
      ANNAUTOTEXT_TOOLTIP_POLYRULER,
      ANNAUTOTEXT_TOOLTIP_RTF,

      /*
         automation transparent color dialog text
      */
      ANNAUTOTEXT_TRANSPARENTCOLOR_CAPTION,
      ANNAUTOTEXT_TRANSPARENTCOLOR_COLORARRAYFRAME,
      ANNAUTOTEXT_TRANSPARENTCOLOR_CURRENTCOLORFRAME,
      ANNAUTOTEXT_TRANSPARENTCOLOR_COLOR,
      ANNAUTOTEXT_TRANSPARENTCOLOR_RED,
      ANNAUTOTEXT_TRANSPARENTCOLOR_GREEN,
      ANNAUTOTEXT_TRANSPARENTCOLOR_BLUE,
      ANNAUTOTEXT_TRANSPARENTCOLOR_OK,
      ANNAUTOTEXT_TRANSPARENTCOLOR_CANCEL,

      /*
         automation capture file dialog text
      */
      ANNAUTOTEXT_CAPTUREFILE_CAPTION,
      ANNAUTOTEXT_CAPTUREFILE_FILTER,

      /*
         automation capture dialog text
      */
      ANNAUTOTEXT_CAPTUREWINDOW_CAPTION,
      ANNAUTOTEXT_CAPTURE_CAPTION,
      ANNAUTOTEXT_CAPTURE_CAPTURE_DEVICES,
      ANNAUTOTEXT_CAPTURE_CAPTURE_VIDEO,
      ANNAUTOTEXT_CAPTURE_CAPTURE_AUDIO,
      ANNAUTOTEXT_CAPTURE_CAPTURE_PROPERTIES,
      ANNAUTOTEXT_CAPTURE_CAPTURE_VIDEO_CAPTURE_SIZE,
      ANNAUTOTEXT_CAPTURE_CAPTURE_AUDIO_FORMAT,
      ANNAUTOTEXT_CAPTURE_COMPRESSORS,
      ANNAUTOTEXT_CAPTURE_COMPRESSORS_VIDEO,
      ANNAUTOTEXT_CAPTURE_COMPRESSORS_AUDIO,
      ANNAUTOTEXT_CAPTURE_COMPRESSORS_VIDEO_PROPERTIES,
      ANNAUTOTEXT_CAPTURE_COMPRESSORS_AUDIO_PROPERTIES,
      ANNAUTOTEXT_CAPTURE_PROCESSORS,
      ANNAUTOTEXT_CAPTURE_PROCESSORS_VIDEO,
      ANNAUTOTEXT_CAPTURE_PROCESSORS_AUDIO,
      ANNAUTOTEXT_CAPTURE_DISPLAYMODE,
      ANNAUTOTEXT_CAPTURE_DISPLAYMODE_PREVIEW,
      ANNAUTOTEXT_CAPTURE_DISPLAYMODE_AUDIO,
      ANNAUTOTEXT_CAPTURE_DISPLAYMODE_VIDEO,
      ANNAUTOTEXT_CAPTURE_DISPLAYMODE_VIDEO_FRAMES_PER_SEC,
      ANNAUTOTEXT_CAPTURE_DISPLAYMODE_SHOW_DIALOG_PREVIEW,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS_FRAMES_PER_SEC,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS_TIME_LIMIT,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS_VIDEO,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS_AUDIO,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS_START,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS_END,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS_FRAMES_CAPTURED,
      ANNAUTOTEXT_CAPTURE_CAPTURE_OPTIONS_FRAMES_DROPPED,
      ANNAUTOTEXT_CAPTURE_CAPTUREFILE,
      ANNAUTOTEXT_CAPTURE_CAPTUREFILE_BROWSE,
      ANNAUTOTEXT_CAPTURE_CAPTUREFILE_ALLOCATE_FILE,
      ANNAUTOTEXT_CAPTURE_CAPTUREFILE_UNITS,
      ANNAUTOTEXT_CAPTURE_OK,
      ANNAUTOTEXT_CAPTURE_APPLY_SETTINGS,
      ANNAUTOTEXT_CAPTURE_CANCEL,
      ANNAUTOTEXT_CAPTURE_DEFAULTNAME,
      ANNAUTOTEXT_CAPTURE_NOVIDEODRIVERS,
      ANNAUTOTEXT_CAPTURE_NOAUDIODRIVERS,
      ANNAUTOTEXT_DIALOG_FONT,
      ANNAUTOTEXT_MM_NOT_PRESENT, 
      ANNAUTOTEXT_ERROR,
      ANNAUTOTEXT_WARNING,
      ANNAUTOTEXT_MISSING_DLL,
      ANNAUTOTEXT_MISSING_FUNCTION,
      ANNAUTOTEXT_MISSING_ORDINAL,
   
      // RTF Object menus and toolbars
      ANNAUTOTEXT_RTF_DEFAULTTEXT, 
      ANNAUTOTEXT_RTF_FIND_MESSAGE,
      ANNAUTOTEXT_RTF_MENU_TITLE,

      ANNAUTOTEXT_RTF_MENU_FILE,                   // "&File"
      ANNAUTOTEXT_RTF_MENU_FILE_OPEN,              // "&Open...",                    LTANN_IDM_RTF_OPEN
      ANNAUTOTEXT_RTF_MENU_FILE_SAVE_AS,           // "&Save as ...",                LTANN_IDM_RTF_SAVE
      ANNAUTOTEXT_RTF_MENU_FILE_PRINT,             // "&Print...",                   LTANN_IDM_RTF_PRINT
      ANNAUTOTEXT_RTF_MENU_FILE_EXIT,              // "E&xit",                       LTANN_IDM_RTF_EXIT
      ANNAUTOTEXT_RTF_MENU_EDIT,                   // "&Edit"
      ANNAUTOTEXT_RTF_MENU_EDIT_UNDO,              // "&Undo",                       LTANN_IDM_RTF_UNDO
      ANNAUTOTEXT_RTF_MENU_EDIT_CUT,               // "Cu&t",                        LTANN_IDM_RTF_CUT
      ANNAUTOTEXT_RTF_MENU_EDIT_COPY,              // "&Copy",                       LTANN_IDM_RTF_COPY
      ANNAUTOTEXT_RTF_MENU_EDIT_PASTE,             // "&Paste",                      LTANN_IDM_RTF_PASTE
      ANNAUTOTEXT_RTF_MENU_EDIT_DELETE,            // "&Delete",                     LTANN_IDM_RTF_DELETE
      ANNAUTOTEXT_RTF_MENU_EDIT_SELECT_ALL,        // "Select &All",                 LTANN_IDM_RTF_SELECTALL
      ANNAUTOTEXT_RTF_MENU_EDIT_FIND,              // "&Find...",                    LTANN_IDM_RTF_FIND
      ANNAUTOTEXT_RTF_MENU_EDIT_REPLACE,           // "&Replace...",                 LTANN_IDM_RTF_REPLACE
      ANNAUTOTEXT_RTF_MENU_FORMAT,                 // "F&ormat"
      ANNAUTOTEXT_RTF_MENU_FORMAT_FONT,            // "&Font...",                    LTANN_IDM_RTF_FONT
      ANNAUTOTEXT_RTF_MENU_FORMAT_BACKCOLOR,
      ANNAUTOTEXT_RTF_MENU_FORMAT_FORECOLOR, 
      ANNAUTOTEXT_RTF_MENU_FORMAT_BULLETS,         // "Bullets",                     LTANN_IDM_RTF_BULLET, CHECKED
      ANNAUTOTEXT_RTF_MENU_FORMAT_NUMBERING,       // "Numbering",                   LTANN_IDM_RTF_NUMBERING, CHECKED
      ANNAUTOTEXT_RTF_MENU_FORMAT_DECREASE_INDENT, // "&Decrease Indent",            LTANN_IDM_RTF_DECREASEINDENT
      ANNAUTOTEXT_RTF_MENU_FORMAT_INCREASE_INDENT, // "&Increase Indent",            LTANN_IDM_RTF_INCREASEINDENT
      ANNAUTOTEXT_RTF_MENU_FORMAT_LEFT,            // "&Left",                       LTANN_IDM_RTF_LEFT, CHECKED
      ANNAUTOTEXT_RTF_MENU_FORMAT_CENTER,          // "&Center",                     LTANN_IDM_RTF_CENTER, CHECKED
      ANNAUTOTEXT_RTF_MENU_FORMAT_RIGHT,           // "&Right",                      LTANN_IDM_RTF_RIGHT, CHECKED

      ANNAUTOTEXT_RTF_TOOLTIP_BOLD,                // "Bold",                        LTANN_ID_RTF_HIDE_BOLD, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_ITALIC,              // "Italic",                      LTANN_ID_RTF_HIDE_ITALIC, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_UNDERLINE,           // "Underline",                   LTANN_ID_RTF_HIDE_UNDERLINE, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_FORECOLOR,           // "Text Color",                  LTANN_ID_RTF_HIDE_FORECOLOR, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_BACKCOLOR,           // "Background Color",            LTANN_ID_RTF_HIDE_BACKCOLOR, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_BULLETS,             // "Bullets",                     LTANN_ID_RTF_HIDE_BULLET, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_NUMBERING,           // "Numbering",                   LTANN_ID_RTF_HIDE_NUMBERING, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_DECREASE_INDENT,     // "Decrease Indent",             LTANN_ID_RTF_HIDE_DECREASEINDENT
      ANNAUTOTEXT_RTF_TOOLTIP_INCREASE_INDENT,     // "Increase Indent",             LTANN_ID_RTF_HIDE_INCREASEINDENT
      ANNAUTOTEXT_RTF_TOOLTIP_LEFT,                // "Left",                        LTANN_ID_RTF_HIDE_LEFT, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_CENTER,              // "Center",                      LTANN_ID_RTF_HIDE_CENTER, CHECKED
      ANNAUTOTEXT_RTF_TOOLTIP_RIGHT,               // "Right",                       LTANN_ID_RTF_HIDE_RIGHT, CHECKED   
      ANNAUTOTEXT_RTF_TOOLTIP_CUSTOM, 

      ANNAUTOTEXT_RTF_FILTER,                      // "Rich Text Format (*.RTF)\0*.rtf\0"
      ANNAUTOTEXT_RTF_EXT,                         // "rtf"
      ANNAUTOTEXT_RTF_DLL_NOT_PRESENT,              // Riched20.dll not installed message


      // Added in middle of v14
      ANNAUTOTEXT_RULER_TICMARKLENGTH,             // Tic Mark Length
      ANNAUTOTEXT_TEXT_TOKEN_DATE_YYYY,            // Four digit year   
      ANNAUTOTEXT_TEXT_TOKEN_DATE_YY,              // Two digit year
      ANNAUTOTEXT_TEXT_TOKEN_DATE_MM,              // Two digit month
      ANNAUTOTEXT_TEXT_TOKEN_DATE_DD,              // Two digit day
      ANNAUTOTEXT_TEXT_TOKEN_DATE_MONTH_NAME,      // Month Name
      ANNAUTOTEXT_TEXT_TOKEN_DATE_DAY_OF_WEEK,     // Day of week name
      ANNAUTOTEXT_TEXT_TOKEN_TIME_HH_12,           // Current time: 12-Hour (01-12)
      ANNAUTOTEXT_TEXT_TOKEN_TIME_HH_24,           // Current time: 24-Hour (00-23)
      ANNAUTOTEXT_TEXT_TOKEN_TIME_MM,              // Current time: minutes
      ANNAUTOTEXT_TEXT_TOKEN_TIME_SS,              // Current time: seconds
      ANNAUTOTEXT_TEXT_TOKEN_TIME_MILLISECONDS,    // Current time: milliseconds
      ANNAUTOTEXT_TEXT_TOKEN_TIME_AM_PM,           // Current time: AM or PM

      // v14.5 -- 
      ANNAUTOTEXT_FILL_ALPHA_RANGE, // v14.5
      ANNAUTOTEXT_FILL_ALPHA,                // v14.5
      ANNAUTOTEXT_MENU_ROTATEPROPERTIES,
      ANNAUTOTEXT_MENU_ROTATE_SHOW,
      ANNAUTOTEXT_MENU_ROTATE_HIDE,
      ANNAUTOTEXT_MENU_ROTATE_RESET,
      ANNAUTOTEXT_MENU_ROTATE_SHOW_ALL,
      ANNAUTOTEXT_MENU_ROTATE_HIDE_ALL,
      ANNAUTOTEXT_MENU_ROTATE_RESET_ALL,

      // Calibrate Dialog
      ANNAUTOTEXT_CALIBRATE_CAPTION,
      ANNAUTOTEXT_CALIBRATE_HELP,
      ANNAUTOTEXT_CALIBRATE_DESCRIPTION,
      ANNAUTOTEXT_CALIBRATE_LENGTH,
      ANNAUTOTEXT_CALIBRATE_UNITS,
      ANNAUTOTEXT_CALIBRATE_ADVANCED,
      ANNAUTOTEXT_CALIBRATE_DPI,
      ANNAUTOTEXT_CALIBRATE_DPI_RATIO,
      ANNAUTOTEXT_CALIBRATE_DPIX,
      ANNAUTOTEXT_CALIBRATE_DPIY,
      ANNAUTOTEXT_CALIBRATE_APPLY_NEW,
      ANNAUTOTEXT_CALIBRATE_APPLY_EXISTING,
      ANNAUTOTEXT_CALIBRATE_CANCEL,
      ANNAUTOTEXT_CALIBRATE_OK,

      // This one goes in the Ruler Options dialog
      ANNAUTOTEXT_RULER_CALIBRATE, 

      // Text dialog
      ANNAUTOTEXT_TEXT_COLOR,   
      ANNAUTOTEXT_TEXT_SHOW_TEXT,   
      ANNAUTOTEXT_TEXT_SHOW_BORDER,

      // This one goes after FOREGROUNDCOLOR and BACKGROUNDCOLOR
      ANNAUTOTEXT_MENU_TEXTCOLOR,

      // This one goes after ANNAUTOTEXT_MENU_METAFILE
      ANNAUTOTEXT_MENU_SHOWSTAMPBORDER,

      ANNAUTOTEXT_CALIBRATE_REPLACE_EXISTING,
      ANNAUTOTEXT_CALIBRATE_ADDTO_EXISTING,


      ANNAUTOTEXT_MAX,
   };

   enum  ToolBarAlignConstants{
      ANNTOOLALIGN_LEFT = 0x0000,
      ANNTOOLALIGN_RIGHT = 0x0001,
      ANNTOOLALIGN_TOP = 0x0000,
      ANNTOOLALIGN_BOTTOM = 0x0002,
   };

   enum  AnnDefineConstants{
      ANNDEFINE_BEGINSET = 0,
      ANNDEFINE_BEGINMOVE = 1,
      ANNDEFINE_BEGINROTATE = 2,
      ANNDEFINE_BEGINRESIZE = 3,
      ANNDEFINE_BEGINSELECT = 4,
      ANNDEFINE_APPEND = 5,
      ANNDEFINE_UPDATE = 6,
      ANNDEFINE_END = 7,
      ANNDEFINE_BEGINMOVESELECTED = 8,
      ANNDEFINE_BEGINROTATESELECTED = 9,
      ANNDEFINE_BEGINRESIZESELECTED = 10,
      ANNDEFINE_BEGINMOVEPOINT = 11,
      ANNDEFINE_BEGINMOVENAME = 12,
      ANNDEFINE_SETANCHORPOINT = 13,

   };

   enum  AnnAutoMenuItemEnableConstants{
      ANNMENU_DISABLED,
      ANNMENU_ENABLED,
      ANNMENU_DEFAULT,
   };

   enum  AnnUserMenuIDConstants{
      LTANNEVENT_MENUFIRST = 0x0100,
      LTANNEVENT_MENULAST  = 0x01FF,
   };

   enum  AnnMenuItemTypeConstants{
      ANNMENUITEMTYPE_NORMAL,
      ANNMENUITEMTYPE_SEPARATOR,
      ANNMENUITEMTYPE_SUBMENU,
   };

   enum AnnHyperlinkTypeConstants{
    ANNLINK_NONE,
    ANNLINK_ANNEVENT,
    ANNLINK_ANNEVENT5,
    ANNLINK_RUN,
    ANNLINK_WEBPAGE
   };

   enum  AnnEnumerateConstants{
      ANNFLAG_SELECTED     = 0x0001,
      ANNFLAG_NOTCONTAINER = 0x0002,
      ANNFLAG_NOTTHIS      = 0x0004,
      ANNFLAG_RECURSE      = 0x0008,
      ANNFLAG_NOINVALIDATE = 0x0010,
      ANNFLAG_CHECKMENU    = 0x0020,
      ANNFLAG_USER      = 0x0040,
   };

      enum  AnnUnitConstants{
         ANNUNIT_INCHES       = 0,
         ANNUNIT_FEET         = 1,
         ANNUNIT_YARDS        = 2,
         ANNUNIT_MICROMETERS  = 3,
         ANNUNIT_MILIMETERS   = 4,
         ANNUNIT_CENTIMETERS  = 5,
         ANNUNIT_METERS       = 6,
         ANNUNIT_TWIPS        = 7,
         ANNUNIT_POINTS       = 8,
         ANNUNIT_PIXELS       = 9,
         ANNUNIT_SMART_METRIC = 10,
         ANNUNIT_SMART_ENGLISH = 11,
         ANNUNIT_DEF_ABBR     = 0x4000,
      };

      enum  AnnFillModeConstants{
         ANNFILLMODE_TRANSPARENT = 0,
         ANNFILLMODE_TRANSLUCENT = 1,
         ANNFILLMODE_OPAQUE = 2,
         ANNFILLMODE_ALPHA = 3
      };

      enum  AnnHitConstants{
         ANNHIT_NONE    = 0,
         ANNHIT_BODY    = 1,
         ANNHIT_HANDLE  = 2
      };

      enum  AnnMetafileModeConstants{
         ANNMETAFILE_USER = 0x7FFF,    // user-defined metafiles
         ANNMETAFILE_APPROVED = 0,     // predefined metafiles
         ANNMETAFILE_ASSIGNED,
         ANNMETAFILE_CHECKED,
         ANNMETAFILE_CLATPRV,
         ANNMETAFILE_COPY,
         ANNMETAFILE_DRAFT,
         ANNMETAFILE_EXTENDED,
         ANNMETAFILE_FAX,
         ANNMETAFILE_FAXED,
         ANNMETAFILE_IMPORTANT,
         ANNMETAFILE_INVOICE,
         ANNMETAFILE_NOTICE,
         ANNMETAFILE_OFFICIAL,
         ANNMETAFILE_ONFILE,
         ANNMETAFILE_PAID,
         ANNMETAFILE_PASSED,
         ANNMETAFILE_PENDING,
         ANNMETAFILE_PROCESSED,
         ANNMETAFILE_RECEIVED,
         ANNMETAFILE_REJECTED,
         ANNMETAFILE_RELEASE,
         ANNMETAFILE_SENT,
         ANNMETAFILE_SHIPPED,
         ANNMETAFILE_TOPSECRET,
         ANNMETAFILE_URGENT,
         ANNMETAFILE_VOID,
         ANNMETAFILE_HOTSPOT,
         ANNMETAFILE_ENCRYPT,
         ANNMETAFILE_DECRYPT,
         ANNMETAFILE_LAST,             // last predefined metafile
      };

      enum  AnnAngleConstants{
         ANNANGLE_DEGREES  = 0,
         ANNANGLE_RADIANS  = 1,
         ANNANGLE_DEF_ABBR = 0x4000
      };

      enum  AnnFillPatternConstants{
         ANNFILLPATTERN_SOLID        = 0,
         ANNFILLPATTERN_BDIAGONAL    = 1,
         ANNFILLPATTERN_CROSS        = 2,
         ANNFILLPATTERN_DIAGCROSS    = 3,
         ANNFILLPATTERN_FDIAGONAL    = 4,
         ANNFILLPATTERN_HORIZONTAL   = 5,
         ANNFILLPATTERN_VERTICAL     = 6
      };

      enum  AnnLineStyleConstants{
         ANNLINE_SOLID        = 0,
         ANNLINE_DASH         = 1,
         ANNLINE_DOT          = 2,
         ANNLINE_DASHDOT      = 3,
         ANNLINE_DASHDOTDOT   = 4,
         ANNLINE_NULL         = 5
      };

      enum  AnnPolyFillModeConstants{
         ANNPOLYFILL_WINDING   = 0,
         ANNPOLYFILL_ALTERNATE = 1
      };

	  typedef enum {
		ANNENCRYPTAPPLY_ENCRYPTOR  = 0x001,
		ANNENCRYPTAPPLY_DECRYPTOR  = 0x002,
		ANNENCRYPTAPPLY_BOTH       = 0x003,
	  } AnnEncryptApplyConstants;


   enum TransitionStyleConstants{
      EFX_TRANSITION_NONE = 0,
      EFX_TRANSITION_GRADIENT = 1,
      EFX_TRANSITION_PATTERN = 2,
      EFX_TRANSITION_MAX = 2
   };


// Pattern
#define EFX_PATTERN_CLASS                        0
#define EFX_PATTERN_SOLID                        (EFX_PATTERN_CLASS+0)
#define EFX_PATTERN_TRANSPARENT                  (EFX_PATTERN_CLASS+1)
#define EFX_PATTERN_HORZ_LINE                    (EFX_PATTERN_CLASS+2)
#define EFX_PATTERN_VERT_LINE                    (EFX_PATTERN_CLASS+3)
#define EFX_PATTERN_DOWNWARD_DIAG                (EFX_PATTERN_CLASS+4)
#define EFX_PATTERN_UPWARD_DIAG                  (EFX_PATTERN_CLASS+5)
#define EFX_PATTERN_CROSS                        (EFX_PATTERN_CLASS+6)
#define EFX_PATTERN_DIAG_CROSS                   (EFX_PATTERN_CLASS+7)
#define EFX_PATTERN_MAX                          (EFX_PATTERN_CLASS+7)


// Linear
#define EFX_GRADIENT_LINE_CLASS                  1000
#define EFX_GRADIENT_LINE_L_TO_R                 (EFX_GRADIENT_LINE_CLASS+0)
#define EFX_GRADIENT_LINE_R_TO_L                 (EFX_GRADIENT_LINE_CLASS+1)
#define EFX_GRADIENT_LINE_T_TO_B                 (EFX_GRADIENT_LINE_CLASS+2)
#define EFX_GRADIENT_LINE_B_TO_T                 (EFX_GRADIENT_LINE_CLASS+3)
#define EFX_GRADIENT_LINE_LT_TO_RB               (EFX_GRADIENT_LINE_CLASS+4)
#define EFX_GRADIENT_LINE_LB_TO_RT               (EFX_GRADIENT_LINE_CLASS+5)
#define EFX_GRADIENT_LINE_RT_TO_LB               (EFX_GRADIENT_LINE_CLASS+6)
#define EFX_GRADIENT_LINE_RB_TO_LT               (EFX_GRADIENT_LINE_CLASS+7)
#define EFX_GRADIENT_LINE2_L_AND_R_TO_C          (EFX_GRADIENT_LINE_CLASS+8)
#define EFX_GRADIENT_LINE2_C_TO_L_AND_R          (EFX_GRADIENT_LINE_CLASS+9)
#define EFX_GRADIENT_LINE2_T_AND_B_TO_C          (EFX_GRADIENT_LINE_CLASS+10)
#define EFX_GRADIENT_LINE2_C_TO_T_AND_B          (EFX_GRADIENT_LINE_CLASS+11)
#define EFX_GRADIENT_LINE2_LT_AND_RB_TO_C        (EFX_GRADIENT_LINE_CLASS+12)
#define EFX_GRADIENT_LINE2_C_TO_LT_AND_RB        (EFX_GRADIENT_LINE_CLASS+13)
#define EFX_GRADIENT_LINE2_RT_AND_LB_TO_C        (EFX_GRADIENT_LINE_CLASS+14)
#define EFX_GRADIENT_LINE2_C_TO_RT_AND_LB        (EFX_GRADIENT_LINE_CLASS+15)
#define EFX_GRADIENT_LINE_MAX                    (EFX_GRADIENT_LINE_CLASS+15)

// Angular
#define EFX_GRADIENT_ANGLE_CLASS                 2000
#define EFX_GRADIENT_ANGLE_TO_LT                 (EFX_GRADIENT_ANGLE_CLASS+0)
#define EFX_GRADIENT_ANGLE_TO_LB                 (EFX_GRADIENT_ANGLE_CLASS+1)
#define EFX_GRADIENT_ANGLE_TO_RT                 (EFX_GRADIENT_ANGLE_CLASS+2)
#define EFX_GRADIENT_ANGLE_TO_RB                 (EFX_GRADIENT_ANGLE_CLASS+3)
#define EFX_GRADIENT_ANGLE_FROM_LT               (EFX_GRADIENT_ANGLE_CLASS+4)
#define EFX_GRADIENT_ANGLE_FROM_LB               (EFX_GRADIENT_ANGLE_CLASS+5)
#define EFX_GRADIENT_ANGLE_FROM_RT               (EFX_GRADIENT_ANGLE_CLASS+6)
#define EFX_GRADIENT_ANGLE_FROM_RB               (EFX_GRADIENT_ANGLE_CLASS+7)
#define EFX_GRADIENT_ANGLE_MAX                   (EFX_GRADIENT_ANGLE_CLASS+7)

// Rectangular
#define EFX_GRADIENT_RECTANGLE_CLASS             3000
#define EFX_GRADIENT_RECTANGLE_TO_C              (EFX_GRADIENT_RECTANGLE_CLASS+0)
#define EFX_GRADIENT_RECTANGLE_FROM_C            (EFX_GRADIENT_RECTANGLE_CLASS+1)
#define EFX_GRADIENT_RECTANGLE_MAX               (EFX_GRADIENT_RECTANGLE_CLASS+1)

// Elliptical
#define EFX_GRADIENT_ELLIPSE_CLASS               4000
#define EFX_GRADIENT_ELLIPSE_TO_C                (EFX_GRADIENT_ELLIPSE_CLASS+0)
#define EFX_GRADIENT_ELLIPSE_FROM_C              (EFX_GRADIENT_ELLIPSE_CLASS+1)
#define EFX_GRADIENT_ELLIPSE_MAX                 (EFX_GRADIENT_ELLIPSE_CLASS+1)

// Conical
#define EFX_GRADIENT_CON_CLASS                  5000
#define EFX_GRADIENT_CON_FROM_L                 (EFX_GRADIENT_CON_CLASS+0)
#define EFX_GRADIENT_CON_FROM_R                 (EFX_GRADIENT_CON_CLASS+1)
#define EFX_GRADIENT_CON_FROM_T                 (EFX_GRADIENT_CON_CLASS+2)
#define EFX_GRADIENT_CON_FROM_B                 (EFX_GRADIENT_CON_CLASS+3)
#define EFX_GRADIENT_CON_FROM_LT                (EFX_GRADIENT_CON_CLASS+4)
#define EFX_GRADIENT_CON_FROM_LB                (EFX_GRADIENT_CON_CLASS+5)
#define EFX_GRADIENT_CON_FROM_RT                (EFX_GRADIENT_CON_CLASS+6)
#define EFX_GRADIENT_CON_FROM_RB                (EFX_GRADIENT_CON_CLASS+7)
#define EFX_GRADIENT_CON_MAX                    (EFX_GRADIENT_CON_CLASS+7)


// Square
#define EFX_SHAPE_SQUARE_CLASS                   1000
#define EFX_SHAPE_SQUARE                         (EFX_SHAPE_SQUARE_CLASS+0)
#define EFX_SHAPE_SQUARE_ROUNDED                 (EFX_SHAPE_SQUARE_CLASS+1)
#define EFX_SHAPE_SQUARE_MAX                     (EFX_SHAPE_SQUARE_CLASS+1)

// Rectangle
#define EFX_SHAPE_RECTANGLE_CLASS                2000
#define EFX_SHAPE_RECTANGLE                      (EFX_SHAPE_RECTANGLE_CLASS+0)
#define EFX_SHAPE_RECTANGLE_ROUNDED              (EFX_SHAPE_RECTANGLE_CLASS+1)
#define EFX_SHAPE_RECTANGLE_MAX                  (EFX_SHAPE_RECTANGLE_CLASS+1)

// Parallelogram
#define EFX_SHAPE_PARALLELOGRAM_CLASS            3000
#define EFX_SHAPE_PARALLELOGRAM_L                (EFX_SHAPE_PARALLELOGRAM_CLASS+0)
#define EFX_SHAPE_PARALLELOGRAM_R                (EFX_SHAPE_PARALLELOGRAM_CLASS+1)
#define EFX_SHAPE_PARALLELOGRAM_T                (EFX_SHAPE_PARALLELOGRAM_CLASS+2)
#define EFX_SHAPE_PARALLELOGRAM_B                (EFX_SHAPE_PARALLELOGRAM_CLASS+3)
#define EFX_SHAPE_PARALLELOGRAM_MAX              (EFX_SHAPE_PARALLELOGRAM_CLASS+3)

// Trapezoid
#define EFX_SHAPE_TRAPEZOID_CLASS                4000
#define EFX_SHAPE_TRAPEZOID_L                    (EFX_SHAPE_TRAPEZOID_CLASS+0)
#define EFX_SHAPE_TRAPEZOID_R                    (EFX_SHAPE_TRAPEZOID_CLASS+1)
#define EFX_SHAPE_TRAPEZOID_T                    (EFX_SHAPE_TRAPEZOID_CLASS+2)
#define EFX_SHAPE_TRAPEZOID_B                    (EFX_SHAPE_TRAPEZOID_CLASS+3)
#define EFX_SHAPE_TRAPEZOID_MAX                  (EFX_SHAPE_TRAPEZOID_CLASS+3)

// Triangle
#define EFX_SHAPE_TRIANGLE_CLASS                 5000
#define EFX_SHAPE_TRIANGLE_EQUILATERAL_L         (EFX_SHAPE_TRIANGLE_CLASS+0)
#define EFX_SHAPE_TRIANGLE_EQUILATERAL_R         (EFX_SHAPE_TRIANGLE_CLASS+1)
#define EFX_SHAPE_TRIANGLE_EQUILATERAL_T         (EFX_SHAPE_TRIANGLE_CLASS+2)
#define EFX_SHAPE_TRIANGLE_EQUILATERAL_B         (EFX_SHAPE_TRIANGLE_CLASS+3)
#define EFX_SHAPE_TRIANGLE_ISOSCELES_L           (EFX_SHAPE_TRIANGLE_CLASS+4)
#define EFX_SHAPE_TRIANGLE_ISOSCELES_R           (EFX_SHAPE_TRIANGLE_CLASS+5)
#define EFX_SHAPE_TRIANGLE_ISOSCELES_T           (EFX_SHAPE_TRIANGLE_CLASS+6)
#define EFX_SHAPE_TRIANGLE_ISOSCELES_B           (EFX_SHAPE_TRIANGLE_CLASS+7)
#define EFX_SHAPE_TRIANGLE_RIGHT_LT              (EFX_SHAPE_TRIANGLE_CLASS+8)
#define EFX_SHAPE_TRIANGLE_RIGHT_LB              (EFX_SHAPE_TRIANGLE_CLASS+9)
#define EFX_SHAPE_TRIANGLE_RIGHT_RT              (EFX_SHAPE_TRIANGLE_CLASS+10)
#define EFX_SHAPE_TRIANGLE_RIGHT_RB              (EFX_SHAPE_TRIANGLE_CLASS+11)
#define EFX_SHAPE_TRIANGLE_MAX                   (EFX_SHAPE_TRIANGLE_CLASS+11)

// Other polygons
#define EFX_SHAPE_OTHER_CLASS                    6000
#define EFX_SHAPE_OCTAGON                        (EFX_SHAPE_OTHER_CLASS+0)
#define EFX_SHAPE_DIAMOND                        (EFX_SHAPE_OTHER_CLASS+1)
#define EFX_SHAPE_PENTAGON                       (EFX_SHAPE_OTHER_CLASS+2)
#define EFX_SHAPE_HEXAGON                        (EFX_SHAPE_OTHER_CLASS+3)
#define EFX_SHAPE_LINE                           (EFX_SHAPE_OTHER_CLASS+4)
#define EFX_SHAPE_REGION                         (EFX_SHAPE_OTHER_CLASS+5)
#define EFX_SHAPE_OTHER_MAX                      (EFX_SHAPE_OTHER_CLASS+5)

// Circle
#define EFX_SHAPE_CIRCLE_CLASS                   7000
#define EFX_SHAPE_CIRCLE                         (EFX_SHAPE_CIRCLE_CLASS+0)
#define EFX_SHAPE_CIRCLE_90_LT                   (EFX_SHAPE_CIRCLE_CLASS+1)
#define EFX_SHAPE_CIRCLE_90_LB                   (EFX_SHAPE_CIRCLE_CLASS+2)
#define EFX_SHAPE_CIRCLE_90_RT                   (EFX_SHAPE_CIRCLE_CLASS+3)
#define EFX_SHAPE_CIRCLE_90_RB                   (EFX_SHAPE_CIRCLE_CLASS+4)
#define EFX_SHAPE_CIRCLE_180_L                   (EFX_SHAPE_CIRCLE_CLASS+5)
#define EFX_SHAPE_CIRCLE_180_R                   (EFX_SHAPE_CIRCLE_CLASS+6)
#define EFX_SHAPE_CIRCLE_180_T                   (EFX_SHAPE_CIRCLE_CLASS+7)
#define EFX_SHAPE_CIRCLE_180_B                   (EFX_SHAPE_CIRCLE_CLASS+8)
#define EFX_SHAPE_CIRCLE_270_LT                  (EFX_SHAPE_CIRCLE_CLASS+9)
#define EFX_SHAPE_CIRCLE_270_LB                  (EFX_SHAPE_CIRCLE_CLASS+10)
#define EFX_SHAPE_CIRCLE_270_RT                  (EFX_SHAPE_CIRCLE_CLASS+11)
#define EFX_SHAPE_CIRCLE_270_RB                  (EFX_SHAPE_CIRCLE_CLASS+12)
#define EFX_SHAPE_CIRCLE_MAX                     (EFX_SHAPE_CIRCLE_CLASS+12)

// Ellipse
#define EFX_SHAPE_ELLIPSE_CLASS                  8000
#define EFX_SHAPE_ELLIPSE                        (EFX_SHAPE_ELLIPSE_CLASS+0)
#define EFX_SHAPE_ELLIPSE_90_LT                  (EFX_SHAPE_ELLIPSE_CLASS+1)
#define EFX_SHAPE_ELLIPSE_90_LB                  (EFX_SHAPE_ELLIPSE_CLASS+2)
#define EFX_SHAPE_ELLIPSE_90_RT                  (EFX_SHAPE_ELLIPSE_CLASS+3)
#define EFX_SHAPE_ELLIPSE_90_RB                  (EFX_SHAPE_ELLIPSE_CLASS+4)
#define EFX_SHAPE_ELLIPSE_180_L                  (EFX_SHAPE_ELLIPSE_CLASS+5)
#define EFX_SHAPE_ELLIPSE_180_R                  (EFX_SHAPE_ELLIPSE_CLASS+6)
#define EFX_SHAPE_ELLIPSE_180_T                  (EFX_SHAPE_ELLIPSE_CLASS+7)
#define EFX_SHAPE_ELLIPSE_180_B                  (EFX_SHAPE_ELLIPSE_CLASS+8)
#define EFX_SHAPE_ELLIPSE_270_LT                 (EFX_SHAPE_ELLIPSE_CLASS+9)
#define EFX_SHAPE_ELLIPSE_270_LB                 (EFX_SHAPE_ELLIPSE_CLASS+10)
#define EFX_SHAPE_ELLIPSE_270_RT                 (EFX_SHAPE_ELLIPSE_CLASS+11)
#define EFX_SHAPE_ELLIPSE_270_RB                 (EFX_SHAPE_ELLIPSE_CLASS+12)
#define EFX_SHAPE_ELLIPSE_MAX                    (EFX_SHAPE_ELLIPSE_CLASS+12)

// Star
#define EFX_SHAPE_STAR_CLASS                     9000
#define EFX_SHAPE_STAR4                          (EFX_SHAPE_STAR_CLASS+0)
#define EFX_SHAPE_STAR5                          (EFX_SHAPE_STAR_CLASS+1)
#define EFX_SHAPE_STAR8                          (EFX_SHAPE_STAR_CLASS+2)
#define EFX_SHAPE_STAR16                         (EFX_SHAPE_STAR_CLASS+3)
#define EFX_SHAPE_STAR24                         (EFX_SHAPE_STAR_CLASS+4)
#define EFX_SHAPE_STAR32                         (EFX_SHAPE_STAR_CLASS+5)
#define EFX_SHAPE_STAR_MAX                       (EFX_SHAPE_STAR_CLASS+5)

// Cross
#define EFX_SHAPE_CROSS_CLASS                    10000
#define EFX_SHAPE_CROSS_SMALL                    (EFX_SHAPE_CROSS_CLASS+0)
#define EFX_SHAPE_CROSS_MEDIUM                   (EFX_SHAPE_CROSS_CLASS+1)
#define EFX_SHAPE_CROSS_LARGE                    (EFX_SHAPE_CROSS_CLASS+2)
#define EFX_SHAPE_CROSS_MAX                      (EFX_SHAPE_CROSS_CLASS+2)

// Arrow
#define EFX_SHAPE_ARROW_CLASS                    11000
#define EFX_SHAPE_ARROW_L                        (EFX_SHAPE_ARROW_CLASS+0)
#define EFX_SHAPE_ARROW_R                        (EFX_SHAPE_ARROW_CLASS+1)
#define EFX_SHAPE_ARROW_T                        (EFX_SHAPE_ARROW_CLASS+2)
#define EFX_SHAPE_ARROW_B                        (EFX_SHAPE_ARROW_CLASS+3)
#define EFX_SHAPE_ARROW2_L_AND_R                 (EFX_SHAPE_ARROW_CLASS+4)
#define EFX_SHAPE_ARROW2_T_AND_B                 (EFX_SHAPE_ARROW_CLASS+5)
#define EFX_SHAPE_ARROW2_L_AND_T                 (EFX_SHAPE_ARROW_CLASS+6)
#define EFX_SHAPE_ARROW2_L_AND_B                 (EFX_SHAPE_ARROW_CLASS+7)
#define EFX_SHAPE_ARROW2_R_AND_T                 (EFX_SHAPE_ARROW_CLASS+8)
#define EFX_SHAPE_ARROW2_R_AND_B                 (EFX_SHAPE_ARROW_CLASS+9)
#define EFX_SHAPE_ARROW3_L_AND_T_AND_R           (EFX_SHAPE_ARROW_CLASS+10)
#define EFX_SHAPE_ARROW3_T_AND_R_AND_B           (EFX_SHAPE_ARROW_CLASS+11)
#define EFX_SHAPE_ARROW3_L_AND_B_AND_R           (EFX_SHAPE_ARROW_CLASS+12)
#define EFX_SHAPE_ARROW3_L_AND_T_AND_B           (EFX_SHAPE_ARROW_CLASS+13)
#define EFX_SHAPE_ARROW4_L_AND_T_AND_R_AND_B     (EFX_SHAPE_ARROW_CLASS+14)
#define EFX_SHAPE_ARROW_MAX                      (EFX_SHAPE_ARROW_CLASS+14)


//Effect types

// No effect
#define EFX_EFFECT_NONE_CLASS                                 1000
#define EFX_EFFECT_NONE                                       (EFX_EFFECT_NONE_CLASS+0)
#define EFX_EFFECT_NO_PICTURE                                 (EFX_EFFECT_NONE_CLASS+1)
#define EFX_EFFECT_NONE_MAX                                   (EFX_EFFECT_NONE_CLASS+1)

// Linear wipe
#define EFX_EFFECT_WIPE_CLASS                                 2000
#define EFX_EFFECT_WIPE_L_TO_R                                (EFX_EFFECT_WIPE_CLASS+0)
#define EFX_EFFECT_WIPE_R_TO_L                                (EFX_EFFECT_WIPE_CLASS+1)
#define EFX_EFFECT_WIPE_T_TO_B                                (EFX_EFFECT_WIPE_CLASS+2)
#define EFX_EFFECT_WIPE_B_TO_T                                (EFX_EFFECT_WIPE_CLASS+3)
#define EFX_EFFECT_WIPE_LT_TO_RB                              (EFX_EFFECT_WIPE_CLASS+4)
#define EFX_EFFECT_WIPE_LB_TO_RT                              (EFX_EFFECT_WIPE_CLASS+5)
#define EFX_EFFECT_WIPE_RT_TO_LB                              (EFX_EFFECT_WIPE_CLASS+6)
#define EFX_EFFECT_WIPE_RB_TO_LT                              (EFX_EFFECT_WIPE_CLASS+7)
#define EFX_EFFECT_WIPE_INTERH_L_TO_R_THEN_L_TO_R             (EFX_EFFECT_WIPE_CLASS+8)
#define EFX_EFFECT_WIPE_INTERV_T_TO_B_THEN_T_TO_B             (EFX_EFFECT_WIPE_CLASS+9)
#define EFX_EFFECT_WIPE_INTERH_L_TO_R_THEN_R_TO_L             (EFX_EFFECT_WIPE_CLASS+10)
#define EFX_EFFECT_WIPE_INTERV_T_TO_B_THEN_B_TO_T             (EFX_EFFECT_WIPE_CLASS+11)
#define EFX_EFFECT_WIPE_INTERV_L_TO_R_THEN_L_TO_R             (EFX_EFFECT_WIPE_CLASS+12)
#define EFX_EFFECT_WIPE_INTERV_R_TO_L_THEN_R_TO_L             (EFX_EFFECT_WIPE_CLASS+13)
#define EFX_EFFECT_WIPE_INTERH_T_TO_B_THEN_T_TO_B             (EFX_EFFECT_WIPE_CLASS+14)
#define EFX_EFFECT_WIPE_INTERH_B_TO_T_THEN_B_TO_T             (EFX_EFFECT_WIPE_CLASS+15)
#define EFX_EFFECT_WIPE_INTERV_L_TO_R_THEN_R_TO_L             (EFX_EFFECT_WIPE_CLASS+16)
#define EFX_EFFECT_WIPE_INTERV_R_TO_L_THEN_L_TO_R             (EFX_EFFECT_WIPE_CLASS+17)
#define EFX_EFFECT_WIPE_INTERH_T_TO_B_THEN_B_TO_T             (EFX_EFFECT_WIPE_CLASS+18)
#define EFX_EFFECT_WIPE_INTERH_B_TO_T_THEN_T_TO_B             (EFX_EFFECT_WIPE_CLASS+19)
#define EFX_EFFECT_WIPE_INTERH_L_TO_R_AND_R_TO_L              (EFX_EFFECT_WIPE_CLASS+20)
#define EFX_EFFECT_WIPE_INTERV_T_TO_B_AND_B_TO_T              (EFX_EFFECT_WIPE_CLASS+21)
#define EFX_EFFECT_WIPE_INTERV_L_AND_R_TO_C                   (EFX_EFFECT_WIPE_CLASS+22)
#define EFX_EFFECT_WIPE_INTERV_C_TO_L_AND_R                   (EFX_EFFECT_WIPE_CLASS+23)
#define EFX_EFFECT_WIPE_INTERH_T_AND_B_TO_C                   (EFX_EFFECT_WIPE_CLASS+24)
#define EFX_EFFECT_WIPE_INTERH_C_TO_T_AND_B                   (EFX_EFFECT_WIPE_CLASS+25)
#define EFX_EFFECT_WIPE_INTERV_L_AND_R_TO_C_THEN_C_TO_L_AND_R (EFX_EFFECT_WIPE_CLASS+26)
#define EFX_EFFECT_WIPE_INTERV_C_TO_L_AND_R_THEN_L_AND_R_TO_C (EFX_EFFECT_WIPE_CLASS+27)
#define EFX_EFFECT_WIPE_INTERH_T_AND_B_TO_C_THEN_C_TO_T_AND_B (EFX_EFFECT_WIPE_CLASS+28)
#define EFX_EFFECT_WIPE_INTERH_C_TO_T_AND_B_THEN_T_AND_B_TO_C (EFX_EFFECT_WIPE_CLASS+29)
#define EFX_EFFECT_WIPE2_LT_AND_RB_TO_C                       (EFX_EFFECT_WIPE_CLASS+30)
#define EFX_EFFECT_WIPE2_LB_AND_RT_TO_C                       (EFX_EFFECT_WIPE_CLASS+31)
#define EFX_EFFECT_WIPE2_C_TO_LT_AND_RB                       (EFX_EFFECT_WIPE_CLASS+32)
#define EFX_EFFECT_WIPE2_C_TO_LB_AND_RT                       (EFX_EFFECT_WIPE_CLASS+33)
#define EFX_EFFECT_WIPE2_LB_AND_RB_TO_T                       (EFX_EFFECT_WIPE_CLASS+34)
#define EFX_EFFECT_WIPE2_LT_AND_RT_TO_B                       (EFX_EFFECT_WIPE_CLASS+35)
#define EFX_EFFECT_WIPE2_RT_AND_RB_TO_L                       (EFX_EFFECT_WIPE_CLASS+36)
#define EFX_EFFECT_WIPE2_LT_AND_LB_TO_R                       (EFX_EFFECT_WIPE_CLASS+37)
#define EFX_EFFECT_WIPE2_L_T_TO_B_AND_R_B_TO_T                (EFX_EFFECT_WIPE_CLASS+38)
#define EFX_EFFECT_WIPE2_T_L_TO_R_AND_B_R_TO_L                (EFX_EFFECT_WIPE_CLASS+39)
#define EFX_EFFECT_WIPE4_CORNERS_OUT                          (EFX_EFFECT_WIPE_CLASS+40)
#define EFX_EFFECT_WIPE4_C_TO_L_AND_R_THEN_T_AND_B            (EFX_EFFECT_WIPE_CLASS+41)
#define EFX_EFFECT_WIPE4_T_AND_B_THEN_L_AND_R_TO_C            (EFX_EFFECT_WIPE_CLASS+42)
#define EFX_EFFECT_WIPE4_L_L_L_L                              (EFX_EFFECT_WIPE_CLASS+43)
#define EFX_EFFECT_WIPE4_L_L_L_R                              (EFX_EFFECT_WIPE_CLASS+44)
#define EFX_EFFECT_WIPE4_L_L_L_T                              (EFX_EFFECT_WIPE_CLASS+45)
#define EFX_EFFECT_WIPE4_L_L_L_B                              (EFX_EFFECT_WIPE_CLASS+46)
#define EFX_EFFECT_WIPE4_L_L_R_L                              (EFX_EFFECT_WIPE_CLASS+47)
#define EFX_EFFECT_WIPE4_L_L_R_R                              (EFX_EFFECT_WIPE_CLASS+48)
#define EFX_EFFECT_WIPE4_L_L_R_T                              (EFX_EFFECT_WIPE_CLASS+49)
#define EFX_EFFECT_WIPE4_L_L_R_B                              (EFX_EFFECT_WIPE_CLASS+50)
#define EFX_EFFECT_WIPE4_L_L_T_L                              (EFX_EFFECT_WIPE_CLASS+51)
#define EFX_EFFECT_WIPE4_L_L_T_R                              (EFX_EFFECT_WIPE_CLASS+52)
#define EFX_EFFECT_WIPE4_L_L_T_T                              (EFX_EFFECT_WIPE_CLASS+53)
#define EFX_EFFECT_WIPE4_L_L_T_B                              (EFX_EFFECT_WIPE_CLASS+54)
#define EFX_EFFECT_WIPE4_L_L_B_L                              (EFX_EFFECT_WIPE_CLASS+55)
#define EFX_EFFECT_WIPE4_L_L_B_R                              (EFX_EFFECT_WIPE_CLASS+56)
#define EFX_EFFECT_WIPE4_L_L_B_T                              (EFX_EFFECT_WIPE_CLASS+57)
#define EFX_EFFECT_WIPE4_L_L_B_B                              (EFX_EFFECT_WIPE_CLASS+58)
#define EFX_EFFECT_WIPE4_L_R_L_L                              (EFX_EFFECT_WIPE_CLASS+59)
#define EFX_EFFECT_WIPE4_L_R_L_R                              (EFX_EFFECT_WIPE_CLASS+60)
#define EFX_EFFECT_WIPE4_L_R_L_T                              (EFX_EFFECT_WIPE_CLASS+61)
#define EFX_EFFECT_WIPE4_L_R_L_B                              (EFX_EFFECT_WIPE_CLASS+62)
#define EFX_EFFECT_WIPE4_L_R_R_L                              (EFX_EFFECT_WIPE_CLASS+63)
#define EFX_EFFECT_WIPE4_L_R_R_R                              (EFX_EFFECT_WIPE_CLASS+64)
#define EFX_EFFECT_WIPE4_L_R_R_T                              (EFX_EFFECT_WIPE_CLASS+65)
#define EFX_EFFECT_WIPE4_L_R_R_B                              (EFX_EFFECT_WIPE_CLASS+66)
#define EFX_EFFECT_WIPE4_L_R_T_L                              (EFX_EFFECT_WIPE_CLASS+67)
#define EFX_EFFECT_WIPE4_L_R_T_R                              (EFX_EFFECT_WIPE_CLASS+68)
#define EFX_EFFECT_WIPE4_L_R_T_T                              (EFX_EFFECT_WIPE_CLASS+69)
#define EFX_EFFECT_WIPE4_L_R_T_B                              (EFX_EFFECT_WIPE_CLASS+70)
#define EFX_EFFECT_WIPE4_L_R_B_L                              (EFX_EFFECT_WIPE_CLASS+71)
#define EFX_EFFECT_WIPE4_L_R_B_R                              (EFX_EFFECT_WIPE_CLASS+72)
#define EFX_EFFECT_WIPE4_L_R_B_T                              (EFX_EFFECT_WIPE_CLASS+73)
#define EFX_EFFECT_WIPE4_L_R_B_B                              (EFX_EFFECT_WIPE_CLASS+74)
#define EFX_EFFECT_WIPE4_L_T_L_L                              (EFX_EFFECT_WIPE_CLASS+75)
#define EFX_EFFECT_WIPE4_L_T_L_R                              (EFX_EFFECT_WIPE_CLASS+76)
#define EFX_EFFECT_WIPE4_L_T_L_T                              (EFX_EFFECT_WIPE_CLASS+77)
#define EFX_EFFECT_WIPE4_L_T_L_B                              (EFX_EFFECT_WIPE_CLASS+78)
#define EFX_EFFECT_WIPE4_L_T_R_L                              (EFX_EFFECT_WIPE_CLASS+79)
#define EFX_EFFECT_WIPE4_L_T_R_R                              (EFX_EFFECT_WIPE_CLASS+80)
#define EFX_EFFECT_WIPE4_L_T_R_T                              (EFX_EFFECT_WIPE_CLASS+81)
#define EFX_EFFECT_WIPE4_L_T_R_B                              (EFX_EFFECT_WIPE_CLASS+82)
#define EFX_EFFECT_WIPE4_L_T_T_L                              (EFX_EFFECT_WIPE_CLASS+83)
#define EFX_EFFECT_WIPE4_L_T_T_R                              (EFX_EFFECT_WIPE_CLASS+84)
#define EFX_EFFECT_WIPE4_L_T_T_T                              (EFX_EFFECT_WIPE_CLASS+85)
#define EFX_EFFECT_WIPE4_L_T_T_B                              (EFX_EFFECT_WIPE_CLASS+86)
#define EFX_EFFECT_WIPE4_L_T_B_L                              (EFX_EFFECT_WIPE_CLASS+87)
#define EFX_EFFECT_WIPE4_L_T_B_R                              (EFX_EFFECT_WIPE_CLASS+88)
#define EFX_EFFECT_WIPE4_L_T_B_T                              (EFX_EFFECT_WIPE_CLASS+89)
#define EFX_EFFECT_WIPE4_L_T_B_B                              (EFX_EFFECT_WIPE_CLASS+90)
#define EFX_EFFECT_WIPE4_L_B_L_L                              (EFX_EFFECT_WIPE_CLASS+91)
#define EFX_EFFECT_WIPE4_L_B_L_R                              (EFX_EFFECT_WIPE_CLASS+92)
#define EFX_EFFECT_WIPE4_L_B_L_T                              (EFX_EFFECT_WIPE_CLASS+93)
#define EFX_EFFECT_WIPE4_L_B_L_B                              (EFX_EFFECT_WIPE_CLASS+94)
#define EFX_EFFECT_WIPE4_L_B_R_L                              (EFX_EFFECT_WIPE_CLASS+95)
#define EFX_EFFECT_WIPE4_L_B_R_R                              (EFX_EFFECT_WIPE_CLASS+96)
#define EFX_EFFECT_WIPE4_L_B_R_T                              (EFX_EFFECT_WIPE_CLASS+97)
#define EFX_EFFECT_WIPE4_L_B_R_B                              (EFX_EFFECT_WIPE_CLASS+98)
#define EFX_EFFECT_WIPE4_L_B_T_L                              (EFX_EFFECT_WIPE_CLASS+99)
#define EFX_EFFECT_WIPE4_L_B_T_R                              (EFX_EFFECT_WIPE_CLASS+100)
#define EFX_EFFECT_WIPE4_L_B_T_T                              (EFX_EFFECT_WIPE_CLASS+101)
#define EFX_EFFECT_WIPE4_L_B_T_B                              (EFX_EFFECT_WIPE_CLASS+102)
#define EFX_EFFECT_WIPE4_L_B_B_L                              (EFX_EFFECT_WIPE_CLASS+103)
#define EFX_EFFECT_WIPE4_L_B_B_R                              (EFX_EFFECT_WIPE_CLASS+104)
#define EFX_EFFECT_WIPE4_L_B_B_T                              (EFX_EFFECT_WIPE_CLASS+105)
#define EFX_EFFECT_WIPE4_L_B_B_B                              (EFX_EFFECT_WIPE_CLASS+106)
#define EFX_EFFECT_WIPE4_R_L_L_L                              (EFX_EFFECT_WIPE_CLASS+107)
#define EFX_EFFECT_WIPE4_R_L_L_R                              (EFX_EFFECT_WIPE_CLASS+108)
#define EFX_EFFECT_WIPE4_R_L_L_T                              (EFX_EFFECT_WIPE_CLASS+109)
#define EFX_EFFECT_WIPE4_R_L_L_B                              (EFX_EFFECT_WIPE_CLASS+110)
#define EFX_EFFECT_WIPE4_R_L_R_L                              (EFX_EFFECT_WIPE_CLASS+111)
#define EFX_EFFECT_WIPE4_R_L_R_R                              (EFX_EFFECT_WIPE_CLASS+112)
#define EFX_EFFECT_WIPE4_R_L_R_T                              (EFX_EFFECT_WIPE_CLASS+113)
#define EFX_EFFECT_WIPE4_R_L_R_B                              (EFX_EFFECT_WIPE_CLASS+114)
#define EFX_EFFECT_WIPE4_R_L_T_L                              (EFX_EFFECT_WIPE_CLASS+115)
#define EFX_EFFECT_WIPE4_R_L_T_R                              (EFX_EFFECT_WIPE_CLASS+116)
#define EFX_EFFECT_WIPE4_R_L_T_T                              (EFX_EFFECT_WIPE_CLASS+117)
#define EFX_EFFECT_WIPE4_R_L_T_B                              (EFX_EFFECT_WIPE_CLASS+118)
#define EFX_EFFECT_WIPE4_R_L_B_L                              (EFX_EFFECT_WIPE_CLASS+119)
#define EFX_EFFECT_WIPE4_R_L_B_R                              (EFX_EFFECT_WIPE_CLASS+120)
#define EFX_EFFECT_WIPE4_R_L_B_T                              (EFX_EFFECT_WIPE_CLASS+121)
#define EFX_EFFECT_WIPE4_R_L_B_B                              (EFX_EFFECT_WIPE_CLASS+122)
#define EFX_EFFECT_WIPE4_R_R_L_L                              (EFX_EFFECT_WIPE_CLASS+123)
#define EFX_EFFECT_WIPE4_R_R_L_R                              (EFX_EFFECT_WIPE_CLASS+124)
#define EFX_EFFECT_WIPE4_R_R_L_T                              (EFX_EFFECT_WIPE_CLASS+125)
#define EFX_EFFECT_WIPE4_R_R_L_B                              (EFX_EFFECT_WIPE_CLASS+126)
#define EFX_EFFECT_WIPE4_R_R_R_L                              (EFX_EFFECT_WIPE_CLASS+127)
#define EFX_EFFECT_WIPE4_R_R_R_R                              (EFX_EFFECT_WIPE_CLASS+128)
#define EFX_EFFECT_WIPE4_R_R_R_T                              (EFX_EFFECT_WIPE_CLASS+129)
#define EFX_EFFECT_WIPE4_R_R_R_B                              (EFX_EFFECT_WIPE_CLASS+130)
#define EFX_EFFECT_WIPE4_R_R_T_L                              (EFX_EFFECT_WIPE_CLASS+131)
#define EFX_EFFECT_WIPE4_R_R_T_R                              (EFX_EFFECT_WIPE_CLASS+132)
#define EFX_EFFECT_WIPE4_R_R_T_T                              (EFX_EFFECT_WIPE_CLASS+133)
#define EFX_EFFECT_WIPE4_R_R_T_B                              (EFX_EFFECT_WIPE_CLASS+134)
#define EFX_EFFECT_WIPE4_R_R_B_L                              (EFX_EFFECT_WIPE_CLASS+135)
#define EFX_EFFECT_WIPE4_R_R_B_R                              (EFX_EFFECT_WIPE_CLASS+136)
#define EFX_EFFECT_WIPE4_R_R_B_T                              (EFX_EFFECT_WIPE_CLASS+137)
#define EFX_EFFECT_WIPE4_R_R_B_B                              (EFX_EFFECT_WIPE_CLASS+138)
#define EFX_EFFECT_WIPE4_R_T_L_L                              (EFX_EFFECT_WIPE_CLASS+139)
#define EFX_EFFECT_WIPE4_R_T_L_R                              (EFX_EFFECT_WIPE_CLASS+140)
#define EFX_EFFECT_WIPE4_R_T_L_T                              (EFX_EFFECT_WIPE_CLASS+141)
#define EFX_EFFECT_WIPE4_R_T_L_B                              (EFX_EFFECT_WIPE_CLASS+142)
#define EFX_EFFECT_WIPE4_R_T_R_L                              (EFX_EFFECT_WIPE_CLASS+143)
#define EFX_EFFECT_WIPE4_R_T_R_R                              (EFX_EFFECT_WIPE_CLASS+144)
#define EFX_EFFECT_WIPE4_R_T_R_T                              (EFX_EFFECT_WIPE_CLASS+145)
#define EFX_EFFECT_WIPE4_R_T_R_B                              (EFX_EFFECT_WIPE_CLASS+146)
#define EFX_EFFECT_WIPE4_R_T_T_L                              (EFX_EFFECT_WIPE_CLASS+147)
#define EFX_EFFECT_WIPE4_R_T_T_R                              (EFX_EFFECT_WIPE_CLASS+148)
#define EFX_EFFECT_WIPE4_R_T_T_T                              (EFX_EFFECT_WIPE_CLASS+149)
#define EFX_EFFECT_WIPE4_R_T_T_B                              (EFX_EFFECT_WIPE_CLASS+150)
#define EFX_EFFECT_WIPE4_R_T_B_L                              (EFX_EFFECT_WIPE_CLASS+151)
#define EFX_EFFECT_WIPE4_R_T_B_R                              (EFX_EFFECT_WIPE_CLASS+152)
#define EFX_EFFECT_WIPE4_R_T_B_T                              (EFX_EFFECT_WIPE_CLASS+153)
#define EFX_EFFECT_WIPE4_R_T_B_B                              (EFX_EFFECT_WIPE_CLASS+154)
#define EFX_EFFECT_WIPE4_R_B_L_L                              (EFX_EFFECT_WIPE_CLASS+155)
#define EFX_EFFECT_WIPE4_R_B_L_R                              (EFX_EFFECT_WIPE_CLASS+156)
#define EFX_EFFECT_WIPE4_R_B_L_T                              (EFX_EFFECT_WIPE_CLASS+157)
#define EFX_EFFECT_WIPE4_R_B_L_B                              (EFX_EFFECT_WIPE_CLASS+158)
#define EFX_EFFECT_WIPE4_R_B_R_L                              (EFX_EFFECT_WIPE_CLASS+159)
#define EFX_EFFECT_WIPE4_R_B_R_R                              (EFX_EFFECT_WIPE_CLASS+160)
#define EFX_EFFECT_WIPE4_R_B_R_T                              (EFX_EFFECT_WIPE_CLASS+161)
#define EFX_EFFECT_WIPE4_R_B_R_B                              (EFX_EFFECT_WIPE_CLASS+162)
#define EFX_EFFECT_WIPE4_R_B_T_L                              (EFX_EFFECT_WIPE_CLASS+163)
#define EFX_EFFECT_WIPE4_R_B_T_R                              (EFX_EFFECT_WIPE_CLASS+164)
#define EFX_EFFECT_WIPE4_R_B_T_T                              (EFX_EFFECT_WIPE_CLASS+165)
#define EFX_EFFECT_WIPE4_R_B_T_B                              (EFX_EFFECT_WIPE_CLASS+166)
#define EFX_EFFECT_WIPE4_R_B_B_L                              (EFX_EFFECT_WIPE_CLASS+167)
#define EFX_EFFECT_WIPE4_R_B_B_R                              (EFX_EFFECT_WIPE_CLASS+168)
#define EFX_EFFECT_WIPE4_R_B_B_T                              (EFX_EFFECT_WIPE_CLASS+169)
#define EFX_EFFECT_WIPE4_R_B_B_B                              (EFX_EFFECT_WIPE_CLASS+170)
#define EFX_EFFECT_WIPE4_T_L_L_L                              (EFX_EFFECT_WIPE_CLASS+171)
#define EFX_EFFECT_WIPE4_T_L_L_R                              (EFX_EFFECT_WIPE_CLASS+172)
#define EFX_EFFECT_WIPE4_T_L_L_T                              (EFX_EFFECT_WIPE_CLASS+173)
#define EFX_EFFECT_WIPE4_T_L_L_B                              (EFX_EFFECT_WIPE_CLASS+174)
#define EFX_EFFECT_WIPE4_T_L_R_L                              (EFX_EFFECT_WIPE_CLASS+175)
#define EFX_EFFECT_WIPE4_T_L_R_R                              (EFX_EFFECT_WIPE_CLASS+176)
#define EFX_EFFECT_WIPE4_T_L_R_T                              (EFX_EFFECT_WIPE_CLASS+177)
#define EFX_EFFECT_WIPE4_T_L_R_B                              (EFX_EFFECT_WIPE_CLASS+178)
#define EFX_EFFECT_WIPE4_T_L_T_L                              (EFX_EFFECT_WIPE_CLASS+179)
#define EFX_EFFECT_WIPE4_T_L_T_R                              (EFX_EFFECT_WIPE_CLASS+180)
#define EFX_EFFECT_WIPE4_T_L_T_T                              (EFX_EFFECT_WIPE_CLASS+181)
#define EFX_EFFECT_WIPE4_T_L_T_B                              (EFX_EFFECT_WIPE_CLASS+182)
#define EFX_EFFECT_WIPE4_T_L_B_L                              (EFX_EFFECT_WIPE_CLASS+183)
#define EFX_EFFECT_WIPE4_T_L_B_R                              (EFX_EFFECT_WIPE_CLASS+184)
#define EFX_EFFECT_WIPE4_T_L_B_T                              (EFX_EFFECT_WIPE_CLASS+185)
#define EFX_EFFECT_WIPE4_T_L_B_B                              (EFX_EFFECT_WIPE_CLASS+186)
#define EFX_EFFECT_WIPE4_T_R_L_L                              (EFX_EFFECT_WIPE_CLASS+187)
#define EFX_EFFECT_WIPE4_T_R_L_R                              (EFX_EFFECT_WIPE_CLASS+188)
#define EFX_EFFECT_WIPE4_T_R_L_T                              (EFX_EFFECT_WIPE_CLASS+189)
#define EFX_EFFECT_WIPE4_T_R_L_B                              (EFX_EFFECT_WIPE_CLASS+190)
#define EFX_EFFECT_WIPE4_T_R_R_L                              (EFX_EFFECT_WIPE_CLASS+191)
#define EFX_EFFECT_WIPE4_T_R_R_R                              (EFX_EFFECT_WIPE_CLASS+192)
#define EFX_EFFECT_WIPE4_T_R_R_T                              (EFX_EFFECT_WIPE_CLASS+193)
#define EFX_EFFECT_WIPE4_T_R_R_B                              (EFX_EFFECT_WIPE_CLASS+194)
#define EFX_EFFECT_WIPE4_T_R_T_L                              (EFX_EFFECT_WIPE_CLASS+195)
#define EFX_EFFECT_WIPE4_T_R_T_R                              (EFX_EFFECT_WIPE_CLASS+196)
#define EFX_EFFECT_WIPE4_T_R_T_T                              (EFX_EFFECT_WIPE_CLASS+197)
#define EFX_EFFECT_WIPE4_T_R_T_B                              (EFX_EFFECT_WIPE_CLASS+198)
#define EFX_EFFECT_WIPE4_T_R_B_L                              (EFX_EFFECT_WIPE_CLASS+199)
#define EFX_EFFECT_WIPE4_T_R_B_R                              (EFX_EFFECT_WIPE_CLASS+200)
#define EFX_EFFECT_WIPE4_T_R_B_T                              (EFX_EFFECT_WIPE_CLASS+201)
#define EFX_EFFECT_WIPE4_T_R_B_B                              (EFX_EFFECT_WIPE_CLASS+202)
#define EFX_EFFECT_WIPE4_T_T_L_L                              (EFX_EFFECT_WIPE_CLASS+203)
#define EFX_EFFECT_WIPE4_T_T_L_R                              (EFX_EFFECT_WIPE_CLASS+204)
#define EFX_EFFECT_WIPE4_T_T_L_T                              (EFX_EFFECT_WIPE_CLASS+205)
#define EFX_EFFECT_WIPE4_T_T_L_B                              (EFX_EFFECT_WIPE_CLASS+206)
#define EFX_EFFECT_WIPE4_T_T_R_L                              (EFX_EFFECT_WIPE_CLASS+207)
#define EFX_EFFECT_WIPE4_T_T_R_R                              (EFX_EFFECT_WIPE_CLASS+208)
#define EFX_EFFECT_WIPE4_T_T_R_T                              (EFX_EFFECT_WIPE_CLASS+209)
#define EFX_EFFECT_WIPE4_T_T_R_B                              (EFX_EFFECT_WIPE_CLASS+210)
#define EFX_EFFECT_WIPE4_T_T_T_L                              (EFX_EFFECT_WIPE_CLASS+211)
#define EFX_EFFECT_WIPE4_T_T_T_R                              (EFX_EFFECT_WIPE_CLASS+212)
#define EFX_EFFECT_WIPE4_T_T_T_T                              (EFX_EFFECT_WIPE_CLASS+213)
#define EFX_EFFECT_WIPE4_T_T_T_B                              (EFX_EFFECT_WIPE_CLASS+214)
#define EFX_EFFECT_WIPE4_T_T_B_L                              (EFX_EFFECT_WIPE_CLASS+215)
#define EFX_EFFECT_WIPE4_T_T_B_R                              (EFX_EFFECT_WIPE_CLASS+216)
#define EFX_EFFECT_WIPE4_T_T_B_T                              (EFX_EFFECT_WIPE_CLASS+217)
#define EFX_EFFECT_WIPE4_T_T_B_B                              (EFX_EFFECT_WIPE_CLASS+218)
#define EFX_EFFECT_WIPE4_T_B_L_L                              (EFX_EFFECT_WIPE_CLASS+219)
#define EFX_EFFECT_WIPE4_T_B_L_R                              (EFX_EFFECT_WIPE_CLASS+220)
#define EFX_EFFECT_WIPE4_T_B_L_T                              (EFX_EFFECT_WIPE_CLASS+221)
#define EFX_EFFECT_WIPE4_T_B_L_B                              (EFX_EFFECT_WIPE_CLASS+222)
#define EFX_EFFECT_WIPE4_T_B_R_L                              (EFX_EFFECT_WIPE_CLASS+223)
#define EFX_EFFECT_WIPE4_T_B_R_R                              (EFX_EFFECT_WIPE_CLASS+224)
#define EFX_EFFECT_WIPE4_T_B_R_T                              (EFX_EFFECT_WIPE_CLASS+225)
#define EFX_EFFECT_WIPE4_T_B_R_B                              (EFX_EFFECT_WIPE_CLASS+226)
#define EFX_EFFECT_WIPE4_T_B_T_L                              (EFX_EFFECT_WIPE_CLASS+227)
#define EFX_EFFECT_WIPE4_T_B_T_R                              (EFX_EFFECT_WIPE_CLASS+228)
#define EFX_EFFECT_WIPE4_T_B_T_T                              (EFX_EFFECT_WIPE_CLASS+229)
#define EFX_EFFECT_WIPE4_T_B_T_B                              (EFX_EFFECT_WIPE_CLASS+230)
#define EFX_EFFECT_WIPE4_T_B_B_L                              (EFX_EFFECT_WIPE_CLASS+231)
#define EFX_EFFECT_WIPE4_T_B_B_R                              (EFX_EFFECT_WIPE_CLASS+232)
#define EFX_EFFECT_WIPE4_T_B_B_T                              (EFX_EFFECT_WIPE_CLASS+233)
#define EFX_EFFECT_WIPE4_T_B_B_B                              (EFX_EFFECT_WIPE_CLASS+234)
#define EFX_EFFECT_WIPE4_B_L_L_L                              (EFX_EFFECT_WIPE_CLASS+235)
#define EFX_EFFECT_WIPE4_B_L_L_R                              (EFX_EFFECT_WIPE_CLASS+236)
#define EFX_EFFECT_WIPE4_B_L_L_T                              (EFX_EFFECT_WIPE_CLASS+237)
#define EFX_EFFECT_WIPE4_B_L_L_B                              (EFX_EFFECT_WIPE_CLASS+238)
#define EFX_EFFECT_WIPE4_B_L_R_L                              (EFX_EFFECT_WIPE_CLASS+239)
#define EFX_EFFECT_WIPE4_B_L_R_R                              (EFX_EFFECT_WIPE_CLASS+240)
#define EFX_EFFECT_WIPE4_B_L_R_T                              (EFX_EFFECT_WIPE_CLASS+241)
#define EFX_EFFECT_WIPE4_B_L_R_B                              (EFX_EFFECT_WIPE_CLASS+242)
#define EFX_EFFECT_WIPE4_B_L_T_L                              (EFX_EFFECT_WIPE_CLASS+243)
#define EFX_EFFECT_WIPE4_B_L_T_R                              (EFX_EFFECT_WIPE_CLASS+244)
#define EFX_EFFECT_WIPE4_B_L_T_T                              (EFX_EFFECT_WIPE_CLASS+245)
#define EFX_EFFECT_WIPE4_B_L_T_B                              (EFX_EFFECT_WIPE_CLASS+246)
#define EFX_EFFECT_WIPE4_B_L_B_L                              (EFX_EFFECT_WIPE_CLASS+247)
#define EFX_EFFECT_WIPE4_B_L_B_R                              (EFX_EFFECT_WIPE_CLASS+248)
#define EFX_EFFECT_WIPE4_B_L_B_T                              (EFX_EFFECT_WIPE_CLASS+249)
#define EFX_EFFECT_WIPE4_B_L_B_B                              (EFX_EFFECT_WIPE_CLASS+250)
#define EFX_EFFECT_WIPE4_B_R_L_L                              (EFX_EFFECT_WIPE_CLASS+251)
#define EFX_EFFECT_WIPE4_B_R_L_R                              (EFX_EFFECT_WIPE_CLASS+252)
#define EFX_EFFECT_WIPE4_B_R_L_T                              (EFX_EFFECT_WIPE_CLASS+253)
#define EFX_EFFECT_WIPE4_B_R_L_B                              (EFX_EFFECT_WIPE_CLASS+254)
#define EFX_EFFECT_WIPE4_B_R_R_L                              (EFX_EFFECT_WIPE_CLASS+255)
#define EFX_EFFECT_WIPE4_B_R_R_R                              (EFX_EFFECT_WIPE_CLASS+256)
#define EFX_EFFECT_WIPE4_B_R_R_T                              (EFX_EFFECT_WIPE_CLASS+257)
#define EFX_EFFECT_WIPE4_B_R_R_B                              (EFX_EFFECT_WIPE_CLASS+258)
#define EFX_EFFECT_WIPE4_B_R_T_L                              (EFX_EFFECT_WIPE_CLASS+259)
#define EFX_EFFECT_WIPE4_B_R_T_R                              (EFX_EFFECT_WIPE_CLASS+260)
#define EFX_EFFECT_WIPE4_B_R_T_T                              (EFX_EFFECT_WIPE_CLASS+261)
#define EFX_EFFECT_WIPE4_B_R_T_B                              (EFX_EFFECT_WIPE_CLASS+262)
#define EFX_EFFECT_WIPE4_B_R_B_L                              (EFX_EFFECT_WIPE_CLASS+263)
#define EFX_EFFECT_WIPE4_B_R_B_R                              (EFX_EFFECT_WIPE_CLASS+264)
#define EFX_EFFECT_WIPE4_B_R_B_T                              (EFX_EFFECT_WIPE_CLASS+265)
#define EFX_EFFECT_WIPE4_B_R_B_B                              (EFX_EFFECT_WIPE_CLASS+266)
#define EFX_EFFECT_WIPE4_B_T_L_L                              (EFX_EFFECT_WIPE_CLASS+267)
#define EFX_EFFECT_WIPE4_B_T_L_R                              (EFX_EFFECT_WIPE_CLASS+268)
#define EFX_EFFECT_WIPE4_B_T_L_T                              (EFX_EFFECT_WIPE_CLASS+269)
#define EFX_EFFECT_WIPE4_B_T_L_B                              (EFX_EFFECT_WIPE_CLASS+270)
#define EFX_EFFECT_WIPE4_B_T_R_L                              (EFX_EFFECT_WIPE_CLASS+271)
#define EFX_EFFECT_WIPE4_B_T_R_R                              (EFX_EFFECT_WIPE_CLASS+272)
#define EFX_EFFECT_WIPE4_B_T_R_T                              (EFX_EFFECT_WIPE_CLASS+273)
#define EFX_EFFECT_WIPE4_B_T_R_B                              (EFX_EFFECT_WIPE_CLASS+274)
#define EFX_EFFECT_WIPE4_B_T_T_L                              (EFX_EFFECT_WIPE_CLASS+275)
#define EFX_EFFECT_WIPE4_B_T_T_R                              (EFX_EFFECT_WIPE_CLASS+276)
#define EFX_EFFECT_WIPE4_B_T_T_T                              (EFX_EFFECT_WIPE_CLASS+277)
#define EFX_EFFECT_WIPE4_B_T_T_B                              (EFX_EFFECT_WIPE_CLASS+278)
#define EFX_EFFECT_WIPE4_B_T_B_L                              (EFX_EFFECT_WIPE_CLASS+279)
#define EFX_EFFECT_WIPE4_B_T_B_R                              (EFX_EFFECT_WIPE_CLASS+280)
#define EFX_EFFECT_WIPE4_B_T_B_T                              (EFX_EFFECT_WIPE_CLASS+281)
#define EFX_EFFECT_WIPE4_B_T_B_B                              (EFX_EFFECT_WIPE_CLASS+282)
#define EFX_EFFECT_WIPE4_B_B_L_L                              (EFX_EFFECT_WIPE_CLASS+283)
#define EFX_EFFECT_WIPE4_B_B_L_R                              (EFX_EFFECT_WIPE_CLASS+284)
#define EFX_EFFECT_WIPE4_B_B_L_T                              (EFX_EFFECT_WIPE_CLASS+285)
#define EFX_EFFECT_WIPE4_B_B_L_B                              (EFX_EFFECT_WIPE_CLASS+286)
#define EFX_EFFECT_WIPE4_B_B_R_L                              (EFX_EFFECT_WIPE_CLASS+287)
#define EFX_EFFECT_WIPE4_B_B_R_R                              (EFX_EFFECT_WIPE_CLASS+288)
#define EFX_EFFECT_WIPE4_B_B_R_T                              (EFX_EFFECT_WIPE_CLASS+289)
#define EFX_EFFECT_WIPE4_B_B_R_B                              (EFX_EFFECT_WIPE_CLASS+290)
#define EFX_EFFECT_WIPE4_B_B_T_L                              (EFX_EFFECT_WIPE_CLASS+291)
#define EFX_EFFECT_WIPE4_B_B_T_R                              (EFX_EFFECT_WIPE_CLASS+292)
#define EFX_EFFECT_WIPE4_B_B_T_T                              (EFX_EFFECT_WIPE_CLASS+293)
#define EFX_EFFECT_WIPE4_B_B_T_B                              (EFX_EFFECT_WIPE_CLASS+294)
#define EFX_EFFECT_WIPE4_B_B_B_L                              (EFX_EFFECT_WIPE_CLASS+295)
#define EFX_EFFECT_WIPE4_B_B_B_R                              (EFX_EFFECT_WIPE_CLASS+296)
#define EFX_EFFECT_WIPE4_B_B_B_T                              (EFX_EFFECT_WIPE_CLASS+297)
#define EFX_EFFECT_WIPE4_B_B_B_B                              (EFX_EFFECT_WIPE_CLASS+298)
#define EFX_EFFECT_WIPE4_LT_LT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+299)
#define EFX_EFFECT_WIPE4_LT_LT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+300)
#define EFX_EFFECT_WIPE4_LT_LT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+301)
#define EFX_EFFECT_WIPE4_LT_LT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+302)
#define EFX_EFFECT_WIPE4_LT_LT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+303)
#define EFX_EFFECT_WIPE4_LT_LT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+304)
#define EFX_EFFECT_WIPE4_LT_LT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+305)
#define EFX_EFFECT_WIPE4_LT_LT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+306)
#define EFX_EFFECT_WIPE4_LT_LT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+307)
#define EFX_EFFECT_WIPE4_LT_LT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+308)
#define EFX_EFFECT_WIPE4_LT_LT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+309)
#define EFX_EFFECT_WIPE4_LT_LT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+310)
#define EFX_EFFECT_WIPE4_LT_LT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+311)
#define EFX_EFFECT_WIPE4_LT_LT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+312)
#define EFX_EFFECT_WIPE4_LT_LT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+313)
#define EFX_EFFECT_WIPE4_LT_LT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+314)
#define EFX_EFFECT_WIPE4_LT_LB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+315)
#define EFX_EFFECT_WIPE4_LT_LB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+316)
#define EFX_EFFECT_WIPE4_LT_LB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+317)
#define EFX_EFFECT_WIPE4_LT_LB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+318)
#define EFX_EFFECT_WIPE4_LT_LB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+319)
#define EFX_EFFECT_WIPE4_LT_LB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+320)
#define EFX_EFFECT_WIPE4_LT_LB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+321)
#define EFX_EFFECT_WIPE4_LT_LB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+322)
#define EFX_EFFECT_WIPE4_LT_LB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+323)
#define EFX_EFFECT_WIPE4_LT_LB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+324)
#define EFX_EFFECT_WIPE4_LT_LB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+325)
#define EFX_EFFECT_WIPE4_LT_LB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+326)
#define EFX_EFFECT_WIPE4_LT_LB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+327)
#define EFX_EFFECT_WIPE4_LT_LB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+328)
#define EFX_EFFECT_WIPE4_LT_LB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+329)
#define EFX_EFFECT_WIPE4_LT_LB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+330)
#define EFX_EFFECT_WIPE4_LT_RT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+331)
#define EFX_EFFECT_WIPE4_LT_RT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+332)
#define EFX_EFFECT_WIPE4_LT_RT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+333)
#define EFX_EFFECT_WIPE4_LT_RT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+334)
#define EFX_EFFECT_WIPE4_LT_RT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+335)
#define EFX_EFFECT_WIPE4_LT_RT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+336)
#define EFX_EFFECT_WIPE4_LT_RT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+337)
#define EFX_EFFECT_WIPE4_LT_RT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+338)
#define EFX_EFFECT_WIPE4_LT_RT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+339)
#define EFX_EFFECT_WIPE4_LT_RT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+340)
#define EFX_EFFECT_WIPE4_LT_RT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+341)
#define EFX_EFFECT_WIPE4_LT_RT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+342)
#define EFX_EFFECT_WIPE4_LT_RT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+343)
#define EFX_EFFECT_WIPE4_LT_RT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+344)
#define EFX_EFFECT_WIPE4_LT_RT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+345)
#define EFX_EFFECT_WIPE4_LT_RT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+346)
#define EFX_EFFECT_WIPE4_LT_RB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+347)
#define EFX_EFFECT_WIPE4_LT_RB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+348)
#define EFX_EFFECT_WIPE4_LT_RB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+349)
#define EFX_EFFECT_WIPE4_LT_RB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+350)
#define EFX_EFFECT_WIPE4_LT_RB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+351)
#define EFX_EFFECT_WIPE4_LT_RB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+352)
#define EFX_EFFECT_WIPE4_LT_RB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+353)
#define EFX_EFFECT_WIPE4_LT_RB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+354)
#define EFX_EFFECT_WIPE4_LT_RB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+355)
#define EFX_EFFECT_WIPE4_LT_RB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+356)
#define EFX_EFFECT_WIPE4_LT_RB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+357)
#define EFX_EFFECT_WIPE4_LT_RB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+358)
#define EFX_EFFECT_WIPE4_LT_RB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+359)
#define EFX_EFFECT_WIPE4_LT_RB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+360)
#define EFX_EFFECT_WIPE4_LT_RB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+361)
#define EFX_EFFECT_WIPE4_LT_RB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+362)
#define EFX_EFFECT_WIPE4_LB_LT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+363)
#define EFX_EFFECT_WIPE4_LB_LT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+364)
#define EFX_EFFECT_WIPE4_LB_LT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+365)
#define EFX_EFFECT_WIPE4_LB_LT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+366)
#define EFX_EFFECT_WIPE4_LB_LT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+367)
#define EFX_EFFECT_WIPE4_LB_LT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+368)
#define EFX_EFFECT_WIPE4_LB_LT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+369)
#define EFX_EFFECT_WIPE4_LB_LT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+370)
#define EFX_EFFECT_WIPE4_LB_LT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+371)
#define EFX_EFFECT_WIPE4_LB_LT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+372)
#define EFX_EFFECT_WIPE4_LB_LT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+373)
#define EFX_EFFECT_WIPE4_LB_LT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+374)
#define EFX_EFFECT_WIPE4_LB_LT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+375)
#define EFX_EFFECT_WIPE4_LB_LT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+376)
#define EFX_EFFECT_WIPE4_LB_LT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+377)
#define EFX_EFFECT_WIPE4_LB_LT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+378)
#define EFX_EFFECT_WIPE4_LB_LB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+379)
#define EFX_EFFECT_WIPE4_LB_LB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+380)
#define EFX_EFFECT_WIPE4_LB_LB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+381)
#define EFX_EFFECT_WIPE4_LB_LB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+382)
#define EFX_EFFECT_WIPE4_LB_LB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+383)
#define EFX_EFFECT_WIPE4_LB_LB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+384)
#define EFX_EFFECT_WIPE4_LB_LB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+385)
#define EFX_EFFECT_WIPE4_LB_LB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+386)
#define EFX_EFFECT_WIPE4_LB_LB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+387)
#define EFX_EFFECT_WIPE4_LB_LB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+388)
#define EFX_EFFECT_WIPE4_LB_LB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+389)
#define EFX_EFFECT_WIPE4_LB_LB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+390)
#define EFX_EFFECT_WIPE4_LB_LB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+391)
#define EFX_EFFECT_WIPE4_LB_LB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+392)
#define EFX_EFFECT_WIPE4_LB_LB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+393)
#define EFX_EFFECT_WIPE4_LB_LB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+394)
#define EFX_EFFECT_WIPE4_LB_RT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+395)
#define EFX_EFFECT_WIPE4_LB_RT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+396)
#define EFX_EFFECT_WIPE4_LB_RT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+397)
#define EFX_EFFECT_WIPE4_LB_RT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+398)
#define EFX_EFFECT_WIPE4_LB_RT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+399)
#define EFX_EFFECT_WIPE4_LB_RT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+400)
#define EFX_EFFECT_WIPE4_LB_RT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+401)
#define EFX_EFFECT_WIPE4_LB_RT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+402)
#define EFX_EFFECT_WIPE4_LB_RT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+403)
#define EFX_EFFECT_WIPE4_LB_RT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+404)
#define EFX_EFFECT_WIPE4_LB_RT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+405)
#define EFX_EFFECT_WIPE4_LB_RT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+406)
#define EFX_EFFECT_WIPE4_LB_RT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+407)
#define EFX_EFFECT_WIPE4_LB_RT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+408)
#define EFX_EFFECT_WIPE4_LB_RT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+409)
#define EFX_EFFECT_WIPE4_LB_RT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+410)
#define EFX_EFFECT_WIPE4_LB_RB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+411)
#define EFX_EFFECT_WIPE4_LB_RB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+412)
#define EFX_EFFECT_WIPE4_LB_RB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+413)
#define EFX_EFFECT_WIPE4_LB_RB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+414)
#define EFX_EFFECT_WIPE4_LB_RB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+415)
#define EFX_EFFECT_WIPE4_LB_RB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+416)
#define EFX_EFFECT_WIPE4_LB_RB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+417)
#define EFX_EFFECT_WIPE4_LB_RB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+418)
#define EFX_EFFECT_WIPE4_LB_RB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+419)
#define EFX_EFFECT_WIPE4_LB_RB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+420)
#define EFX_EFFECT_WIPE4_LB_RB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+421)
#define EFX_EFFECT_WIPE4_LB_RB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+422)
#define EFX_EFFECT_WIPE4_LB_RB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+423)
#define EFX_EFFECT_WIPE4_LB_RB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+424)
#define EFX_EFFECT_WIPE4_LB_RB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+425)
#define EFX_EFFECT_WIPE4_LB_RB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+426)
#define EFX_EFFECT_WIPE4_RT_LT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+427)
#define EFX_EFFECT_WIPE4_RT_LT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+428)
#define EFX_EFFECT_WIPE4_RT_LT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+429)
#define EFX_EFFECT_WIPE4_RT_LT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+430)
#define EFX_EFFECT_WIPE4_RT_LT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+431)
#define EFX_EFFECT_WIPE4_RT_LT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+432)
#define EFX_EFFECT_WIPE4_RT_LT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+433)
#define EFX_EFFECT_WIPE4_RT_LT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+434)
#define EFX_EFFECT_WIPE4_RT_LT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+435)
#define EFX_EFFECT_WIPE4_RT_LT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+436)
#define EFX_EFFECT_WIPE4_RT_LT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+437)
#define EFX_EFFECT_WIPE4_RT_LT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+438)
#define EFX_EFFECT_WIPE4_RT_LT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+439)
#define EFX_EFFECT_WIPE4_RT_LT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+440)
#define EFX_EFFECT_WIPE4_RT_LT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+441)
#define EFX_EFFECT_WIPE4_RT_LT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+442)
#define EFX_EFFECT_WIPE4_RT_LB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+443)
#define EFX_EFFECT_WIPE4_RT_LB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+444)
#define EFX_EFFECT_WIPE4_RT_LB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+445)
#define EFX_EFFECT_WIPE4_RT_LB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+446)
#define EFX_EFFECT_WIPE4_RT_LB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+447)
#define EFX_EFFECT_WIPE4_RT_LB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+448)
#define EFX_EFFECT_WIPE4_RT_LB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+449)
#define EFX_EFFECT_WIPE4_RT_LB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+450)
#define EFX_EFFECT_WIPE4_RT_LB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+451)
#define EFX_EFFECT_WIPE4_RT_LB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+452)
#define EFX_EFFECT_WIPE4_RT_LB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+453)
#define EFX_EFFECT_WIPE4_RT_LB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+454)
#define EFX_EFFECT_WIPE4_RT_LB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+455)
#define EFX_EFFECT_WIPE4_RT_LB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+456)
#define EFX_EFFECT_WIPE4_RT_LB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+457)
#define EFX_EFFECT_WIPE4_RT_LB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+458)
#define EFX_EFFECT_WIPE4_RT_RT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+459)
#define EFX_EFFECT_WIPE4_RT_RT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+460)
#define EFX_EFFECT_WIPE4_RT_RT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+461)
#define EFX_EFFECT_WIPE4_RT_RT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+462)
#define EFX_EFFECT_WIPE4_RT_RT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+463)
#define EFX_EFFECT_WIPE4_RT_RT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+464)
#define EFX_EFFECT_WIPE4_RT_RT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+465)
#define EFX_EFFECT_WIPE4_RT_RT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+466)
#define EFX_EFFECT_WIPE4_RT_RT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+467)
#define EFX_EFFECT_WIPE4_RT_RT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+468)
#define EFX_EFFECT_WIPE4_RT_RT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+469)
#define EFX_EFFECT_WIPE4_RT_RT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+470)
#define EFX_EFFECT_WIPE4_RT_RT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+471)
#define EFX_EFFECT_WIPE4_RT_RT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+472)
#define EFX_EFFECT_WIPE4_RT_RT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+473)
#define EFX_EFFECT_WIPE4_RT_RT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+474)
#define EFX_EFFECT_WIPE4_RT_RB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+475)
#define EFX_EFFECT_WIPE4_RT_RB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+476)
#define EFX_EFFECT_WIPE4_RT_RB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+477)
#define EFX_EFFECT_WIPE4_RT_RB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+478)
#define EFX_EFFECT_WIPE4_RT_RB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+479)
#define EFX_EFFECT_WIPE4_RT_RB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+480)
#define EFX_EFFECT_WIPE4_RT_RB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+481)
#define EFX_EFFECT_WIPE4_RT_RB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+482)
#define EFX_EFFECT_WIPE4_RT_RB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+483)
#define EFX_EFFECT_WIPE4_RT_RB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+484)
#define EFX_EFFECT_WIPE4_RT_RB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+485)
#define EFX_EFFECT_WIPE4_RT_RB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+486)
#define EFX_EFFECT_WIPE4_RT_RB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+487)
#define EFX_EFFECT_WIPE4_RT_RB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+488)
#define EFX_EFFECT_WIPE4_RT_RB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+489)
#define EFX_EFFECT_WIPE4_RT_RB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+490)
#define EFX_EFFECT_WIPE4_RB_LT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+491)
#define EFX_EFFECT_WIPE4_RB_LT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+492)
#define EFX_EFFECT_WIPE4_RB_LT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+493)
#define EFX_EFFECT_WIPE4_RB_LT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+494)
#define EFX_EFFECT_WIPE4_RB_LT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+495)
#define EFX_EFFECT_WIPE4_RB_LT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+496)
#define EFX_EFFECT_WIPE4_RB_LT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+497)
#define EFX_EFFECT_WIPE4_RB_LT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+498)
#define EFX_EFFECT_WIPE4_RB_LT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+499)
#define EFX_EFFECT_WIPE4_RB_LT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+500)
#define EFX_EFFECT_WIPE4_RB_LT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+501)
#define EFX_EFFECT_WIPE4_RB_LT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+502)
#define EFX_EFFECT_WIPE4_RB_LT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+503)
#define EFX_EFFECT_WIPE4_RB_LT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+504)
#define EFX_EFFECT_WIPE4_RB_LT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+505)
#define EFX_EFFECT_WIPE4_RB_LT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+506)
#define EFX_EFFECT_WIPE4_RB_LB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+507)
#define EFX_EFFECT_WIPE4_RB_LB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+508)
#define EFX_EFFECT_WIPE4_RB_LB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+509)
#define EFX_EFFECT_WIPE4_RB_LB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+510)
#define EFX_EFFECT_WIPE4_RB_LB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+511)
#define EFX_EFFECT_WIPE4_RB_LB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+512)
#define EFX_EFFECT_WIPE4_RB_LB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+513)
#define EFX_EFFECT_WIPE4_RB_LB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+514)
#define EFX_EFFECT_WIPE4_RB_LB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+515)
#define EFX_EFFECT_WIPE4_RB_LB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+516)
#define EFX_EFFECT_WIPE4_RB_LB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+517)
#define EFX_EFFECT_WIPE4_RB_LB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+518)
#define EFX_EFFECT_WIPE4_RB_LB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+519)
#define EFX_EFFECT_WIPE4_RB_LB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+520)
#define EFX_EFFECT_WIPE4_RB_LB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+521)
#define EFX_EFFECT_WIPE4_RB_LB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+522)
#define EFX_EFFECT_WIPE4_RB_RT_LT_LT                          (EFX_EFFECT_WIPE_CLASS+523)
#define EFX_EFFECT_WIPE4_RB_RT_LT_LB                          (EFX_EFFECT_WIPE_CLASS+524)
#define EFX_EFFECT_WIPE4_RB_RT_LT_RT                          (EFX_EFFECT_WIPE_CLASS+525)
#define EFX_EFFECT_WIPE4_RB_RT_LT_RB                          (EFX_EFFECT_WIPE_CLASS+526)
#define EFX_EFFECT_WIPE4_RB_RT_LB_LT                          (EFX_EFFECT_WIPE_CLASS+527)
#define EFX_EFFECT_WIPE4_RB_RT_LB_LB                          (EFX_EFFECT_WIPE_CLASS+528)
#define EFX_EFFECT_WIPE4_RB_RT_LB_RT                          (EFX_EFFECT_WIPE_CLASS+529)
#define EFX_EFFECT_WIPE4_RB_RT_LB_RB                          (EFX_EFFECT_WIPE_CLASS+530)
#define EFX_EFFECT_WIPE4_RB_RT_RT_LT                          (EFX_EFFECT_WIPE_CLASS+531)
#define EFX_EFFECT_WIPE4_RB_RT_RT_LB                          (EFX_EFFECT_WIPE_CLASS+532)
#define EFX_EFFECT_WIPE4_RB_RT_RT_RT                          (EFX_EFFECT_WIPE_CLASS+533)
#define EFX_EFFECT_WIPE4_RB_RT_RT_RB                          (EFX_EFFECT_WIPE_CLASS+534)
#define EFX_EFFECT_WIPE4_RB_RT_RB_LT                          (EFX_EFFECT_WIPE_CLASS+535)
#define EFX_EFFECT_WIPE4_RB_RT_RB_LB                          (EFX_EFFECT_WIPE_CLASS+536)
#define EFX_EFFECT_WIPE4_RB_RT_RB_RT                          (EFX_EFFECT_WIPE_CLASS+537)
#define EFX_EFFECT_WIPE4_RB_RT_RB_RB                          (EFX_EFFECT_WIPE_CLASS+538)
#define EFX_EFFECT_WIPE4_RB_RB_LT_LT                          (EFX_EFFECT_WIPE_CLASS+539)
#define EFX_EFFECT_WIPE4_RB_RB_LT_LB                          (EFX_EFFECT_WIPE_CLASS+540)
#define EFX_EFFECT_WIPE4_RB_RB_LT_RT                          (EFX_EFFECT_WIPE_CLASS+541)
#define EFX_EFFECT_WIPE4_RB_RB_LT_RB                          (EFX_EFFECT_WIPE_CLASS+542)
#define EFX_EFFECT_WIPE4_RB_RB_LB_LT                          (EFX_EFFECT_WIPE_CLASS+543)
#define EFX_EFFECT_WIPE4_RB_RB_LB_LB                          (EFX_EFFECT_WIPE_CLASS+544)
#define EFX_EFFECT_WIPE4_RB_RB_LB_RT                          (EFX_EFFECT_WIPE_CLASS+545)
#define EFX_EFFECT_WIPE4_RB_RB_LB_RB                          (EFX_EFFECT_WIPE_CLASS+546)
#define EFX_EFFECT_WIPE4_RB_RB_RT_LT                          (EFX_EFFECT_WIPE_CLASS+547)
#define EFX_EFFECT_WIPE4_RB_RB_RT_LB                          (EFX_EFFECT_WIPE_CLASS+548)
#define EFX_EFFECT_WIPE4_RB_RB_RT_RT                          (EFX_EFFECT_WIPE_CLASS+549)
#define EFX_EFFECT_WIPE4_RB_RB_RT_RB                          (EFX_EFFECT_WIPE_CLASS+550)
#define EFX_EFFECT_WIPE4_RB_RB_RB_LT                          (EFX_EFFECT_WIPE_CLASS+551)
#define EFX_EFFECT_WIPE4_RB_RB_RB_LB                          (EFX_EFFECT_WIPE_CLASS+552)
#define EFX_EFFECT_WIPE4_RB_RB_RB_RT                          (EFX_EFFECT_WIPE_CLASS+553)
#define EFX_EFFECT_WIPE4_RB_RB_RB_RB                          (EFX_EFFECT_WIPE_CLASS+554)
#define EFX_EFFECT_WIPE_MAX                                   (EFX_EFFECT_WIPE_CLASS+554)

// Rectangular wipe
#define EFX_EFFECT_WIPE_RECTANGLE_CLASS                       3000
#define EFX_EFFECT_WIPE_RECTANGLE_IN                          (EFX_EFFECT_WIPE_RECTANGLE_CLASS+0)
#define EFX_EFFECT_WIPE_RECTANGLE_OUT                         (EFX_EFFECT_WIPE_RECTANGLE_CLASS+1)
#define EFX_EFFECT_WIPE_RECTANGLE_INTER_IN_THEN_IN            (EFX_EFFECT_WIPE_RECTANGLE_CLASS+2)
#define EFX_EFFECT_WIPE_RECTANGLE_INTER_OUT_THEN_OUT          (EFX_EFFECT_WIPE_RECTANGLE_CLASS+3)
#define EFX_EFFECT_WIPE_RECTANGLE_INTER_IN_THEN_OUT           (EFX_EFFECT_WIPE_RECTANGLE_CLASS+4)
#define EFX_EFFECT_WIPE_RECTANGLE_INTER_OUT_THEN_IN           (EFX_EFFECT_WIPE_RECTANGLE_CLASS+5)
#define EFX_EFFECT_WIPE_RECTANGLE_MAX                         (EFX_EFFECT_WIPE_RECTANGLE_CLASS+5)

// Circular wipe
#define EFX_EFFECT_WIPE_CIRCLE_CLASS                          4000
#define EFX_EFFECT_WIPE_CIRCLE_C_CW_FROM_L                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+0)
#define EFX_EFFECT_WIPE_CIRCLE_C_CCW_FROM_L                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+1)
#define EFX_EFFECT_WIPE_CIRCLE_C_CW_FROM_R                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+2)
#define EFX_EFFECT_WIPE_CIRCLE_C_CCW_FROM_R                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+3)
#define EFX_EFFECT_WIPE_CIRCLE_C_CW_FROM_T                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+4)
#define EFX_EFFECT_WIPE_CIRCLE_C_CCW_FROM_T                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+5)
#define EFX_EFFECT_WIPE_CIRCLE_C_CW_FROM_B                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+6)
#define EFX_EFFECT_WIPE_CIRCLE_C_CCW_FROM_B                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+7)
#define EFX_EFFECT_WIPE_CIRCLE_L_CCW_FROM_B                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+8)
#define EFX_EFFECT_WIPE_CIRCLE_L_CW_FROM_T                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+9)
#define EFX_EFFECT_WIPE_CIRCLE_R_CW_FROM_B                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+10)
#define EFX_EFFECT_WIPE_CIRCLE_R_CCW_FROM_T                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+11)
#define EFX_EFFECT_WIPE_CIRCLE_T_CCW_FROM_L                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+12)
#define EFX_EFFECT_WIPE_CIRCLE_T_CW_FROM_R                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+13)
#define EFX_EFFECT_WIPE_CIRCLE_B_CW_FROM_L                    (EFX_EFFECT_WIPE_CIRCLE_CLASS+14)
#define EFX_EFFECT_WIPE_CIRCLE_B_CCW_FROM_R                   (EFX_EFFECT_WIPE_CIRCLE_CLASS+15)
#define EFX_EFFECT_WIPE_CIRCLE2_C_CW_FROM_L_AND_FROM_R        (EFX_EFFECT_WIPE_CIRCLE_CLASS+16)
#define EFX_EFFECT_WIPE_CIRCLE2_C_CCW_FROM_L_AND_FROM_R       (EFX_EFFECT_WIPE_CIRCLE_CLASS+17)
#define EFX_EFFECT_WIPE_CIRCLE2_C_CW_FROM_T_AND_FROM_B        (EFX_EFFECT_WIPE_CIRCLE_CLASS+18)
#define EFX_EFFECT_WIPE_CIRCLE2_C_CCW_FROM_T_AND_FROM_B       (EFX_EFFECT_WIPE_CIRCLE_CLASS+19)
#define EFX_EFFECT_WIPE_CIRCLE2_L_CW_FROM_T_AND_CCW_FROM_B    (EFX_EFFECT_WIPE_CIRCLE_CLASS+20)
#define EFX_EFFECT_WIPE_CIRCLE2_L_CCW_TO_T_AND_CW_TO_B        (EFX_EFFECT_WIPE_CIRCLE_CLASS+21)
#define EFX_EFFECT_WIPE_CIRCLE2_R_CCW_FROM_T_AND_CW_FROM_B    (EFX_EFFECT_WIPE_CIRCLE_CLASS+22)
#define EFX_EFFECT_WIPE_CIRCLE2_R_CW_TO_T_AND_CCW_TO_B        (EFX_EFFECT_WIPE_CIRCLE_CLASS+23)
#define EFX_EFFECT_WIPE_CIRCLE2_T_CCW_FROM_L_AND_CW_FROM_R    (EFX_EFFECT_WIPE_CIRCLE_CLASS+24)
#define EFX_EFFECT_WIPE_CIRCLE2_T_CW_TO_L_AND_CCW_TO_R        (EFX_EFFECT_WIPE_CIRCLE_CLASS+25)
#define EFX_EFFECT_WIPE_CIRCLE2_B_CW_FROM_L_AND_CCW_FROM_R    (EFX_EFFECT_WIPE_CIRCLE_CLASS+26)
#define EFX_EFFECT_WIPE_CIRCLE2_B_CCW_TO_L_AND_CW_TO_R        (EFX_EFFECT_WIPE_CIRCLE_CLASS+27)
#define EFX_EFFECT_WIPE_CIRCLE4_C_CW_CCW_CCW_CW               (EFX_EFFECT_WIPE_CIRCLE_CLASS+28)
#define EFX_EFFECT_WIPE_CIRCLE4_C_CCW_CW_CW_CCW               (EFX_EFFECT_WIPE_CIRCLE_CLASS+29)
#define EFX_EFFECT_WIPE_CIRCLE_MAX                            (EFX_EFFECT_WIPE_CIRCLE_CLASS+29)

// Push
#define EFX_EFFECT_PUSH_CLASS                                 5000
#define EFX_EFFECT_PUSH_R_TO_L                                (EFX_EFFECT_PUSH_CLASS+0)
#define EFX_EFFECT_PUSH_L_TO_R                                (EFX_EFFECT_PUSH_CLASS+1)
#define EFX_EFFECT_PUSH_B_TO_T                                (EFX_EFFECT_PUSH_CLASS+2)
#define EFX_EFFECT_PUSH_T_TO_B                                (EFX_EFFECT_PUSH_CLASS+3)
#define EFX_EFFECT_PUSH2_T_R_TO_L_AND_B_L_TO_R                (EFX_EFFECT_PUSH_CLASS+4)
#define EFX_EFFECT_PUSH2_T_L_TO_R_AND_B_R_TO_L                (EFX_EFFECT_PUSH_CLASS+5)
#define EFX_EFFECT_PUSH2_L_B_TO_T_AND_R_T_TO_B                (EFX_EFFECT_PUSH_CLASS+6)
#define EFX_EFFECT_PUSH2_L_T_TO_B_AND_R_B_TO_T                (EFX_EFFECT_PUSH_CLASS+7)
#define EFX_EFFECT_PUSH4_L_L_L_L                              (EFX_EFFECT_PUSH_CLASS+8)
#define EFX_EFFECT_PUSH4_L_L_L_R                              (EFX_EFFECT_PUSH_CLASS+9)
#define EFX_EFFECT_PUSH4_L_L_L_T                              (EFX_EFFECT_PUSH_CLASS+10)
#define EFX_EFFECT_PUSH4_L_L_L_B                              (EFX_EFFECT_PUSH_CLASS+11)
#define EFX_EFFECT_PUSH4_L_L_R_L                              (EFX_EFFECT_PUSH_CLASS+12)
#define EFX_EFFECT_PUSH4_L_L_R_R                              (EFX_EFFECT_PUSH_CLASS+13)
#define EFX_EFFECT_PUSH4_L_L_R_T                              (EFX_EFFECT_PUSH_CLASS+14)
#define EFX_EFFECT_PUSH4_L_L_R_B                              (EFX_EFFECT_PUSH_CLASS+15)
#define EFX_EFFECT_PUSH4_L_L_T_L                              (EFX_EFFECT_PUSH_CLASS+16)
#define EFX_EFFECT_PUSH4_L_L_T_R                              (EFX_EFFECT_PUSH_CLASS+17)
#define EFX_EFFECT_PUSH4_L_L_T_T                              (EFX_EFFECT_PUSH_CLASS+18)
#define EFX_EFFECT_PUSH4_L_L_T_B                              (EFX_EFFECT_PUSH_CLASS+19)
#define EFX_EFFECT_PUSH4_L_L_B_L                              (EFX_EFFECT_PUSH_CLASS+20)
#define EFX_EFFECT_PUSH4_L_L_B_R                              (EFX_EFFECT_PUSH_CLASS+21)
#define EFX_EFFECT_PUSH4_L_L_B_T                              (EFX_EFFECT_PUSH_CLASS+22)
#define EFX_EFFECT_PUSH4_L_L_B_B                              (EFX_EFFECT_PUSH_CLASS+23)
#define EFX_EFFECT_PUSH4_L_R_L_L                              (EFX_EFFECT_PUSH_CLASS+24)
#define EFX_EFFECT_PUSH4_L_R_L_R                              (EFX_EFFECT_PUSH_CLASS+25)
#define EFX_EFFECT_PUSH4_L_R_L_T                              (EFX_EFFECT_PUSH_CLASS+26)
#define EFX_EFFECT_PUSH4_L_R_L_B                              (EFX_EFFECT_PUSH_CLASS+27)
#define EFX_EFFECT_PUSH4_L_R_R_L                              (EFX_EFFECT_PUSH_CLASS+28)
#define EFX_EFFECT_PUSH4_L_R_R_R                              (EFX_EFFECT_PUSH_CLASS+29)
#define EFX_EFFECT_PUSH4_L_R_R_T                              (EFX_EFFECT_PUSH_CLASS+30)
#define EFX_EFFECT_PUSH4_L_R_R_B                              (EFX_EFFECT_PUSH_CLASS+31)
#define EFX_EFFECT_PUSH4_L_R_T_L                              (EFX_EFFECT_PUSH_CLASS+32)
#define EFX_EFFECT_PUSH4_L_R_T_R                              (EFX_EFFECT_PUSH_CLASS+33)
#define EFX_EFFECT_PUSH4_L_R_T_T                              (EFX_EFFECT_PUSH_CLASS+34)
#define EFX_EFFECT_PUSH4_L_R_T_B                              (EFX_EFFECT_PUSH_CLASS+35)
#define EFX_EFFECT_PUSH4_L_R_B_L                              (EFX_EFFECT_PUSH_CLASS+36)
#define EFX_EFFECT_PUSH4_L_R_B_R                              (EFX_EFFECT_PUSH_CLASS+37)
#define EFX_EFFECT_PUSH4_L_R_B_T                              (EFX_EFFECT_PUSH_CLASS+38)
#define EFX_EFFECT_PUSH4_L_R_B_B                              (EFX_EFFECT_PUSH_CLASS+39)
#define EFX_EFFECT_PUSH4_L_T_L_L                              (EFX_EFFECT_PUSH_CLASS+40)
#define EFX_EFFECT_PUSH4_L_T_L_R                              (EFX_EFFECT_PUSH_CLASS+41)
#define EFX_EFFECT_PUSH4_L_T_L_T                              (EFX_EFFECT_PUSH_CLASS+42)
#define EFX_EFFECT_PUSH4_L_T_L_B                              (EFX_EFFECT_PUSH_CLASS+43)
#define EFX_EFFECT_PUSH4_L_T_R_L                              (EFX_EFFECT_PUSH_CLASS+44)
#define EFX_EFFECT_PUSH4_L_T_R_R                              (EFX_EFFECT_PUSH_CLASS+45)
#define EFX_EFFECT_PUSH4_L_T_R_T                              (EFX_EFFECT_PUSH_CLASS+46)
#define EFX_EFFECT_PUSH4_L_T_R_B                              (EFX_EFFECT_PUSH_CLASS+47)
#define EFX_EFFECT_PUSH4_L_T_T_L                              (EFX_EFFECT_PUSH_CLASS+48)
#define EFX_EFFECT_PUSH4_L_T_T_R                              (EFX_EFFECT_PUSH_CLASS+49)
#define EFX_EFFECT_PUSH4_L_T_T_T                              (EFX_EFFECT_PUSH_CLASS+50)
#define EFX_EFFECT_PUSH4_L_T_T_B                              (EFX_EFFECT_PUSH_CLASS+51)
#define EFX_EFFECT_PUSH4_L_T_B_L                              (EFX_EFFECT_PUSH_CLASS+52)
#define EFX_EFFECT_PUSH4_L_T_B_R                              (EFX_EFFECT_PUSH_CLASS+53)
#define EFX_EFFECT_PUSH4_L_T_B_T                              (EFX_EFFECT_PUSH_CLASS+54)
#define EFX_EFFECT_PUSH4_L_T_B_B                              (EFX_EFFECT_PUSH_CLASS+55)
#define EFX_EFFECT_PUSH4_L_B_L_L                              (EFX_EFFECT_PUSH_CLASS+56)
#define EFX_EFFECT_PUSH4_L_B_L_R                              (EFX_EFFECT_PUSH_CLASS+57)
#define EFX_EFFECT_PUSH4_L_B_L_T                              (EFX_EFFECT_PUSH_CLASS+58)
#define EFX_EFFECT_PUSH4_L_B_L_B                              (EFX_EFFECT_PUSH_CLASS+59)
#define EFX_EFFECT_PUSH4_L_B_R_L                              (EFX_EFFECT_PUSH_CLASS+60)
#define EFX_EFFECT_PUSH4_L_B_R_R                              (EFX_EFFECT_PUSH_CLASS+61)
#define EFX_EFFECT_PUSH4_L_B_R_T                              (EFX_EFFECT_PUSH_CLASS+62)
#define EFX_EFFECT_PUSH4_L_B_R_B                              (EFX_EFFECT_PUSH_CLASS+63)
#define EFX_EFFECT_PUSH4_L_B_T_L                              (EFX_EFFECT_PUSH_CLASS+64)
#define EFX_EFFECT_PUSH4_L_B_T_R                              (EFX_EFFECT_PUSH_CLASS+65)
#define EFX_EFFECT_PUSH4_L_B_T_T                              (EFX_EFFECT_PUSH_CLASS+66)
#define EFX_EFFECT_PUSH4_L_B_T_B                              (EFX_EFFECT_PUSH_CLASS+67)
#define EFX_EFFECT_PUSH4_L_B_B_L                              (EFX_EFFECT_PUSH_CLASS+68)
#define EFX_EFFECT_PUSH4_L_B_B_R                              (EFX_EFFECT_PUSH_CLASS+69)
#define EFX_EFFECT_PUSH4_L_B_B_T                              (EFX_EFFECT_PUSH_CLASS+70)
#define EFX_EFFECT_PUSH4_L_B_B_B                              (EFX_EFFECT_PUSH_CLASS+71)
#define EFX_EFFECT_PUSH4_R_L_L_L                              (EFX_EFFECT_PUSH_CLASS+72)
#define EFX_EFFECT_PUSH4_R_L_L_R                              (EFX_EFFECT_PUSH_CLASS+73)
#define EFX_EFFECT_PUSH4_R_L_L_T                              (EFX_EFFECT_PUSH_CLASS+74)
#define EFX_EFFECT_PUSH4_R_L_L_B                              (EFX_EFFECT_PUSH_CLASS+75)
#define EFX_EFFECT_PUSH4_R_L_R_L                              (EFX_EFFECT_PUSH_CLASS+76)
#define EFX_EFFECT_PUSH4_R_L_R_R                              (EFX_EFFECT_PUSH_CLASS+77)
#define EFX_EFFECT_PUSH4_R_L_R_T                              (EFX_EFFECT_PUSH_CLASS+78)
#define EFX_EFFECT_PUSH4_R_L_R_B                              (EFX_EFFECT_PUSH_CLASS+79)
#define EFX_EFFECT_PUSH4_R_L_T_L                              (EFX_EFFECT_PUSH_CLASS+80)
#define EFX_EFFECT_PUSH4_R_L_T_R                              (EFX_EFFECT_PUSH_CLASS+81)
#define EFX_EFFECT_PUSH4_R_L_T_T                              (EFX_EFFECT_PUSH_CLASS+82)
#define EFX_EFFECT_PUSH4_R_L_T_B                              (EFX_EFFECT_PUSH_CLASS+83)
#define EFX_EFFECT_PUSH4_R_L_B_L                              (EFX_EFFECT_PUSH_CLASS+84)
#define EFX_EFFECT_PUSH4_R_L_B_R                              (EFX_EFFECT_PUSH_CLASS+85)
#define EFX_EFFECT_PUSH4_R_L_B_T                              (EFX_EFFECT_PUSH_CLASS+86)
#define EFX_EFFECT_PUSH4_R_L_B_B                              (EFX_EFFECT_PUSH_CLASS+87)
#define EFX_EFFECT_PUSH4_R_R_L_L                              (EFX_EFFECT_PUSH_CLASS+88)
#define EFX_EFFECT_PUSH4_R_R_L_R                              (EFX_EFFECT_PUSH_CLASS+89)
#define EFX_EFFECT_PUSH4_R_R_L_T                              (EFX_EFFECT_PUSH_CLASS+90)
#define EFX_EFFECT_PUSH4_R_R_L_B                              (EFX_EFFECT_PUSH_CLASS+91)
#define EFX_EFFECT_PUSH4_R_R_R_L                              (EFX_EFFECT_PUSH_CLASS+92)
#define EFX_EFFECT_PUSH4_R_R_R_R                              (EFX_EFFECT_PUSH_CLASS+93)
#define EFX_EFFECT_PUSH4_R_R_R_T                              (EFX_EFFECT_PUSH_CLASS+94)
#define EFX_EFFECT_PUSH4_R_R_R_B                              (EFX_EFFECT_PUSH_CLASS+95)
#define EFX_EFFECT_PUSH4_R_R_T_L                              (EFX_EFFECT_PUSH_CLASS+96)
#define EFX_EFFECT_PUSH4_R_R_T_R                              (EFX_EFFECT_PUSH_CLASS+97)
#define EFX_EFFECT_PUSH4_R_R_T_T                              (EFX_EFFECT_PUSH_CLASS+98)
#define EFX_EFFECT_PUSH4_R_R_T_B                              (EFX_EFFECT_PUSH_CLASS+99)
#define EFX_EFFECT_PUSH4_R_R_B_L                              (EFX_EFFECT_PUSH_CLASS+100)
#define EFX_EFFECT_PUSH4_R_R_B_R                              (EFX_EFFECT_PUSH_CLASS+101)
#define EFX_EFFECT_PUSH4_R_R_B_T                              (EFX_EFFECT_PUSH_CLASS+102)
#define EFX_EFFECT_PUSH4_R_R_B_B                              (EFX_EFFECT_PUSH_CLASS+103)
#define EFX_EFFECT_PUSH4_R_T_L_L                              (EFX_EFFECT_PUSH_CLASS+104)
#define EFX_EFFECT_PUSH4_R_T_L_R                              (EFX_EFFECT_PUSH_CLASS+105)
#define EFX_EFFECT_PUSH4_R_T_L_T                              (EFX_EFFECT_PUSH_CLASS+106)
#define EFX_EFFECT_PUSH4_R_T_L_B                              (EFX_EFFECT_PUSH_CLASS+107)
#define EFX_EFFECT_PUSH4_R_T_R_L                              (EFX_EFFECT_PUSH_CLASS+108)
#define EFX_EFFECT_PUSH4_R_T_R_R                              (EFX_EFFECT_PUSH_CLASS+109)
#define EFX_EFFECT_PUSH4_R_T_R_T                              (EFX_EFFECT_PUSH_CLASS+110)
#define EFX_EFFECT_PUSH4_R_T_R_B                              (EFX_EFFECT_PUSH_CLASS+111)
#define EFX_EFFECT_PUSH4_R_T_T_L                              (EFX_EFFECT_PUSH_CLASS+112)
#define EFX_EFFECT_PUSH4_R_T_T_R                              (EFX_EFFECT_PUSH_CLASS+113)
#define EFX_EFFECT_PUSH4_R_T_T_T                              (EFX_EFFECT_PUSH_CLASS+114)
#define EFX_EFFECT_PUSH4_R_T_T_B                              (EFX_EFFECT_PUSH_CLASS+115)
#define EFX_EFFECT_PUSH4_R_T_B_L                              (EFX_EFFECT_PUSH_CLASS+116)
#define EFX_EFFECT_PUSH4_R_T_B_R                              (EFX_EFFECT_PUSH_CLASS+117)
#define EFX_EFFECT_PUSH4_R_T_B_T                              (EFX_EFFECT_PUSH_CLASS+118)
#define EFX_EFFECT_PUSH4_R_T_B_B                              (EFX_EFFECT_PUSH_CLASS+119)
#define EFX_EFFECT_PUSH4_R_B_L_L                              (EFX_EFFECT_PUSH_CLASS+120)
#define EFX_EFFECT_PUSH4_R_B_L_R                              (EFX_EFFECT_PUSH_CLASS+121)
#define EFX_EFFECT_PUSH4_R_B_L_T                              (EFX_EFFECT_PUSH_CLASS+122)
#define EFX_EFFECT_PUSH4_R_B_L_B                              (EFX_EFFECT_PUSH_CLASS+123)
#define EFX_EFFECT_PUSH4_R_B_R_L                              (EFX_EFFECT_PUSH_CLASS+124)
#define EFX_EFFECT_PUSH4_R_B_R_R                              (EFX_EFFECT_PUSH_CLASS+125)
#define EFX_EFFECT_PUSH4_R_B_R_T                              (EFX_EFFECT_PUSH_CLASS+126)
#define EFX_EFFECT_PUSH4_R_B_R_B                              (EFX_EFFECT_PUSH_CLASS+127)
#define EFX_EFFECT_PUSH4_R_B_T_L                              (EFX_EFFECT_PUSH_CLASS+128)
#define EFX_EFFECT_PUSH4_R_B_T_R                              (EFX_EFFECT_PUSH_CLASS+129)
#define EFX_EFFECT_PUSH4_R_B_T_T                              (EFX_EFFECT_PUSH_CLASS+130)
#define EFX_EFFECT_PUSH4_R_B_T_B                              (EFX_EFFECT_PUSH_CLASS+131)
#define EFX_EFFECT_PUSH4_R_B_B_L                              (EFX_EFFECT_PUSH_CLASS+132)
#define EFX_EFFECT_PUSH4_R_B_B_R                              (EFX_EFFECT_PUSH_CLASS+133)
#define EFX_EFFECT_PUSH4_R_B_B_T                              (EFX_EFFECT_PUSH_CLASS+134)
#define EFX_EFFECT_PUSH4_R_B_B_B                              (EFX_EFFECT_PUSH_CLASS+135)
#define EFX_EFFECT_PUSH4_T_L_L_L                              (EFX_EFFECT_PUSH_CLASS+136)
#define EFX_EFFECT_PUSH4_T_L_L_R                              (EFX_EFFECT_PUSH_CLASS+137)
#define EFX_EFFECT_PUSH4_T_L_L_T                              (EFX_EFFECT_PUSH_CLASS+138)
#define EFX_EFFECT_PUSH4_T_L_L_B                              (EFX_EFFECT_PUSH_CLASS+139)
#define EFX_EFFECT_PUSH4_T_L_R_L                              (EFX_EFFECT_PUSH_CLASS+140)
#define EFX_EFFECT_PUSH4_T_L_R_R                              (EFX_EFFECT_PUSH_CLASS+141)
#define EFX_EFFECT_PUSH4_T_L_R_T                              (EFX_EFFECT_PUSH_CLASS+142)
#define EFX_EFFECT_PUSH4_T_L_R_B                              (EFX_EFFECT_PUSH_CLASS+143)
#define EFX_EFFECT_PUSH4_T_L_T_L                              (EFX_EFFECT_PUSH_CLASS+144)
#define EFX_EFFECT_PUSH4_T_L_T_R                              (EFX_EFFECT_PUSH_CLASS+145)
#define EFX_EFFECT_PUSH4_T_L_T_T                              (EFX_EFFECT_PUSH_CLASS+146)
#define EFX_EFFECT_PUSH4_T_L_T_B                              (EFX_EFFECT_PUSH_CLASS+147)
#define EFX_EFFECT_PUSH4_T_L_B_L                              (EFX_EFFECT_PUSH_CLASS+148)
#define EFX_EFFECT_PUSH4_T_L_B_R                              (EFX_EFFECT_PUSH_CLASS+149)
#define EFX_EFFECT_PUSH4_T_L_B_T                              (EFX_EFFECT_PUSH_CLASS+150)
#define EFX_EFFECT_PUSH4_T_L_B_B                              (EFX_EFFECT_PUSH_CLASS+151)
#define EFX_EFFECT_PUSH4_T_R_L_L                              (EFX_EFFECT_PUSH_CLASS+152)
#define EFX_EFFECT_PUSH4_T_R_L_R                              (EFX_EFFECT_PUSH_CLASS+153)
#define EFX_EFFECT_PUSH4_T_R_L_T                              (EFX_EFFECT_PUSH_CLASS+154)
#define EFX_EFFECT_PUSH4_T_R_L_B                              (EFX_EFFECT_PUSH_CLASS+155)
#define EFX_EFFECT_PUSH4_T_R_R_L                              (EFX_EFFECT_PUSH_CLASS+156)
#define EFX_EFFECT_PUSH4_T_R_R_R                              (EFX_EFFECT_PUSH_CLASS+157)
#define EFX_EFFECT_PUSH4_T_R_R_T                              (EFX_EFFECT_PUSH_CLASS+158)
#define EFX_EFFECT_PUSH4_T_R_R_B                              (EFX_EFFECT_PUSH_CLASS+159)
#define EFX_EFFECT_PUSH4_T_R_T_L                              (EFX_EFFECT_PUSH_CLASS+160)
#define EFX_EFFECT_PUSH4_T_R_T_R                              (EFX_EFFECT_PUSH_CLASS+161)
#define EFX_EFFECT_PUSH4_T_R_T_T                              (EFX_EFFECT_PUSH_CLASS+162)
#define EFX_EFFECT_PUSH4_T_R_T_B                              (EFX_EFFECT_PUSH_CLASS+163)
#define EFX_EFFECT_PUSH4_T_R_B_L                              (EFX_EFFECT_PUSH_CLASS+164)
#define EFX_EFFECT_PUSH4_T_R_B_R                              (EFX_EFFECT_PUSH_CLASS+165)
#define EFX_EFFECT_PUSH4_T_R_B_T                              (EFX_EFFECT_PUSH_CLASS+166)
#define EFX_EFFECT_PUSH4_T_R_B_B                              (EFX_EFFECT_PUSH_CLASS+167)
#define EFX_EFFECT_PUSH4_T_T_L_L                              (EFX_EFFECT_PUSH_CLASS+168)
#define EFX_EFFECT_PUSH4_T_T_L_R                              (EFX_EFFECT_PUSH_CLASS+169)
#define EFX_EFFECT_PUSH4_T_T_L_T                              (EFX_EFFECT_PUSH_CLASS+170)
#define EFX_EFFECT_PUSH4_T_T_L_B                              (EFX_EFFECT_PUSH_CLASS+171)
#define EFX_EFFECT_PUSH4_T_T_R_L                              (EFX_EFFECT_PUSH_CLASS+172)
#define EFX_EFFECT_PUSH4_T_T_R_R                              (EFX_EFFECT_PUSH_CLASS+173)
#define EFX_EFFECT_PUSH4_T_T_R_T                              (EFX_EFFECT_PUSH_CLASS+174)
#define EFX_EFFECT_PUSH4_T_T_R_B                              (EFX_EFFECT_PUSH_CLASS+175)
#define EFX_EFFECT_PUSH4_T_T_T_L                              (EFX_EFFECT_PUSH_CLASS+176)
#define EFX_EFFECT_PUSH4_T_T_T_R                              (EFX_EFFECT_PUSH_CLASS+177)
#define EFX_EFFECT_PUSH4_T_T_T_T                              (EFX_EFFECT_PUSH_CLASS+178)
#define EFX_EFFECT_PUSH4_T_T_T_B                              (EFX_EFFECT_PUSH_CLASS+179)
#define EFX_EFFECT_PUSH4_T_T_B_L                              (EFX_EFFECT_PUSH_CLASS+180)
#define EFX_EFFECT_PUSH4_T_T_B_R                              (EFX_EFFECT_PUSH_CLASS+181)
#define EFX_EFFECT_PUSH4_T_T_B_T                              (EFX_EFFECT_PUSH_CLASS+182)
#define EFX_EFFECT_PUSH4_T_T_B_B                              (EFX_EFFECT_PUSH_CLASS+183)
#define EFX_EFFECT_PUSH4_T_B_L_L                              (EFX_EFFECT_PUSH_CLASS+184)
#define EFX_EFFECT_PUSH4_T_B_L_R                              (EFX_EFFECT_PUSH_CLASS+185)
#define EFX_EFFECT_PUSH4_T_B_L_T                              (EFX_EFFECT_PUSH_CLASS+186)
#define EFX_EFFECT_PUSH4_T_B_L_B                              (EFX_EFFECT_PUSH_CLASS+187)
#define EFX_EFFECT_PUSH4_T_B_R_L                              (EFX_EFFECT_PUSH_CLASS+188)
#define EFX_EFFECT_PUSH4_T_B_R_R                              (EFX_EFFECT_PUSH_CLASS+189)
#define EFX_EFFECT_PUSH4_T_B_R_T                              (EFX_EFFECT_PUSH_CLASS+190)
#define EFX_EFFECT_PUSH4_T_B_R_B                              (EFX_EFFECT_PUSH_CLASS+191)
#define EFX_EFFECT_PUSH4_T_B_T_L                              (EFX_EFFECT_PUSH_CLASS+192)
#define EFX_EFFECT_PUSH4_T_B_T_R                              (EFX_EFFECT_PUSH_CLASS+193)
#define EFX_EFFECT_PUSH4_T_B_T_T                              (EFX_EFFECT_PUSH_CLASS+194)
#define EFX_EFFECT_PUSH4_T_B_T_B                              (EFX_EFFECT_PUSH_CLASS+195)
#define EFX_EFFECT_PUSH4_T_B_B_L                              (EFX_EFFECT_PUSH_CLASS+196)
#define EFX_EFFECT_PUSH4_T_B_B_R                              (EFX_EFFECT_PUSH_CLASS+197)
#define EFX_EFFECT_PUSH4_T_B_B_T                              (EFX_EFFECT_PUSH_CLASS+198)
#define EFX_EFFECT_PUSH4_T_B_B_B                              (EFX_EFFECT_PUSH_CLASS+199)
#define EFX_EFFECT_PUSH4_B_L_L_L                              (EFX_EFFECT_PUSH_CLASS+200)
#define EFX_EFFECT_PUSH4_B_L_L_R                              (EFX_EFFECT_PUSH_CLASS+201)
#define EFX_EFFECT_PUSH4_B_L_L_T                              (EFX_EFFECT_PUSH_CLASS+202)
#define EFX_EFFECT_PUSH4_B_L_L_B                              (EFX_EFFECT_PUSH_CLASS+203)
#define EFX_EFFECT_PUSH4_B_L_R_L                              (EFX_EFFECT_PUSH_CLASS+204)
#define EFX_EFFECT_PUSH4_B_L_R_R                              (EFX_EFFECT_PUSH_CLASS+205)
#define EFX_EFFECT_PUSH4_B_L_R_T                              (EFX_EFFECT_PUSH_CLASS+206)
#define EFX_EFFECT_PUSH4_B_L_R_B                              (EFX_EFFECT_PUSH_CLASS+207)
#define EFX_EFFECT_PUSH4_B_L_T_L                              (EFX_EFFECT_PUSH_CLASS+208)
#define EFX_EFFECT_PUSH4_B_L_T_R                              (EFX_EFFECT_PUSH_CLASS+209)
#define EFX_EFFECT_PUSH4_B_L_T_T                              (EFX_EFFECT_PUSH_CLASS+210)
#define EFX_EFFECT_PUSH4_B_L_T_B                              (EFX_EFFECT_PUSH_CLASS+211)
#define EFX_EFFECT_PUSH4_B_L_B_L                              (EFX_EFFECT_PUSH_CLASS+212)
#define EFX_EFFECT_PUSH4_B_L_B_R                              (EFX_EFFECT_PUSH_CLASS+213)
#define EFX_EFFECT_PUSH4_B_L_B_T                              (EFX_EFFECT_PUSH_CLASS+214)
#define EFX_EFFECT_PUSH4_B_L_B_B                              (EFX_EFFECT_PUSH_CLASS+215)
#define EFX_EFFECT_PUSH4_B_R_L_L                              (EFX_EFFECT_PUSH_CLASS+216)
#define EFX_EFFECT_PUSH4_B_R_L_R                              (EFX_EFFECT_PUSH_CLASS+217)
#define EFX_EFFECT_PUSH4_B_R_L_T                              (EFX_EFFECT_PUSH_CLASS+218)
#define EFX_EFFECT_PUSH4_B_R_L_B                              (EFX_EFFECT_PUSH_CLASS+219)
#define EFX_EFFECT_PUSH4_B_R_R_L                              (EFX_EFFECT_PUSH_CLASS+220)
#define EFX_EFFECT_PUSH4_B_R_R_R                              (EFX_EFFECT_PUSH_CLASS+221)
#define EFX_EFFECT_PUSH4_B_R_R_T                              (EFX_EFFECT_PUSH_CLASS+222)
#define EFX_EFFECT_PUSH4_B_R_R_B                              (EFX_EFFECT_PUSH_CLASS+223)
#define EFX_EFFECT_PUSH4_B_R_T_L                              (EFX_EFFECT_PUSH_CLASS+224)
#define EFX_EFFECT_PUSH4_B_R_T_R                              (EFX_EFFECT_PUSH_CLASS+225)
#define EFX_EFFECT_PUSH4_B_R_T_T                              (EFX_EFFECT_PUSH_CLASS+226)
#define EFX_EFFECT_PUSH4_B_R_T_B                              (EFX_EFFECT_PUSH_CLASS+227)
#define EFX_EFFECT_PUSH4_B_R_B_L                              (EFX_EFFECT_PUSH_CLASS+228)
#define EFX_EFFECT_PUSH4_B_R_B_R                              (EFX_EFFECT_PUSH_CLASS+229)
#define EFX_EFFECT_PUSH4_B_R_B_T                              (EFX_EFFECT_PUSH_CLASS+230)
#define EFX_EFFECT_PUSH4_B_R_B_B                              (EFX_EFFECT_PUSH_CLASS+231)
#define EFX_EFFECT_PUSH4_B_T_L_L                              (EFX_EFFECT_PUSH_CLASS+232)
#define EFX_EFFECT_PUSH4_B_T_L_R                              (EFX_EFFECT_PUSH_CLASS+233)
#define EFX_EFFECT_PUSH4_B_T_L_T                              (EFX_EFFECT_PUSH_CLASS+234)
#define EFX_EFFECT_PUSH4_B_T_L_B                              (EFX_EFFECT_PUSH_CLASS+235)
#define EFX_EFFECT_PUSH4_B_T_R_L                              (EFX_EFFECT_PUSH_CLASS+236)
#define EFX_EFFECT_PUSH4_B_T_R_R                              (EFX_EFFECT_PUSH_CLASS+237)
#define EFX_EFFECT_PUSH4_B_T_R_T                              (EFX_EFFECT_PUSH_CLASS+238)
#define EFX_EFFECT_PUSH4_B_T_R_B                              (EFX_EFFECT_PUSH_CLASS+239)
#define EFX_EFFECT_PUSH4_B_T_T_L                              (EFX_EFFECT_PUSH_CLASS+240)
#define EFX_EFFECT_PUSH4_B_T_T_R                              (EFX_EFFECT_PUSH_CLASS+241)
#define EFX_EFFECT_PUSH4_B_T_T_T                              (EFX_EFFECT_PUSH_CLASS+242)
#define EFX_EFFECT_PUSH4_B_T_T_B                              (EFX_EFFECT_PUSH_CLASS+243)
#define EFX_EFFECT_PUSH4_B_T_B_L                              (EFX_EFFECT_PUSH_CLASS+244)
#define EFX_EFFECT_PUSH4_B_T_B_R                              (EFX_EFFECT_PUSH_CLASS+245)
#define EFX_EFFECT_PUSH4_B_T_B_T                              (EFX_EFFECT_PUSH_CLASS+246)
#define EFX_EFFECT_PUSH4_B_T_B_B                              (EFX_EFFECT_PUSH_CLASS+247)
#define EFX_EFFECT_PUSH4_B_B_L_L                              (EFX_EFFECT_PUSH_CLASS+248)
#define EFX_EFFECT_PUSH4_B_B_L_R                              (EFX_EFFECT_PUSH_CLASS+249)
#define EFX_EFFECT_PUSH4_B_B_L_T                              (EFX_EFFECT_PUSH_CLASS+250)
#define EFX_EFFECT_PUSH4_B_B_L_B                              (EFX_EFFECT_PUSH_CLASS+251)
#define EFX_EFFECT_PUSH4_B_B_R_L                              (EFX_EFFECT_PUSH_CLASS+252)
#define EFX_EFFECT_PUSH4_B_B_R_R                              (EFX_EFFECT_PUSH_CLASS+253)
#define EFX_EFFECT_PUSH4_B_B_R_T                              (EFX_EFFECT_PUSH_CLASS+254)
#define EFX_EFFECT_PUSH4_B_B_R_B                              (EFX_EFFECT_PUSH_CLASS+255)
#define EFX_EFFECT_PUSH4_B_B_T_L                              (EFX_EFFECT_PUSH_CLASS+256)
#define EFX_EFFECT_PUSH4_B_B_T_R                              (EFX_EFFECT_PUSH_CLASS+257)
#define EFX_EFFECT_PUSH4_B_B_T_T                              (EFX_EFFECT_PUSH_CLASS+258)
#define EFX_EFFECT_PUSH4_B_B_T_B                              (EFX_EFFECT_PUSH_CLASS+259)
#define EFX_EFFECT_PUSH4_B_B_B_L                              (EFX_EFFECT_PUSH_CLASS+260)
#define EFX_EFFECT_PUSH4_B_B_B_R                              (EFX_EFFECT_PUSH_CLASS+261)
#define EFX_EFFECT_PUSH4_B_B_B_T                              (EFX_EFFECT_PUSH_CLASS+262)
#define EFX_EFFECT_PUSH4_B_B_B_B                              (EFX_EFFECT_PUSH_CLASS+263)
#define EFX_EFFECT_PUSH_MAX                                   (EFX_EFFECT_PUSH_CLASS+263)

// Slide
#define EFX_EFFECT_SLIDE_CLASS                                6000
#define EFX_EFFECT_SLIDE_R_TO_L                               (EFX_EFFECT_SLIDE_CLASS+0)
#define EFX_EFFECT_SLIDE_L_TO_R                               (EFX_EFFECT_SLIDE_CLASS+1)
#define EFX_EFFECT_SLIDE_B_TO_T                               (EFX_EFFECT_SLIDE_CLASS+2)
#define EFX_EFFECT_SLIDE_T_TO_B                               (EFX_EFFECT_SLIDE_CLASS+3)
#define EFX_EFFECT_SLIDE_LT_TO_RB                             (EFX_EFFECT_SLIDE_CLASS+4)
#define EFX_EFFECT_SLIDE_RT_TO_LB                             (EFX_EFFECT_SLIDE_CLASS+5)
#define EFX_EFFECT_SLIDE_LB_TO_RT                             (EFX_EFFECT_SLIDE_CLASS+6)
#define EFX_EFFECT_SLIDE_RB_TO_LT                             (EFX_EFFECT_SLIDE_CLASS+7)
#define EFX_EFFECT_SLIDE_INTERH_L_TO_R_AND_R_TO_L             (EFX_EFFECT_SLIDE_CLASS+8)
#define EFX_EFFECT_SLIDE_INTERV_T_TO_B_AND_B_TO_T             (EFX_EFFECT_SLIDE_CLASS+9)
#define EFX_EFFECT_SLIDE2_T_R_TO_L_AND_B_L_TO_R               (EFX_EFFECT_SLIDE_CLASS+10)
#define EFX_EFFECT_SLIDE2_T_L_TO_R_AND_B_R_TO_L               (EFX_EFFECT_SLIDE_CLASS+11)
#define EFX_EFFECT_SLIDE2_L_B_TO_T_AND_R_T_TO_B               (EFX_EFFECT_SLIDE_CLASS+12)
#define EFX_EFFECT_SLIDE2_L_T_TO_B_AND_R_B_TO_T               (EFX_EFFECT_SLIDE_CLASS+13)
#define EFX_EFFECT_SLIDE4_L_L_L_L                             (EFX_EFFECT_SLIDE_CLASS+14)
#define EFX_EFFECT_SLIDE4_L_L_L_R                             (EFX_EFFECT_SLIDE_CLASS+15)
#define EFX_EFFECT_SLIDE4_L_L_L_T                             (EFX_EFFECT_SLIDE_CLASS+16)
#define EFX_EFFECT_SLIDE4_L_L_L_B                             (EFX_EFFECT_SLIDE_CLASS+17)
#define EFX_EFFECT_SLIDE4_L_L_R_L                             (EFX_EFFECT_SLIDE_CLASS+18)
#define EFX_EFFECT_SLIDE4_L_L_R_R                             (EFX_EFFECT_SLIDE_CLASS+19)
#define EFX_EFFECT_SLIDE4_L_L_R_T                             (EFX_EFFECT_SLIDE_CLASS+20)
#define EFX_EFFECT_SLIDE4_L_L_R_B                             (EFX_EFFECT_SLIDE_CLASS+21)
#define EFX_EFFECT_SLIDE4_L_L_T_L                             (EFX_EFFECT_SLIDE_CLASS+22)
#define EFX_EFFECT_SLIDE4_L_L_T_R                             (EFX_EFFECT_SLIDE_CLASS+23)
#define EFX_EFFECT_SLIDE4_L_L_T_T                             (EFX_EFFECT_SLIDE_CLASS+24)
#define EFX_EFFECT_SLIDE4_L_L_T_B                             (EFX_EFFECT_SLIDE_CLASS+25)
#define EFX_EFFECT_SLIDE4_L_L_B_L                             (EFX_EFFECT_SLIDE_CLASS+26)
#define EFX_EFFECT_SLIDE4_L_L_B_R                             (EFX_EFFECT_SLIDE_CLASS+27)
#define EFX_EFFECT_SLIDE4_L_L_B_T                             (EFX_EFFECT_SLIDE_CLASS+28)
#define EFX_EFFECT_SLIDE4_L_L_B_B                             (EFX_EFFECT_SLIDE_CLASS+29)
#define EFX_EFFECT_SLIDE4_L_R_L_L                             (EFX_EFFECT_SLIDE_CLASS+30)
#define EFX_EFFECT_SLIDE4_L_R_L_R                             (EFX_EFFECT_SLIDE_CLASS+31)
#define EFX_EFFECT_SLIDE4_L_R_L_T                             (EFX_EFFECT_SLIDE_CLASS+32)
#define EFX_EFFECT_SLIDE4_L_R_L_B                             (EFX_EFFECT_SLIDE_CLASS+33)
#define EFX_EFFECT_SLIDE4_L_R_R_L                             (EFX_EFFECT_SLIDE_CLASS+34)
#define EFX_EFFECT_SLIDE4_L_R_R_R                             (EFX_EFFECT_SLIDE_CLASS+35)
#define EFX_EFFECT_SLIDE4_L_R_R_T                             (EFX_EFFECT_SLIDE_CLASS+36)
#define EFX_EFFECT_SLIDE4_L_R_R_B                             (EFX_EFFECT_SLIDE_CLASS+37)
#define EFX_EFFECT_SLIDE4_L_R_T_L                             (EFX_EFFECT_SLIDE_CLASS+38)
#define EFX_EFFECT_SLIDE4_L_R_T_R                             (EFX_EFFECT_SLIDE_CLASS+39)
#define EFX_EFFECT_SLIDE4_L_R_T_T                             (EFX_EFFECT_SLIDE_CLASS+40)
#define EFX_EFFECT_SLIDE4_L_R_T_B                             (EFX_EFFECT_SLIDE_CLASS+41)
#define EFX_EFFECT_SLIDE4_L_R_B_L                             (EFX_EFFECT_SLIDE_CLASS+42)
#define EFX_EFFECT_SLIDE4_L_R_B_R                             (EFX_EFFECT_SLIDE_CLASS+43)
#define EFX_EFFECT_SLIDE4_L_R_B_T                             (EFX_EFFECT_SLIDE_CLASS+44)
#define EFX_EFFECT_SLIDE4_L_R_B_B                             (EFX_EFFECT_SLIDE_CLASS+45)
#define EFX_EFFECT_SLIDE4_L_T_L_L                             (EFX_EFFECT_SLIDE_CLASS+46)
#define EFX_EFFECT_SLIDE4_L_T_L_R                             (EFX_EFFECT_SLIDE_CLASS+47)
#define EFX_EFFECT_SLIDE4_L_T_L_T                             (EFX_EFFECT_SLIDE_CLASS+48)
#define EFX_EFFECT_SLIDE4_L_T_L_B                             (EFX_EFFECT_SLIDE_CLASS+49)
#define EFX_EFFECT_SLIDE4_L_T_R_L                             (EFX_EFFECT_SLIDE_CLASS+50)
#define EFX_EFFECT_SLIDE4_L_T_R_R                             (EFX_EFFECT_SLIDE_CLASS+51)
#define EFX_EFFECT_SLIDE4_L_T_R_T                             (EFX_EFFECT_SLIDE_CLASS+52)
#define EFX_EFFECT_SLIDE4_L_T_R_B                             (EFX_EFFECT_SLIDE_CLASS+53)
#define EFX_EFFECT_SLIDE4_L_T_T_L                             (EFX_EFFECT_SLIDE_CLASS+54)
#define EFX_EFFECT_SLIDE4_L_T_T_R                             (EFX_EFFECT_SLIDE_CLASS+55)
#define EFX_EFFECT_SLIDE4_L_T_T_T                             (EFX_EFFECT_SLIDE_CLASS+56)
#define EFX_EFFECT_SLIDE4_L_T_T_B                             (EFX_EFFECT_SLIDE_CLASS+57)
#define EFX_EFFECT_SLIDE4_L_T_B_L                             (EFX_EFFECT_SLIDE_CLASS+58)
#define EFX_EFFECT_SLIDE4_L_T_B_R                             (EFX_EFFECT_SLIDE_CLASS+59)
#define EFX_EFFECT_SLIDE4_L_T_B_T                             (EFX_EFFECT_SLIDE_CLASS+60)
#define EFX_EFFECT_SLIDE4_L_T_B_B                             (EFX_EFFECT_SLIDE_CLASS+61)
#define EFX_EFFECT_SLIDE4_L_B_L_L                             (EFX_EFFECT_SLIDE_CLASS+62)
#define EFX_EFFECT_SLIDE4_L_B_L_R                             (EFX_EFFECT_SLIDE_CLASS+63)
#define EFX_EFFECT_SLIDE4_L_B_L_T                             (EFX_EFFECT_SLIDE_CLASS+64)
#define EFX_EFFECT_SLIDE4_L_B_L_B                             (EFX_EFFECT_SLIDE_CLASS+65)
#define EFX_EFFECT_SLIDE4_L_B_R_L                             (EFX_EFFECT_SLIDE_CLASS+66)
#define EFX_EFFECT_SLIDE4_L_B_R_R                             (EFX_EFFECT_SLIDE_CLASS+67)
#define EFX_EFFECT_SLIDE4_L_B_R_T                             (EFX_EFFECT_SLIDE_CLASS+68)
#define EFX_EFFECT_SLIDE4_L_B_R_B                             (EFX_EFFECT_SLIDE_CLASS+69)
#define EFX_EFFECT_SLIDE4_L_B_T_L                             (EFX_EFFECT_SLIDE_CLASS+70)
#define EFX_EFFECT_SLIDE4_L_B_T_R                             (EFX_EFFECT_SLIDE_CLASS+71)
#define EFX_EFFECT_SLIDE4_L_B_T_T                             (EFX_EFFECT_SLIDE_CLASS+72)
#define EFX_EFFECT_SLIDE4_L_B_T_B                             (EFX_EFFECT_SLIDE_CLASS+73)
#define EFX_EFFECT_SLIDE4_L_B_B_L                             (EFX_EFFECT_SLIDE_CLASS+74)
#define EFX_EFFECT_SLIDE4_L_B_B_R                             (EFX_EFFECT_SLIDE_CLASS+75)
#define EFX_EFFECT_SLIDE4_L_B_B_T                             (EFX_EFFECT_SLIDE_CLASS+76)
#define EFX_EFFECT_SLIDE4_L_B_B_B                             (EFX_EFFECT_SLIDE_CLASS+77)
#define EFX_EFFECT_SLIDE4_R_L_L_L                             (EFX_EFFECT_SLIDE_CLASS+78)
#define EFX_EFFECT_SLIDE4_R_L_L_R                             (EFX_EFFECT_SLIDE_CLASS+79)
#define EFX_EFFECT_SLIDE4_R_L_L_T                             (EFX_EFFECT_SLIDE_CLASS+80)
#define EFX_EFFECT_SLIDE4_R_L_L_B                             (EFX_EFFECT_SLIDE_CLASS+81)
#define EFX_EFFECT_SLIDE4_R_L_R_L                             (EFX_EFFECT_SLIDE_CLASS+82)
#define EFX_EFFECT_SLIDE4_R_L_R_R                             (EFX_EFFECT_SLIDE_CLASS+83)
#define EFX_EFFECT_SLIDE4_R_L_R_T                             (EFX_EFFECT_SLIDE_CLASS+84)
#define EFX_EFFECT_SLIDE4_R_L_R_B                             (EFX_EFFECT_SLIDE_CLASS+85)
#define EFX_EFFECT_SLIDE4_R_L_T_L                             (EFX_EFFECT_SLIDE_CLASS+86)
#define EFX_EFFECT_SLIDE4_R_L_T_R                             (EFX_EFFECT_SLIDE_CLASS+87)
#define EFX_EFFECT_SLIDE4_R_L_T_T                             (EFX_EFFECT_SLIDE_CLASS+88)
#define EFX_EFFECT_SLIDE4_R_L_T_B                             (EFX_EFFECT_SLIDE_CLASS+89)
#define EFX_EFFECT_SLIDE4_R_L_B_L                             (EFX_EFFECT_SLIDE_CLASS+90)
#define EFX_EFFECT_SLIDE4_R_L_B_R                             (EFX_EFFECT_SLIDE_CLASS+91)
#define EFX_EFFECT_SLIDE4_R_L_B_T                             (EFX_EFFECT_SLIDE_CLASS+92)
#define EFX_EFFECT_SLIDE4_R_L_B_B                             (EFX_EFFECT_SLIDE_CLASS+93)
#define EFX_EFFECT_SLIDE4_R_R_L_L                             (EFX_EFFECT_SLIDE_CLASS+94)
#define EFX_EFFECT_SLIDE4_R_R_L_R                             (EFX_EFFECT_SLIDE_CLASS+95)
#define EFX_EFFECT_SLIDE4_R_R_L_T                             (EFX_EFFECT_SLIDE_CLASS+96)
#define EFX_EFFECT_SLIDE4_R_R_L_B                             (EFX_EFFECT_SLIDE_CLASS+97)
#define EFX_EFFECT_SLIDE4_R_R_R_L                             (EFX_EFFECT_SLIDE_CLASS+98)
#define EFX_EFFECT_SLIDE4_R_R_R_R                             (EFX_EFFECT_SLIDE_CLASS+99)
#define EFX_EFFECT_SLIDE4_R_R_R_T                             (EFX_EFFECT_SLIDE_CLASS+100)
#define EFX_EFFECT_SLIDE4_R_R_R_B                             (EFX_EFFECT_SLIDE_CLASS+101)
#define EFX_EFFECT_SLIDE4_R_R_T_L                             (EFX_EFFECT_SLIDE_CLASS+102)
#define EFX_EFFECT_SLIDE4_R_R_T_R                             (EFX_EFFECT_SLIDE_CLASS+103)
#define EFX_EFFECT_SLIDE4_R_R_T_T                             (EFX_EFFECT_SLIDE_CLASS+104)
#define EFX_EFFECT_SLIDE4_R_R_T_B                             (EFX_EFFECT_SLIDE_CLASS+105)
#define EFX_EFFECT_SLIDE4_R_R_B_L                             (EFX_EFFECT_SLIDE_CLASS+106)
#define EFX_EFFECT_SLIDE4_R_R_B_R                             (EFX_EFFECT_SLIDE_CLASS+107)
#define EFX_EFFECT_SLIDE4_R_R_B_T                             (EFX_EFFECT_SLIDE_CLASS+108)
#define EFX_EFFECT_SLIDE4_R_R_B_B                             (EFX_EFFECT_SLIDE_CLASS+109)
#define EFX_EFFECT_SLIDE4_R_T_L_L                             (EFX_EFFECT_SLIDE_CLASS+110)
#define EFX_EFFECT_SLIDE4_R_T_L_R                             (EFX_EFFECT_SLIDE_CLASS+111)
#define EFX_EFFECT_SLIDE4_R_T_L_T                             (EFX_EFFECT_SLIDE_CLASS+112)
#define EFX_EFFECT_SLIDE4_R_T_L_B                             (EFX_EFFECT_SLIDE_CLASS+113)
#define EFX_EFFECT_SLIDE4_R_T_R_L                             (EFX_EFFECT_SLIDE_CLASS+114)
#define EFX_EFFECT_SLIDE4_R_T_R_R                             (EFX_EFFECT_SLIDE_CLASS+115)
#define EFX_EFFECT_SLIDE4_R_T_R_T                             (EFX_EFFECT_SLIDE_CLASS+116)
#define EFX_EFFECT_SLIDE4_R_T_R_B                             (EFX_EFFECT_SLIDE_CLASS+117)
#define EFX_EFFECT_SLIDE4_R_T_T_L                             (EFX_EFFECT_SLIDE_CLASS+118)
#define EFX_EFFECT_SLIDE4_R_T_T_R                             (EFX_EFFECT_SLIDE_CLASS+119)
#define EFX_EFFECT_SLIDE4_R_T_T_T                             (EFX_EFFECT_SLIDE_CLASS+120)
#define EFX_EFFECT_SLIDE4_R_T_T_B                             (EFX_EFFECT_SLIDE_CLASS+121)
#define EFX_EFFECT_SLIDE4_R_T_B_L                             (EFX_EFFECT_SLIDE_CLASS+122)
#define EFX_EFFECT_SLIDE4_R_T_B_R                             (EFX_EFFECT_SLIDE_CLASS+123)
#define EFX_EFFECT_SLIDE4_R_T_B_T                             (EFX_EFFECT_SLIDE_CLASS+124)
#define EFX_EFFECT_SLIDE4_R_T_B_B                             (EFX_EFFECT_SLIDE_CLASS+125)
#define EFX_EFFECT_SLIDE4_R_B_L_L                             (EFX_EFFECT_SLIDE_CLASS+126)
#define EFX_EFFECT_SLIDE4_R_B_L_R                             (EFX_EFFECT_SLIDE_CLASS+127)
#define EFX_EFFECT_SLIDE4_R_B_L_T                             (EFX_EFFECT_SLIDE_CLASS+128)
#define EFX_EFFECT_SLIDE4_R_B_L_B                             (EFX_EFFECT_SLIDE_CLASS+129)
#define EFX_EFFECT_SLIDE4_R_B_R_L                             (EFX_EFFECT_SLIDE_CLASS+130)
#define EFX_EFFECT_SLIDE4_R_B_R_R                             (EFX_EFFECT_SLIDE_CLASS+131)
#define EFX_EFFECT_SLIDE4_R_B_R_T                             (EFX_EFFECT_SLIDE_CLASS+132)
#define EFX_EFFECT_SLIDE4_R_B_R_B                             (EFX_EFFECT_SLIDE_CLASS+133)
#define EFX_EFFECT_SLIDE4_R_B_T_L                             (EFX_EFFECT_SLIDE_CLASS+134)
#define EFX_EFFECT_SLIDE4_R_B_T_R                             (EFX_EFFECT_SLIDE_CLASS+135)
#define EFX_EFFECT_SLIDE4_R_B_T_T                             (EFX_EFFECT_SLIDE_CLASS+136)
#define EFX_EFFECT_SLIDE4_R_B_T_B                             (EFX_EFFECT_SLIDE_CLASS+137)
#define EFX_EFFECT_SLIDE4_R_B_B_L                             (EFX_EFFECT_SLIDE_CLASS+138)
#define EFX_EFFECT_SLIDE4_R_B_B_R                             (EFX_EFFECT_SLIDE_CLASS+139)
#define EFX_EFFECT_SLIDE4_R_B_B_T                             (EFX_EFFECT_SLIDE_CLASS+140)
#define EFX_EFFECT_SLIDE4_R_B_B_B                             (EFX_EFFECT_SLIDE_CLASS+141)
#define EFX_EFFECT_SLIDE4_T_L_L_L                             (EFX_EFFECT_SLIDE_CLASS+142)
#define EFX_EFFECT_SLIDE4_T_L_L_R                             (EFX_EFFECT_SLIDE_CLASS+143)
#define EFX_EFFECT_SLIDE4_T_L_L_T                             (EFX_EFFECT_SLIDE_CLASS+144)
#define EFX_EFFECT_SLIDE4_T_L_L_B                             (EFX_EFFECT_SLIDE_CLASS+145)
#define EFX_EFFECT_SLIDE4_T_L_R_L                             (EFX_EFFECT_SLIDE_CLASS+146)
#define EFX_EFFECT_SLIDE4_T_L_R_R                             (EFX_EFFECT_SLIDE_CLASS+147)
#define EFX_EFFECT_SLIDE4_T_L_R_T                             (EFX_EFFECT_SLIDE_CLASS+148)
#define EFX_EFFECT_SLIDE4_T_L_R_B                             (EFX_EFFECT_SLIDE_CLASS+149)
#define EFX_EFFECT_SLIDE4_T_L_T_L                             (EFX_EFFECT_SLIDE_CLASS+150)
#define EFX_EFFECT_SLIDE4_T_L_T_R                             (EFX_EFFECT_SLIDE_CLASS+151)
#define EFX_EFFECT_SLIDE4_T_L_T_T                             (EFX_EFFECT_SLIDE_CLASS+152)
#define EFX_EFFECT_SLIDE4_T_L_T_B                             (EFX_EFFECT_SLIDE_CLASS+153)
#define EFX_EFFECT_SLIDE4_T_L_B_L                             (EFX_EFFECT_SLIDE_CLASS+154)
#define EFX_EFFECT_SLIDE4_T_L_B_R                             (EFX_EFFECT_SLIDE_CLASS+155)
#define EFX_EFFECT_SLIDE4_T_L_B_T                             (EFX_EFFECT_SLIDE_CLASS+156)
#define EFX_EFFECT_SLIDE4_T_L_B_B                             (EFX_EFFECT_SLIDE_CLASS+157)
#define EFX_EFFECT_SLIDE4_T_R_L_L                             (EFX_EFFECT_SLIDE_CLASS+158)
#define EFX_EFFECT_SLIDE4_T_R_L_R                             (EFX_EFFECT_SLIDE_CLASS+159)
#define EFX_EFFECT_SLIDE4_T_R_L_T                             (EFX_EFFECT_SLIDE_CLASS+160)
#define EFX_EFFECT_SLIDE4_T_R_L_B                             (EFX_EFFECT_SLIDE_CLASS+161)
#define EFX_EFFECT_SLIDE4_T_R_R_L                             (EFX_EFFECT_SLIDE_CLASS+162)
#define EFX_EFFECT_SLIDE4_T_R_R_R                             (EFX_EFFECT_SLIDE_CLASS+163)
#define EFX_EFFECT_SLIDE4_T_R_R_T                             (EFX_EFFECT_SLIDE_CLASS+164)
#define EFX_EFFECT_SLIDE4_T_R_R_B                             (EFX_EFFECT_SLIDE_CLASS+165)
#define EFX_EFFECT_SLIDE4_T_R_T_L                             (EFX_EFFECT_SLIDE_CLASS+166)
#define EFX_EFFECT_SLIDE4_T_R_T_R                             (EFX_EFFECT_SLIDE_CLASS+167)
#define EFX_EFFECT_SLIDE4_T_R_T_T                             (EFX_EFFECT_SLIDE_CLASS+168)
#define EFX_EFFECT_SLIDE4_T_R_T_B                             (EFX_EFFECT_SLIDE_CLASS+169)
#define EFX_EFFECT_SLIDE4_T_R_B_L                             (EFX_EFFECT_SLIDE_CLASS+170)
#define EFX_EFFECT_SLIDE4_T_R_B_R                             (EFX_EFFECT_SLIDE_CLASS+171)
#define EFX_EFFECT_SLIDE4_T_R_B_T                             (EFX_EFFECT_SLIDE_CLASS+172)
#define EFX_EFFECT_SLIDE4_T_R_B_B                             (EFX_EFFECT_SLIDE_CLASS+173)
#define EFX_EFFECT_SLIDE4_T_T_L_L                             (EFX_EFFECT_SLIDE_CLASS+174)
#define EFX_EFFECT_SLIDE4_T_T_L_R                             (EFX_EFFECT_SLIDE_CLASS+175)
#define EFX_EFFECT_SLIDE4_T_T_L_T                             (EFX_EFFECT_SLIDE_CLASS+176)
#define EFX_EFFECT_SLIDE4_T_T_L_B                             (EFX_EFFECT_SLIDE_CLASS+177)
#define EFX_EFFECT_SLIDE4_T_T_R_L                             (EFX_EFFECT_SLIDE_CLASS+178)
#define EFX_EFFECT_SLIDE4_T_T_R_R                             (EFX_EFFECT_SLIDE_CLASS+179)
#define EFX_EFFECT_SLIDE4_T_T_R_T                             (EFX_EFFECT_SLIDE_CLASS+180)
#define EFX_EFFECT_SLIDE4_T_T_R_B                             (EFX_EFFECT_SLIDE_CLASS+181)
#define EFX_EFFECT_SLIDE4_T_T_T_L                             (EFX_EFFECT_SLIDE_CLASS+182)
#define EFX_EFFECT_SLIDE4_T_T_T_R                             (EFX_EFFECT_SLIDE_CLASS+183)
#define EFX_EFFECT_SLIDE4_T_T_T_T                             (EFX_EFFECT_SLIDE_CLASS+184)
#define EFX_EFFECT_SLIDE4_T_T_T_B                             (EFX_EFFECT_SLIDE_CLASS+185)
#define EFX_EFFECT_SLIDE4_T_T_B_L                             (EFX_EFFECT_SLIDE_CLASS+186)
#define EFX_EFFECT_SLIDE4_T_T_B_R                             (EFX_EFFECT_SLIDE_CLASS+187)
#define EFX_EFFECT_SLIDE4_T_T_B_T                             (EFX_EFFECT_SLIDE_CLASS+188)
#define EFX_EFFECT_SLIDE4_T_T_B_B                             (EFX_EFFECT_SLIDE_CLASS+189)
#define EFX_EFFECT_SLIDE4_T_B_L_L                             (EFX_EFFECT_SLIDE_CLASS+190)
#define EFX_EFFECT_SLIDE4_T_B_L_R                             (EFX_EFFECT_SLIDE_CLASS+191)
#define EFX_EFFECT_SLIDE4_T_B_L_T                             (EFX_EFFECT_SLIDE_CLASS+192)
#define EFX_EFFECT_SLIDE4_T_B_L_B                             (EFX_EFFECT_SLIDE_CLASS+193)
#define EFX_EFFECT_SLIDE4_T_B_R_L                             (EFX_EFFECT_SLIDE_CLASS+194)
#define EFX_EFFECT_SLIDE4_T_B_R_R                             (EFX_EFFECT_SLIDE_CLASS+195)
#define EFX_EFFECT_SLIDE4_T_B_R_T                             (EFX_EFFECT_SLIDE_CLASS+196)
#define EFX_EFFECT_SLIDE4_T_B_R_B                             (EFX_EFFECT_SLIDE_CLASS+197)
#define EFX_EFFECT_SLIDE4_T_B_T_L                             (EFX_EFFECT_SLIDE_CLASS+198)
#define EFX_EFFECT_SLIDE4_T_B_T_R                             (EFX_EFFECT_SLIDE_CLASS+199)
#define EFX_EFFECT_SLIDE4_T_B_T_T                             (EFX_EFFECT_SLIDE_CLASS+200)
#define EFX_EFFECT_SLIDE4_T_B_T_B                             (EFX_EFFECT_SLIDE_CLASS+201)
#define EFX_EFFECT_SLIDE4_T_B_B_L                             (EFX_EFFECT_SLIDE_CLASS+202)
#define EFX_EFFECT_SLIDE4_T_B_B_R                             (EFX_EFFECT_SLIDE_CLASS+203)
#define EFX_EFFECT_SLIDE4_T_B_B_T                             (EFX_EFFECT_SLIDE_CLASS+204)
#define EFX_EFFECT_SLIDE4_T_B_B_B                             (EFX_EFFECT_SLIDE_CLASS+205)
#define EFX_EFFECT_SLIDE4_B_L_L_L                             (EFX_EFFECT_SLIDE_CLASS+206)
#define EFX_EFFECT_SLIDE4_B_L_L_R                             (EFX_EFFECT_SLIDE_CLASS+207)
#define EFX_EFFECT_SLIDE4_B_L_L_T                             (EFX_EFFECT_SLIDE_CLASS+208)
#define EFX_EFFECT_SLIDE4_B_L_L_B                             (EFX_EFFECT_SLIDE_CLASS+209)
#define EFX_EFFECT_SLIDE4_B_L_R_L                             (EFX_EFFECT_SLIDE_CLASS+210)
#define EFX_EFFECT_SLIDE4_B_L_R_R                             (EFX_EFFECT_SLIDE_CLASS+211)
#define EFX_EFFECT_SLIDE4_B_L_R_T                             (EFX_EFFECT_SLIDE_CLASS+212)
#define EFX_EFFECT_SLIDE4_B_L_R_B                             (EFX_EFFECT_SLIDE_CLASS+213)
#define EFX_EFFECT_SLIDE4_B_L_T_L                             (EFX_EFFECT_SLIDE_CLASS+214)
#define EFX_EFFECT_SLIDE4_B_L_T_R                             (EFX_EFFECT_SLIDE_CLASS+215)
#define EFX_EFFECT_SLIDE4_B_L_T_T                             (EFX_EFFECT_SLIDE_CLASS+216)
#define EFX_EFFECT_SLIDE4_B_L_T_B                             (EFX_EFFECT_SLIDE_CLASS+217)
#define EFX_EFFECT_SLIDE4_B_L_B_L                             (EFX_EFFECT_SLIDE_CLASS+218)
#define EFX_EFFECT_SLIDE4_B_L_B_R                             (EFX_EFFECT_SLIDE_CLASS+219)
#define EFX_EFFECT_SLIDE4_B_L_B_T                             (EFX_EFFECT_SLIDE_CLASS+220)
#define EFX_EFFECT_SLIDE4_B_L_B_B                             (EFX_EFFECT_SLIDE_CLASS+221)
#define EFX_EFFECT_SLIDE4_B_R_L_L                             (EFX_EFFECT_SLIDE_CLASS+222)
#define EFX_EFFECT_SLIDE4_B_R_L_R                             (EFX_EFFECT_SLIDE_CLASS+223)
#define EFX_EFFECT_SLIDE4_B_R_L_T                             (EFX_EFFECT_SLIDE_CLASS+224)
#define EFX_EFFECT_SLIDE4_B_R_L_B                             (EFX_EFFECT_SLIDE_CLASS+225)
#define EFX_EFFECT_SLIDE4_B_R_R_L                             (EFX_EFFECT_SLIDE_CLASS+226)
#define EFX_EFFECT_SLIDE4_B_R_R_R                             (EFX_EFFECT_SLIDE_CLASS+227)
#define EFX_EFFECT_SLIDE4_B_R_R_T                             (EFX_EFFECT_SLIDE_CLASS+228)
#define EFX_EFFECT_SLIDE4_B_R_R_B                             (EFX_EFFECT_SLIDE_CLASS+229)
#define EFX_EFFECT_SLIDE4_B_R_T_L                             (EFX_EFFECT_SLIDE_CLASS+230)
#define EFX_EFFECT_SLIDE4_B_R_T_R                             (EFX_EFFECT_SLIDE_CLASS+231)
#define EFX_EFFECT_SLIDE4_B_R_T_T                             (EFX_EFFECT_SLIDE_CLASS+232)
#define EFX_EFFECT_SLIDE4_B_R_T_B                             (EFX_EFFECT_SLIDE_CLASS+233)
#define EFX_EFFECT_SLIDE4_B_R_B_L                             (EFX_EFFECT_SLIDE_CLASS+234)
#define EFX_EFFECT_SLIDE4_B_R_B_R                             (EFX_EFFECT_SLIDE_CLASS+235)
#define EFX_EFFECT_SLIDE4_B_R_B_T                             (EFX_EFFECT_SLIDE_CLASS+236)
#define EFX_EFFECT_SLIDE4_B_R_B_B                             (EFX_EFFECT_SLIDE_CLASS+237)
#define EFX_EFFECT_SLIDE4_B_T_L_L                             (EFX_EFFECT_SLIDE_CLASS+238)
#define EFX_EFFECT_SLIDE4_B_T_L_R                             (EFX_EFFECT_SLIDE_CLASS+239)
#define EFX_EFFECT_SLIDE4_B_T_L_T                             (EFX_EFFECT_SLIDE_CLASS+240)
#define EFX_EFFECT_SLIDE4_B_T_L_B                             (EFX_EFFECT_SLIDE_CLASS+241)
#define EFX_EFFECT_SLIDE4_B_T_R_L                             (EFX_EFFECT_SLIDE_CLASS+242)
#define EFX_EFFECT_SLIDE4_B_T_R_R                             (EFX_EFFECT_SLIDE_CLASS+243)
#define EFX_EFFECT_SLIDE4_B_T_R_T                             (EFX_EFFECT_SLIDE_CLASS+244)
#define EFX_EFFECT_SLIDE4_B_T_R_B                             (EFX_EFFECT_SLIDE_CLASS+245)
#define EFX_EFFECT_SLIDE4_B_T_T_L                             (EFX_EFFECT_SLIDE_CLASS+246)
#define EFX_EFFECT_SLIDE4_B_T_T_R                             (EFX_EFFECT_SLIDE_CLASS+247)
#define EFX_EFFECT_SLIDE4_B_T_T_T                             (EFX_EFFECT_SLIDE_CLASS+248)
#define EFX_EFFECT_SLIDE4_B_T_T_B                             (EFX_EFFECT_SLIDE_CLASS+249)
#define EFX_EFFECT_SLIDE4_B_T_B_L                             (EFX_EFFECT_SLIDE_CLASS+250)
#define EFX_EFFECT_SLIDE4_B_T_B_R                             (EFX_EFFECT_SLIDE_CLASS+251)
#define EFX_EFFECT_SLIDE4_B_T_B_T                             (EFX_EFFECT_SLIDE_CLASS+252)
#define EFX_EFFECT_SLIDE4_B_T_B_B                             (EFX_EFFECT_SLIDE_CLASS+253)
#define EFX_EFFECT_SLIDE4_B_B_L_L                             (EFX_EFFECT_SLIDE_CLASS+254)
#define EFX_EFFECT_SLIDE4_B_B_L_R                             (EFX_EFFECT_SLIDE_CLASS+255)
#define EFX_EFFECT_SLIDE4_B_B_L_T                             (EFX_EFFECT_SLIDE_CLASS+256)
#define EFX_EFFECT_SLIDE4_B_B_L_B                             (EFX_EFFECT_SLIDE_CLASS+257)
#define EFX_EFFECT_SLIDE4_B_B_R_L                             (EFX_EFFECT_SLIDE_CLASS+258)
#define EFX_EFFECT_SLIDE4_B_B_R_R                             (EFX_EFFECT_SLIDE_CLASS+259)
#define EFX_EFFECT_SLIDE4_B_B_R_T                             (EFX_EFFECT_SLIDE_CLASS+260)
#define EFX_EFFECT_SLIDE4_B_B_R_B                             (EFX_EFFECT_SLIDE_CLASS+261)
#define EFX_EFFECT_SLIDE4_B_B_T_L                             (EFX_EFFECT_SLIDE_CLASS+262)
#define EFX_EFFECT_SLIDE4_B_B_T_R                             (EFX_EFFECT_SLIDE_CLASS+263)
#define EFX_EFFECT_SLIDE4_B_B_T_T                             (EFX_EFFECT_SLIDE_CLASS+264)
#define EFX_EFFECT_SLIDE4_B_B_T_B                             (EFX_EFFECT_SLIDE_CLASS+265)
#define EFX_EFFECT_SLIDE4_B_B_B_L                             (EFX_EFFECT_SLIDE_CLASS+266)
#define EFX_EFFECT_SLIDE4_B_B_B_R                             (EFX_EFFECT_SLIDE_CLASS+267)
#define EFX_EFFECT_SLIDE4_B_B_B_T                             (EFX_EFFECT_SLIDE_CLASS+268)
#define EFX_EFFECT_SLIDE4_B_B_B_B                             (EFX_EFFECT_SLIDE_CLASS+269)
#define EFX_EFFECT_SLIDE4_LT_LT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+270)
#define EFX_EFFECT_SLIDE4_LT_LT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+271)
#define EFX_EFFECT_SLIDE4_LT_LT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+272)
#define EFX_EFFECT_SLIDE4_LT_LT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+273)
#define EFX_EFFECT_SLIDE4_LT_LT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+274)
#define EFX_EFFECT_SLIDE4_LT_LT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+275)
#define EFX_EFFECT_SLIDE4_LT_LT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+276)
#define EFX_EFFECT_SLIDE4_LT_LT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+277)
#define EFX_EFFECT_SLIDE4_LT_LT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+278)
#define EFX_EFFECT_SLIDE4_LT_LT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+279)
#define EFX_EFFECT_SLIDE4_LT_LT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+280)
#define EFX_EFFECT_SLIDE4_LT_LT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+281)
#define EFX_EFFECT_SLIDE4_LT_LT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+282)
#define EFX_EFFECT_SLIDE4_LT_LT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+283)
#define EFX_EFFECT_SLIDE4_LT_LT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+284)
#define EFX_EFFECT_SLIDE4_LT_LT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+285)
#define EFX_EFFECT_SLIDE4_LT_LB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+286)
#define EFX_EFFECT_SLIDE4_LT_LB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+287)
#define EFX_EFFECT_SLIDE4_LT_LB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+288)
#define EFX_EFFECT_SLIDE4_LT_LB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+289)
#define EFX_EFFECT_SLIDE4_LT_LB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+290)
#define EFX_EFFECT_SLIDE4_LT_LB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+291)
#define EFX_EFFECT_SLIDE4_LT_LB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+292)
#define EFX_EFFECT_SLIDE4_LT_LB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+293)
#define EFX_EFFECT_SLIDE4_LT_LB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+294)
#define EFX_EFFECT_SLIDE4_LT_LB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+295)
#define EFX_EFFECT_SLIDE4_LT_LB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+296)
#define EFX_EFFECT_SLIDE4_LT_LB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+297)
#define EFX_EFFECT_SLIDE4_LT_LB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+298)
#define EFX_EFFECT_SLIDE4_LT_LB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+299)
#define EFX_EFFECT_SLIDE4_LT_LB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+300)
#define EFX_EFFECT_SLIDE4_LT_LB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+301)
#define EFX_EFFECT_SLIDE4_LT_RT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+302)
#define EFX_EFFECT_SLIDE4_LT_RT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+303)
#define EFX_EFFECT_SLIDE4_LT_RT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+304)
#define EFX_EFFECT_SLIDE4_LT_RT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+305)
#define EFX_EFFECT_SLIDE4_LT_RT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+306)
#define EFX_EFFECT_SLIDE4_LT_RT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+307)
#define EFX_EFFECT_SLIDE4_LT_RT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+308)
#define EFX_EFFECT_SLIDE4_LT_RT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+309)
#define EFX_EFFECT_SLIDE4_LT_RT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+310)
#define EFX_EFFECT_SLIDE4_LT_RT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+311)
#define EFX_EFFECT_SLIDE4_LT_RT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+312)
#define EFX_EFFECT_SLIDE4_LT_RT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+313)
#define EFX_EFFECT_SLIDE4_LT_RT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+314)
#define EFX_EFFECT_SLIDE4_LT_RT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+315)
#define EFX_EFFECT_SLIDE4_LT_RT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+316)
#define EFX_EFFECT_SLIDE4_LT_RT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+317)
#define EFX_EFFECT_SLIDE4_LT_RB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+318)
#define EFX_EFFECT_SLIDE4_LT_RB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+319)
#define EFX_EFFECT_SLIDE4_LT_RB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+320)
#define EFX_EFFECT_SLIDE4_LT_RB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+321)
#define EFX_EFFECT_SLIDE4_LT_RB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+322)
#define EFX_EFFECT_SLIDE4_LT_RB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+323)
#define EFX_EFFECT_SLIDE4_LT_RB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+324)
#define EFX_EFFECT_SLIDE4_LT_RB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+325)
#define EFX_EFFECT_SLIDE4_LT_RB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+326)
#define EFX_EFFECT_SLIDE4_LT_RB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+327)
#define EFX_EFFECT_SLIDE4_LT_RB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+328)
#define EFX_EFFECT_SLIDE4_LT_RB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+329)
#define EFX_EFFECT_SLIDE4_LT_RB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+330)
#define EFX_EFFECT_SLIDE4_LT_RB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+331)
#define EFX_EFFECT_SLIDE4_LT_RB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+332)
#define EFX_EFFECT_SLIDE4_LT_RB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+333)
#define EFX_EFFECT_SLIDE4_LB_LT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+334)
#define EFX_EFFECT_SLIDE4_LB_LT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+335)
#define EFX_EFFECT_SLIDE4_LB_LT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+336)
#define EFX_EFFECT_SLIDE4_LB_LT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+337)
#define EFX_EFFECT_SLIDE4_LB_LT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+338)
#define EFX_EFFECT_SLIDE4_LB_LT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+339)
#define EFX_EFFECT_SLIDE4_LB_LT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+340)
#define EFX_EFFECT_SLIDE4_LB_LT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+341)
#define EFX_EFFECT_SLIDE4_LB_LT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+342)
#define EFX_EFFECT_SLIDE4_LB_LT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+343)
#define EFX_EFFECT_SLIDE4_LB_LT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+344)
#define EFX_EFFECT_SLIDE4_LB_LT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+345)
#define EFX_EFFECT_SLIDE4_LB_LT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+346)
#define EFX_EFFECT_SLIDE4_LB_LT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+347)
#define EFX_EFFECT_SLIDE4_LB_LT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+348)
#define EFX_EFFECT_SLIDE4_LB_LT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+349)
#define EFX_EFFECT_SLIDE4_LB_LB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+350)
#define EFX_EFFECT_SLIDE4_LB_LB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+351)
#define EFX_EFFECT_SLIDE4_LB_LB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+352)
#define EFX_EFFECT_SLIDE4_LB_LB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+353)
#define EFX_EFFECT_SLIDE4_LB_LB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+354)
#define EFX_EFFECT_SLIDE4_LB_LB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+355)
#define EFX_EFFECT_SLIDE4_LB_LB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+356)
#define EFX_EFFECT_SLIDE4_LB_LB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+357)
#define EFX_EFFECT_SLIDE4_LB_LB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+358)
#define EFX_EFFECT_SLIDE4_LB_LB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+359)
#define EFX_EFFECT_SLIDE4_LB_LB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+360)
#define EFX_EFFECT_SLIDE4_LB_LB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+361)
#define EFX_EFFECT_SLIDE4_LB_LB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+362)
#define EFX_EFFECT_SLIDE4_LB_LB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+363)
#define EFX_EFFECT_SLIDE4_LB_LB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+364)
#define EFX_EFFECT_SLIDE4_LB_LB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+365)
#define EFX_EFFECT_SLIDE4_LB_RT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+366)
#define EFX_EFFECT_SLIDE4_LB_RT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+367)
#define EFX_EFFECT_SLIDE4_LB_RT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+368)
#define EFX_EFFECT_SLIDE4_LB_RT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+369)
#define EFX_EFFECT_SLIDE4_LB_RT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+370)
#define EFX_EFFECT_SLIDE4_LB_RT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+371)
#define EFX_EFFECT_SLIDE4_LB_RT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+372)
#define EFX_EFFECT_SLIDE4_LB_RT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+373)
#define EFX_EFFECT_SLIDE4_LB_RT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+374)
#define EFX_EFFECT_SLIDE4_LB_RT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+375)
#define EFX_EFFECT_SLIDE4_LB_RT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+376)
#define EFX_EFFECT_SLIDE4_LB_RT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+377)
#define EFX_EFFECT_SLIDE4_LB_RT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+378)
#define EFX_EFFECT_SLIDE4_LB_RT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+379)
#define EFX_EFFECT_SLIDE4_LB_RT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+380)
#define EFX_EFFECT_SLIDE4_LB_RT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+381)
#define EFX_EFFECT_SLIDE4_LB_RB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+382)
#define EFX_EFFECT_SLIDE4_LB_RB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+383)
#define EFX_EFFECT_SLIDE4_LB_RB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+384)
#define EFX_EFFECT_SLIDE4_LB_RB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+385)
#define EFX_EFFECT_SLIDE4_LB_RB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+386)
#define EFX_EFFECT_SLIDE4_LB_RB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+387)
#define EFX_EFFECT_SLIDE4_LB_RB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+388)
#define EFX_EFFECT_SLIDE4_LB_RB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+389)
#define EFX_EFFECT_SLIDE4_LB_RB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+390)
#define EFX_EFFECT_SLIDE4_LB_RB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+391)
#define EFX_EFFECT_SLIDE4_LB_RB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+392)
#define EFX_EFFECT_SLIDE4_LB_RB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+393)
#define EFX_EFFECT_SLIDE4_LB_RB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+394)
#define EFX_EFFECT_SLIDE4_LB_RB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+395)
#define EFX_EFFECT_SLIDE4_LB_RB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+396)
#define EFX_EFFECT_SLIDE4_LB_RB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+397)
#define EFX_EFFECT_SLIDE4_RT_LT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+398)
#define EFX_EFFECT_SLIDE4_RT_LT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+399)
#define EFX_EFFECT_SLIDE4_RT_LT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+400)
#define EFX_EFFECT_SLIDE4_RT_LT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+401)
#define EFX_EFFECT_SLIDE4_RT_LT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+402)
#define EFX_EFFECT_SLIDE4_RT_LT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+403)
#define EFX_EFFECT_SLIDE4_RT_LT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+404)
#define EFX_EFFECT_SLIDE4_RT_LT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+405)
#define EFX_EFFECT_SLIDE4_RT_LT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+406)
#define EFX_EFFECT_SLIDE4_RT_LT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+407)
#define EFX_EFFECT_SLIDE4_RT_LT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+408)
#define EFX_EFFECT_SLIDE4_RT_LT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+409)
#define EFX_EFFECT_SLIDE4_RT_LT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+410)
#define EFX_EFFECT_SLIDE4_RT_LT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+411)
#define EFX_EFFECT_SLIDE4_RT_LT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+412)
#define EFX_EFFECT_SLIDE4_RT_LT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+413)
#define EFX_EFFECT_SLIDE4_RT_LB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+414)
#define EFX_EFFECT_SLIDE4_RT_LB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+415)
#define EFX_EFFECT_SLIDE4_RT_LB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+416)
#define EFX_EFFECT_SLIDE4_RT_LB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+417)
#define EFX_EFFECT_SLIDE4_RT_LB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+418)
#define EFX_EFFECT_SLIDE4_RT_LB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+419)
#define EFX_EFFECT_SLIDE4_RT_LB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+420)
#define EFX_EFFECT_SLIDE4_RT_LB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+421)
#define EFX_EFFECT_SLIDE4_RT_LB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+422)
#define EFX_EFFECT_SLIDE4_RT_LB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+423)
#define EFX_EFFECT_SLIDE4_RT_LB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+424)
#define EFX_EFFECT_SLIDE4_RT_LB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+425)
#define EFX_EFFECT_SLIDE4_RT_LB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+426)
#define EFX_EFFECT_SLIDE4_RT_LB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+427)
#define EFX_EFFECT_SLIDE4_RT_LB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+428)
#define EFX_EFFECT_SLIDE4_RT_LB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+429)
#define EFX_EFFECT_SLIDE4_RT_RT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+430)
#define EFX_EFFECT_SLIDE4_RT_RT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+431)
#define EFX_EFFECT_SLIDE4_RT_RT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+432)
#define EFX_EFFECT_SLIDE4_RT_RT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+433)
#define EFX_EFFECT_SLIDE4_RT_RT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+434)
#define EFX_EFFECT_SLIDE4_RT_RT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+435)
#define EFX_EFFECT_SLIDE4_RT_RT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+436)
#define EFX_EFFECT_SLIDE4_RT_RT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+437)
#define EFX_EFFECT_SLIDE4_RT_RT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+438)
#define EFX_EFFECT_SLIDE4_RT_RT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+439)
#define EFX_EFFECT_SLIDE4_RT_RT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+440)
#define EFX_EFFECT_SLIDE4_RT_RT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+441)
#define EFX_EFFECT_SLIDE4_RT_RT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+442)
#define EFX_EFFECT_SLIDE4_RT_RT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+443)
#define EFX_EFFECT_SLIDE4_RT_RT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+444)
#define EFX_EFFECT_SLIDE4_RT_RT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+445)
#define EFX_EFFECT_SLIDE4_RT_RB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+446)
#define EFX_EFFECT_SLIDE4_RT_RB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+447)
#define EFX_EFFECT_SLIDE4_RT_RB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+448)
#define EFX_EFFECT_SLIDE4_RT_RB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+449)
#define EFX_EFFECT_SLIDE4_RT_RB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+450)
#define EFX_EFFECT_SLIDE4_RT_RB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+451)
#define EFX_EFFECT_SLIDE4_RT_RB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+452)
#define EFX_EFFECT_SLIDE4_RT_RB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+453)
#define EFX_EFFECT_SLIDE4_RT_RB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+454)
#define EFX_EFFECT_SLIDE4_RT_RB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+455)
#define EFX_EFFECT_SLIDE4_RT_RB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+456)
#define EFX_EFFECT_SLIDE4_RT_RB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+457)
#define EFX_EFFECT_SLIDE4_RT_RB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+458)
#define EFX_EFFECT_SLIDE4_RT_RB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+459)
#define EFX_EFFECT_SLIDE4_RT_RB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+460)
#define EFX_EFFECT_SLIDE4_RT_RB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+461)
#define EFX_EFFECT_SLIDE4_RB_LT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+462)
#define EFX_EFFECT_SLIDE4_RB_LT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+463)
#define EFX_EFFECT_SLIDE4_RB_LT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+464)
#define EFX_EFFECT_SLIDE4_RB_LT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+465)
#define EFX_EFFECT_SLIDE4_RB_LT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+466)
#define EFX_EFFECT_SLIDE4_RB_LT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+467)
#define EFX_EFFECT_SLIDE4_RB_LT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+468)
#define EFX_EFFECT_SLIDE4_RB_LT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+469)
#define EFX_EFFECT_SLIDE4_RB_LT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+470)
#define EFX_EFFECT_SLIDE4_RB_LT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+471)
#define EFX_EFFECT_SLIDE4_RB_LT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+472)
#define EFX_EFFECT_SLIDE4_RB_LT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+473)
#define EFX_EFFECT_SLIDE4_RB_LT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+474)
#define EFX_EFFECT_SLIDE4_RB_LT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+475)
#define EFX_EFFECT_SLIDE4_RB_LT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+476)
#define EFX_EFFECT_SLIDE4_RB_LT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+477)
#define EFX_EFFECT_SLIDE4_RB_LB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+478)
#define EFX_EFFECT_SLIDE4_RB_LB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+479)
#define EFX_EFFECT_SLIDE4_RB_LB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+480)
#define EFX_EFFECT_SLIDE4_RB_LB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+481)
#define EFX_EFFECT_SLIDE4_RB_LB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+482)
#define EFX_EFFECT_SLIDE4_RB_LB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+483)
#define EFX_EFFECT_SLIDE4_RB_LB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+484)
#define EFX_EFFECT_SLIDE4_RB_LB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+485)
#define EFX_EFFECT_SLIDE4_RB_LB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+486)
#define EFX_EFFECT_SLIDE4_RB_LB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+487)
#define EFX_EFFECT_SLIDE4_RB_LB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+488)
#define EFX_EFFECT_SLIDE4_RB_LB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+489)
#define EFX_EFFECT_SLIDE4_RB_LB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+490)
#define EFX_EFFECT_SLIDE4_RB_LB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+491)
#define EFX_EFFECT_SLIDE4_RB_LB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+492)
#define EFX_EFFECT_SLIDE4_RB_LB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+493)
#define EFX_EFFECT_SLIDE4_RB_RT_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+494)
#define EFX_EFFECT_SLIDE4_RB_RT_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+495)
#define EFX_EFFECT_SLIDE4_RB_RT_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+496)
#define EFX_EFFECT_SLIDE4_RB_RT_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+497)
#define EFX_EFFECT_SLIDE4_RB_RT_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+498)
#define EFX_EFFECT_SLIDE4_RB_RT_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+499)
#define EFX_EFFECT_SLIDE4_RB_RT_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+500)
#define EFX_EFFECT_SLIDE4_RB_RT_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+501)
#define EFX_EFFECT_SLIDE4_RB_RT_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+502)
#define EFX_EFFECT_SLIDE4_RB_RT_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+503)
#define EFX_EFFECT_SLIDE4_RB_RT_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+504)
#define EFX_EFFECT_SLIDE4_RB_RT_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+505)
#define EFX_EFFECT_SLIDE4_RB_RT_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+506)
#define EFX_EFFECT_SLIDE4_RB_RT_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+507)
#define EFX_EFFECT_SLIDE4_RB_RT_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+508)
#define EFX_EFFECT_SLIDE4_RB_RT_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+509)
#define EFX_EFFECT_SLIDE4_RB_RB_LT_LT                         (EFX_EFFECT_SLIDE_CLASS+510)
#define EFX_EFFECT_SLIDE4_RB_RB_LT_LB                         (EFX_EFFECT_SLIDE_CLASS+511)
#define EFX_EFFECT_SLIDE4_RB_RB_LT_RT                         (EFX_EFFECT_SLIDE_CLASS+512)
#define EFX_EFFECT_SLIDE4_RB_RB_LT_RB                         (EFX_EFFECT_SLIDE_CLASS+513)
#define EFX_EFFECT_SLIDE4_RB_RB_LB_LT                         (EFX_EFFECT_SLIDE_CLASS+514)
#define EFX_EFFECT_SLIDE4_RB_RB_LB_LB                         (EFX_EFFECT_SLIDE_CLASS+515)
#define EFX_EFFECT_SLIDE4_RB_RB_LB_RT                         (EFX_EFFECT_SLIDE_CLASS+516)
#define EFX_EFFECT_SLIDE4_RB_RB_LB_RB                         (EFX_EFFECT_SLIDE_CLASS+517)
#define EFX_EFFECT_SLIDE4_RB_RB_RT_LT                         (EFX_EFFECT_SLIDE_CLASS+518)
#define EFX_EFFECT_SLIDE4_RB_RB_RT_LB                         (EFX_EFFECT_SLIDE_CLASS+519)
#define EFX_EFFECT_SLIDE4_RB_RB_RT_RT                         (EFX_EFFECT_SLIDE_CLASS+520)
#define EFX_EFFECT_SLIDE4_RB_RB_RT_RB                         (EFX_EFFECT_SLIDE_CLASS+521)
#define EFX_EFFECT_SLIDE4_RB_RB_RB_LT                         (EFX_EFFECT_SLIDE_CLASS+522)
#define EFX_EFFECT_SLIDE4_RB_RB_RB_LB                         (EFX_EFFECT_SLIDE_CLASS+523)
#define EFX_EFFECT_SLIDE4_RB_RB_RB_RT                         (EFX_EFFECT_SLIDE_CLASS+524)
#define EFX_EFFECT_SLIDE4_RB_RB_RB_RB                         (EFX_EFFECT_SLIDE_CLASS+525)
#define EFX_EFFECT_SLIDE_MAX                                  (EFX_EFFECT_SLIDE_CLASS+525)

// Roll
#define EFX_EFFECT_ROLL_CLASS                                 7000
#define EFX_EFFECT_ROLL_L_TO_R                                (EFX_EFFECT_ROLL_CLASS+0)
#define EFX_EFFECT_ROLL_R_TO_L                                (EFX_EFFECT_ROLL_CLASS+1)
#define EFX_EFFECT_ROLL_T_TO_B                                (EFX_EFFECT_ROLL_CLASS+2)
#define EFX_EFFECT_ROLL_B_TO_T                                (EFX_EFFECT_ROLL_CLASS+3)
#define EFX_EFFECT_ROLL2_T_R_TO_L_AND_B_L_TO_R                (EFX_EFFECT_ROLL_CLASS+4)
#define EFX_EFFECT_ROLL2_T_L_TO_R_AND_B_R_TO_L                (EFX_EFFECT_ROLL_CLASS+5)
#define EFX_EFFECT_ROLL2_L_B_TO_T_AND_R_T_TO_B                (EFX_EFFECT_ROLL_CLASS+6)
#define EFX_EFFECT_ROLL2_L_T_TO_B_AND_R_B_TO_T                (EFX_EFFECT_ROLL_CLASS+7)
#define EFX_EFFECT_ROLL4_L_L_L_L                              (EFX_EFFECT_ROLL_CLASS+8)
#define EFX_EFFECT_ROLL4_L_L_L_R                              (EFX_EFFECT_ROLL_CLASS+9)
#define EFX_EFFECT_ROLL4_L_L_L_T                              (EFX_EFFECT_ROLL_CLASS+10)
#define EFX_EFFECT_ROLL4_L_L_L_B                              (EFX_EFFECT_ROLL_CLASS+11)
#define EFX_EFFECT_ROLL4_L_L_R_L                              (EFX_EFFECT_ROLL_CLASS+12)
#define EFX_EFFECT_ROLL4_L_L_R_R                              (EFX_EFFECT_ROLL_CLASS+13)
#define EFX_EFFECT_ROLL4_L_L_R_T                              (EFX_EFFECT_ROLL_CLASS+14)
#define EFX_EFFECT_ROLL4_L_L_R_B                              (EFX_EFFECT_ROLL_CLASS+15)
#define EFX_EFFECT_ROLL4_L_L_T_L                              (EFX_EFFECT_ROLL_CLASS+16)
#define EFX_EFFECT_ROLL4_L_L_T_R                              (EFX_EFFECT_ROLL_CLASS+17)
#define EFX_EFFECT_ROLL4_L_L_T_T                              (EFX_EFFECT_ROLL_CLASS+18)
#define EFX_EFFECT_ROLL4_L_L_T_B                              (EFX_EFFECT_ROLL_CLASS+19)
#define EFX_EFFECT_ROLL4_L_L_B_L                              (EFX_EFFECT_ROLL_CLASS+20)
#define EFX_EFFECT_ROLL4_L_L_B_R                              (EFX_EFFECT_ROLL_CLASS+21)
#define EFX_EFFECT_ROLL4_L_L_B_T                              (EFX_EFFECT_ROLL_CLASS+22)
#define EFX_EFFECT_ROLL4_L_L_B_B                              (EFX_EFFECT_ROLL_CLASS+23)
#define EFX_EFFECT_ROLL4_L_R_L_L                              (EFX_EFFECT_ROLL_CLASS+24)
#define EFX_EFFECT_ROLL4_L_R_L_R                              (EFX_EFFECT_ROLL_CLASS+25)
#define EFX_EFFECT_ROLL4_L_R_L_T                              (EFX_EFFECT_ROLL_CLASS+26)
#define EFX_EFFECT_ROLL4_L_R_L_B                              (EFX_EFFECT_ROLL_CLASS+27)
#define EFX_EFFECT_ROLL4_L_R_R_L                              (EFX_EFFECT_ROLL_CLASS+28)
#define EFX_EFFECT_ROLL4_L_R_R_R                              (EFX_EFFECT_ROLL_CLASS+29)
#define EFX_EFFECT_ROLL4_L_R_R_T                              (EFX_EFFECT_ROLL_CLASS+30)
#define EFX_EFFECT_ROLL4_L_R_R_B                              (EFX_EFFECT_ROLL_CLASS+31)
#define EFX_EFFECT_ROLL4_L_R_T_L                              (EFX_EFFECT_ROLL_CLASS+32)
#define EFX_EFFECT_ROLL4_L_R_T_R                              (EFX_EFFECT_ROLL_CLASS+33)
#define EFX_EFFECT_ROLL4_L_R_T_T                              (EFX_EFFECT_ROLL_CLASS+34)
#define EFX_EFFECT_ROLL4_L_R_T_B                              (EFX_EFFECT_ROLL_CLASS+35)
#define EFX_EFFECT_ROLL4_L_R_B_L                              (EFX_EFFECT_ROLL_CLASS+36)
#define EFX_EFFECT_ROLL4_L_R_B_R                              (EFX_EFFECT_ROLL_CLASS+37)
#define EFX_EFFECT_ROLL4_L_R_B_T                              (EFX_EFFECT_ROLL_CLASS+38)
#define EFX_EFFECT_ROLL4_L_R_B_B                              (EFX_EFFECT_ROLL_CLASS+39)
#define EFX_EFFECT_ROLL4_L_T_L_L                              (EFX_EFFECT_ROLL_CLASS+40)
#define EFX_EFFECT_ROLL4_L_T_L_R                              (EFX_EFFECT_ROLL_CLASS+41)
#define EFX_EFFECT_ROLL4_L_T_L_T                              (EFX_EFFECT_ROLL_CLASS+42)
#define EFX_EFFECT_ROLL4_L_T_L_B                              (EFX_EFFECT_ROLL_CLASS+43)
#define EFX_EFFECT_ROLL4_L_T_R_L                              (EFX_EFFECT_ROLL_CLASS+44)
#define EFX_EFFECT_ROLL4_L_T_R_R                              (EFX_EFFECT_ROLL_CLASS+45)
#define EFX_EFFECT_ROLL4_L_T_R_T                              (EFX_EFFECT_ROLL_CLASS+46)
#define EFX_EFFECT_ROLL4_L_T_R_B                              (EFX_EFFECT_ROLL_CLASS+47)
#define EFX_EFFECT_ROLL4_L_T_T_L                              (EFX_EFFECT_ROLL_CLASS+48)
#define EFX_EFFECT_ROLL4_L_T_T_R                              (EFX_EFFECT_ROLL_CLASS+49)
#define EFX_EFFECT_ROLL4_L_T_T_T                              (EFX_EFFECT_ROLL_CLASS+50)
#define EFX_EFFECT_ROLL4_L_T_T_B                              (EFX_EFFECT_ROLL_CLASS+51)
#define EFX_EFFECT_ROLL4_L_T_B_L                              (EFX_EFFECT_ROLL_CLASS+52)
#define EFX_EFFECT_ROLL4_L_T_B_R                              (EFX_EFFECT_ROLL_CLASS+53)
#define EFX_EFFECT_ROLL4_L_T_B_T                              (EFX_EFFECT_ROLL_CLASS+54)
#define EFX_EFFECT_ROLL4_L_T_B_B                              (EFX_EFFECT_ROLL_CLASS+55)
#define EFX_EFFECT_ROLL4_L_B_L_L                              (EFX_EFFECT_ROLL_CLASS+56)
#define EFX_EFFECT_ROLL4_L_B_L_R                              (EFX_EFFECT_ROLL_CLASS+57)
#define EFX_EFFECT_ROLL4_L_B_L_T                              (EFX_EFFECT_ROLL_CLASS+58)
#define EFX_EFFECT_ROLL4_L_B_L_B                              (EFX_EFFECT_ROLL_CLASS+59)
#define EFX_EFFECT_ROLL4_L_B_R_L                              (EFX_EFFECT_ROLL_CLASS+60)
#define EFX_EFFECT_ROLL4_L_B_R_R                              (EFX_EFFECT_ROLL_CLASS+61)
#define EFX_EFFECT_ROLL4_L_B_R_T                              (EFX_EFFECT_ROLL_CLASS+62)
#define EFX_EFFECT_ROLL4_L_B_R_B                              (EFX_EFFECT_ROLL_CLASS+63)
#define EFX_EFFECT_ROLL4_L_B_T_L                              (EFX_EFFECT_ROLL_CLASS+64)
#define EFX_EFFECT_ROLL4_L_B_T_R                              (EFX_EFFECT_ROLL_CLASS+65)
#define EFX_EFFECT_ROLL4_L_B_T_T                              (EFX_EFFECT_ROLL_CLASS+66)
#define EFX_EFFECT_ROLL4_L_B_T_B                              (EFX_EFFECT_ROLL_CLASS+67)
#define EFX_EFFECT_ROLL4_L_B_B_L                              (EFX_EFFECT_ROLL_CLASS+68)
#define EFX_EFFECT_ROLL4_L_B_B_R                              (EFX_EFFECT_ROLL_CLASS+69)
#define EFX_EFFECT_ROLL4_L_B_B_T                              (EFX_EFFECT_ROLL_CLASS+70)
#define EFX_EFFECT_ROLL4_L_B_B_B                              (EFX_EFFECT_ROLL_CLASS+71)
#define EFX_EFFECT_ROLL4_R_L_L_L                              (EFX_EFFECT_ROLL_CLASS+72)
#define EFX_EFFECT_ROLL4_R_L_L_R                              (EFX_EFFECT_ROLL_CLASS+73)
#define EFX_EFFECT_ROLL4_R_L_L_T                              (EFX_EFFECT_ROLL_CLASS+74)
#define EFX_EFFECT_ROLL4_R_L_L_B                              (EFX_EFFECT_ROLL_CLASS+75)
#define EFX_EFFECT_ROLL4_R_L_R_L                              (EFX_EFFECT_ROLL_CLASS+76)
#define EFX_EFFECT_ROLL4_R_L_R_R                              (EFX_EFFECT_ROLL_CLASS+77)
#define EFX_EFFECT_ROLL4_R_L_R_T                              (EFX_EFFECT_ROLL_CLASS+78)
#define EFX_EFFECT_ROLL4_R_L_R_B                              (EFX_EFFECT_ROLL_CLASS+79)
#define EFX_EFFECT_ROLL4_R_L_T_L                              (EFX_EFFECT_ROLL_CLASS+80)
#define EFX_EFFECT_ROLL4_R_L_T_R                              (EFX_EFFECT_ROLL_CLASS+81)
#define EFX_EFFECT_ROLL4_R_L_T_T                              (EFX_EFFECT_ROLL_CLASS+82)
#define EFX_EFFECT_ROLL4_R_L_T_B                              (EFX_EFFECT_ROLL_CLASS+83)
#define EFX_EFFECT_ROLL4_R_L_B_L                              (EFX_EFFECT_ROLL_CLASS+84)
#define EFX_EFFECT_ROLL4_R_L_B_R                              (EFX_EFFECT_ROLL_CLASS+85)
#define EFX_EFFECT_ROLL4_R_L_B_T                              (EFX_EFFECT_ROLL_CLASS+86)
#define EFX_EFFECT_ROLL4_R_L_B_B                              (EFX_EFFECT_ROLL_CLASS+87)
#define EFX_EFFECT_ROLL4_R_R_L_L                              (EFX_EFFECT_ROLL_CLASS+88)
#define EFX_EFFECT_ROLL4_R_R_L_R                              (EFX_EFFECT_ROLL_CLASS+89)
#define EFX_EFFECT_ROLL4_R_R_L_T                              (EFX_EFFECT_ROLL_CLASS+90)
#define EFX_EFFECT_ROLL4_R_R_L_B                              (EFX_EFFECT_ROLL_CLASS+91)
#define EFX_EFFECT_ROLL4_R_R_R_L                              (EFX_EFFECT_ROLL_CLASS+92)
#define EFX_EFFECT_ROLL4_R_R_R_R                              (EFX_EFFECT_ROLL_CLASS+93)
#define EFX_EFFECT_ROLL4_R_R_R_T                              (EFX_EFFECT_ROLL_CLASS+94)
#define EFX_EFFECT_ROLL4_R_R_R_B                              (EFX_EFFECT_ROLL_CLASS+95)
#define EFX_EFFECT_ROLL4_R_R_T_L                              (EFX_EFFECT_ROLL_CLASS+96)
#define EFX_EFFECT_ROLL4_R_R_T_R                              (EFX_EFFECT_ROLL_CLASS+97)
#define EFX_EFFECT_ROLL4_R_R_T_T                              (EFX_EFFECT_ROLL_CLASS+98)
#define EFX_EFFECT_ROLL4_R_R_T_B                              (EFX_EFFECT_ROLL_CLASS+99)
#define EFX_EFFECT_ROLL4_R_R_B_L                              (EFX_EFFECT_ROLL_CLASS+100)
#define EFX_EFFECT_ROLL4_R_R_B_R                              (EFX_EFFECT_ROLL_CLASS+101)
#define EFX_EFFECT_ROLL4_R_R_B_T                              (EFX_EFFECT_ROLL_CLASS+102)
#define EFX_EFFECT_ROLL4_R_R_B_B                              (EFX_EFFECT_ROLL_CLASS+103)
#define EFX_EFFECT_ROLL4_R_T_L_L                              (EFX_EFFECT_ROLL_CLASS+104)
#define EFX_EFFECT_ROLL4_R_T_L_R                              (EFX_EFFECT_ROLL_CLASS+105)
#define EFX_EFFECT_ROLL4_R_T_L_T                              (EFX_EFFECT_ROLL_CLASS+106)
#define EFX_EFFECT_ROLL4_R_T_L_B                              (EFX_EFFECT_ROLL_CLASS+107)
#define EFX_EFFECT_ROLL4_R_T_R_L                              (EFX_EFFECT_ROLL_CLASS+108)
#define EFX_EFFECT_ROLL4_R_T_R_R                              (EFX_EFFECT_ROLL_CLASS+109)
#define EFX_EFFECT_ROLL4_R_T_R_T                              (EFX_EFFECT_ROLL_CLASS+110)
#define EFX_EFFECT_ROLL4_R_T_R_B                              (EFX_EFFECT_ROLL_CLASS+111)
#define EFX_EFFECT_ROLL4_R_T_T_L                              (EFX_EFFECT_ROLL_CLASS+112)
#define EFX_EFFECT_ROLL4_R_T_T_R                              (EFX_EFFECT_ROLL_CLASS+113)
#define EFX_EFFECT_ROLL4_R_T_T_T                              (EFX_EFFECT_ROLL_CLASS+114)
#define EFX_EFFECT_ROLL4_R_T_T_B                              (EFX_EFFECT_ROLL_CLASS+115)
#define EFX_EFFECT_ROLL4_R_T_B_L                              (EFX_EFFECT_ROLL_CLASS+116)
#define EFX_EFFECT_ROLL4_R_T_B_R                              (EFX_EFFECT_ROLL_CLASS+117)
#define EFX_EFFECT_ROLL4_R_T_B_T                              (EFX_EFFECT_ROLL_CLASS+118)
#define EFX_EFFECT_ROLL4_R_T_B_B                              (EFX_EFFECT_ROLL_CLASS+119)
#define EFX_EFFECT_ROLL4_R_B_L_L                              (EFX_EFFECT_ROLL_CLASS+120)
#define EFX_EFFECT_ROLL4_R_B_L_R                              (EFX_EFFECT_ROLL_CLASS+121)
#define EFX_EFFECT_ROLL4_R_B_L_T                              (EFX_EFFECT_ROLL_CLASS+122)
#define EFX_EFFECT_ROLL4_R_B_L_B                              (EFX_EFFECT_ROLL_CLASS+123)
#define EFX_EFFECT_ROLL4_R_B_R_L                              (EFX_EFFECT_ROLL_CLASS+124)
#define EFX_EFFECT_ROLL4_R_B_R_R                              (EFX_EFFECT_ROLL_CLASS+125)
#define EFX_EFFECT_ROLL4_R_B_R_T                              (EFX_EFFECT_ROLL_CLASS+126)
#define EFX_EFFECT_ROLL4_R_B_R_B                              (EFX_EFFECT_ROLL_CLASS+127)
#define EFX_EFFECT_ROLL4_R_B_T_L                              (EFX_EFFECT_ROLL_CLASS+128)
#define EFX_EFFECT_ROLL4_R_B_T_R                              (EFX_EFFECT_ROLL_CLASS+129)
#define EFX_EFFECT_ROLL4_R_B_T_T                              (EFX_EFFECT_ROLL_CLASS+130)
#define EFX_EFFECT_ROLL4_R_B_T_B                              (EFX_EFFECT_ROLL_CLASS+131)
#define EFX_EFFECT_ROLL4_R_B_B_L                              (EFX_EFFECT_ROLL_CLASS+132)
#define EFX_EFFECT_ROLL4_R_B_B_R                              (EFX_EFFECT_ROLL_CLASS+133)
#define EFX_EFFECT_ROLL4_R_B_B_T                              (EFX_EFFECT_ROLL_CLASS+134)
#define EFX_EFFECT_ROLL4_R_B_B_B                              (EFX_EFFECT_ROLL_CLASS+135)
#define EFX_EFFECT_ROLL4_T_L_L_L                              (EFX_EFFECT_ROLL_CLASS+136)
#define EFX_EFFECT_ROLL4_T_L_L_R                              (EFX_EFFECT_ROLL_CLASS+137)
#define EFX_EFFECT_ROLL4_T_L_L_T                              (EFX_EFFECT_ROLL_CLASS+138)
#define EFX_EFFECT_ROLL4_T_L_L_B                              (EFX_EFFECT_ROLL_CLASS+139)
#define EFX_EFFECT_ROLL4_T_L_R_L                              (EFX_EFFECT_ROLL_CLASS+140)
#define EFX_EFFECT_ROLL4_T_L_R_R                              (EFX_EFFECT_ROLL_CLASS+141)
#define EFX_EFFECT_ROLL4_T_L_R_T                              (EFX_EFFECT_ROLL_CLASS+142)
#define EFX_EFFECT_ROLL4_T_L_R_B                              (EFX_EFFECT_ROLL_CLASS+143)
#define EFX_EFFECT_ROLL4_T_L_T_L                              (EFX_EFFECT_ROLL_CLASS+144)
#define EFX_EFFECT_ROLL4_T_L_T_R                              (EFX_EFFECT_ROLL_CLASS+145)
#define EFX_EFFECT_ROLL4_T_L_T_T                              (EFX_EFFECT_ROLL_CLASS+146)
#define EFX_EFFECT_ROLL4_T_L_T_B                              (EFX_EFFECT_ROLL_CLASS+147)
#define EFX_EFFECT_ROLL4_T_L_B_L                              (EFX_EFFECT_ROLL_CLASS+148)
#define EFX_EFFECT_ROLL4_T_L_B_R                              (EFX_EFFECT_ROLL_CLASS+149)
#define EFX_EFFECT_ROLL4_T_L_B_T                              (EFX_EFFECT_ROLL_CLASS+150)
#define EFX_EFFECT_ROLL4_T_L_B_B                              (EFX_EFFECT_ROLL_CLASS+151)
#define EFX_EFFECT_ROLL4_T_R_L_L                              (EFX_EFFECT_ROLL_CLASS+152)
#define EFX_EFFECT_ROLL4_T_R_L_R                              (EFX_EFFECT_ROLL_CLASS+153)
#define EFX_EFFECT_ROLL4_T_R_L_T                              (EFX_EFFECT_ROLL_CLASS+154)
#define EFX_EFFECT_ROLL4_T_R_L_B                              (EFX_EFFECT_ROLL_CLASS+155)
#define EFX_EFFECT_ROLL4_T_R_R_L                              (EFX_EFFECT_ROLL_CLASS+156)
#define EFX_EFFECT_ROLL4_T_R_R_R                              (EFX_EFFECT_ROLL_CLASS+157)
#define EFX_EFFECT_ROLL4_T_R_R_T                              (EFX_EFFECT_ROLL_CLASS+158)
#define EFX_EFFECT_ROLL4_T_R_R_B                              (EFX_EFFECT_ROLL_CLASS+159)
#define EFX_EFFECT_ROLL4_T_R_T_L                              (EFX_EFFECT_ROLL_CLASS+160)
#define EFX_EFFECT_ROLL4_T_R_T_R                              (EFX_EFFECT_ROLL_CLASS+161)
#define EFX_EFFECT_ROLL4_T_R_T_T                              (EFX_EFFECT_ROLL_CLASS+162)
#define EFX_EFFECT_ROLL4_T_R_T_B                              (EFX_EFFECT_ROLL_CLASS+163)
#define EFX_EFFECT_ROLL4_T_R_B_L                              (EFX_EFFECT_ROLL_CLASS+164)
#define EFX_EFFECT_ROLL4_T_R_B_R                              (EFX_EFFECT_ROLL_CLASS+165)
#define EFX_EFFECT_ROLL4_T_R_B_T                              (EFX_EFFECT_ROLL_CLASS+166)
#define EFX_EFFECT_ROLL4_T_R_B_B                              (EFX_EFFECT_ROLL_CLASS+167)
#define EFX_EFFECT_ROLL4_T_T_L_L                              (EFX_EFFECT_ROLL_CLASS+168)
#define EFX_EFFECT_ROLL4_T_T_L_R                              (EFX_EFFECT_ROLL_CLASS+169)
#define EFX_EFFECT_ROLL4_T_T_L_T                              (EFX_EFFECT_ROLL_CLASS+170)
#define EFX_EFFECT_ROLL4_T_T_L_B                              (EFX_EFFECT_ROLL_CLASS+171)
#define EFX_EFFECT_ROLL4_T_T_R_L                              (EFX_EFFECT_ROLL_CLASS+172)
#define EFX_EFFECT_ROLL4_T_T_R_R                              (EFX_EFFECT_ROLL_CLASS+173)
#define EFX_EFFECT_ROLL4_T_T_R_T                              (EFX_EFFECT_ROLL_CLASS+174)
#define EFX_EFFECT_ROLL4_T_T_R_B                              (EFX_EFFECT_ROLL_CLASS+175)
#define EFX_EFFECT_ROLL4_T_T_T_L                              (EFX_EFFECT_ROLL_CLASS+176)
#define EFX_EFFECT_ROLL4_T_T_T_R                              (EFX_EFFECT_ROLL_CLASS+177)
#define EFX_EFFECT_ROLL4_T_T_T_T                              (EFX_EFFECT_ROLL_CLASS+178)
#define EFX_EFFECT_ROLL4_T_T_T_B                              (EFX_EFFECT_ROLL_CLASS+179)
#define EFX_EFFECT_ROLL4_T_T_B_L                              (EFX_EFFECT_ROLL_CLASS+180)
#define EFX_EFFECT_ROLL4_T_T_B_R                              (EFX_EFFECT_ROLL_CLASS+181)
#define EFX_EFFECT_ROLL4_T_T_B_T                              (EFX_EFFECT_ROLL_CLASS+182)
#define EFX_EFFECT_ROLL4_T_T_B_B                              (EFX_EFFECT_ROLL_CLASS+183)
#define EFX_EFFECT_ROLL4_T_B_L_L                              (EFX_EFFECT_ROLL_CLASS+184)
#define EFX_EFFECT_ROLL4_T_B_L_R                              (EFX_EFFECT_ROLL_CLASS+185)
#define EFX_EFFECT_ROLL4_T_B_L_T                              (EFX_EFFECT_ROLL_CLASS+186)
#define EFX_EFFECT_ROLL4_T_B_L_B                              (EFX_EFFECT_ROLL_CLASS+187)
#define EFX_EFFECT_ROLL4_T_B_R_L                              (EFX_EFFECT_ROLL_CLASS+188)
#define EFX_EFFECT_ROLL4_T_B_R_R                              (EFX_EFFECT_ROLL_CLASS+189)
#define EFX_EFFECT_ROLL4_T_B_R_T                              (EFX_EFFECT_ROLL_CLASS+190)
#define EFX_EFFECT_ROLL4_T_B_R_B                              (EFX_EFFECT_ROLL_CLASS+191)
#define EFX_EFFECT_ROLL4_T_B_T_L                              (EFX_EFFECT_ROLL_CLASS+192)
#define EFX_EFFECT_ROLL4_T_B_T_R                              (EFX_EFFECT_ROLL_CLASS+193)
#define EFX_EFFECT_ROLL4_T_B_T_T                              (EFX_EFFECT_ROLL_CLASS+194)
#define EFX_EFFECT_ROLL4_T_B_T_B                              (EFX_EFFECT_ROLL_CLASS+195)
#define EFX_EFFECT_ROLL4_T_B_B_L                              (EFX_EFFECT_ROLL_CLASS+196)
#define EFX_EFFECT_ROLL4_T_B_B_R                              (EFX_EFFECT_ROLL_CLASS+197)
#define EFX_EFFECT_ROLL4_T_B_B_T                              (EFX_EFFECT_ROLL_CLASS+198)
#define EFX_EFFECT_ROLL4_T_B_B_B                              (EFX_EFFECT_ROLL_CLASS+199)
#define EFX_EFFECT_ROLL4_B_L_L_L                              (EFX_EFFECT_ROLL_CLASS+200)
#define EFX_EFFECT_ROLL4_B_L_L_R                              (EFX_EFFECT_ROLL_CLASS+201)
#define EFX_EFFECT_ROLL4_B_L_L_T                              (EFX_EFFECT_ROLL_CLASS+202)
#define EFX_EFFECT_ROLL4_B_L_L_B                              (EFX_EFFECT_ROLL_CLASS+203)
#define EFX_EFFECT_ROLL4_B_L_R_L                              (EFX_EFFECT_ROLL_CLASS+204)
#define EFX_EFFECT_ROLL4_B_L_R_R                              (EFX_EFFECT_ROLL_CLASS+205)
#define EFX_EFFECT_ROLL4_B_L_R_T                              (EFX_EFFECT_ROLL_CLASS+206)
#define EFX_EFFECT_ROLL4_B_L_R_B                              (EFX_EFFECT_ROLL_CLASS+207)
#define EFX_EFFECT_ROLL4_B_L_T_L                              (EFX_EFFECT_ROLL_CLASS+208)
#define EFX_EFFECT_ROLL4_B_L_T_R                              (EFX_EFFECT_ROLL_CLASS+209)
#define EFX_EFFECT_ROLL4_B_L_T_T                              (EFX_EFFECT_ROLL_CLASS+210)
#define EFX_EFFECT_ROLL4_B_L_T_B                              (EFX_EFFECT_ROLL_CLASS+211)
#define EFX_EFFECT_ROLL4_B_L_B_L                              (EFX_EFFECT_ROLL_CLASS+212)
#define EFX_EFFECT_ROLL4_B_L_B_R                              (EFX_EFFECT_ROLL_CLASS+213)
#define EFX_EFFECT_ROLL4_B_L_B_T                              (EFX_EFFECT_ROLL_CLASS+214)
#define EFX_EFFECT_ROLL4_B_L_B_B                              (EFX_EFFECT_ROLL_CLASS+215)
#define EFX_EFFECT_ROLL4_B_R_L_L                              (EFX_EFFECT_ROLL_CLASS+216)
#define EFX_EFFECT_ROLL4_B_R_L_R                              (EFX_EFFECT_ROLL_CLASS+217)
#define EFX_EFFECT_ROLL4_B_R_L_T                              (EFX_EFFECT_ROLL_CLASS+218)
#define EFX_EFFECT_ROLL4_B_R_L_B                              (EFX_EFFECT_ROLL_CLASS+219)
#define EFX_EFFECT_ROLL4_B_R_R_L                              (EFX_EFFECT_ROLL_CLASS+220)
#define EFX_EFFECT_ROLL4_B_R_R_R                              (EFX_EFFECT_ROLL_CLASS+221)
#define EFX_EFFECT_ROLL4_B_R_R_T                              (EFX_EFFECT_ROLL_CLASS+222)
#define EFX_EFFECT_ROLL4_B_R_R_B                              (EFX_EFFECT_ROLL_CLASS+223)
#define EFX_EFFECT_ROLL4_B_R_T_L                              (EFX_EFFECT_ROLL_CLASS+224)
#define EFX_EFFECT_ROLL4_B_R_T_R                              (EFX_EFFECT_ROLL_CLASS+225)
#define EFX_EFFECT_ROLL4_B_R_T_T                              (EFX_EFFECT_ROLL_CLASS+226)
#define EFX_EFFECT_ROLL4_B_R_T_B                              (EFX_EFFECT_ROLL_CLASS+227)
#define EFX_EFFECT_ROLL4_B_R_B_L                              (EFX_EFFECT_ROLL_CLASS+228)
#define EFX_EFFECT_ROLL4_B_R_B_R                              (EFX_EFFECT_ROLL_CLASS+229)
#define EFX_EFFECT_ROLL4_B_R_B_T                              (EFX_EFFECT_ROLL_CLASS+230)
#define EFX_EFFECT_ROLL4_B_R_B_B                              (EFX_EFFECT_ROLL_CLASS+231)
#define EFX_EFFECT_ROLL4_B_T_L_L                              (EFX_EFFECT_ROLL_CLASS+232)
#define EFX_EFFECT_ROLL4_B_T_L_R                              (EFX_EFFECT_ROLL_CLASS+233)
#define EFX_EFFECT_ROLL4_B_T_L_T                              (EFX_EFFECT_ROLL_CLASS+234)
#define EFX_EFFECT_ROLL4_B_T_L_B                              (EFX_EFFECT_ROLL_CLASS+235)
#define EFX_EFFECT_ROLL4_B_T_R_L                              (EFX_EFFECT_ROLL_CLASS+236)
#define EFX_EFFECT_ROLL4_B_T_R_R                              (EFX_EFFECT_ROLL_CLASS+237)
#define EFX_EFFECT_ROLL4_B_T_R_T                              (EFX_EFFECT_ROLL_CLASS+238)
#define EFX_EFFECT_ROLL4_B_T_R_B                              (EFX_EFFECT_ROLL_CLASS+239)
#define EFX_EFFECT_ROLL4_B_T_T_L                              (EFX_EFFECT_ROLL_CLASS+240)
#define EFX_EFFECT_ROLL4_B_T_T_R                              (EFX_EFFECT_ROLL_CLASS+241)
#define EFX_EFFECT_ROLL4_B_T_T_T                              (EFX_EFFECT_ROLL_CLASS+242)
#define EFX_EFFECT_ROLL4_B_T_T_B                              (EFX_EFFECT_ROLL_CLASS+243)
#define EFX_EFFECT_ROLL4_B_T_B_L                              (EFX_EFFECT_ROLL_CLASS+244)
#define EFX_EFFECT_ROLL4_B_T_B_R                              (EFX_EFFECT_ROLL_CLASS+245)
#define EFX_EFFECT_ROLL4_B_T_B_T                              (EFX_EFFECT_ROLL_CLASS+246)
#define EFX_EFFECT_ROLL4_B_T_B_B                              (EFX_EFFECT_ROLL_CLASS+247)
#define EFX_EFFECT_ROLL4_B_B_L_L                              (EFX_EFFECT_ROLL_CLASS+248)
#define EFX_EFFECT_ROLL4_B_B_L_R                              (EFX_EFFECT_ROLL_CLASS+249)
#define EFX_EFFECT_ROLL4_B_B_L_T                              (EFX_EFFECT_ROLL_CLASS+250)
#define EFX_EFFECT_ROLL4_B_B_L_B                              (EFX_EFFECT_ROLL_CLASS+251)
#define EFX_EFFECT_ROLL4_B_B_R_L                              (EFX_EFFECT_ROLL_CLASS+252)
#define EFX_EFFECT_ROLL4_B_B_R_R                              (EFX_EFFECT_ROLL_CLASS+253)
#define EFX_EFFECT_ROLL4_B_B_R_T                              (EFX_EFFECT_ROLL_CLASS+254)
#define EFX_EFFECT_ROLL4_B_B_R_B                              (EFX_EFFECT_ROLL_CLASS+255)
#define EFX_EFFECT_ROLL4_B_B_T_L                              (EFX_EFFECT_ROLL_CLASS+256)
#define EFX_EFFECT_ROLL4_B_B_T_R                              (EFX_EFFECT_ROLL_CLASS+257)
#define EFX_EFFECT_ROLL4_B_B_T_T                              (EFX_EFFECT_ROLL_CLASS+258)
#define EFX_EFFECT_ROLL4_B_B_T_B                              (EFX_EFFECT_ROLL_CLASS+259)
#define EFX_EFFECT_ROLL4_B_B_B_L                              (EFX_EFFECT_ROLL_CLASS+260)
#define EFX_EFFECT_ROLL4_B_B_B_R                              (EFX_EFFECT_ROLL_CLASS+261)
#define EFX_EFFECT_ROLL4_B_B_B_T                              (EFX_EFFECT_ROLL_CLASS+262)
#define EFX_EFFECT_ROLL4_B_B_B_B                              (EFX_EFFECT_ROLL_CLASS+263)
#define EFX_EFFECT_ROLL_MAX                                   (EFX_EFFECT_ROLL_CLASS+263)

// Rotate
#define EFX_EFFECT_ROTATE_CLASS                               8000
#define EFX_EFFECT_ROTATE_L_TO_R                              (EFX_EFFECT_ROTATE_CLASS+0)
#define EFX_EFFECT_ROTATE_R_TO_L                              (EFX_EFFECT_ROTATE_CLASS+1)
#define EFX_EFFECT_ROTATE_T_TO_B                              (EFX_EFFECT_ROTATE_CLASS+2)
#define EFX_EFFECT_ROTATE_B_TO_T                              (EFX_EFFECT_ROTATE_CLASS+3)
#define EFX_EFFECT_ROTATE4_L_L_L_L                            (EFX_EFFECT_ROTATE_CLASS+4)
#define EFX_EFFECT_ROTATE4_L_L_L_R                            (EFX_EFFECT_ROTATE_CLASS+5)
#define EFX_EFFECT_ROTATE4_L_L_L_T                            (EFX_EFFECT_ROTATE_CLASS+6)
#define EFX_EFFECT_ROTATE4_L_L_L_B                            (EFX_EFFECT_ROTATE_CLASS+7)
#define EFX_EFFECT_ROTATE4_L_L_R_L                            (EFX_EFFECT_ROTATE_CLASS+8)
#define EFX_EFFECT_ROTATE4_L_L_R_R                            (EFX_EFFECT_ROTATE_CLASS+9)
#define EFX_EFFECT_ROTATE4_L_L_R_T                            (EFX_EFFECT_ROTATE_CLASS+10)
#define EFX_EFFECT_ROTATE4_L_L_R_B                            (EFX_EFFECT_ROTATE_CLASS+11)
#define EFX_EFFECT_ROTATE4_L_L_T_L                            (EFX_EFFECT_ROTATE_CLASS+12)
#define EFX_EFFECT_ROTATE4_L_L_T_R                            (EFX_EFFECT_ROTATE_CLASS+13)
#define EFX_EFFECT_ROTATE4_L_L_T_T                            (EFX_EFFECT_ROTATE_CLASS+14)
#define EFX_EFFECT_ROTATE4_L_L_T_B                            (EFX_EFFECT_ROTATE_CLASS+15)
#define EFX_EFFECT_ROTATE4_L_L_B_L                            (EFX_EFFECT_ROTATE_CLASS+16)
#define EFX_EFFECT_ROTATE4_L_L_B_R                            (EFX_EFFECT_ROTATE_CLASS+17)
#define EFX_EFFECT_ROTATE4_L_L_B_T                            (EFX_EFFECT_ROTATE_CLASS+18)
#define EFX_EFFECT_ROTATE4_L_L_B_B                            (EFX_EFFECT_ROTATE_CLASS+19)
#define EFX_EFFECT_ROTATE4_L_R_L_L                            (EFX_EFFECT_ROTATE_CLASS+20)
#define EFX_EFFECT_ROTATE4_L_R_L_R                            (EFX_EFFECT_ROTATE_CLASS+21)
#define EFX_EFFECT_ROTATE4_L_R_L_T                            (EFX_EFFECT_ROTATE_CLASS+22)
#define EFX_EFFECT_ROTATE4_L_R_L_B                            (EFX_EFFECT_ROTATE_CLASS+23)
#define EFX_EFFECT_ROTATE4_L_R_R_L                            (EFX_EFFECT_ROTATE_CLASS+24)
#define EFX_EFFECT_ROTATE4_L_R_R_R                            (EFX_EFFECT_ROTATE_CLASS+25)
#define EFX_EFFECT_ROTATE4_L_R_R_T                            (EFX_EFFECT_ROTATE_CLASS+26)
#define EFX_EFFECT_ROTATE4_L_R_R_B                            (EFX_EFFECT_ROTATE_CLASS+27)
#define EFX_EFFECT_ROTATE4_L_R_T_L                            (EFX_EFFECT_ROTATE_CLASS+28)
#define EFX_EFFECT_ROTATE4_L_R_T_R                            (EFX_EFFECT_ROTATE_CLASS+29)
#define EFX_EFFECT_ROTATE4_L_R_T_T                            (EFX_EFFECT_ROTATE_CLASS+30)
#define EFX_EFFECT_ROTATE4_L_R_T_B                            (EFX_EFFECT_ROTATE_CLASS+31)
#define EFX_EFFECT_ROTATE4_L_R_B_L                            (EFX_EFFECT_ROTATE_CLASS+32)
#define EFX_EFFECT_ROTATE4_L_R_B_R                            (EFX_EFFECT_ROTATE_CLASS+33)
#define EFX_EFFECT_ROTATE4_L_R_B_T                            (EFX_EFFECT_ROTATE_CLASS+34)
#define EFX_EFFECT_ROTATE4_L_R_B_B                            (EFX_EFFECT_ROTATE_CLASS+35)
#define EFX_EFFECT_ROTATE4_L_T_L_L                            (EFX_EFFECT_ROTATE_CLASS+36)
#define EFX_EFFECT_ROTATE4_L_T_L_R                            (EFX_EFFECT_ROTATE_CLASS+37)
#define EFX_EFFECT_ROTATE4_L_T_L_T                            (EFX_EFFECT_ROTATE_CLASS+38)
#define EFX_EFFECT_ROTATE4_L_T_L_B                            (EFX_EFFECT_ROTATE_CLASS+39)
#define EFX_EFFECT_ROTATE4_L_T_R_L                            (EFX_EFFECT_ROTATE_CLASS+40)
#define EFX_EFFECT_ROTATE4_L_T_R_R                            (EFX_EFFECT_ROTATE_CLASS+41)
#define EFX_EFFECT_ROTATE4_L_T_R_T                            (EFX_EFFECT_ROTATE_CLASS+42)
#define EFX_EFFECT_ROTATE4_L_T_R_B                            (EFX_EFFECT_ROTATE_CLASS+43)
#define EFX_EFFECT_ROTATE4_L_T_T_L                            (EFX_EFFECT_ROTATE_CLASS+44)
#define EFX_EFFECT_ROTATE4_L_T_T_R                            (EFX_EFFECT_ROTATE_CLASS+45)
#define EFX_EFFECT_ROTATE4_L_T_T_T                            (EFX_EFFECT_ROTATE_CLASS+46)
#define EFX_EFFECT_ROTATE4_L_T_T_B                            (EFX_EFFECT_ROTATE_CLASS+47)
#define EFX_EFFECT_ROTATE4_L_T_B_L                            (EFX_EFFECT_ROTATE_CLASS+48)
#define EFX_EFFECT_ROTATE4_L_T_B_R                            (EFX_EFFECT_ROTATE_CLASS+49)
#define EFX_EFFECT_ROTATE4_L_T_B_T                            (EFX_EFFECT_ROTATE_CLASS+50)
#define EFX_EFFECT_ROTATE4_L_T_B_B                            (EFX_EFFECT_ROTATE_CLASS+51)
#define EFX_EFFECT_ROTATE4_L_B_L_L                            (EFX_EFFECT_ROTATE_CLASS+52)
#define EFX_EFFECT_ROTATE4_L_B_L_R                            (EFX_EFFECT_ROTATE_CLASS+53)
#define EFX_EFFECT_ROTATE4_L_B_L_T                            (EFX_EFFECT_ROTATE_CLASS+54)
#define EFX_EFFECT_ROTATE4_L_B_L_B                            (EFX_EFFECT_ROTATE_CLASS+55)
#define EFX_EFFECT_ROTATE4_L_B_R_L                            (EFX_EFFECT_ROTATE_CLASS+56)
#define EFX_EFFECT_ROTATE4_L_B_R_R                            (EFX_EFFECT_ROTATE_CLASS+57)
#define EFX_EFFECT_ROTATE4_L_B_R_T                            (EFX_EFFECT_ROTATE_CLASS+58)
#define EFX_EFFECT_ROTATE4_L_B_R_B                            (EFX_EFFECT_ROTATE_CLASS+59)
#define EFX_EFFECT_ROTATE4_L_B_T_L                            (EFX_EFFECT_ROTATE_CLASS+60)
#define EFX_EFFECT_ROTATE4_L_B_T_R                            (EFX_EFFECT_ROTATE_CLASS+61)
#define EFX_EFFECT_ROTATE4_L_B_T_T                            (EFX_EFFECT_ROTATE_CLASS+62)
#define EFX_EFFECT_ROTATE4_L_B_T_B                            (EFX_EFFECT_ROTATE_CLASS+63)
#define EFX_EFFECT_ROTATE4_L_B_B_L                            (EFX_EFFECT_ROTATE_CLASS+64)
#define EFX_EFFECT_ROTATE4_L_B_B_R                            (EFX_EFFECT_ROTATE_CLASS+65)
#define EFX_EFFECT_ROTATE4_L_B_B_T                            (EFX_EFFECT_ROTATE_CLASS+66)
#define EFX_EFFECT_ROTATE4_L_B_B_B                            (EFX_EFFECT_ROTATE_CLASS+67)
#define EFX_EFFECT_ROTATE4_R_L_L_L                            (EFX_EFFECT_ROTATE_CLASS+68)
#define EFX_EFFECT_ROTATE4_R_L_L_R                            (EFX_EFFECT_ROTATE_CLASS+69)
#define EFX_EFFECT_ROTATE4_R_L_L_T                            (EFX_EFFECT_ROTATE_CLASS+70)
#define EFX_EFFECT_ROTATE4_R_L_L_B                            (EFX_EFFECT_ROTATE_CLASS+71)
#define EFX_EFFECT_ROTATE4_R_L_R_L                            (EFX_EFFECT_ROTATE_CLASS+72)
#define EFX_EFFECT_ROTATE4_R_L_R_R                            (EFX_EFFECT_ROTATE_CLASS+73)
#define EFX_EFFECT_ROTATE4_R_L_R_T                            (EFX_EFFECT_ROTATE_CLASS+74)
#define EFX_EFFECT_ROTATE4_R_L_R_B                            (EFX_EFFECT_ROTATE_CLASS+75)
#define EFX_EFFECT_ROTATE4_R_L_T_L                            (EFX_EFFECT_ROTATE_CLASS+76)
#define EFX_EFFECT_ROTATE4_R_L_T_R                            (EFX_EFFECT_ROTATE_CLASS+77)
#define EFX_EFFECT_ROTATE4_R_L_T_T                            (EFX_EFFECT_ROTATE_CLASS+78)
#define EFX_EFFECT_ROTATE4_R_L_T_B                            (EFX_EFFECT_ROTATE_CLASS+79)
#define EFX_EFFECT_ROTATE4_R_L_B_L                            (EFX_EFFECT_ROTATE_CLASS+80)
#define EFX_EFFECT_ROTATE4_R_L_B_R                            (EFX_EFFECT_ROTATE_CLASS+81)
#define EFX_EFFECT_ROTATE4_R_L_B_T                            (EFX_EFFECT_ROTATE_CLASS+82)
#define EFX_EFFECT_ROTATE4_R_L_B_B                            (EFX_EFFECT_ROTATE_CLASS+83)
#define EFX_EFFECT_ROTATE4_R_R_L_L                            (EFX_EFFECT_ROTATE_CLASS+84)
#define EFX_EFFECT_ROTATE4_R_R_L_R                            (EFX_EFFECT_ROTATE_CLASS+85)
#define EFX_EFFECT_ROTATE4_R_R_L_T                            (EFX_EFFECT_ROTATE_CLASS+86)
#define EFX_EFFECT_ROTATE4_R_R_L_B                            (EFX_EFFECT_ROTATE_CLASS+87)
#define EFX_EFFECT_ROTATE4_R_R_R_L                            (EFX_EFFECT_ROTATE_CLASS+88)
#define EFX_EFFECT_ROTATE4_R_R_R_R                            (EFX_EFFECT_ROTATE_CLASS+89)
#define EFX_EFFECT_ROTATE4_R_R_R_T                            (EFX_EFFECT_ROTATE_CLASS+90)
#define EFX_EFFECT_ROTATE4_R_R_R_B                            (EFX_EFFECT_ROTATE_CLASS+91)
#define EFX_EFFECT_ROTATE4_R_R_T_L                            (EFX_EFFECT_ROTATE_CLASS+92)
#define EFX_EFFECT_ROTATE4_R_R_T_R                            (EFX_EFFECT_ROTATE_CLASS+93)
#define EFX_EFFECT_ROTATE4_R_R_T_T                            (EFX_EFFECT_ROTATE_CLASS+94)
#define EFX_EFFECT_ROTATE4_R_R_T_B                            (EFX_EFFECT_ROTATE_CLASS+95)
#define EFX_EFFECT_ROTATE4_R_R_B_L                            (EFX_EFFECT_ROTATE_CLASS+96)
#define EFX_EFFECT_ROTATE4_R_R_B_R                            (EFX_EFFECT_ROTATE_CLASS+97)
#define EFX_EFFECT_ROTATE4_R_R_B_T                            (EFX_EFFECT_ROTATE_CLASS+98)
#define EFX_EFFECT_ROTATE4_R_R_B_B                            (EFX_EFFECT_ROTATE_CLASS+99)
#define EFX_EFFECT_ROTATE4_R_T_L_L                            (EFX_EFFECT_ROTATE_CLASS+100)
#define EFX_EFFECT_ROTATE4_R_T_L_R                            (EFX_EFFECT_ROTATE_CLASS+101)
#define EFX_EFFECT_ROTATE4_R_T_L_T                            (EFX_EFFECT_ROTATE_CLASS+102)
#define EFX_EFFECT_ROTATE4_R_T_L_B                            (EFX_EFFECT_ROTATE_CLASS+103)
#define EFX_EFFECT_ROTATE4_R_T_R_L                            (EFX_EFFECT_ROTATE_CLASS+104)
#define EFX_EFFECT_ROTATE4_R_T_R_R                            (EFX_EFFECT_ROTATE_CLASS+105)
#define EFX_EFFECT_ROTATE4_R_T_R_T                            (EFX_EFFECT_ROTATE_CLASS+106)
#define EFX_EFFECT_ROTATE4_R_T_R_B                            (EFX_EFFECT_ROTATE_CLASS+107)
#define EFX_EFFECT_ROTATE4_R_T_T_L                            (EFX_EFFECT_ROTATE_CLASS+108)
#define EFX_EFFECT_ROTATE4_R_T_T_R                            (EFX_EFFECT_ROTATE_CLASS+109)
#define EFX_EFFECT_ROTATE4_R_T_T_T                            (EFX_EFFECT_ROTATE_CLASS+110)
#define EFX_EFFECT_ROTATE4_R_T_T_B                            (EFX_EFFECT_ROTATE_CLASS+111)
#define EFX_EFFECT_ROTATE4_R_T_B_L                            (EFX_EFFECT_ROTATE_CLASS+112)
#define EFX_EFFECT_ROTATE4_R_T_B_R                            (EFX_EFFECT_ROTATE_CLASS+113)
#define EFX_EFFECT_ROTATE4_R_T_B_T                            (EFX_EFFECT_ROTATE_CLASS+114)
#define EFX_EFFECT_ROTATE4_R_T_B_B                            (EFX_EFFECT_ROTATE_CLASS+115)
#define EFX_EFFECT_ROTATE4_R_B_L_L                            (EFX_EFFECT_ROTATE_CLASS+116)
#define EFX_EFFECT_ROTATE4_R_B_L_R                            (EFX_EFFECT_ROTATE_CLASS+117)
#define EFX_EFFECT_ROTATE4_R_B_L_T                            (EFX_EFFECT_ROTATE_CLASS+118)
#define EFX_EFFECT_ROTATE4_R_B_L_B                            (EFX_EFFECT_ROTATE_CLASS+119)
#define EFX_EFFECT_ROTATE4_R_B_R_L                            (EFX_EFFECT_ROTATE_CLASS+120)
#define EFX_EFFECT_ROTATE4_R_B_R_R                            (EFX_EFFECT_ROTATE_CLASS+121)
#define EFX_EFFECT_ROTATE4_R_B_R_T                            (EFX_EFFECT_ROTATE_CLASS+122)
#define EFX_EFFECT_ROTATE4_R_B_R_B                            (EFX_EFFECT_ROTATE_CLASS+123)
#define EFX_EFFECT_ROTATE4_R_B_T_L                            (EFX_EFFECT_ROTATE_CLASS+124)
#define EFX_EFFECT_ROTATE4_R_B_T_R                            (EFX_EFFECT_ROTATE_CLASS+125)
#define EFX_EFFECT_ROTATE4_R_B_T_T                            (EFX_EFFECT_ROTATE_CLASS+126)
#define EFX_EFFECT_ROTATE4_R_B_T_B                            (EFX_EFFECT_ROTATE_CLASS+127)
#define EFX_EFFECT_ROTATE4_R_B_B_L                            (EFX_EFFECT_ROTATE_CLASS+128)
#define EFX_EFFECT_ROTATE4_R_B_B_R                            (EFX_EFFECT_ROTATE_CLASS+129)
#define EFX_EFFECT_ROTATE4_R_B_B_T                            (EFX_EFFECT_ROTATE_CLASS+130)
#define EFX_EFFECT_ROTATE4_R_B_B_B                            (EFX_EFFECT_ROTATE_CLASS+131)
#define EFX_EFFECT_ROTATE4_T_L_L_L                            (EFX_EFFECT_ROTATE_CLASS+132)
#define EFX_EFFECT_ROTATE4_T_L_L_R                            (EFX_EFFECT_ROTATE_CLASS+133)
#define EFX_EFFECT_ROTATE4_T_L_L_T                            (EFX_EFFECT_ROTATE_CLASS+134)
#define EFX_EFFECT_ROTATE4_T_L_L_B                            (EFX_EFFECT_ROTATE_CLASS+135)
#define EFX_EFFECT_ROTATE4_T_L_R_L                            (EFX_EFFECT_ROTATE_CLASS+136)
#define EFX_EFFECT_ROTATE4_T_L_R_R                            (EFX_EFFECT_ROTATE_CLASS+137)
#define EFX_EFFECT_ROTATE4_T_L_R_T                            (EFX_EFFECT_ROTATE_CLASS+138)
#define EFX_EFFECT_ROTATE4_T_L_R_B                            (EFX_EFFECT_ROTATE_CLASS+139)
#define EFX_EFFECT_ROTATE4_T_L_T_L                            (EFX_EFFECT_ROTATE_CLASS+140)
#define EFX_EFFECT_ROTATE4_T_L_T_R                            (EFX_EFFECT_ROTATE_CLASS+141)
#define EFX_EFFECT_ROTATE4_T_L_T_T                            (EFX_EFFECT_ROTATE_CLASS+142)
#define EFX_EFFECT_ROTATE4_T_L_T_B                            (EFX_EFFECT_ROTATE_CLASS+143)
#define EFX_EFFECT_ROTATE4_T_L_B_L                            (EFX_EFFECT_ROTATE_CLASS+144)
#define EFX_EFFECT_ROTATE4_T_L_B_R                            (EFX_EFFECT_ROTATE_CLASS+145)
#define EFX_EFFECT_ROTATE4_T_L_B_T                            (EFX_EFFECT_ROTATE_CLASS+146)
#define EFX_EFFECT_ROTATE4_T_L_B_B                            (EFX_EFFECT_ROTATE_CLASS+147)
#define EFX_EFFECT_ROTATE4_T_R_L_L                            (EFX_EFFECT_ROTATE_CLASS+148)
#define EFX_EFFECT_ROTATE4_T_R_L_R                            (EFX_EFFECT_ROTATE_CLASS+149)
#define EFX_EFFECT_ROTATE4_T_R_L_T                            (EFX_EFFECT_ROTATE_CLASS+150)
#define EFX_EFFECT_ROTATE4_T_R_L_B                            (EFX_EFFECT_ROTATE_CLASS+151)
#define EFX_EFFECT_ROTATE4_T_R_R_L                            (EFX_EFFECT_ROTATE_CLASS+152)
#define EFX_EFFECT_ROTATE4_T_R_R_R                            (EFX_EFFECT_ROTATE_CLASS+153)
#define EFX_EFFECT_ROTATE4_T_R_R_T                            (EFX_EFFECT_ROTATE_CLASS+154)
#define EFX_EFFECT_ROTATE4_T_R_R_B                            (EFX_EFFECT_ROTATE_CLASS+155)
#define EFX_EFFECT_ROTATE4_T_R_T_L                            (EFX_EFFECT_ROTATE_CLASS+156)
#define EFX_EFFECT_ROTATE4_T_R_T_R                            (EFX_EFFECT_ROTATE_CLASS+157)
#define EFX_EFFECT_ROTATE4_T_R_T_T                            (EFX_EFFECT_ROTATE_CLASS+158)
#define EFX_EFFECT_ROTATE4_T_R_T_B                            (EFX_EFFECT_ROTATE_CLASS+159)
#define EFX_EFFECT_ROTATE4_T_R_B_L                            (EFX_EFFECT_ROTATE_CLASS+160)
#define EFX_EFFECT_ROTATE4_T_R_B_R                            (EFX_EFFECT_ROTATE_CLASS+161)
#define EFX_EFFECT_ROTATE4_T_R_B_T                            (EFX_EFFECT_ROTATE_CLASS+162)
#define EFX_EFFECT_ROTATE4_T_R_B_B                            (EFX_EFFECT_ROTATE_CLASS+163)
#define EFX_EFFECT_ROTATE4_T_T_L_L                            (EFX_EFFECT_ROTATE_CLASS+164)
#define EFX_EFFECT_ROTATE4_T_T_L_R                            (EFX_EFFECT_ROTATE_CLASS+165)
#define EFX_EFFECT_ROTATE4_T_T_L_T                            (EFX_EFFECT_ROTATE_CLASS+166)
#define EFX_EFFECT_ROTATE4_T_T_L_B                            (EFX_EFFECT_ROTATE_CLASS+167)
#define EFX_EFFECT_ROTATE4_T_T_R_L                            (EFX_EFFECT_ROTATE_CLASS+168)
#define EFX_EFFECT_ROTATE4_T_T_R_R                            (EFX_EFFECT_ROTATE_CLASS+169)
#define EFX_EFFECT_ROTATE4_T_T_R_T                            (EFX_EFFECT_ROTATE_CLASS+170)
#define EFX_EFFECT_ROTATE4_T_T_R_B                            (EFX_EFFECT_ROTATE_CLASS+171)
#define EFX_EFFECT_ROTATE4_T_T_T_L                            (EFX_EFFECT_ROTATE_CLASS+172)
#define EFX_EFFECT_ROTATE4_T_T_T_R                            (EFX_EFFECT_ROTATE_CLASS+173)
#define EFX_EFFECT_ROTATE4_T_T_T_T                            (EFX_EFFECT_ROTATE_CLASS+174)
#define EFX_EFFECT_ROTATE4_T_T_T_B                            (EFX_EFFECT_ROTATE_CLASS+175)
#define EFX_EFFECT_ROTATE4_T_T_B_L                            (EFX_EFFECT_ROTATE_CLASS+176)
#define EFX_EFFECT_ROTATE4_T_T_B_R                            (EFX_EFFECT_ROTATE_CLASS+177)
#define EFX_EFFECT_ROTATE4_T_T_B_T                            (EFX_EFFECT_ROTATE_CLASS+178)
#define EFX_EFFECT_ROTATE4_T_T_B_B                            (EFX_EFFECT_ROTATE_CLASS+179)
#define EFX_EFFECT_ROTATE4_T_B_L_L                            (EFX_EFFECT_ROTATE_CLASS+180)
#define EFX_EFFECT_ROTATE4_T_B_L_R                            (EFX_EFFECT_ROTATE_CLASS+181)
#define EFX_EFFECT_ROTATE4_T_B_L_T                            (EFX_EFFECT_ROTATE_CLASS+182)
#define EFX_EFFECT_ROTATE4_T_B_L_B                            (EFX_EFFECT_ROTATE_CLASS+183)
#define EFX_EFFECT_ROTATE4_T_B_R_L                            (EFX_EFFECT_ROTATE_CLASS+184)
#define EFX_EFFECT_ROTATE4_T_B_R_R                            (EFX_EFFECT_ROTATE_CLASS+185)
#define EFX_EFFECT_ROTATE4_T_B_R_T                            (EFX_EFFECT_ROTATE_CLASS+186)
#define EFX_EFFECT_ROTATE4_T_B_R_B                            (EFX_EFFECT_ROTATE_CLASS+187)
#define EFX_EFFECT_ROTATE4_T_B_T_L                            (EFX_EFFECT_ROTATE_CLASS+188)
#define EFX_EFFECT_ROTATE4_T_B_T_R                            (EFX_EFFECT_ROTATE_CLASS+189)
#define EFX_EFFECT_ROTATE4_T_B_T_T                            (EFX_EFFECT_ROTATE_CLASS+190)
#define EFX_EFFECT_ROTATE4_T_B_T_B                            (EFX_EFFECT_ROTATE_CLASS+191)
#define EFX_EFFECT_ROTATE4_T_B_B_L                            (EFX_EFFECT_ROTATE_CLASS+192)
#define EFX_EFFECT_ROTATE4_T_B_B_R                            (EFX_EFFECT_ROTATE_CLASS+193)
#define EFX_EFFECT_ROTATE4_T_B_B_T                            (EFX_EFFECT_ROTATE_CLASS+194)
#define EFX_EFFECT_ROTATE4_T_B_B_B                            (EFX_EFFECT_ROTATE_CLASS+195)
#define EFX_EFFECT_ROTATE4_B_L_L_L                            (EFX_EFFECT_ROTATE_CLASS+196)
#define EFX_EFFECT_ROTATE4_B_L_L_R                            (EFX_EFFECT_ROTATE_CLASS+197)
#define EFX_EFFECT_ROTATE4_B_L_L_T                            (EFX_EFFECT_ROTATE_CLASS+198)
#define EFX_EFFECT_ROTATE4_B_L_L_B                            (EFX_EFFECT_ROTATE_CLASS+199)
#define EFX_EFFECT_ROTATE4_B_L_R_L                            (EFX_EFFECT_ROTATE_CLASS+200)
#define EFX_EFFECT_ROTATE4_B_L_R_R                            (EFX_EFFECT_ROTATE_CLASS+201)
#define EFX_EFFECT_ROTATE4_B_L_R_T                            (EFX_EFFECT_ROTATE_CLASS+202)
#define EFX_EFFECT_ROTATE4_B_L_R_B                            (EFX_EFFECT_ROTATE_CLASS+203)
#define EFX_EFFECT_ROTATE4_B_L_T_L                            (EFX_EFFECT_ROTATE_CLASS+204)
#define EFX_EFFECT_ROTATE4_B_L_T_R                            (EFX_EFFECT_ROTATE_CLASS+205)
#define EFX_EFFECT_ROTATE4_B_L_T_T                            (EFX_EFFECT_ROTATE_CLASS+206)
#define EFX_EFFECT_ROTATE4_B_L_T_B                            (EFX_EFFECT_ROTATE_CLASS+207)
#define EFX_EFFECT_ROTATE4_B_L_B_L                            (EFX_EFFECT_ROTATE_CLASS+208)
#define EFX_EFFECT_ROTATE4_B_L_B_R                            (EFX_EFFECT_ROTATE_CLASS+209)
#define EFX_EFFECT_ROTATE4_B_L_B_T                            (EFX_EFFECT_ROTATE_CLASS+210)
#define EFX_EFFECT_ROTATE4_B_L_B_B                            (EFX_EFFECT_ROTATE_CLASS+211)
#define EFX_EFFECT_ROTATE4_B_R_L_L                            (EFX_EFFECT_ROTATE_CLASS+212)
#define EFX_EFFECT_ROTATE4_B_R_L_R                            (EFX_EFFECT_ROTATE_CLASS+213)
#define EFX_EFFECT_ROTATE4_B_R_L_T                            (EFX_EFFECT_ROTATE_CLASS+214)
#define EFX_EFFECT_ROTATE4_B_R_L_B                            (EFX_EFFECT_ROTATE_CLASS+215)
#define EFX_EFFECT_ROTATE4_B_R_R_L                            (EFX_EFFECT_ROTATE_CLASS+216)
#define EFX_EFFECT_ROTATE4_B_R_R_R                            (EFX_EFFECT_ROTATE_CLASS+217)
#define EFX_EFFECT_ROTATE4_B_R_R_T                            (EFX_EFFECT_ROTATE_CLASS+218)
#define EFX_EFFECT_ROTATE4_B_R_R_B                            (EFX_EFFECT_ROTATE_CLASS+219)
#define EFX_EFFECT_ROTATE4_B_R_T_L                            (EFX_EFFECT_ROTATE_CLASS+220)
#define EFX_EFFECT_ROTATE4_B_R_T_R                            (EFX_EFFECT_ROTATE_CLASS+221)
#define EFX_EFFECT_ROTATE4_B_R_T_T                            (EFX_EFFECT_ROTATE_CLASS+222)
#define EFX_EFFECT_ROTATE4_B_R_T_B                            (EFX_EFFECT_ROTATE_CLASS+223)
#define EFX_EFFECT_ROTATE4_B_R_B_L                            (EFX_EFFECT_ROTATE_CLASS+224)
#define EFX_EFFECT_ROTATE4_B_R_B_R                            (EFX_EFFECT_ROTATE_CLASS+225)
#define EFX_EFFECT_ROTATE4_B_R_B_T                            (EFX_EFFECT_ROTATE_CLASS+226)
#define EFX_EFFECT_ROTATE4_B_R_B_B                            (EFX_EFFECT_ROTATE_CLASS+227)
#define EFX_EFFECT_ROTATE4_B_T_L_L                            (EFX_EFFECT_ROTATE_CLASS+228)
#define EFX_EFFECT_ROTATE4_B_T_L_R                            (EFX_EFFECT_ROTATE_CLASS+229)
#define EFX_EFFECT_ROTATE4_B_T_L_T                            (EFX_EFFECT_ROTATE_CLASS+230)
#define EFX_EFFECT_ROTATE4_B_T_L_B                            (EFX_EFFECT_ROTATE_CLASS+231)
#define EFX_EFFECT_ROTATE4_B_T_R_L                            (EFX_EFFECT_ROTATE_CLASS+232)
#define EFX_EFFECT_ROTATE4_B_T_R_R                            (EFX_EFFECT_ROTATE_CLASS+233)
#define EFX_EFFECT_ROTATE4_B_T_R_T                            (EFX_EFFECT_ROTATE_CLASS+234)
#define EFX_EFFECT_ROTATE4_B_T_R_B                            (EFX_EFFECT_ROTATE_CLASS+235)
#define EFX_EFFECT_ROTATE4_B_T_T_L                            (EFX_EFFECT_ROTATE_CLASS+236)
#define EFX_EFFECT_ROTATE4_B_T_T_R                            (EFX_EFFECT_ROTATE_CLASS+237)
#define EFX_EFFECT_ROTATE4_B_T_T_T                            (EFX_EFFECT_ROTATE_CLASS+238)
#define EFX_EFFECT_ROTATE4_B_T_T_B                            (EFX_EFFECT_ROTATE_CLASS+239)
#define EFX_EFFECT_ROTATE4_B_T_B_L                            (EFX_EFFECT_ROTATE_CLASS+240)
#define EFX_EFFECT_ROTATE4_B_T_B_R                            (EFX_EFFECT_ROTATE_CLASS+241)
#define EFX_EFFECT_ROTATE4_B_T_B_T                            (EFX_EFFECT_ROTATE_CLASS+242)
#define EFX_EFFECT_ROTATE4_B_T_B_B                            (EFX_EFFECT_ROTATE_CLASS+243)
#define EFX_EFFECT_ROTATE4_B_B_L_L                            (EFX_EFFECT_ROTATE_CLASS+244)
#define EFX_EFFECT_ROTATE4_B_B_L_R                            (EFX_EFFECT_ROTATE_CLASS+245)
#define EFX_EFFECT_ROTATE4_B_B_L_T                            (EFX_EFFECT_ROTATE_CLASS+246)
#define EFX_EFFECT_ROTATE4_B_B_L_B                            (EFX_EFFECT_ROTATE_CLASS+247)
#define EFX_EFFECT_ROTATE4_B_B_R_L                            (EFX_EFFECT_ROTATE_CLASS+248)
#define EFX_EFFECT_ROTATE4_B_B_R_R                            (EFX_EFFECT_ROTATE_CLASS+249)
#define EFX_EFFECT_ROTATE4_B_B_R_T                            (EFX_EFFECT_ROTATE_CLASS+250)
#define EFX_EFFECT_ROTATE4_B_B_R_B                            (EFX_EFFECT_ROTATE_CLASS+251)
#define EFX_EFFECT_ROTATE4_B_B_T_L                            (EFX_EFFECT_ROTATE_CLASS+252)
#define EFX_EFFECT_ROTATE4_B_B_T_R                            (EFX_EFFECT_ROTATE_CLASS+253)
#define EFX_EFFECT_ROTATE4_B_B_T_T                            (EFX_EFFECT_ROTATE_CLASS+254)
#define EFX_EFFECT_ROTATE4_B_B_T_B                            (EFX_EFFECT_ROTATE_CLASS+255)
#define EFX_EFFECT_ROTATE4_B_B_B_L                            (EFX_EFFECT_ROTATE_CLASS+256)
#define EFX_EFFECT_ROTATE4_B_B_B_R                            (EFX_EFFECT_ROTATE_CLASS+257)
#define EFX_EFFECT_ROTATE4_B_B_B_T                            (EFX_EFFECT_ROTATE_CLASS+258)
#define EFX_EFFECT_ROTATE4_B_B_B_B                            (EFX_EFFECT_ROTATE_CLASS+259)
#define EFX_EFFECT_ROTATE_MAX                                 (EFX_EFFECT_ROTATE_CLASS+259)

// Zoom
#define EFX_EFFECT_ZOOM_CLASS                                 9000
#define EFX_EFFECT_ZOOM_TO_C                                  (EFX_EFFECT_ZOOM_CLASS+0)
#define EFX_EFFECT_ZOOM_FROM_C                                (EFX_EFFECT_ZOOM_CLASS+1)
#define EFX_EFFECT_ZOOM_MAX                                   (EFX_EFFECT_ZOOM_CLASS+1)

// Drip
#define EFX_EFFECT_DRIP_CLASS                                 10000
#define EFX_EFFECT_DRIP_T_TO_B                                (EFX_EFFECT_DRIP_CLASS+0)
#define EFX_EFFECT_DRIP_B_TO_T                                (EFX_EFFECT_DRIP_CLASS+1)
#define EFX_EFFECT_DRIP_L_TO_R                                (EFX_EFFECT_DRIP_CLASS+2)
#define EFX_EFFECT_DRIP_R_TO_L                                (EFX_EFFECT_DRIP_CLASS+3)
#define EFX_EFFECT_DRIP_MAX                                   (EFX_EFFECT_DRIP_CLASS+3)

// Blind
#define EFX_EFFECT_BLIND_CLASS                                11000
#define EFX_EFFECT_BLIND_T_TO_B                               (EFX_EFFECT_BLIND_CLASS+0)
#define EFX_EFFECT_BLIND_L_TO_R                               (EFX_EFFECT_BLIND_CLASS+1)
#define EFX_EFFECT_BLIND_GROWING_L_TO_R                       (EFX_EFFECT_BLIND_CLASS+2)
#define EFX_EFFECT_BLIND_GROWING_R_TO_L                       (EFX_EFFECT_BLIND_CLASS+3)
#define EFX_EFFECT_BLIND_GROWING_T_TO_B                       (EFX_EFFECT_BLIND_CLASS+4)
#define EFX_EFFECT_BLIND_GROWING_B_TO_T                       (EFX_EFFECT_BLIND_CLASS+5)
#define EFX_EFFECT_BLIND2_T_TO_B_AND_L_TO_R                   (EFX_EFFECT_BLIND_CLASS+6)
#define EFX_EFFECT_BLIND_MAX                                  (EFX_EFFECT_BLIND_CLASS+6)

// Random
#define EFX_EFFECT_RANDOM_CLASS                               12000
#define EFX_EFFECT_RANDOM_BARS_R_TO_L                         (EFX_EFFECT_RANDOM_CLASS+0)
#define EFX_EFFECT_RANDOM_BARS_L_TO_R                         (EFX_EFFECT_RANDOM_CLASS+1)
#define EFX_EFFECT_RANDOM_BARS_B_TO_T                         (EFX_EFFECT_RANDOM_CLASS+2)
#define EFX_EFFECT_RANDOM_BARS_T_TO_B                         (EFX_EFFECT_RANDOM_CLASS+3)
#define EFX_EFFECT_RANDOM_LINES                               (EFX_EFFECT_RANDOM_CLASS+4)
#define EFX_EFFECT_RANDOM_BLOCKS                              (EFX_EFFECT_RANDOM_CLASS+5)
#define EFX_EFFECT_RANDOM_BLOCKS_R_TO_L                       (EFX_EFFECT_RANDOM_CLASS+6)
#define EFX_EFFECT_RANDOM_BLOCKS_L_TO_R                       (EFX_EFFECT_RANDOM_CLASS+7)
#define EFX_EFFECT_RANDOM_BLOCKS_B_TO_T                       (EFX_EFFECT_RANDOM_CLASS+8)
#define EFX_EFFECT_RANDOM_BLOCKS_T_TO_B                       (EFX_EFFECT_RANDOM_CLASS+9)
#define EFX_EFFECT_RANDOM_MAX                                 (EFX_EFFECT_RANDOM_CLASS+9)

// Checkerboard
#define EFX_EFFECT_CHECKERBOARD_CLASS                         13000
#define EFX_EFFECT_CHECKERBOARD_T_TO_B_THEN_T_TO_B            (EFX_EFFECT_CHECKERBOARD_CLASS+0)
#define EFX_EFFECT_CHECKERBOARD_B_TO_T_THEN_B_TO_T            (EFX_EFFECT_CHECKERBOARD_CLASS+1)
#define EFX_EFFECT_CHECKERBOARD_L_TO_R_THEN_L_TO_R            (EFX_EFFECT_CHECKERBOARD_CLASS+2)
#define EFX_EFFECT_CHECKERBOARD_R_TO_L_THEN_R_TO_L            (EFX_EFFECT_CHECKERBOARD_CLASS+3)
#define EFX_EFFECT_CHECKERBOARD_T_TO_B_THEN_B_TO_T            (EFX_EFFECT_CHECKERBOARD_CLASS+4)
#define EFX_EFFECT_CHECKERBOARD_B_TO_T_THEN_T_TO_B            (EFX_EFFECT_CHECKERBOARD_CLASS+5)
#define EFX_EFFECT_CHECKERBOARD_L_TO_R_THEN_R_TO_L            (EFX_EFFECT_CHECKERBOARD_CLASS+6)
#define EFX_EFFECT_CHECKERBOARD_R_TO_L_THEN_L_TO_R            (EFX_EFFECT_CHECKERBOARD_CLASS+7)
#define EFX_EFFECT_CHECKERBOARD_MAX                           (EFX_EFFECT_CHECKERBOARD_CLASS+7)

// Blocks
#define EFX_EFFECT_BLOCKS_CLASS                               14000
#define EFX_EFFECT_BLOCKS_T_TO_B                              (EFX_EFFECT_BLOCKS_CLASS+0)
#define EFX_EFFECT_BLOCKS_B_TO_T                              (EFX_EFFECT_BLOCKS_CLASS+1)
#define EFX_EFFECT_BLOCKS_L_TO_R                              (EFX_EFFECT_BLOCKS_CLASS+2)
#define EFX_EFFECT_BLOCKS_R_TO_L                              (EFX_EFFECT_BLOCKS_CLASS+3)
#define EFX_EFFECT_BLOCKS_SWIRL_CW_TO_C                       (EFX_EFFECT_BLOCKS_CLASS+4)
#define EFX_EFFECT_BLOCKS_SWIRL_CW_FROM_C                     (EFX_EFFECT_BLOCKS_CLASS+5)
#define EFX_EFFECT_BLOCKS_SWIRL_CCW_TO_C                      (EFX_EFFECT_BLOCKS_CLASS+6)
#define EFX_EFFECT_BLOCKS_SWIRL_CCW_FROM_C                    (EFX_EFFECT_BLOCKS_CLASS+7)
#define EFX_EFFECT_BLOCKS_MAX                                 (EFX_EFFECT_BLOCKS_CLASS+7)

// Circle
#define EFX_EFFECT_CIRCLE_CLASS                               15000
#define EFX_EFFECT_CIRCLE_C_IN                                (EFX_EFFECT_CIRCLE_CLASS+0)
#define EFX_EFFECT_CIRCLE_C_OUT                               (EFX_EFFECT_CIRCLE_CLASS+1)
#define EFX_EFFECT_CIRCLE_L_IN                                (EFX_EFFECT_CIRCLE_CLASS+2)
#define EFX_EFFECT_CIRCLE_L_OUT                               (EFX_EFFECT_CIRCLE_CLASS+3)
#define EFX_EFFECT_CIRCLE_R_IN                                (EFX_EFFECT_CIRCLE_CLASS+4)
#define EFX_EFFECT_CIRCLE_R_OUT                               (EFX_EFFECT_CIRCLE_CLASS+5)
#define EFX_EFFECT_CIRCLE_T_IN                                (EFX_EFFECT_CIRCLE_CLASS+6)
#define EFX_EFFECT_CIRCLE_T_OUT                               (EFX_EFFECT_CIRCLE_CLASS+7)
#define EFX_EFFECT_CIRCLE_B_IN                                (EFX_EFFECT_CIRCLE_CLASS+8)
#define EFX_EFFECT_CIRCLE_B_OUT                               (EFX_EFFECT_CIRCLE_CLASS+9)
#define EFX_EFFECT_CIRCLE_C_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+10)
#define EFX_EFFECT_CIRCLE_C_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+11)
#define EFX_EFFECT_CIRCLE_C_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+12)
#define EFX_EFFECT_CIRCLE_C_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+13)
#define EFX_EFFECT_CIRCLE_L_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+14)
#define EFX_EFFECT_CIRCLE_L_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+15)
#define EFX_EFFECT_CIRCLE_L_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+16)
#define EFX_EFFECT_CIRCLE_L_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+17)
#define EFX_EFFECT_CIRCLE_R_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+18)
#define EFX_EFFECT_CIRCLE_R_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+19)
#define EFX_EFFECT_CIRCLE_R_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+20)
#define EFX_EFFECT_CIRCLE_R_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+21)
#define EFX_EFFECT_CIRCLE_T_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+22)
#define EFX_EFFECT_CIRCLE_T_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+23)
#define EFX_EFFECT_CIRCLE_T_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+24)
#define EFX_EFFECT_CIRCLE_T_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+25)
#define EFX_EFFECT_CIRCLE_B_INTER_IN_THEN_IN                  (EFX_EFFECT_CIRCLE_CLASS+26)
#define EFX_EFFECT_CIRCLE_B_INTER_OUT_THEN_OUT                (EFX_EFFECT_CIRCLE_CLASS+27)
#define EFX_EFFECT_CIRCLE_B_INTER_IN_THEN_OUT                 (EFX_EFFECT_CIRCLE_CLASS+28)
#define EFX_EFFECT_CIRCLE_B_INTER_OUT_THEN_IN                 (EFX_EFFECT_CIRCLE_CLASS+29)
#define EFX_EFFECT_CIRCLE_L_R_INTER                           (EFX_EFFECT_CIRCLE_CLASS+30)
#define EFX_EFFECT_CIRCLE_T_B_INTER                           (EFX_EFFECT_CIRCLE_CLASS+31)
#define EFX_EFFECT_CIRCLE2_L_R_OUT                            (EFX_EFFECT_CIRCLE_CLASS+32)
#define EFX_EFFECT_CIRCLE2_L_R_IN                             (EFX_EFFECT_CIRCLE_CLASS+33)
#define EFX_EFFECT_CIRCLE2_T_B_OUT                            (EFX_EFFECT_CIRCLE_CLASS+34)
#define EFX_EFFECT_CIRCLE2_T_B_IN                             (EFX_EFFECT_CIRCLE_CLASS+35)
#define EFX_EFFECT_CIRCLE_MAX                                 (EFX_EFFECT_CIRCLE_CLASS+35)

// Ellipse
#define EFX_EFFECT_ELLIPSE_CLASS                              16000
#define EFX_EFFECT_ELLIPSE_C_IN                               (EFX_EFFECT_ELLIPSE_CLASS+0)
#define EFX_EFFECT_ELLIPSE_C_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+1)
#define EFX_EFFECT_ELLIPSE_L_IN                               (EFX_EFFECT_ELLIPSE_CLASS+2)
#define EFX_EFFECT_ELLIPSE_L_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+3)
#define EFX_EFFECT_ELLIPSE_R_IN                               (EFX_EFFECT_ELLIPSE_CLASS+4)
#define EFX_EFFECT_ELLIPSE_R_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+5)
#define EFX_EFFECT_ELLIPSE_T_IN                               (EFX_EFFECT_ELLIPSE_CLASS+6)
#define EFX_EFFECT_ELLIPSE_T_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+7)
#define EFX_EFFECT_ELLIPSE_B_IN                               (EFX_EFFECT_ELLIPSE_CLASS+8)
#define EFX_EFFECT_ELLIPSE_B_OUT                              (EFX_EFFECT_ELLIPSE_CLASS+9)
#define EFX_EFFECT_ELLIPSE_C_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+10)
#define EFX_EFFECT_ELLIPSE_C_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+11)
#define EFX_EFFECT_ELLIPSE_C_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+12)
#define EFX_EFFECT_ELLIPSE_C_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+13)
#define EFX_EFFECT_ELLIPSE_L_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+14)
#define EFX_EFFECT_ELLIPSE_L_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+15)
#define EFX_EFFECT_ELLIPSE_L_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+16)
#define EFX_EFFECT_ELLIPSE_L_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+17)
#define EFX_EFFECT_ELLIPSE_R_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+18)
#define EFX_EFFECT_ELLIPSE_R_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+19)
#define EFX_EFFECT_ELLIPSE_R_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+20)
#define EFX_EFFECT_ELLIPSE_R_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+21)
#define EFX_EFFECT_ELLIPSE_T_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+22)
#define EFX_EFFECT_ELLIPSE_T_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+23)
#define EFX_EFFECT_ELLIPSE_T_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+24)
#define EFX_EFFECT_ELLIPSE_T_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+25)
#define EFX_EFFECT_ELLIPSE_B_INTER_IN_THEN_IN                 (EFX_EFFECT_ELLIPSE_CLASS+26)
#define EFX_EFFECT_ELLIPSE_B_INTER_OUT_THEN_OUT               (EFX_EFFECT_ELLIPSE_CLASS+27)
#define EFX_EFFECT_ELLIPSE_B_INTER_IN_THEN_OUT                (EFX_EFFECT_ELLIPSE_CLASS+28)
#define EFX_EFFECT_ELLIPSE_B_INTER_OUT_THEN_IN                (EFX_EFFECT_ELLIPSE_CLASS+29)
#define EFX_EFFECT_ELLIPSE_L_R_INTER                          (EFX_EFFECT_ELLIPSE_CLASS+30)
#define EFX_EFFECT_ELLIPSE_T_B_INTER                          (EFX_EFFECT_ELLIPSE_CLASS+31)
#define EFX_EFFECT_ELLIPSE2_L_R_OUT                           (EFX_EFFECT_ELLIPSE_CLASS+32)
#define EFX_EFFECT_ELLIPSE2_L_R_IN                            (EFX_EFFECT_ELLIPSE_CLASS+33)
#define EFX_EFFECT_ELLIPSE2_T_B_OUT                           (EFX_EFFECT_ELLIPSE_CLASS+34)
#define EFX_EFFECT_ELLIPSE2_T_B_IN                            (EFX_EFFECT_ELLIPSE_CLASS+35)
#define EFX_EFFECT_ELLIPSE_MAX                                (EFX_EFFECT_ELLIPSE_CLASS+35)

#define EFFECT_NONE                 EFX_EFFECT_NONE
#define EFFECT_WIPE_DOWN            EFX_EFFECT_WIPE_T_TO_B
#define EFFECT_WIPE_UP              EFX_EFFECT_WIPE_B_TO_T
#define EFFECT_WIPE_RIGHT           EFX_EFFECT_WIPE_L_TO_R
#define EFFECT_WIPE_LEFT            EFX_EFFECT_WIPE_R_TO_L
#define EFFECT_RANDOM_PIXEL         EFX_EFFECT_RANDOM_BLOCK                         // close match - use the grain size
#define EFFECT_RANDOM_SMALLBOX      EFX_EFFECT_RANDOM_BLOCK                         // close match - use the grain size
#define EFFECT_RANDOM_MEDIUMBOX     EFX_EFFECT_RANDOM_BLOCK                         // close match - use the grain size
#define EFFECT_RANDOM_LARGEBOX      EFX_EFFECT_RANDOM_BLOCK                         // close match - use the grain size
#define EFFECT_RANDOM_HLINE         EFX_EFFECT_RANDOM_LINES                         // close match - use the grain size
#define EFFECT_RANDOM_VLINE         EFX_EFFECT_RANDOM_LINES                         // close match - use the grain size
#define EFFECT_VMIDDLE              EFX_EFFECT_WIPE2_C_TO_L_AND_R
#define EFFECT_HMIDDLE              EFX_EFFECT_WIPE2_C_TO_T_AND_B
#define EFFECT_HINTERLACE           EFX_EFFECT_BLIND_T_TO_B                         // close match
#define EFFECT_VINTERLACE           EFX_EFFECT_BLIND_L_TO_R                         // close match
#define EFFECT_HINTERLACE_OUTWARD   EFX_EFFECT_WIPE_INTERV_C_TO_L_AND_R             // close match
#define EFFECT_VINTERLACE_OUTWARD   EFX_EFFECT_WIPE_INTERH_C_TO_T_AND_B             // close match
#define EFFECT_OUTTOIN              EFX_EFFECT_WIPE_RECTANGLE_IN
#define EFFECT_INTOOUT              EFX_EFFECT_WIPE_RECTANGLE_OUT
#define EFFECT_OUTTOIN_INTERLACE    EFX_EFFECT_WIPE_RECTANGLE_INTER_IN_THEN_IN      // close match
#define EFFECT_INTOOUT_INTERLACE    EFX_EFFECT_WIPE_RECTANGLE_INTER_OUT_THEN_OUT    // close match
#define EFFECT_SPIRAL_IN            EFX_EFFECT_BLOCKS_SWIRL_CW_TO_C
#define EFFECT_SPIRAL_OUT           EFX_EFFECT_BLOCKS_SWIRL_CW_FROM_C
#define EFFECT_HMIDDLE_INWARD       EFX_EFFECT_WIPE2_T_AND_B_TO_C
#define EFFECT_VMIDDLE_INWARD       EFX_EFFECT_WIPE2_L_AND_R_TO_C
#define EFFECT_VINTERLACE_INWARD    EFX_EFFECT_WIPE_INTERV_T_AND_B_TO_C             // close match
#define EFFECT_HINTERLACE_INWARD    EFX_EFFECT_WIPE_INTERH_L_AND_R_TO_C             // close match
#define EFFECT_VINTERLACE_RIGHT     EFX_EFFECT_BLIND_T_TO_B                         // close match
#define EFFECT_VINTERLACE_LEFT      EFX_EFFECT_BLIND_T_TO_B                         // close match
#define EFFECT_HINTERLACE_UP        EFX_EFFECT_BLIND_L_TO_R                         // close match
#define EFFECT_HINTERLACE_DOWN      EFX_EFFECT_BLIND_L_TO_R                         // close match


   enum EfxNotifyConstants{
    EFX_NOTIFY_TRANSITION = 0,
    EFX_NOTIFY_IMAGE = 1,
   };

   enum EfxBackStyleConstants{
    EFX_BACKSTYLE_TRANSPARENT = 0,
    EFX_BACKSTYLE_OPAQUE = 1,
    EFX_BACKSTYLE_TRANSLUCENT = 2,
    EFX_BACKSTYLE_TILED_IMAGE = 3,
    EFX_BACKSTYLE_TRANSLUCENT_TILED_IMAGE = 4,
    EFX_BACKSTYLE_GRADIENT = 5,
    EFX_BACKSTYLE_TRANSLUCENT_GRADIENT = 6,
    EFX_BACKSTYLE_MAX = 6
   };

   enum EfxBorderStyleConstants{
    EFX_BORDERSTYLE_TRANSPARENT = 0,
    EFX_BORDERSTYLE_SOLID = 1,
    EFX_BORDERSTYLE_DASH = 2,
    EFX_BORDERSTYLE_DOT = 3,
    EFX_BORDERSTYLE_DASHDOT = 4,
    EFX_BORDERSTYLE_DASHDOTDOT = 5,
    EFX_BORDERSTYLE_MAX = 5
   };

   enum EfxInnerStyleConstants{
    EFX_INNERSTYLE_INSET = 0,
    EFX_INNERSTYLE_RAISED = 1,
    EFX_INNERSTYLE_MAX = 1
   };

   enum EfxOuterStyleConstants{
    EFX_OUTERSTYLE_INSET = 0,
    EFX_OUTERSTYLE_RAISED = 1,
    EFX_OUTERSTYLE_MAX = 1
   };

   enum EfxTextStyleConstants{
    EFX_TEXTSTYLE_NORMAL = 0,
    EFX_TEXTSTYLE_INSETLIGHT = 1,
    EFX_TEXTSTYLE_INSETHEAVY = 2,
    EFX_TEXTSTYLE_RAISEDLIGHT = 3,
    EFX_TEXTSTYLE_RAISEDHEAVY = 4,
    EFX_TEXTSTYLE_DROPSHADOW = 5,
    EFX_TEXTSTYLE_BLOCKSHADOW = 6,
    EFX_TEXTSTYLE_OUTLINEBLOCK = 7,
    EFX_TEXTSTYLE_INSETEXTRALIGHT = 8,
    EFX_TEXTSTYLE_INSETEXTRAHEAVY = 9,
    EFX_TEXTSTYLE_RAISEDEXTRALIGHT = 10,
    EFX_TEXTSTYLE_RAISEDEXTRAHEAVY = 11,
    EFX_TEXTSTYLE_MAX = 11
   };

   enum EfxTextAlignConstants{
    EFX_TEXTALIGN_LEFT_TOP = 0,
    EFX_TEXTALIGN_LEFT_VCENTER = 1,
    EFX_TEXTALIGN_LEFT_BOTTOM = 2,
    EFX_TEXTALIGN_HCENTER_TOP = 3,
    EFX_TEXTALIGN_HCENTER_VCENTER = 4,
    EFX_TEXTALIGN_HCENTER_BOTTOM = 5,
    EFX_TEXTALIGN_RIGHT_TOP = 6,
    EFX_TEXTALIGN_RIGHT_VCENTER = 7,
    EFX_TEXTALIGN_RIGHT_BOTTOM = 8,
    EFX_TEXTALIGN_MAX = 8
   };

   enum PaintSizeModeConstants {
      PAINTSIZEMODE_NORMAL,
      PAINTSIZEMODE_STRETCH,
      PAINTSIZEMODE_FITSIDES,
      PAINTSIZEMODE_FIT,
      PAINTSIZEMODE_ZOOM,
   };

   enum DrawingFillStyleConstants {
      DRAWFILLSTYLE_SOLID = 0, 
      DRAWFILLSTYLE_TRANSPARENT = 1, 
      DRAWFILLSTYLE_HORIZONTAL_LINE = 2, 
      DRAWFILLSTYLE_VERTICAL_LINE = 3, 
      DRAWFILLSTYLE_UPWARD_DIAGONAL = 4, 
      DRAWFILLSTYLE_DOWNWARD_DIAGONAL = 5, 
      DRAWFILLSTYLE_CROSS = 6, 
      DRAWFILLSTYLE_DIAGONAL_CROSS = 7, 
      DRAWFILLSTYLE_DIAGONAL_LAST = 7,
   };

   enum DrawingPenStyleConstants {
      DRAWPENSTYLE_SOLID = 0, 
      DRAWPENSTYLE_DASH = 1, 
      DRAWPENSTYLE_DOT  = 2, 
      DRAWPENSTYLE_DASH_DOT = 3,  
      DRAWPENSTYLE_DASH_DOT_DOT = 4, 
      DRAWPENSTYLE_TRANSPARENT = 5, 
      DRAWPENSTYLE_INSIDE_SOLID = 6, 
      DRAWPENSTYLE_LAST = 6,
   };

   enum DrawingModeConstants {
      DRAWMODE_BLACKNESS = 1, 
      DRAWMODE_NOT_MERGE_PEN = 2, 
      DRAWMODE_MASK_NOT_PEN = 3, 
      DRAWMODE_NOT_COPY_PEN = 4, 
      DRAWMODE_MASK_PEN_NOT = 5, 
      DRAWMODE_INVERT = 6, 
      DRAWMODE_XOR_PEN = 7, 
      DRAWMODE_NOT_MASK_PEN = 8, 
      DRAWMODE_MASK_PEN = 9, 
      DRAWMODE_NOT_XOR_PEN = 10, 
      DRAWMODE_NOP = 11, 
      DRAWMODE_MERGE_NOT_PEN = 12, 
      DRAWMODE_COPY_PEN = 13, 
      DRAWMODE_MERGE_PEN_NOT = 14, 
      DRAWMODE_MERGE_PEN = 15, 
      DRAWMODE_WHITENESS = 16, 
      DRAWMODE_LAST = 16,
   };

   enum BitonalScalingConstants {
      BITONALSCALING_NORMAL,
      BITONALSCALING_FAVORBLACK,
      BITONALSCALING_SCALETOGRAY
   };

   enum PaintScalingConstants {
      PAINTSCALING_NORMAL,
      PAINTSCALING_RESAMPLE,
      PAINTSCALING_BICUBIC,
   };

   enum PaintDitherConstants {
      PAINTDITHER_ORDERED,
      PAINTDITHER_DIFFUSION
   };

   enum PaintPaletteConstants {
      PAINTPALETTE_AUTO,
      PAINTPALETTE_FIXED,
      PAINTPALETTE_AUTONETSCAPE,
      PAINTPALETTE_FIXEDNETSCAPE,
   };

   enum dbEditModeConstants {
      DB_EDITMODE_NONE,
      DB_EDITMODE_EDIT,
      DB_EDITMODE_ADDNEW
   };

   enum dbOpenOptionsConstants {
      DB_OPENOPTIONS_NONE = 0,
      DB_OPENOPTIONS_READONLY = 4,
      DB_OPENOPTIONS_APPENDONLY = 8,
   };

   enum dbLockingModeConstants {
      DB_LOCKINGMODE_OPTIMISTIC,
      DB_LOCKINGMODE_PESSIMISTIC
   };

   enum RgnMarkingModeConstants {
      RGNMARK_NONE,
      RGNMARK_RECT,
      RGNMARK_ROUNDRECT,
      RGNMARK_ELLIPSE,
      RGNMARK_FREEHAND,
    };

   enum RgnFrameTypeConstants {
      RGNFRAME_NONE,
      RGNFRAME_STATIC,
      RGNFRAME_ANIMATED,
      RGNFRAME_COLOR,
      RGNFRAME_MAX
   };

   enum AutoAnimationLoopConstants {
      AUTOANIMATIONLOOP_DEFAULT,
      AUTOANIMATIONLOOP_INFINITE,
   };

   enum AppearanceConstants {
       APPEARANCE_FLAT,
       APPEARANCE_THREED,
   };

   enum VersionLevelConstants {
      VERSIONLEVEL_DOCUMENT = 0,
      VERSIONLEVEL_RASTER = 1,
      VERSIONLEVEL_MEDICAL = 2,
   };

#ifndef WINDOWLEVELFILLLUT_CONSTANTS
#define WINDOWLEVELFILLLUT_CONSTANTS
   enum WindowLevelFillLUTConstants {
         FILLLUT_INSIDE	      = 0x0001, 
         FILLLUT_OUTSIDE      = 0x0002, 

         FILLLUT_LINEAR	      = 0x0010,
         FILLLUT_EXPONENTIAL  = 0x0020,
         FILLLUT_LOGARITHMIC  = 0x0030,
         FILLLUT_SIGMOID      = 0x0040,
         
         FILLLUT_UNSIGNED     = 0x0000,
         FILLLUT_SIGNED       = 0x0100,
      };
#endif

#ifndef CROSSHAIR_CONSTANTS
#define CROSSHAIR_CONSTANTS
   enum  CrosshairConstants{
      CROSSHAIR_NONE         = 0,
      CROSSHAIR_FINE         = 1,
      CROSSHAIR_INVERTPEN    = 2,
      CROSSHAIR_INVERTSCREEN = 3,
   };
#endif

#ifndef MAGGLASS_FLAGS_CONSTANTS
#define MAGGLASS_FLAGS_CONSTANTS
   enum  MagGlassFlagsConstants{
      MAGGLASS_MASK_NORMAL       = 0x0000,
      MAGGLASS_MASK_INVERT       = 0x0001,
      MAGGLASS_MANUAL_UPDATE     = 0x0002,
      MAGGLASS_MASK_FROM_REGION  = 0x0004,
   };
#endif

#ifndef MAGGLASS_SHAPE_CONSTANTS
#define MAGGLASS_SHAPE_CONSTANTS
   enum  MagGlassShapeConstants{
         MAGGLASS_SHAPE_RECT    = 0,
         MAGGLASS_SHAPE_ELLIPSE = 1,
         MAGGLASS_SHAPE_HRGN    = 2,
   };
#endif

#ifndef ANN_NEW_OPTIONS_FLAGS
#define ANN_NEW_OPTIONS_FLAGS
   enum AnnNewOptionsFlags{
         OPTIONS_NEW_STAMP_METAFILES  = 0x0001,
         OPTIONS_NEW_TOOLBAR          = 0x0002,
         OPTIONS_NEW_ALPHA            = 0x0004,
         OPTIONS_NEW_ROTATE           = 0x0008,
         OPTIONS_NEW_SIDE_HANDLES     = 0x0010,
         OPTIONS_NEW_MULTI_SELECT     = 0x0020,
         OPTIONS_NEW_CURSORS          = 0x0040,
         OPTIONS_NEW_ESC_CANCEL       = 0x0080,
         OPTIONS_NEW_CALIBRATE_RULER  = 0x0100,
         OPTIONS_NEW_DOT_DASH_LINES   = 0x0200,
         OPTIONS_NEW_TEXT_OPTIONS     = 0x0400,
         OPTIONS_NEW_ALL              = 0x07ff,
   } ;
#endif


/*********************


  ******************************/


/* this macro that can be used in Combine method to copy data from the
source to the destination */
#ifndef L_SRC_COPY
   #define L_SRC_COPY (CB_DST_0 | CB_OP_OR)
#endif

   enum  PanWinFlagConstants{
      PANWIN_CREATED   = 0,
      PANWIN_UPDATED   = 1,
      PANWIN_DESTROYED = 2
   };

#endif //_L_OCX_H
