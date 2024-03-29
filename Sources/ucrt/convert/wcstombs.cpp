﻿/***
*wcstombs.c - Convert wide char string to multibyte char string.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert a wide char string into the equivalent multibyte char string.
*
*******************************************************************************/
#include <corecrt_internal_mbstring.h>
#include <corecrt_internal_securecrt.h>
#include <ctype.h>
#include <locale.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

/***
*size_t __cdecl wcsncnt - count wide characters in a string, up to n.
*
*Purpose:
*       Internal local support function. Counts characters in string including nullptr.
*       If nullptr not found in n chars, then return n.
*
*Entry:
*       const wchar_t *string   - start of string
*       size_t n                - character count
*
*Exit:
*       returns number of wide characters from start of string to
*       nullptr (inclusive), up to n.
*
*Exceptions:
*
*******************************************************************************/

_Post_satisfies_(return <= cnt && return <= _String_length_(string + 1))
static size_t __cdecl wcsncnt (
        _In_reads_or_z_(cnt) const wchar_t *string,
        _In_                 size_t cnt
        )
{
        size_t n = cnt+1;
        wchar_t *cp = (wchar_t *)string;

        while (--n && *cp)
            cp++;

        if (n && !*cp)
            return cp - string + 1;
        return cnt;
}

/***
*size_t wcstombs() - Convert wide char string to multibyte char string.
*
*Purpose:
*       Convert a wide char string into the equivalent multibyte char string,
*       according to the LC_CTYPE category of the current locale.
*       [ANSI].
*
*       The destination string is null terminated only if the null terminator
*       is copied from the source string.
*
*Entry:
*       char *s            = pointer to destination multibyte char string
*       const wchar_t *pwc = pointer to source wide character string
*       size_t           n = maximum number of bytes to store in s
*
*Exit:
*       If s != nullptr, returns    (size_t)-1 (if a wchar cannot be converted)
*       Otherwise:       Number of bytes modified (<=n), not including
*                    the terminating NUL, if any.
*
*Exceptions:
*       Returns (size_t)-1 if an error is encountered.
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

_Success_(return != static_cast<size_t>(-1))
static size_t __cdecl _wcstombs_l_helper (
        _Out_writes_(n) char * s,
        _In_z_          const wchar_t * pwcs,
        _In_            size_t n,
        _In_opt_        _locale_t plocinfo
        )
{
    size_t count = 0;
    int i, retval;
    char buffer[MB_LEN_MAX];
    BOOL defused = 0;

    if (s && n == 0)
        /* dest string exists, but 0 bytes converted */
        return 0;

    /* validation section */
    _VALIDATE_RETURN(pwcs != nullptr, EINVAL, (size_t)-1);


    /* if destination string exists, fill it in */

    //_LocaleUpdate _loc_update(plocinfo);
	LCID locale;
	int _locale_mb_cur_max;
	int _locale_lc_codepage;

	if (plocinfo)
	{
		locale = plocinfo->locinfo->lc_handle[LC_CTYPE];
		_locale_mb_cur_max = plocinfo->locinfo->_locale_mb_cur_max;
		_locale_lc_codepage = plocinfo->locinfo->_locale_lc_codepage;
	}
	else
	{
		locale = ___lc_handle_func()[LC_CTYPE];
		_locale_mb_cur_max = ___mb_cur_max_func();

		_locale_lc_codepage = ___lc_codepage_func();
	}

    if (_locale_lc_codepage == CP_UTF8)
    {
        mbstate_t state{};
        return __crt_mbstring::__wcsrtombs_utf8(s, &pwcs, n, &state);
    }

    if (s)
    {
        if ( locale == 0 )
        {
            /* C locale: easy and fast */
            /* Actually, there are such wchar_t characters which are > 255,
             * but they can be transformed to a valid single byte char
             * (i.e. a char in the C locale case). Like, for example,
             * alternative digits in unicode like Arabic-Indic U+0660..U+0669.
             * The problem is that WideCharToMultiByte() does not translate those
             * wchar_t unless we pass the correct codepage (1256, Arabic).
             * See bug VSW:192653.
             */
            while(count < n)
            {
                if (static_cast<uint16_t>(*pwcs) > 0xFF)  /* validate high byte */
                {
                    errno = EILSEQ;
                    return (size_t)-1;  /* error */
                }
                s[count] = (char) *pwcs;
                if (*pwcs++ == L'\0')
                    return count;
                count++;
            }
            return count;
        } else {

            if (1 == _locale_mb_cur_max)
            {
                /* If SBCS, one wchar_t maps to one char */

                /* WideCharToMultiByte will compare past nullptr - reset n */
                if (n > 0)
                    n = wcsncnt(pwcs, n);
                if ( ((count = __acrt_WideCharToMultiByte( _locale_lc_codepage,
                                                    0,
                                                    pwcs,
                                                    (int)n,
                                                    s,
                                                    (int)n,
                                                    nullptr,
                                                    &defused )) != 0) &&
                     (!defused) )
                {
                    if (s[count - 1] == '\0') {
                        count--; /* don't count NUL */
                    }

                    return count;
                }

                errno = EILSEQ;
                return (size_t)-1;
            }
            else {

                /* If MBCS, wchar_t to char mapping unknown */

                /* Assume that usually the buffer is large enough */
                if ( ((count = __acrt_WideCharToMultiByte( _locale_lc_codepage,
                                                    0,
                                                    pwcs,
                                                    -1,
                                                    s,
                                                    (int)n,
                                                    nullptr,
                                                    &defused )) != 0) &&
                     (!defused) )
                {
                    return count - 1; /* don't count NUL */
                }

                if (defused || GetLastError() != ERROR_INSUFFICIENT_BUFFER)
                {
                    errno = EILSEQ;
                    return (size_t)-1;
                }

                /* buffer not large enough, must do char by char */
                while (count < n)
                {
                    int mb_cur_max = _locale_mb_cur_max;
                    if ( ((retval = __acrt_WideCharToMultiByte( _locale_lc_codepage,
                                                         0,
                                                         pwcs,
                                                         1,
                                                         buffer,
                                                         __min(MB_LEN_MAX, mb_cur_max),
                                                         nullptr,
                                                         &defused )) == 0)
                         || defused )
                    {
                        errno = EILSEQ;
                        return (size_t)-1;
                    }

                    /* enforce this for prefast */
                    if (retval < 0 ||
                        retval > _countof(buffer))
                    {
                        errno = EILSEQ;
                        return (size_t)-1;
                    }

                    if (count + retval > n)
                        return count;

                    for (i = 0; i < retval; i++, count++) /* store character */
                        if((s[count] = buffer[i])=='\0')
                            return count;

                    pwcs++;
                }

                return count;
            }
        }
    }
    else { /* s == nullptr, get size only, pwcs must be NUL-terminated */
        if (locale==0)
        {
            size_t len = 0;
            for (wchar_t *pw = (wchar_t *)pwcs; *pw != 0; pw++)  /* validate high byte */
            {
                if (*pw > 255)  /* validate high byte */
                {
                    errno = EILSEQ;
                    return (size_t)-1;  /* error */
                }
                ++len;
            }

            return len;
        }
        else {
            if ( ((count = __acrt_WideCharToMultiByte( _locale_lc_codepage,
                                                0,
                                                pwcs,
                                                -1,
                                                nullptr,
                                                0,
                                                nullptr,
                                                &defused )) == 0) ||
                 (defused) )
            {
                errno = EILSEQ;
                return (size_t)-1;
            }

            return count - 1;
        }
    }

}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" size_t __cdecl _wcstombs_l (
        char * s,
        const wchar_t * pwcs,
        size_t n,
        _locale_t plocinfo
        )
{
    return _wcstombs_l_helper(s, pwcs, n, plocinfo);
}

