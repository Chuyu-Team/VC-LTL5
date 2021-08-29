//
// llabs.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Defines llabs(), which computes the absolute value of a number.
//
#include <stdlib.h>



#pragma function(llabs)


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
extern "C" long long __cdecl llabs(long long const number)
{
	return number >= 0 ? number : -number;
}

_LCRT_DEFINE_IAT_SYMBOL(llabs);
#endif