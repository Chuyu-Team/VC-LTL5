﻿//
// _mbslen.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Defines the mbstrlen functions, which return the number of multibyte
// characters in a multibyte string, excluding the null terminator.  This
// function is locale-dependent.  If the string contains invalid multibyte
// characters, -1 is returned and errno is set to EILSEQ.
//
// There are two variations of these functions:  mbstrnlen (note the 'n'),
// which takes a max_size, which is the maximum number of bytes to scan
// in the string.  These variations also validate their arguments.  The
// variations without the 'n' (mbstrlen) stop only when the null terminator
// is reached and do not validate their arguments.
//
#include <corecrt_internal.h>
#include <stdlib.h>
#include <locale.h>




_Check_return_
_Post_satisfies_((return <= _String_length_(string) && return <= max_size) || return == (size_t)-1)
static size_t __cdecl common_mbstrlen_l(
    char const* const string,
    size_t      const max_size,
    _locale_t   const locale
    )
{
    //_LocaleUpdate locale_update(locale);
	int _locale_mb_cur_max;
	unsigned int _locale_lc_codepage;
	if (locale)
	{
		_locale_mb_cur_max = locale->locinfo->_locale_mb_cur_max;
		_locale_lc_codepage = locale->locinfo->_locale_lc_codepage;
	}
	else
	{
		_locale_mb_cur_max = __mb_cur_max;
		_locale_lc_codepage = ___lc_codepage_func();
	}

    _ASSERTE(
        _locale_mb_cur_max == 1 ||
        _locale_mb_cur_max == 2);

    // Handle single byte character sets:
    if (_locale_mb_cur_max == 1)
    {
        return strnlen(string, max_size);
    }

    // Verify that all of the multibyte characters are valid:
    if (__acrt_MultiByteToWideChar(
            _locale_lc_codepage,
            MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
            string,
            static_cast<int>(max_size),
            nullptr,
            0
        ) == 0)
    {
        // There was a bad multibyte character:
        errno = EILSEQ;
        return static_cast<size_t>(-1);
    }

    // Count multibyte characters:
    size_t n    = 0; // Number of multibyte characters read
    size_t size = 0; // Number of bytes read
    for (char const* it = string; size < max_size && *it; ++n, ++it, ++size)
    {
        if (_isleadbyte_fast_internal(static_cast<unsigned char>(*it), locale))
        {
            ++size;
            if (size >= max_size)
                break;

            ++it;
            if (*it == '\0')
                break;
        }
    }

    return size >= max_size ? max_size : n;
}


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" size_t __cdecl _mbstrlen_l(
    char const* const string,
    _locale_t   const locale
    )
{
    return common_mbstrlen_l(string, _CRT_UNBOUNDED_BUFFER_SIZE, locale);
}

_LCRT_DEFINE_IAT_SYMBOL(_mbstrlen_l);
#endif

#if 0
extern "C" size_t __cdecl _mbstrlen(char const* const string)
{
    if (!__acrt_locale_changed())
    {
        return strlen(string);
    }
    else
    {
        return _mbstrlen_l(string, nullptr);
    }
}
#endif


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" size_t __cdecl _mbstrnlen_l(
    char const* const string,
    size_t      const max_size,
    _locale_t   const locale
    )
{
    _VALIDATE_RETURN(string != nullptr,   EINVAL, static_cast<size_t>(-1));
    _VALIDATE_RETURN(max_size <= INT_MAX, EINVAL, static_cast<size_t>(-1));

    return common_mbstrlen_l(string, max_size, locale);
}

_LCRT_DEFINE_IAT_SYMBOL(_mbstrnlen_l);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" size_t __cdecl _mbstrnlen(
    char const* const string,
    size_t      const max_size
    )
{
    return _mbstrnlen_l(string, max_size, nullptr);
}

_LCRT_DEFINE_IAT_SYMBOL(_mbstrnlen);
#endif