_LCRT_DEFINE_IAT_SYMBOL(_wcstombs_l);
#endif

#if 0
extern "C" size_t __cdecl wcstombs (
        char * s,
        const wchar_t * pwcs,
        size_t n
        )
{
    return _wcstombs_l_helper(s, pwcs, n, nullptr);
}
#endif

/***
*errno_t wcstombs_s() - Convert wide char string to multibyte char string.
*
*Purpose:
*       Convert a wide char string into the equivalent multibyte char string,
*       according to the LC_CTYPE category of the current locale.
*
*       The destination string is always null terminated.
*
*Entry:
*       size_t *pConvertedChars = Number of bytes modified including the terminating nullptr
*                                 This pointer can be nullptr.
*       char *dst = pointer to destination multibyte char string
*       size_t sizeInBytes = size of the destination buffer
*       const wchar_t *src = pointer to source wide character string
*       size_t n = maximum number of bytes to store in s (not including the terminating nullptr)
*
*Exit:
*       The error code.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wcstombs_s_l (
        size_t *pConvertedChars,
        char * dst,
        size_t sizeInBytes,
        const wchar_t * src,
        size_t n,
        _locale_t plocinfo
        )
{
    size_t retsize;
    errno_t retvalue = 0;

    /* validation section */
    _VALIDATE_RETURN_ERRCODE((dst != nullptr && sizeInBytes > 0) || (dst == nullptr && sizeInBytes == 0), EINVAL);
    if (dst != nullptr)
    {
        _RESET_STRING(dst, sizeInBytes);
    }

    if (pConvertedChars != nullptr)
    {
        *pConvertedChars = 0;
    }

    size_t bufferSize = n > sizeInBytes ? sizeInBytes : n;
    _VALIDATE_RETURN_ERRCODE(bufferSize <= INT_MAX, EINVAL);

    retsize = _wcstombs_l_helper(dst, src, bufferSize, plocinfo);

    if (retsize == (size_t)-1)
    {
        if (dst != nullptr)
        {
            _RESET_STRING(dst, sizeInBytes);
        }
        return errno;
    }

    /* count the null terminator */
    retsize++;

    if (dst != nullptr)
    {
        /* return error if the string does not fit, unless n == _TRUNCATE */
        if (retsize > sizeInBytes)
        {
            if (n != _TRUNCATE)
            {
                _RESET_STRING(dst, sizeInBytes);
                _VALIDATE_RETURN_ERRCODE(sizeInBytes > retsize, ERANGE);
            }
            retsize = sizeInBytes;
            retvalue = STRUNCATE;
        }

        /* ensure the string is null terminated */
        dst[retsize - 1] = '\0';
    }

    if (pConvertedChars != nullptr)
    {
        *pConvertedChars = retsize;
    }

    return retvalue;
}

_LCRT_DEFINE_IAT_SYMBOL(_wcstombs_s_l);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl wcstombs_s (
        size_t *pConvertedChars,
        char * dst,
        size_t sizeInBytes,
        const wchar_t * src,
        size_t n
        )
{
    return _wcstombs_s_l(pConvertedChars, dst, sizeInBytes, src, n, nullptr);
}

_LCRT_DEFINE_IAT_SYMBOL(wcstombs_s);
#endif
