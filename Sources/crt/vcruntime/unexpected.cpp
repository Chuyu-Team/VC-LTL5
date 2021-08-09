//
// unexpected.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// The unexpected handler
//
#include <eh.h>
#include <vcruntime_internal.h>
#include <framework.h>

#if 0
static unexpected_handler __cdecl get_unexpected_or_default(
    RENAME_BASE_PTD(__vcrt_ptd) const* const ptd
    ) noexcept
{
    return ptd->_unexpected ? ptd->_unexpected : &terminate;
}
#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" unexpected_handler __cdecl _get_unexpected() noexcept
{
    //return get_unexpected_or_default(RENAME_BASE_PTD(__vcrt_getptd)());
    //通过二次调用 set_unexpected 获得
    auto old_handler = set_unexpected(nullptr);
    set_unexpected(old_handler);
    return old_handler;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_unexpected);

#endif

#if 0
extern "C" unexpected_handler __cdecl set_unexpected(
    unexpected_handler const new_handler
    ) noexcept
{
    RENAME_BASE_PTD(__vcrt_ptd)* const ptd = RENAME_BASE_PTD(__vcrt_getptd)();

    unexpected_handler const old_handler = get_unexpected_or_default(ptd);

    ptd->_unexpected = new_handler;

    return old_handler;
}
#endif

#if 0
extern "C" void __cdecl unexpected() noexcept(false)
{
    unexpected_handler const handler = RENAME_BASE_PTD(__vcrt_getptd)()->_unexpected;
    if (handler)
    {
        handler();
    }

    terminate();
}
#endif