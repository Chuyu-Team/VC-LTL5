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
#else

//说明静态依赖，所以我们不需要定义 IAT_SYMBOL符号
#define _LCRT_DEFINE_IAT_SYMBOL(f)

#endif


#define __MakeVersion(Major, Minor, Build) ((Major << 12) + (Minor << 8) + Build)