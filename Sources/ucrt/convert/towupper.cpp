/***
*towupper.cpp - convert wide character to upper case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines towupper().
*
*******************************************************************************/
#include <corecrt_internal.h>
#include <ctype.h>
#include <locale.h>

/***
*wint_t _towupper_l(c, ptloci) - convert wide character to upper case
*
*Purpose:
*       Multi-thread function only! Non-locking version of towupper.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" wint_t __cdecl _towupper_l (
        wint_t c,
        _locale_t plocinfo
        )
{
    wint_t widechar;

    if (c == WEOF)
    {
        return c;
    }

	if (!plocinfo)
		return towupper(c);

    //_LocaleUpdate _loc_update(plocinfo);

    /* if checking case of c does not require API call, do it */
    if ( c < 256 )
    {
        return _towupper_fast_internal(static_cast<unsigned char>(c), plocinfo);
    }

    if ( plocinfo->locinfo->lc_handle[LC_CTYPE] == 0 )
    {
        /* If the locale is C, then the only characters that would be transformed are <256
           and have been processed already. */
        return c;
    }

    /* convert wide char to uppercase */
    if ( 0 == __acrt_LCMapStringW(
                plocinfo->locinfo->lc_handle[LC_CTYPE],
                LCMAP_UPPERCASE,
                (LPCWSTR)&c,
                1,
                (LPWSTR)&widechar,
                1 ) )
    {
        return c;
    }

    return widechar;

}

_LCRT_DEFINE_IAT_SYMBOL(_towupper_l);
#endif

/***
*wint_t towupper(c) - convert wide character to upper case
*
*Purpose:
*       towupper() returns the uppercase equivalent of its argument
*
*Entry:
*       c - wint_t value of character to be converted
*
*Exit:
*       if c is a lower case letter, returns wint_t value of upper case
*       representation of c. otherwise, it returns c.
*
*Exceptions:
*
*******************************************************************************/

#if 0
extern "C" wint_t __cdecl towupper (
        wint_t c
        )
{

    return _towupper_l(c, nullptr);
}
#endif
