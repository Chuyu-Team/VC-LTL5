﻿//
// wmemcpy_s.cpp
//
//      Copyright (c) Microsoft Corporation.  All rights reserved.
//
// Defines wmemcpy_s(), which copies wide characters from a source buffer to a
// destination buffer.  Overlapping buffers are not treated specially, so
// propagation may occur.  If propagation must be avoided, wmemmove_s() must be
// used.
//
// Returns 0 if successful; an error code on failure.
//
#include <corecrt_internal.h>
#include <wchar.h>


#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" errno_t __cdecl wmemcpy_s(
    wchar_t*       const destination,
    size_t         const size_in_elements,
    wchar_t const* const source,
    size_t         const count
    )
{
    if (count == 0)
        return 0;

    _VALIDATE_RETURN_ERRCODE(destination != nullptr, EINVAL);

    if (source == nullptr || size_in_elements < count)
    {
        // Zero the destination buffer:
        wmemset(destination, 0, size_in_elements);

        _VALIDATE_RETURN_ERRCODE(source != nullptr, EINVAL);
        _VALIDATE_RETURN_ERRCODE(size_in_elements >= count, ERANGE);
        
        // Useless, but prefast is confused:
        return EINVAL;
    }

#pragma warning(suppress:__WARNING_BANNED_API_USAGEL2 __WARNING_BUFFER_COPY_NO_KNOWN_SIZEEXPR) /* 28726 22104 */
    wmemcpy(destination, source, count);
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(wmemcpy_s);

#endif