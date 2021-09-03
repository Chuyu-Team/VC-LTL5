
#ifdef _DEBUG

/*
这些调试函数仅仅用于编译通过处理，实际没有任何的功能。
如果需要真正的使用这些调试功能，那么请使用微软原版的 调试版！！！
*/

#include <crtdbg.h>

extern "C" int __cdecl _CrtDbgReport(
    _In_       int         _ReportType,
    _In_opt_z_ char const* _FileName,
    _In_       int         _Linenumber,
    _In_opt_z_ char const* _ModuleName,
    _In_opt_z_ char const* _Format,
    ...)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtDbgReport);


extern "C" int __cdecl _CrtDbgReportW(
    _In_       int            _ReportType,
    _In_opt_z_ wchar_t const* _FileName,
    _In_       int            _LineNumber,
    _In_opt_z_ wchar_t const* _ModuleName,
    _In_opt_z_ wchar_t const* _Format,
    ...)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtDbgReportW);


extern "C" int __cdecl _VCrtDbgReportA(
    _In_       int         _ReportType,
    _In_opt_   void* _ReturnAddress,
    _In_opt_z_ char const* _FileName,
    _In_       int         _LineNumber,
    _In_opt_z_ char const* _ModuleName,
    _In_opt_z_ char const* _Format,
    va_list     _ArgList
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_VCrtDbgReportA);

extern "C" int __cdecl _VCrtDbgReportW(
    _In_       int            _ReportType,
    _In_opt_   void* _ReturnAddress,
    _In_opt_z_ wchar_t const* _FileName,
    _In_       int            _LineNumber,
    _In_opt_z_ wchar_t const* _ModuleName,
    _In_opt_z_ wchar_t const* _Format,
    va_list        _ArgList
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_VCrtDbgReportW);


extern "C" size_t __cdecl _CrtSetDebugFillThreshold(
    _In_ size_t _NewDebugFillThreshold
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetDebugFillThreshold);

extern "C" size_t __cdecl _CrtGetDebugFillThreshold(void)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtGetDebugFillThreshold);


extern "C" _HFILE __cdecl _CrtSetReportFile(
    _In_     int    _ReportType,
    _In_opt_ _HFILE _ReportFile
    )
{
    return NULL;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportFile);


extern "C" int __cdecl _CrtSetReportMode(
    _In_ int _ReportType,
    _In_ int _ReportMode
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportMode);


extern "C" int* __cdecl __p__crtDbgFlag(void)
{
    static int crtDbgFlag = 0;
    return &crtDbgFlag;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__crtDbgFlag);

extern "C" long* __cdecl __p__crtBreakAlloc(void)
{
    static long crtBreakAlloc = 0;
    return &crtBreakAlloc;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__crtBreakAlloc);

extern "C" void __cdecl _CrtSetDbgBlockType(
    void* const block,
    int   const block_use
    )
{
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetDbgBlockType);

extern "C" _CRT_ALLOC_HOOK __cdecl _CrtGetAllocHook(void)
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtGetAllocHook);

extern "C" _CRT_ALLOC_HOOK __cdecl _CrtSetAllocHook(
    _In_opt_ _CRT_ALLOC_HOOK _PfnNewHook
    )
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetAllocHook);

extern "C" _CRT_DUMP_CLIENT __cdecl _CrtGetDumpClient(void)
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtGetDumpClient);

extern "C" _CRT_DUMP_CLIENT __cdecl _CrtSetDumpClient(
    _In_opt_ _CRT_DUMP_CLIENT _PFnNewDump
    )
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetDumpClient);


extern "C" int __cdecl _CrtCheckMemory(void)
{
    return 1;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtCheckMemory);

extern "C" void __cdecl _CrtDoForAllClientObjects(
    _In_ _CrtDoForAllClientObjectsCallback _Callback,
    _In_ void* _Context
    )
{

}

_LCRT_DEFINE_IAT_SYMBOL(_CrtDoForAllClientObjects);

extern "C" int __cdecl _CrtDumpMemoryLeaks(void)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtDumpMemoryLeaks);

extern "C" int __cdecl _CrtIsMemoryBlock(
    _In_opt_  void const* _Block,
    _In_      unsigned int _Size,
    _Out_opt_ long* _RequestNumber,
    _Out_opt_ char** _FileName,
    _Out_opt_ int* _LineNumber
)
{
    if (_RequestNumber)
        *_RequestNumber = 0;

    if (_FileName)
        *_FileName = "";

    if (_LineNumber)
        *_LineNumber = 0;

    return 1;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtIsMemoryBlock);

_Check_return_
extern "C" int __cdecl _CrtIsValidHeapPointer(
    _In_opt_ void const* _Pointer
)
{
    return 1;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtIsValidHeapPointer);

_Check_return_
extern "C" int __cdecl _CrtIsValidPointer(
    _In_opt_ void const* _Pointer,
    _In_     unsigned int _Size,
    _In_     int          _ReadWrite
    )
{
    return 1;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtIsValidPointer);

extern "C" void __cdecl _CrtMemCheckpoint(
    _Out_ _CrtMemState * _State
    )
{

}

_LCRT_DEFINE_IAT_SYMBOL(_CrtMemCheckpoint);

extern "C" int __cdecl _CrtMemDifference(
    _Out_ _CrtMemState * _State,
    _In_  _CrtMemState const* _OldState,
    _In_  _CrtMemState const* _NewState
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtMemDifference);

extern "C" void __cdecl _CrtMemDumpAllObjectsSince(
    _In_opt_ _CrtMemState const* _State
    )
{
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtMemDumpAllObjectsSince);

extern "C" void __cdecl _CrtMemDumpStatistics(
    _In_ _CrtMemState const* _State
    )
{

}

_LCRT_DEFINE_IAT_SYMBOL(_CrtMemDumpStatistics);

_Check_return_
extern "C" int __cdecl _CrtReportBlockType(
    _In_opt_ void const* _Block
    )
{
    return -1;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtReportBlockType);


extern "C" long __cdecl _CrtSetBreakAlloc(
    _In_ long _NewValue
)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetBreakAlloc);

extern "C" int __cdecl _CrtSetDbgFlag(
    _In_ int _NewFlag
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetDbgFlag);



extern "C" _CRT_REPORT_HOOK __cdecl _CrtGetReportHook(void)
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtGetReportHook);

extern "C" _CRT_REPORT_HOOK __cdecl _CrtSetReportHook(
    _In_opt_ _CRT_REPORT_HOOK _PFnNewHook
    )
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportHook);

extern "C" int __cdecl _CrtSetReportHook2(
    _In_     int              _Mode,
    _In_opt_ _CRT_REPORT_HOOK _PFnNewHook
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportHook2);

extern "C" int __cdecl _CrtSetReportHookW2(
    _In_     int               _Mode,
    _In_opt_ _CRT_REPORT_HOOKW _PFnNewHook
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetReportHookW2);


#endif