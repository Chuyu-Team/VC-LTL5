﻿/***
*mbtolwr.c - Convert character to lower case (MBCS).
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Convert character to lower case (MBCS).
*
*******************************************************************************/
#ifndef _MBCS
    #error This file should only be compiled with _MBCS defined
#endif

#include <corecrt_internal_mbstring.h>
#include <locale.h>


/***
* _mbctolower - Convert character to lower case (MBCS)
*
*Purpose:
*       If the given character is upper case, convert it to lower case.
*       Handles MBCS characters correctly.
*
*       Note:  Use test against 0x00FF instead of _ISLEADBYTE
*       to ensure that we don't call SBCS routine with a two-byte
*       value.
*
*Entry:
*       unsigned int c = character to convert
*
*Exit:
*       Returns converted character
*
*Exceptions:
*
*******************************************************************************/

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" unsigned int __cdecl _mbctolower_l (
        unsigned int c,
        _locale_t plocinfo
        )
{
		if (!plocinfo)
			return _mbctolower(c);

        unsigned char val[2];
        unsigned char ret[4];
        //_LocaleUpdate _loc_update(plocinfo);

        if (c > 0x00FF)
        {
            val[0] = (c >> 8) & 0xFF;
            val[1] = c & 0xFF;

            if ( !_ismbblead_l(val[0], plocinfo) )
                return c;


            if ( __acrt_LCMapStringA(
                        plocinfo,
                        plocinfo->mbcinfo->mblcid,
                        LCMAP_LOWERCASE,
                        (const char *)val,
                        2,
                        (char *)ret,
                        2,
                        plocinfo->mbcinfo->mbcodepage,
                        TRUE ) == 0 )
                return c;

            c = ret[1];
            c += ret[0] << 8;

            return c;


        }
        else
            return (unsigned int)_mbbtolower_l((int)c, plocinfo);
}

_LCRT_DEFINE_IAT_SYMBOL(_mbctolower_l);
#endif

#if 0
extern "C" unsigned int (__cdecl _mbctolower) (
        unsigned int c
        )
{
    return _mbctolower_l(c, nullptr);
}
#endif
