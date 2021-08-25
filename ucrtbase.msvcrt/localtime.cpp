#include <time.h>
#include <internal_shared.h>

static __forceinline struct tm* __cdecl _localtime_t(_In_ __time32_t const* _Time)
{
	return _localtime32(_Time);
}

static __forceinline struct tm* __cdecl _localtime_t(_In_ __time64_t const* _Time)
{
	return _localtime64(_Time);
}

template<typename time_t>
static __forceinline errno_t __cdecl common_localtime_s(
	_Out_ struct tm*        _Tm,
	_In_  time_t const* _Time
	)
{
	_VALIDATE_RETURN_ERRCODE(_Tm != nullptr, EINVAL);
	memset(_Tm, 0xff, sizeof(*_Tm));

	_VALIDATE_RETURN_ERRCODE(_Time != nullptr, EINVAL);


	#pragma warning(suppress : 4996)
	const struct tm* t = _localtime_t(_Time);
	if (!t)
		return errno;

	*_Tm = *t;

	return 0;
}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _localtime32_s(
	_Out_ struct tm*        _Tm,
	_In_  __time32_t const* _Time
	)
{
	return common_localtime_s(_Tm, _Time);
}

_LCRT_DEFINE_IAT_SYMBOL(_localtime32_s);


extern "C" errno_t __cdecl _localtime64_s(
	_Out_ struct tm*        _Tm,
	_In_  __time64_t const* _Time
	)
{
	return common_localtime_s(_Tm, _Time);
}

_LCRT_DEFINE_IAT_SYMBOL(_localtime64_s);
#endif
