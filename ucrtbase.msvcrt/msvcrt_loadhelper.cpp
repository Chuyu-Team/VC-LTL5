#include <Windows.h>

#include <internal_shared.h>

#include "msvcrt_loadhelper.h"

#pragma comment(linker, "/merge:.YYLT1=.data")
#pragma comment(linker, "/merge:.YYLT2=.rdata")

typedef void* (__cdecl* InitFunType)();

static HMODULE __fastcall __LTL_GetMSVCRTModule()
{
	static HMODULE MSVCRTModuleCache;


	if (MSVCRTModuleCache)
	{
		if (MSVCRTModuleCache == (HMODULE)INVALID_HANDLE_VALUE)
		{
			return NULL;
		}
		return MSVCRTModuleCache;
	}


	auto hMSVCRT = GetModuleHandleW(L"msvcrt.dll");

	MSVCRTModuleCache = hMSVCRT ? hMSVCRT : (HMODULE)INVALID_HANDLE_VALUE;

	return hMSVCRT;
}

__declspec(allocate(".YYLT1$DAA")) static void* s_pfnReserve[] = { nullptr };
#define __LTL_THUNKS_FUN_START (s_pfnReserve + 1) //指针缓存开始位置
__declspec(allocate(".YYLT1$DAC")) static void* __LTL_THUNKS_FUN_END[] = { nullptr };   //指针缓存结束位置

__declspec(allocate(".YYLT2$IFA")) static void* const s_pfnInitReserve[] = { nullptr };
#define __LTL_THUNKS_INIT_FUN_START (s_pfnInitReserve + 1) //函数初始化开始位置
__declspec(allocate(".YYLT2$IFC")) static void* const __LTL_THUNKS_INIT_FUN_END[] = { nullptr }; //函数初始化开始位置

bool __cdecl __LTL_ThunksInit()
{
    for (auto p = __LTL_THUNKS_FUN_START; p != __LTL_THUNKS_FUN_END; ++p)
    {
        *p = __crt_fast_encode_pointer(nullptr);
    }

    for (auto p = (InitFunType*)__LTL_THUNKS_INIT_FUN_START; p != (InitFunType*)__LTL_THUNKS_INIT_FUN_END; ++p)
    {
        if (*p == nullptr)
            continue;

        (*p)();
    }

    return true;
}

namespace YY
{
    namespace VC_LTL
    {
        void* __fastcall try_get_function(
            void** _ppFunAddress,
            char      const* const _szName)
        {
            auto _p = __crt_fast_decode_pointer(*_ppFunAddress);
            if (_p)
            {
                return _p == INVALID_HANDLE_VALUE ? nullptr : _p;
            }
            
            do
            {
                auto _hModule = __LTL_GetMSVCRTModule();
                if (!_hModule)
                    break;

                _p = GetProcAddress(_hModule, _szName);

                InterlockedExchange((uintptr_t*)_ppFunAddress, (uintptr_t)__crt_fast_encode_pointer(_p ? _p : INVALID_HANDLE_VALUE));
            } while (false);
            
            return _p;
        }
    }
}
