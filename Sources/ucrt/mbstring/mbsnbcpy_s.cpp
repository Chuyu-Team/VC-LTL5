/***
*mbsnbcpy_s.c - Copy one string to another, n bytes only (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Copy one string to another, n bytes only (MBCS)
*
*******************************************************************************/
#ifndef _MBCS
    #error This file should only be compiled with _MBCS defined
#endif

#include <corecrt_internal_mbstring.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
_REDIRECT_TO_L_VERSION_4(errno_t, _mbsnbcpy_s, unsigned char *, size_t, const unsigned char *, size_t)

_LCRT_DEFINE_IAT_SYMBOL(_mbsnbcpy_s);
#endif