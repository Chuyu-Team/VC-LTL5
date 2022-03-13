#ifdef _DEBUG

#include <crtdbg.h>



extern "C" int __cdecl _CrtSetReportMode(
    int nRptType,
    int fMode
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportMode);

extern "C" _HFILE __cdecl _CrtSetReportFile(
    int nRptType,
    _HFILE hFile
    )
{
    return NULL;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportFile);

extern "C" _CRT_REPORT_HOOK __cdecl _CrtSetReportHook(_CRT_REPORT_HOOK pfnNewHook)
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportHook);

extern "C" _CRT_REPORT_HOOK __cdecl _CrtGetReportHook(void)
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtGetReportHook);


extern "C" int __cdecl _VCrtDbgReportA(
    int nRptType,
    void* returnAddress,
    char const* szFile,
    int nLine,
    char const* szModule,
    char const* szFormat,
    va_list arglist
)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_VCrtDbgReportA);

extern "C" int __cdecl _VCrtDbgReportW(
    int nRptType,
    void * returnAddress,
    wchar_t const* szFile,
    int nLine,
    wchar_t const* szModule,
    wchar_t const* szFormat,
    va_list arglist
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_VCrtDbgReportW);

#endif
