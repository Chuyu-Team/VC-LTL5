﻿//
// wmemmove_s.cpp
//
//      Copyright (c) Microsoft Corporation.  All rights reserved.
//
// Defines wmemmove_s(), which copies wide characters from a source buffer to a
// destination buffer.  Overlapping buffers are treated specially, to avoid
// propagation.
//
// Returns 0 if successful; an error code on failure.
//
#include <corecrt_internal.h>
#include <wchar.h>


#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" errno_t __cdecl wmemmove_s(
    wchar_t*       const destination,
    size_t         const size_in_elements,
    wchar_t const* const source,
    size_t         const count
    )
{
    if (count == 0)
        return 0;

#pragma warning(suppress:__WARNING_HIGH_PRIORITY_OVERFLOW_POSTCONDITION)
    _VALIDATE_RETURN_ERRCODE(destination != nullptr, EINVAL);
    _VALIDATE_RETURN_ERRCODE(source != nullptr, EINVAL);
    _VALIDATE_RETURN_ERRCODE(size_in_elements >= count, ERANGE);

#pragma warning(suppress:__WARNING_BANNED_API_USAGEL2 __WARNING_BUFFER_COPY_NO_KNOWN_SIZEEXPR) /* 28726 22104 */
    wmemmove(destination, source, count);
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(wmemmove_s);
#endif