//
// ncommode.cpp
//
//      Copyright (c) Microsoft Corporation.  All rights reserved.
//
// Defines _commode, which sets the default file commit mode to nocommit.
//
#include <corecrt_internal_stdio.h>


#undef _commode

// Set default file commit mode to nocommit
namespace LTL
{
	extern "C" __declspec(dllimport) extern int _commode;
}

#if defined _M_AMD64 || defined _M_ARM64 || defined _M_ARM

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
extern "C" int* __cdecl __p__commode()
{
    return &LTL::_commode;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__commode);

#endif

#endif
