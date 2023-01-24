/***
*uncaught_exceptions.cpp - Some target-independent helper routines used by the EH frame handler.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
****/

#if 1
#include "framework.h"
#endif
#include <vcruntime_internal.h>

#if 1
extern "C" int* __cdecl __processing_throw();
#endif

///////////////////////////////////////////////////////////////////////////////
//
// __uncaught_exceptions() - Returns the number of exceptions that have been
//                           thrown but not yet caught.
//
// If a thread/fiber does not yet have thread/fiber locals, these functions
// just return false/0, without trying to allocate/initialize thread/fiber locals.
//

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 14393)
extern "C" int __cdecl __uncaught_exceptions()
{
#if WindowsTargetPlatformMinVersion >= __MakeVersion(10, 0, 10240)
    RENAME_BASE_PTD(__vcrt_ptd)* const ptd = RENAME_BASE_PTD(__vcrt_getptd_noinit)();
    return ptd ? ptd->_ProcessingThrow : 0;
#else
    return *__processing_throw();
#endif // WindowsTargetPlatformMinVersion >= __MakeVersion(10, 0, 10240)
}

_LCRT_DEFINE_IAT_SYMBOL(__uncaught_exceptions);
#endif // WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 14393)
