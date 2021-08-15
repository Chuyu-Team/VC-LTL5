
#include <stdlib.h>

//msvcrt模式无需初始化 environment，__wgetmainargs 时就初始化了

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)

extern "C" int __cdecl _initialize_narrow_environment()
{
	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_initialize_narrow_environment);


extern "C" int __cdecl _initialize_wide_environment()
{
	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_initialize_wide_environment);

#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)

#undef _environ
#undef _wenviron

#if defined _M_IX86 || defined _M_AMD64
extern "C" __declspec(dllimport) extern char** _environ;
extern "C" __declspec(dllimport) extern wchar_t** _wenviron;
#elif defined _M_ARM64 || defined _M_ARM
extern "C" extern char** _environ;
extern "C" extern wchar_t** _wenviron;
#else
#error 不支持哦。
#endif

extern "C" char** __cdecl _get_initial_narrow_environment()
{
	return _environ;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_initial_narrow_environment);

extern "C" wchar_t** __cdecl _get_initial_wide_environment()
{
	return _wenviron;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_initial_wide_environment);

#endif

#if defined _M_AMD64 || defined _M_ARM64 || defined _M_ARM

extern "C" char*** __cdecl __p__environ()
{
	return &_environ;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__environ);

extern "C" wchar_t*** __cdecl __p__wenviron()
{
	return &_wenviron;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__wenviron);

#endif
