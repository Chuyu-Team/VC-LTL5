﻿//
// c16rtomb.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//

#include <uchar.h>
#include <corecrt_internal_mbstring.h>

using namespace __crt_mbstring;

namespace
{
    inline void store_first_surrogate(char16_t c16, mbstate_t* ps)
    {
        // Store the first (high) surrogate masked and shifted, so that all that's left to do
        // is to add the maksed second surrogate.
        // Note: _Wchar is the only field used in mbstate_t for this function
        const char32_t c32 = ((c16 & ~0xd800) << 10) + 0x10000;
        ps->_Wchar = c32;
    }

    inline char32_t combine_second_surrogate(char16_t c16, mbstate_t* ps)
    {
        return ps->_Wchar + (c16 & ~0xdc00);
    }

    inline constexpr bool has_surrogate(mbstate_t* ps)
    {
        return ps->_Wchar != 0;
    }

    inline constexpr bool is_first_surrogate(char16_t c16)
    {
        // UTF-16 high surrogate
        return 0xd800 <= c16 && c16 <= 0xdbff;
    }

    inline constexpr bool is_second_surrogate(char16_t c16)
    {
        // UTF-16 low surrogate
        return 0xdc00 <= c16 && c16 <= 0xdfff;
    }
}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
extern "C" size_t __cdecl c16rtomb(char* s, char16_t c16, mbstate_t* ps)
{
    // TODO: Bug 13307590 says this is always assuming UTF-8.
    return __c16rtomb_utf8(s, c16, ps);
}

_LCRT_DEFINE_IAT_SYMBOL(c16rtomb);
#endif

size_t __cdecl __crt_mbstring::__c16rtomb_utf8(char* s, char16_t c16, mbstate_t* ps)
{
    static mbstate_t internal_pst{};
    if (ps == nullptr)
    {
        ps = &internal_pst;
    }

    if (!has_surrogate(ps))
    {
        if (is_second_surrogate(c16))
        {
            return return_illegal_sequence(ps);
        }
        else if (is_first_surrogate(c16))
        {
            store_first_surrogate(c16, ps);
            return 0;
        }
        else
        {
            return c32rtomb(s, static_cast<char32_t>(c16), ps);
        }
    }
    else
    {
        // We've already seen the first (high) surrogate, so we're
        // expecting to complete this code point with its other half
        // in the second (low) surrogate
        if (!is_second_surrogate(c16))
        {
            return return_illegal_sequence(ps);
        }
        const char32_t c32 = combine_second_surrogate(c16, ps);

        mbstate_t temp{};
        const size_t retval = c32rtomb(s, c32, &temp);
        return reset_and_return(retval, ps);
    }
}

