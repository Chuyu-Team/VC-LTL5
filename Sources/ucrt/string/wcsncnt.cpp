//
// wcsncnt.cpp
//
//      Copyright (c) Microsoft Corporation.  All rights reserved.
//
// Defines __wcsncnt(), which returns the number of characters in a string.  If
// the string is longer than the given 'count', 'count' is returned.
//
#include <string.h>



#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" size_t __cdecl __wcsncnt(
    wchar_t const* const string,
    size_t         const count
    )
{
    wchar_t const* it = string;
    size_t n = 0;

    for (; *it && n != count; ++it, ++n) { }

    return n;
}

_LCRT_DEFINE_IAT_SYMBOL(__wcsncnt);
#endif
