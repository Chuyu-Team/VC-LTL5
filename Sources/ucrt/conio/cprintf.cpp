﻿//
// cprintf.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// The standard output functions, which perform formatted output to the
// console.
//
#include <corecrt_internal_stdio_output.h>



using namespace __crt_stdio_output;



template <template <typename, typename> class Base, typename Character>
static int __cdecl common_vcprintf(
    unsigned __int64 const options,
    Character const* const format,
    _locale_t        const locale,
    va_list          const arglist
    )
{
    typedef output_processor<
        Character,
        console_output_adapter<Character>,
        Base<Character, console_output_adapter<Character>>
    > processor_type;

    //_LocaleUpdate locale_update(locale);
    processor_type processor(
        console_output_adapter<Character>(),
        options,
        format,
        /*locale_update.GetLocaleT()*/locale,
        arglist);

    return processor.process();
}

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" int __cdecl __conio_common_vcprintf(
    unsigned __int64 const options,
    char const*      const format,
    _locale_t        const locale,
    va_list          const arglist
    )
{
    return common_vcprintf<standard_base>(options, format, locale, arglist);
}

_LCRT_DEFINE_IAT_SYMBOL(__conio_common_vcprintf);

#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" int __cdecl __conio_common_vcprintf_s(
    unsigned __int64 const options,
    char const*      const format,
    _locale_t        const locale,
    va_list          const arglist
    )
{
    return common_vcprintf<format_validation_base>(options, format, locale, arglist);
}

_LCRT_DEFINE_IAT_SYMBOL(__conio_common_vcprintf_s);
#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" int __cdecl __conio_common_vcprintf_p(
    unsigned __int64 const options,
    char const*      const format,
    _locale_t        const locale,
    va_list          const arglist
    )
{
    return common_vcprintf<positional_parameter_base>(options, format, locale, arglist);
}

_LCRT_DEFINE_IAT_SYMBOL(__conio_common_vcprintf_p);
#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" int __cdecl __conio_common_vcwprintf(
    unsigned __int64 const options,
    wchar_t const*   const format,
    _locale_t        const locale,
    va_list          const arglist
    )
{
    return common_vcprintf<standard_base>(options, format, locale, arglist);
}

_LCRT_DEFINE_IAT_SYMBOL(__conio_common_vcwprintf);
#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" int __cdecl __conio_common_vcwprintf_s(
    unsigned __int64 const options,
    wchar_t const*   const format,
    _locale_t        const locale,
    va_list          const arglist
    )
{
    return common_vcprintf<format_validation_base>(options, format, locale, arglist);
}

_LCRT_DEFINE_IAT_SYMBOL(__conio_common_vcwprintf_s);
#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" int __cdecl __conio_common_vcwprintf_p(
    unsigned __int64 const options,
    wchar_t const*   const format,
    _locale_t        const locale,
    va_list          const arglist
    )
{
    return common_vcprintf<positional_parameter_base>(options, format, locale, arglist);
}

_LCRT_DEFINE_IAT_SYMBOL(__conio_common_vcwprintf_p);
#endif