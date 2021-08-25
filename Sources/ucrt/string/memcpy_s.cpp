//
// memcpy_s.cpp
//
//      Copyright (c) Microsoft Corporation.  All rights reserved.
//
// Provides external definitions of the inline functions memcpy_s and memmove_s
// for use by objects compiled with older versions of the CRT headers.
//
#define _CRT_MEMCPY_S_INLINE extern __inline
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
#include <corecrt_memcpy_s.h>

_LCRT_DEFINE_IAT_SYMBOL(memcpy_s);
_LCRT_DEFINE_IAT_SYMBOL(memmove_s);
#endif
