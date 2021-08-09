//
// purevirt.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// The _purecall handler, called by compiler-generated code when a pure virtual
// call occurs.
//
#include <vcruntime_internal.h>
#include <stdlib.h>
#include <framework.h>


static _purecall_handler __pPurecall;


#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" int __cdecl _purecall()
{
    _purecall_handler const purecall_handler = _get_purecall_handler();
    if (purecall_handler)
    {
        purecall_handler();

        // The user-registered purecall handler should not return, but if it does,
        // continue with the default termination behavior.
    }

    abort();
}

_LCRT_DEFINE_IAT_SYMBOL(_purecall);

#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" _purecall_handler __cdecl _set_purecall_handler(
    _purecall_handler const new_handler
    )
{
    return __crt_interlocked_exchange_pointer(&__pPurecall, new_handler);
}

_LCRT_DEFINE_IAT_SYMBOL(_set_purecall_handler);

#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" _purecall_handler __cdecl _get_purecall_handler()
{
    return __crt_interlocked_read_pointer(&__pPurecall);
}

_LCRT_DEFINE_IAT_SYMBOL(_get_purecall_handler);

#endif