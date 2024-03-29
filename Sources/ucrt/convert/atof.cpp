﻿//
// atof.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Definitions of the functions that convert strings to floating point numbers.
// Note that the str- and wcs-prefixed functions are defined elsewhere.  The
// functions defined here are provided for legacy support.
//
// The _atoxxx family of functions convert a string into a floating point
// number and return either zero (success), or _UNDERFLOW or _OVERFLOW.
//
#include <corecrt_internal_strtox.h>



template <typename FloatingType, typename Character>
static int __cdecl common_atodbl_l(
    FloatingType*    const result,
    Character const* const string,
    _locale_t        const locale
    ) throw()
{
    _VALIDATE_RETURN(result != nullptr, EINVAL, _DOMAIN);

    //_LocaleUpdate locale_update(locale);
    SLD_STATUS const status = __crt_strtox::parse_floating_point(
        locale,
        __crt_strtox::make_c_string_character_source(string, nullptr),
        result);

    switch (status)
    {
    case SLD_OVERFLOW:  return _OVERFLOW;
    case SLD_UNDERFLOW: return _UNDERFLOW;
    default:            return 0;
    }
}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int __cdecl _atoflt_l(_CRT_FLOAT* const result, char const* const string, _locale_t const locale)
{
    return common_atodbl_l(&result->f, string, locale);
}

_LCRT_DEFINE_IAT_SYMBOL(_atoflt_l);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
extern "C" int __cdecl _atoflt(_CRT_FLOAT* const result, char const* const string)
{
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
    return common_atodbl_l(&result->f, string, nullptr);
#else
    return _atoflt_l(result, string, nullptr);
#endif
}

_LCRT_DEFINE_IAT_SYMBOL(_atoflt);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int __cdecl _atodbl_l(_CRT_DOUBLE* const result, char* const string, _locale_t const locale)
{
    return common_atodbl_l(&result->x, string, locale);
}

_LCRT_DEFINE_IAT_SYMBOL(_atodbl_l);
#endif

#if 0
extern "C" int __cdecl _atodbl(_CRT_DOUBLE* const result, char* const string)
{
    return common_atodbl_l(&result->x, string, nullptr);
}
#endif


template <typename Character>
static double __cdecl common_atof_l(Character const* const string, _locale_t const locale) throw()
{
    _VALIDATE_RETURN(string != nullptr, EINVAL, 0.0);

    //_LocaleUpdate locale_update(locale);

    double result{};
    __crt_strtox::parse_floating_point(
        locale,
        __crt_strtox::make_c_string_character_source(string, nullptr),
        &result);
    return result;
}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" double __cdecl _atof_l(char const* const string, _locale_t const locale)
{
    return common_atof_l(string, locale);
}

_LCRT_DEFINE_IAT_SYMBOL(_atof_l);
#endif

#if 0
extern "C" double __cdecl atof(char const* const string)
{
    return common_atof_l(string, nullptr);
}
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" double __cdecl _wtof_l(wchar_t const* const string, _locale_t const locale)
{
    return common_atof_l(string, locale);
}

_LCRT_DEFINE_IAT_SYMBOL(_wtof_l);
#endif

#if 0
extern "C" double __cdecl _wtof(wchar_t const* const string)
{
    return common_atof_l(string, nullptr);
}
#endif
