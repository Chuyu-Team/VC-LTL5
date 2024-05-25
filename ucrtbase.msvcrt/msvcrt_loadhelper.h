#pragma once
#include <Windows.h>

#pragma section(".YYLT1$DAA",    long, read, write)
#pragma section(".YYLT1$DAB",    long, read, write) //LTL函数缓存节点
#pragma section(".YYLT1$DAC",    long, read, write) //保留，暂时用于边界结束

#pragma section(".YYLT2$IFA",    long, read)
#pragma section(".YYLT2$IFB",    long, read)        //LTL函数缓存初始化函数
#pragma section(".YYLT2$IFC",    long, read)        //LTL函数缓存初始化函数结束

namespace YY
{
    namespace VC_LTL
    {
        void* __fastcall try_get_function(
            void** _ppFunAddress,
            char      const* const _szName);
    }
}

#define __EXPAND_MSVCRT_FUN(name)                                                                           \
static decltype(name)* _CRT_CONCATENATE(try_get_, name)()                                                   \
{                                                                                                           \
    __declspec(allocate(".YYLT2$IFB")) static void* const s_pfnInitFun = &_CRT_CONCATENATE(try_get_, name); \
    __pragma(warning(suppress:6031))                                                                        \
	_bittest((LONG*)(&s_pfnInitFun), 0);                                                                    \
    __declspec(allocate(".YYLT1$DAB")) static void* _CRT_CONCATENATE(s_pfn_, name);                         \
    return (decltype(name)*)YY::VC_LTL::try_get_function(&_CRT_CONCATENATE(s_pfn_, name), #name);           \
}
