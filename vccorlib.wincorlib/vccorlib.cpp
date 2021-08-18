#include "pch.h"

#pragma warning(disable :4483)

namespace Platform {
	namespace Details {
#ifdef _X86_
		__declspec(dllimport) HRESULT __stdcall __identifier("?InitializeData@Details@Platform@@YGJH@Z")(int threadingModel);
		VCCORLIB_API HRESULT InitializeData(int threadingModel)
		{
			return __identifier("?InitializeData@Details@Platform@@YGJH@Z")(threadingModel);
		}

		_LCRT_DEFINE_IAT_SYMBOL(InitializeData);


		__declspec(dllimport) void __stdcall __identifier("?UninitializeData@Details@Platform@@YGXH@Z")(int threadingModel);
		VCCORLIB_API void UninitializeData(int threadingModel)
		{
			__identifier("?UninitializeData@Details@Platform@@YGXH@Z")(threadingModel);
		}

		_LCRT_DEFINE_IAT_SYMBOL(UninitializeData);


		__declspec(dllimport) wchar_t** __stdcall __identifier("?GetCmdArguments@Details@Platform@@YGPAPA_WPAH@Z")(_Out_ int* argc);
		VCCORLIB_API wchar_t** GetCmdArguments(_Out_ int* argc)
		{
			return __identifier("?GetCmdArguments@Details@Platform@@YGPAPA_WPAH@Z")(argc);
		}

		_LCRT_DEFINE_IAT_SYMBOL(GetCmdArguments);
#endif
	}
}