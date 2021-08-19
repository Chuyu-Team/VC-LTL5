// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>



// TODO: 在此处引用程序需要的其他头文件
#ifndef VCCORLIB_API
// __declspec(dllexport)
#define VCCORLIB_API
#endif

#ifndef CPPCLI_FUNC
// __declspec(dllexport)
#define CPPCLI_FUNC
#endif

//#include <stdio.h>
#include <wrl/client.h>
//#include <wrl/module.h>


//size_t SafeMultiply(size_t left, size_t right);

#if defined(_M_IX86)
#define _LCRT_DEFINE_IAT_RAW_SYMBOL(NAME, NAME_x64, NAME_arm, NAME_arm64)                 \
    __pragma(warning(suppress:4483))                                                                 \
    extern "C" extern void* __identifier(_CRT_STRINGIZE_(NAME));                                     \
	__pragma(warning(suppress:4483))                                                                 \
	extern "C" __declspec(selectany) void const* const __identifier(_CRT_STRINGIZE_(_imp_ ## NAME))  \
        = reinterpret_cast<void const*>(&__identifier(_CRT_STRINGIZE_(NAME)))
#elif defined(_M_AMD64)
#define _LCRT_DEFINE_IAT_RAW_SYMBOL(NAME_X86, NAME, NAME_arm, NAME_arm64)                 \
    __pragma(warning(suppress:4483))                                                                 \
    extern "C" extern void* __identifier(_CRT_STRINGIZE_(NAME));                                     \
	__pragma(warning(suppress:4483))                                                                 \
	extern "C" __declspec(selectany) void const* const __identifier(_CRT_STRINGIZE_(__imp_ ## NAME)) \
        = reinterpret_cast<void const*>(&__identifier(_CRT_STRINGIZE_(NAME)))
#elif defined(_M_ARM)
#define _LCRT_DEFINE_IAT_RAW_SYMBOL(NAME_X86, NAME_x64, NAME, NAME_arm64)                 \
    __pragma(warning(suppress:4483))                                                                 \
    extern "C" extern void* __identifier(_CRT_STRINGIZE_(NAME));                                     \
	__pragma(warning(suppress:4483))                                                                 \
	extern "C" __declspec(selectany) void const* const __identifier(_CRT_STRINGIZE_(__imp_ ## NAME)) \
        = reinterpret_cast<void const*>(&__identifier(_CRT_STRINGIZE_(NAME)))
#elif defined(_M_ARM64)
#define _LCRT_DEFINE_IAT_RAW_SYMBOL(NAME_X86, NAME_x64, NAME_arm, NAME)                   \
    __pragma(warning(suppress:4483))                                                                 \
    extern "C" extern void* __identifier(_CRT_STRINGIZE_(NAME));                                     \
	__pragma(warning(suppress:4483))                                                                 \
	extern "C" __declspec(selectany) void const* const __identifier(_CRT_STRINGIZE_(__imp_ ## NAME)) \
        = reinterpret_cast<void const*>(&__identifier(_CRT_STRINGIZE_(NAME)))
#endif
