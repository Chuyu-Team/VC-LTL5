﻿/***
*ismbbyte.c - Function versions of MBCS ctype macros
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       This files provides function versions of the character
*       classification a*d conversion macros in mbctype.h.
*
*******************************************************************************/
#ifndef _MBCS
    #error This file should only be compiled with _MBCS defined
#endif

#include <corecrt_internal_mbstring.h>
#include <locale.h>

/* defined in mbctype.h
; Define masks

; set bit masks for the possible kanji character types
; (all MBCS bit masks start with "_M")

_MS             equ     01h     ; MBCS non-ascii single byte char
_MP             equ     02h     ; MBCS punct
_M1             equ     04h     ; MBCS 1st (lead) byte
_M2             equ     08h     ; MBCS 2nd byte

*/

/* defined in ctype.h
; set bit masks for the possible character types

_UPPER          equ     01h     ; upper case letter
_LOWER          equ     02h     ; lower case letter
_DIGIT          equ     04h     ; digit[0-9]
_SPACE          equ     08h     ; tab, carriage return, newline,
                                ; vertical tab or form feed
_PUNCT          equ     10h     ; punctuation character
_CONTROL        equ     20h     ; control character
_BLANK          equ     40h     ; space char
_HEX            equ     80h     ; hexadecimal digit

*/

/* defined in ctype.h, mbdata.h
        extrn   __mbctype:byte          ; MBCS ctype table
        extrn   __ctype_:byte           ; ANSI/ASCII ctype table
*/


/***
* ismbbyte - Function versions of mbctype macros
*
*Purpose:
*
*Entry:
*       int = character to be tested
*Exit:
*       ax = non-zero = character is of the requested type
*          =        0 = character is NOT of the requested type
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl x_ismbbtype_l(_locale_t plocinfo, unsigned int, int, int) throw();


/* ismbbk functions */

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbkalnum_l) (unsigned int tst, _locale_t plocinfo)
{
    return x_ismbbtype_l(plocinfo,tst,0,_MS);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbkalnum_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbkalnum) (unsigned int tst)
{
    return x_ismbbtype_l(nullptr,tst,0,_MS);
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbkprint_l) (unsigned int tst, _locale_t plocinfo)
{
    return x_ismbbtype_l(plocinfo,tst,0,(_MS | _MP));
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbkprint_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbkprint) (unsigned int tst)
{
    return x_ismbbtype_l(nullptr,tst,0,(_MS | _MP));
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbkpunct_l) (unsigned int tst, _locale_t plocinfo)
{
    return x_ismbbtype_l(plocinfo,tst,0,_MP);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbkpunct_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbkpunct) (unsigned int tst)
{
    return x_ismbbtype_l(nullptr,tst,0,_MP);
}
#endif

/* ismbb functions */
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbalnum_l) (unsigned int tst, _locale_t plocinfo)
{
    return x_ismbbtype_l(plocinfo,tst,(_ALPHA | _DIGIT), _MS);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbalnum_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbalnum) (unsigned int tst)
{
    return x_ismbbtype_l(nullptr,tst,(_ALPHA | _DIGIT), _MS);
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbalpha_l) (unsigned int tst, _locale_t plocinfo)
{
    return x_ismbbtype_l(plocinfo,tst,_ALPHA, _MS);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbalpha_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbalpha) (unsigned int tst)
{
    return x_ismbbtype_l(nullptr,tst,_ALPHA, _MS);
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbgraph_l) (unsigned int tst, _locale_t plocinfo)
{
    return x_ismbbtype_l(plocinfo,tst,(_PUNCT | _ALPHA | _DIGIT),(_MS | _MP));
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbgraph_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbgraph) (unsigned int tst)
{
    return x_ismbbtype_l(nullptr,tst,(_PUNCT | _ALPHA | _DIGIT),(_MS | _MP));
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbprint_l) (unsigned int tst, _locale_t plocinfo)
{
    return x_ismbbtype_l(plocinfo,tst,(_BLANK | _PUNCT | _ALPHA | _DIGIT),(_MS | _MP));
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbprint_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbprint) (unsigned int tst)
{
    return x_ismbbtype_l(nullptr,tst,(_BLANK | _PUNCT | _ALPHA | _DIGIT),(_MS | _MP));
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbpunct_l) (unsigned int tst, _locale_t plocinfo)
{
    return x_ismbbtype_l(plocinfo,tst,_PUNCT, _MP);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbpunct_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbpunct) (unsigned int tst)
{
    return x_ismbbtype_l(nullptr,tst,_PUNCT, _MP);
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
extern "C" int (__cdecl _ismbbblank_l) (unsigned int tst, _locale_t plocinfo)
{
    return (tst == '\t') ? _BLANK : x_ismbbtype_l(plocinfo,tst,_BLANK, _MP);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbblank_l);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
extern "C" int (__cdecl _ismbbblank) (unsigned int tst)
{
    return (tst == '\t') ? _BLANK : x_ismbbtype_l(nullptr,tst,_BLANK, _MP);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbblank);
#endif

/* lead and trail */
// These expect single or double byte codepages and are undefined for UTF-8
// UTF-8 queries will always return false for lead/trail bytes.
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbblead_l) (unsigned int tst, _locale_t plocinfo)
{
    // Note: Lookup is always FALSE for UTF-8
	if (!plocinfo)
		return _ismbblead(tst);

    return x_ismbbtype_l(plocinfo,tst,0,_M1);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbblead_l);
#endif

#if 0
extern "C" int (__cdecl _ismbblead) (unsigned int tst)
{
    // Note: Lookup is always FALSE for UTF-8
    return x_ismbbtype_l(nullptr, tst, 0, _M1);
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbtrail_l) (unsigned int tst, _locale_t plocinfo)
{
    // Note: Lookup is always FALSE for UTF-8
	if (!plocinfo)
		return _ismbbtrail(tst);

    return x_ismbbtype_l(plocinfo,tst,0,_M2);
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbtrail_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbtrail) (unsigned int tst)
{
    // Note: Lookup is always FALSE for UTF-8
    return x_ismbbtype_l(nullptr, tst, 0, _M2);
}
#endif


/* 932 specific */

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int (__cdecl _ismbbkana_l) (unsigned int tst, _locale_t plocinfo)
{
    //_LocaleUpdate _loc_update(plocinfo);
    if (!plocinfo)
		return _ismbbkana(tst);

    if(plocinfo->mbcinfo &&
       plocinfo->mbcinfo->mbcodepage == _KANJI_CP)
    {
        return x_ismbbtype_l(plocinfo,tst,0,(_MS | _MP));
    }
    return FALSE;
}

_LCRT_DEFINE_IAT_SYMBOL(_ismbbkana_l);
#endif

#if 0
extern "C" int (__cdecl _ismbbkana) (unsigned int tst)
{
    return _ismbbkana_l(tst, nullptr);
}
#endif

/***
* Common code
*
*      cmask = mask for _ctype[] table
*      kmask = mask for _mbctype[] table
*
*******************************************************************************/

static int __cdecl x_ismbbtype_l (_locale_t plocinfo, unsigned int tst, int cmask, int kmask) throw()
{
    //_LocaleUpdate _loc_update(plocinfo);
	if (!plocinfo)
	{
		return _ismbbprint(tst) & (cmask | kmask);
	}
    /*
     * get input character and make sure < 256
     */
    tst = (unsigned int)(unsigned char)tst;

    return  ((*(plocinfo->mbcinfo->mbctype+1+tst)) & kmask) ||
            ((cmask) ? ((*(plocinfo->locinfo->_locale_pctype + tst)) & cmask) : 0);
}
