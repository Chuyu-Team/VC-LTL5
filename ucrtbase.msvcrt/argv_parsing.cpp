
#include <Windows.h>
#include <internal_shared.h>
#include <corecrt_internal.h>

#undef _environ
#undef _wenviron

#if defined _M_ARM64 || defined _M_ARM
#define __USING_LOCAL_ENVIRON
extern "C" char** _environ;
extern "C" wchar_t** _wenviron;
#endif


#pragma pack(push,_CRT_PACKING)
//结构体来自于 VC 2013 internal.h，逆向了下msvcrt.dll确实只有一个成员。
typedef struct
{
	int newmode;
} _startupinfo;

#pragma pack(pop)


extern "C" __declspec(dllimport) int __cdecl __getmainargs(
	_Out_ int * _Argc,
	_Outptr_result_buffer_(*_Argc) char *** _Argv,
	_Outptr_result_maybenull_ char *** _Env,
	_In_ int _DoWildCard,
	_In_ _startupinfo * _StartInfo
);

extern "C" __declspec(dllimport) int __cdecl __wgetmainargs(
	_Out_ int * _Argc,
	_Outptr_result_buffer_(*_Argc) wchar_t *** _Argv,
	_Outptr_result_maybenull_ wchar_t *** _Env,
	_In_ int _DoWildCard,
	_In_ _startupinfo * _StartInfo
);


extern "C" int __CRTDECL _get_startup_new_mode();

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)

EXTERN_C errno_t __cdecl _configure_narrow_argv(
	_In_ _crt_argv_mode mode
	)
{
	int _Argc;
	char ** _Argv;
#ifndef __USING_LOCAL_ENVIRON
	char **    _environ;
#endif
	_startupinfo _StartInfon = { _get_startup_new_mode() };

	//不支持_crt_argv_no_arguments
	return __getmainargs(&_Argc, &_Argv, &_environ, mode == _crt_argv_mode::_crt_argv_unexpanded_arguments ? 0 : 1, &_StartInfon);
}

_LCRT_DEFINE_IAT_SYMBOL(_configure_narrow_argv);

#endif


#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)

EXTERN_C errno_t __cdecl _configure_wide_argv(
	_In_ _crt_argv_mode mode
	)
{
	int _Argc;
	wchar_t ** _Argv;
#ifndef __USING_LOCAL_ENVIRON
	wchar_t ** _wenviron;
#endif
	_startupinfo _StartInfon = { _get_startup_new_mode() };

	//不支持_crt_argv_no_arguments
	return __wgetmainargs(&_Argc, &_Argv, &_wenviron, mode == _crt_argv_mode::_crt_argv_unexpanded_arguments ? 0 : 1, &_StartInfon);
}

_LCRT_DEFINE_IAT_SYMBOL(_configure_wide_argv);

#endif
