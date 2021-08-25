//
// abs.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Defines abs() and _abs64(), which compute the absolute value of a number.
//
#include <stdlib.h>



#pragma function(abs, _abs64)


#if 0
extern "C" int __cdecl abs(int const number)
{
    return number >= 0 ? number : -number;
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
extern "C" __int64 __cdecl _abs64(__int64 const number)
{
    return number >= 0 ? number : -number;
}

_LCRT_DEFINE_IAT_SYMBOL(_abs64);

#endif