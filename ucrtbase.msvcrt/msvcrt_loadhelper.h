#pragma once

HMODULE __fastcall __LTL_GetMSVCRTModule();

#define __EXPAND_MSVCRT_FUN(name) \
static decltype(name)* _CRT_CONCATENATE(pMSVCRT_, name); \
if(_CRT_CONCATENATE(pMSVCRT_, name) == NULL)\
{ \
    auto fun = GetProcAddress(__LTL_GetMSVCRTModule(), #name);\
    _CRT_CONCATENATE(pMSVCRT_, name) = fun ? (decltype(name)*)fun : (decltype(name)*)INVALID_HANDLE_VALUE;\
}


#define __Get_MSVCRT_FUN(name) (_CRT_CONCATENATE(pMSVCRT_, name) != (decltype(name)*)INVALID_HANDLE_VALUE ? _CRT_CONCATENATE(pMSVCRT_, name) : (decltype(name)*)NULL)

