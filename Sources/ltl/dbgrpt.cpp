#ifdef _DEBUG

#include <crtdbg.h>


/*---------------------------------------------------------------------------
 *
 * Debug Reporting
 *
 --------------------------------------------------------------------------*/


extern "C" int __cdecl _CrtSetReportHook2(
    int              const mode,
    _CRT_REPORT_HOOK const new_hook
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportHook2);

extern "C" int __cdecl _CrtSetReportHookW2(
    int               const mode,
    _CRT_REPORT_HOOKW const new_hook
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportHookW2);




extern "C" int __cdecl _CrtDbgReport(
    int         const report_type,
    char const* const file_name,
    int         const line_number,
    char const* const module_name,
    char const* const format,
    ...)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtDbgReport);


extern "C" int __cdecl _CrtDbgReportW(
    int            const report_type,
    wchar_t const* const file_name,
    int            const line_number,
    wchar_t const* const module_name,
    wchar_t const* const format,
    ...)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtDbgReportW);


#endif