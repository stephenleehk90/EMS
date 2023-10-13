/*************************************************************
   Ltver.h - LEADTOOLS version definition
   Copyright (c) 1991-2010 LEAD Technologies, Inc.
   All Rights Reserved.
*************************************************************/

#if !defined(LTVER_H)
#define LTVER_H

#if defined(LTV15_CONFIG)
#define LTVER_   1500
#define L_VER_DESIGNATOR
#elif defined(LTV16_CONFIG)
#define LTVER_   1600
#define L_VER_DESIGNATOR
#elif defined(LTV17_CONFIG)
#define LTVER_   1700
#define L_VER_DESIGNATOR
#else
#error LEADTOOLS Vxx_CONFIG not found!
#endif // #if defined(LTV15_CONFIG)

#if (_MSC_VER == 1600)
#define L_PLATFORM_DESIGNATOR "CDLLVC10"
#else
#define L_PLATFORM_DESIGNATOR "CDLL"
#endif // #if (_MSC_VER == 1600)

#if LTVER_ >= 1600
#define LEADTOOLS_V16_OR_LATER
#endif

#if LTVER_ >= 1700
#define LEADTOOLS_V17_OR_LATER
#endif

#if LTVER_ >= 1750
#define LEADTOOLS_V175_OR_LATER
#endif

#endif // #if !defined(LTVER_H)
