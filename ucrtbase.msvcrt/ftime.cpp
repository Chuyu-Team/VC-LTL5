#include <sys/timeb.h>
#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ftime32_s(
	_Out_ struct __timeb32* _Time
	)
{
	_VALIDATE_RETURN_ERRCODE(_Time != nullptr, EINVAL);

	_ftime32(_Time);
		
	return 0;
}
_LCRT_DEFINE_IAT_SYMBOL(_ftime32_s);

extern "C" errno_t __cdecl _ftime64_s(
	_Out_ struct __timeb64* _Time
	)
{
	_VALIDATE_RETURN_ERRCODE(_Time != nullptr, EINVAL);

	_ftime64(_Time);

	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_ftime64_s);
#endif