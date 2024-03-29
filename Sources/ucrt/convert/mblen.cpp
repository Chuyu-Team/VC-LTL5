﻿//
// mblen.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// The mblen() and _mblen_l() functions, which return the number of bytes
// contained in a multibyte character.
//
#include <corecrt_internal_mbstring.h>

using namespace __crt_mbstring;

// Computes the number of bytes contained in a multibyte character.  If the string
// is null, zero is returned to indicate that we support only state-independent
// character encodings.  If the next max_count bytes of the string are not a valid
// multibyte character, -1 is returned.  Otherwise, the number of bytes that
// compose the next multibyte character are returned.
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int __cdecl _mblen_l(
    char const* const string,
    size_t      const max_count,
    _locale_t   const locale
    )
{
    mbstate_t internal_state{};
    if (!string || *string == '\0' || max_count == 0)
    {
        internal_state = {};
        return 0;
    }

    //_LocaleUpdate locale_update(locale);
	if (!locale)
		return mblen(string, max_count);

    if (locale->locinfo->_locale_lc_codepage == CP_UTF8)
    {
        int result = static_cast<int>(__mbrtowc_utf8(nullptr, string, max_count, &internal_state));
        if (result < 0)
            result = -1;
        return result;
    }

    _ASSERTE(
        locale->locinfo->_public._locale_mb_cur_max == 1 ||
        locale->locinfo->_public._locale_mb_cur_max == 2);

    if (_isleadbyte_l(static_cast<unsigned char>(*string), locale))
    {
        _ASSERTE(locale->locinfo->_public._locale_lc_codepage != CP_UTF8 && L"UTF-8 isn't supported in this _mblen_l function yet!!!");

        // If this is a lead byte, then the codepage better be a multibyte codepage
        _ASSERTE(locale->locinfo->_public._locale_mb_cur_max > 1);

        // Multi-byte character; verify that it is valid:
        if (locale->locinfo->_locale_mb_cur_max <= 1)
            return -1;

        if (max_count > INT_MAX || static_cast<int>(max_count) < locale->locinfo->_locale_mb_cur_max)
            return -1;

        int const status = __acrt_MultiByteToWideChar(
            locale->locinfo->_locale_lc_codepage,
            MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            string,
            locale->locinfo->_locale_mb_cur_max,
            nullptr,
            0);

        if (status == 0)
            return -1;

        return locale->locinfo->_locale_mb_cur_max;
    }
    else
    {
        // Single byte character; verify that it is valid:
        // CP_ACP is known to be valid for all values
        if (locale->locinfo->_locale_lc_codepage != CP_ACP)
        {
            int const status = __acrt_MultiByteToWideChar(
                locale->locinfo->_locale_lc_codepage,
                MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
                string,
                1,
                nullptr,
                0);

            if (status == 0)
                return -1;
        }

        return sizeof(char);
    }
}

_LCRT_DEFINE_IAT_SYMBOL(_mblen_l);
#endif


#if 0
extern "C" int __cdecl mblen(
    char const* const string,
    size_t      const max_count
    )
{
    if (!__acrt_locale_changed())
    {
        return _mblen_l(string, max_count, &__acrt_initial_locale_pointers);
    }
    else
    {
        return _mblen_l(string, max_count, nullptr);
    }
}
#endif