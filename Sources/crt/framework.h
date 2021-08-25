#pragma once

#include <vcruntime.h>

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容

#if defined _M_IX86
#if defined _M_HYBRID
#define _LCRT_DECLARE_ALTERNATE_NAME_PREFIX "#"
#else
#define _LCRT_DECLARE_ALTERNATE_NAME_PREFIX "_"
#endif
#define _LCRT_DECLARE_ALTERNATE_NAME_PREFIX_DATA "_"
#elif defined _M_X64 || defined _M_ARM || defined _M_ARM64
#define _LCRT_DECLARE_ALTERNATE_NAME_PREFIX ""
#define _LCRT_DECLARE_ALTERNATE_NAME_PREFIX_DATA ""
#else
#error Unsupported architecture
#endif

// The _VCRT_DEFINE_IAT_SYMBOL macro provides an architecture-neutral way of
// defining IAT symbols (__imp_- or _imp__-prefixed symbols).
#ifdef _M_IX86
#define _LCRT_DEFINE_IAT_SYMBOL_MAKE_NAME(f) _CRT_CONCATENATE(_imp__, f)
#else
#define _LCRT_DEFINE_IAT_SYMBOL_MAKE_NAME(f) _CRT_CONCATENATE(__imp_, f)
#endif

#ifdef __cplusplus
#define __IAT_EXTERN_C extern "C"
#define __IAT_reinterpret_cast(f) reinterpret_cast<void const*>(f) 
#else
#define __IAT_EXTERN_C
#define __IAT_reinterpret_cast(f) f
#endif

#ifdef _DLL

#define _LCRT_DEFINE_IAT_SYMBOL(f)                                                              \
    __IAT_EXTERN_C __declspec(selectany) void const* const _LCRT_DEFINE_IAT_SYMBOL_MAKE_NAME(f) \
        = __IAT_reinterpret_cast(f)

#define _LCRT_DEFINE_IAT_NATIVE_SYMBOL(symbol_name, f) \
    __pragma(warning(suppress:4483))                                                                 \
    extern "C" __declspec(selectany) void const* const __identifier(_CRT_STRINGIZE_(_imp_ ## symbol_name))  \
              = reinterpret_cast<void const*>(&f)

#else

//说明静态依赖，所以我们不需要定义 IAT_SYMBOL符号
#define _LCRT_DEFINE_IAT_SYMBOL(f)
#define _LCRT_DEFINE_IAT_NATIVE_SYMBOL(symbol_name, f)

#endif


#define _LCRT_DEFINE_IAT_SYMBOL_EXTERN(f)                                                       \
    __IAT_EXTERN_C void __cdecl f();                                                            \
    _LCRT_DEFINE_IAT_SYMBOL(f)



#define __MakeVersion(Major, Minor, Build) ((Major << 12) + (Minor << 8) + Build)

#ifndef __WARNING_UNUSED_ASSIGNMENT
#define __WARNING_UNUSED_ASSIGNMENT 28931
#endif

#define WindowsTargetPlatformWindowsXP       __MakeVersion(5, 1, 2600)
//也包含Windows XP x64
#define WindowsTargetPlatformWindows2003     __MakeVersion(5, 2, 3790)
//Windows Vista or 2008
#define WindowsTargetPlatformWindows6        __MakeVersion(6, 0, 6000)
//Windows 7 or 2008 R2
#define WindowsTargetPlatformWindows7        __MakeVersion(6, 1, 7600)
//Windows 8 or 2012
#define WindowsTargetPlatformWindows8        __MakeVersion(6, 2, 9200)
//Windows 8.1 or 2012 R2
#define WindowsTargetPlatformWindowsBlue     __MakeVersion(6, 3, 9600)
#define WindowsTargetPlatformWindows10_10240 __MakeVersion(10,0, 10240)

#ifndef UMDF_USING_NTSTATUS
#define UMDF_USING_NTSTATUS
#endif
#ifndef _NTDEF_
typedef _Return_type_success_(return >= 0) long NTSTATUS;
typedef NTSTATUS* PNTSTATUS;
#endif