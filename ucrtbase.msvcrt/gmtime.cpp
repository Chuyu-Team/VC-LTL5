#include <time.h>
#include <internal_shared.h>


__forceinline struct tm* __cdecl _gmtime_t(
	_In_  __time32_t const* _Time
)
{
	return _gmtime32(_Time);
}

__forceinline  struct tm* __cdecl _gmtime_t(
	_In_  __time64_t const* _Time
)
{
	return _gmtime64(_Time);
}

template<typename time_t>
__forceinline errno_t __cdecl common_gmtime_t_s(
	_Out_ struct tm* _Tm,
	_In_  time_t const* _Time
)
{
	_VALIDATE_RETURN_ERRCODE(_Tm != nullptr, EINVAL);

	memset(_Tm, 0xff, sizeof(*_Tm));

	_VALIDATE_RETURN_ERRCODE(_Time != nullptr, EINVAL);


	auto tm = _gmtime_t(_Time);
	if (!tm)
		return errno;

	*_Tm = *tm;
	return 0;
}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _gmtime32_s(
	_Out_ struct tm* _Tm,
	_In_  __time32_t const* _Time
	)
{
	return common_gmtime_t_s(_Tm, _Time);
}

_LCRT_DEFINE_IAT_SYMBOL(_gmtime32_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _gmtime64_s(
	_Out_ struct tm* _Tm,
	_In_  __time64_t const* _Time
	)
{
	return common_gmtime_t_s(_Tm, _Time);
}

_LCRT_DEFINE_IAT_SYMBOL(_gmtime64_s);
#endif
