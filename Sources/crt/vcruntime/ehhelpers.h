//
// Copyright (c) Microsoft Corporation. All Rights reserved.
//

#pragma once
#include <framework.h>

#if defined(_M_X64) || defined(_M_ARM_NT) || defined(_M_ARM64) || defined(_CHPE_X86_ARM64_EH_)

#if WindowsTargetPlatformMinVersion >= WindowsTargetPlatformWindows10_10240
#define _pForeignExcept   (*((EHExceptionRecord **)&(RENAME_BASE_PTD(__vcrt_getptd)()->_pForeignException)))
#elif WindowsTargetPlatformMinVersion >= WindowsTargetPlatformWindows6
#define _pForeignExcept   (*((EHExceptionRecord **)&(((_ptd_msvcrt_win6_shared*)RENAME_BASE_PTD(__vcrt_getptd)())->_pForeignException)))
#endif

#endif

#ifdef __BuildWithMSVCRT

#if(WindowsTargetPlatformMinVersion >= __MakeVersion(6, 0, 6000))
#define pFrameInfoChain   (*((FRAMEINFO **)    &(((_ptd_msvcrt_win6_shared*)RENAME_BASE_PTD(__vcrt_getptd)())->_pFrameInfoChain)))
#else
#error 暂时不支持
#endif

#else
#define pFrameInfoChain   (*((FRAMEINFO **)    &(RENAME_BASE_PTD(__vcrt_getptd)()->_pFrameInfoChain)))
#endif

// Pre-V4 managed exception code
#define MANAGED_EXCEPTION_CODE  0XE0434F4D

// V4 and later managed exception code
#define MANAGED_EXCEPTION_CODE_V4  0XE0434352

//extern "C" void
//__except_validate_context_record(
//    _In_ PCONTEXT ContextRecord
//    );
//这个函数仅仅是验证内存是否在堆栈中，而且要开启 guard ICall才行，所以直接砍掉吧
#define __except_validate_context_record(ContextRecord)

extern "C" _VCRTIMP void * __AdjustPointer(
    void *,
    const PMD&
);

extern "C" _VCRTIMP void * __GetPlatformExceptionInfo(
    int *
);

extern "C" _VCRTIMP int __cdecl __FrameUnwindFilter(
    EXCEPTION_POINTERS *
);

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern PGETWINRT_OOM_EXCEPTION __WinRTOutOfMemoryExceptionCallback;
#else
#define __WinRTOutOfMemoryExceptionCallback (PGETWINRT_OOM_EXCEPTION(NULL))
#endif

extern "C" _VCRTIMP void __cdecl __DestructExceptionObject(
    EHExceptionRecord *,
    BOOLEAN
);

extern "C" _VCRTIMP void __cdecl RENAME_EH_EXTERN(__BuildCatchObject)(
    EHExceptionRecord *,
    void *,
    HandlerType *,
    CatchableType *
);

extern "C" _VCRTIMP int __cdecl RENAME_EH_EXTERN(__TypeMatch4)(
    FH4::HandlerType4 *,
    CatchableType *,
    ThrowInfo *
    );

extern "C" _VCRTIMP int __cdecl RENAME_EH_EXTERN(__TypeMatch)(
    HandlerType *,
    CatchableType *,
    ThrowInfo *
);

//
// Prototype for the internal handler
//

template<class T>
EXCEPTION_DISPOSITION __InternalCxxFrameHandler(
    EHExceptionRecord    *pExcept,
    EHRegistrationNode   *pRN,
    CONTEXT              *pContext,
    DispatcherContext    *pDC,
    typename T::FuncInfo *pFuncInfo,
    int                  CatchDepth,
    EHRegistrationNode   *pMarkerRN,
    BOOLEAN              recursive);

#if _EH_RELATIVE_TYPEINFO

#undef THROW_COUNT
#define THROW_COUNT(ti)         THROW_COUNT_IB(ti, _GetThrowImageBase())
#undef THROW_CTLIST
#define THROW_CTLIST(ti)        THROW_CTLIST_IB(ti, _GetThrowImageBase())

#endif // _EH_RELATIVE_TYPEINFO
