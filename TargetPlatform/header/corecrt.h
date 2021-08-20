//
// corecrt.h
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Declarations used throughout the CoreCRT library.
//
#pragma once

//rename __crt_locale_data_public to __crt_locale_data_public_ucrt
#define __crt_locale_data_public __crt_locale_data_public_ucrt
#include <..\ucrt\corecrt.h>
#undef __crt_locale_data_public


#ifndef _TAGLC_ID_DEFINED
typedef struct tagLC_ID {
    unsigned short wLanguage;
    unsigned short wCountry;
    unsigned short wCodePage;
} LC_ID, * LPLC_ID;
#define _TAGLC_ID_DEFINED
#endif  /* _TAGLC_ID_DEFINED */

typedef struct __crt_locale_refcount
{
    char* locale;
    wchar_t* wlocale;
    long* refcount;
    long* wrefcount;
} __crt_locale_refcount;


typedef struct __crt_locale_data_public_msvcrt
{
    long                      refcount;

    //unsigned int lc_codepage;
    unsigned int              _locale_lc_codepage;

    unsigned int              lc_collate_cp;
    unsigned long lc_handle[6]; /* LCID */
    LC_ID lc_id[6];
    __crt_locale_refcount     lc_category[6];
    int                        lc_clike;

    //int mb_cur_max;
    _Field_range_(1, 2) int    _locale_mb_cur_max;

    long* lconv_intl_refcount;
    long* lconv_num_refcount;
    long* lconv_mon_refcount;
    struct lconv* lconv;
    long* ctype1_refcount;
    unsigned short* ctype1;
    //const unsigned short * pctype;
    unsigned short const* _locale_pctype;

    unsigned char const* pclmap;
    unsigned char const* pcumap;

    //struct __lc_time_data * lc_time_curr;
    struct __lc_time_data* lc_time_curr;
} __crt_locale_data_public_msvcrt;


#if defined(_LTL_ucrt_module_type) && _LTL_ucrt_module_type == 0
typedef __crt_locale_data_public_ucrt __crt_locale_data_public;
#elif _LTL_ucrt_module_type == 1
typedef __crt_locale_data_public_msvcrt __crt_locale_data_public;
#else
#error _LTL_ucrt_module_type???
#endif





