#include "pch.h"

#pragma warning(disable :4483)

namespace Platform {
	namespace Details {
#ifdef _X86_

		#define _LCRT_DEFINE_IAT_RAW_SYMBOL2(NAME,FUNC)                 \
		__pragma(warning(suppress:4483))                                                                 \
		extern "C" __declspec(selectany) void const* const __identifier(_CRT_STRINGIZE_(_imp_ ## NAME))  \
        = reinterpret_cast<void const*>(&FUNC)


		extern "C" extern HRESULT (__stdcall* __identifier("_imp_?InitializeData@Details@Platform@@YGJH@Z"))(int threadingModel);
		VCCORLIB_API HRESULT InitializeData(int threadingModel)
		{
			return __identifier("_imp_?InitializeData@Details@Platform@@YGJH@Z")(threadingModel);
		}

		_LCRT_DEFINE_IAT_RAW_SYMBOL2(?InitializeData@Details@Platform@@YAJH@Z, InitializeData);


		extern "C" extern void (__stdcall* __identifier("_imp_?UninitializeData@Details@Platform@@YGXH@Z"))(int threadingModel);
		VCCORLIB_API void UninitializeData(int threadingModel)
		{
			__identifier("_imp_?UninitializeData@Details@Platform@@YGXH@Z")(threadingModel);
		}

		_LCRT_DEFINE_IAT_RAW_SYMBOL2(?UninitializeData@Details@Platform@@YAXH@Z, UninitializeData);


		extern "C" extern wchar_t** (__stdcall* __identifier("_imp_?GetCmdArguments@Details@Platform@@YGPAPA_WPAH@Z"))(_Out_ int* argc);
		VCCORLIB_API wchar_t** GetCmdArguments(_Out_ int* argc)
		{
			return __identifier("_imp_?GetCmdArguments@Details@Platform@@YGPAPA_WPAH@Z")(argc);
		}

		_LCRT_DEFINE_IAT_RAW_SYMBOL2(?GetCmdArguments@Details@Platform@@YAPAPA_WPAH@Z, GetCmdArguments);
#endif
	}
}