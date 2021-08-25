#include <time.h>
#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
extern "C" __time64_t __cdecl _mkgmtime64(
	_Inout_ struct tm* _Tm
	)
{
	_VALIDATE_RETURN(_Tm != nullptr, EINVAL, -1);

	auto tbtemp = *_Tm;

	auto time = _mktime64(&tbtemp);
	if (time == -1)
	{
		return -1;
	}

	//修正本地时间与UTC时间的差异
	time -= _timezone;

	if (_Tm->tm_isdst > 0 || (_Tm->tm_isdst < 0 && tbtemp.tm_isdst > 0))
	{
		time -= _dstbias;
	}

	auto tm = _gmtime64(&time);
	if (!tm)
		return -1;

	*_Tm = *tm;

	return time;
}

_LCRT_DEFINE_IAT_SYMBOL(_mkgmtime64);
#endif

// Maximum local time adjustment (GMT + 13 Hours, DST -0 Hours)
#define _MAX_LOCAL_TIME (13 * 60 * 60)

	// Minimum local time adjustment (GMT - 11 Hours, DST - 1 Hours)
#define _MIN_LOCAL_TIME (-12 * 60 * 60)

	// Number of seconds from 00:00:00, 01/01/1970 UTC to 23:59:59, 01/18/2038 UTC
#define _MAX_TIME32 0x7fffd27fll

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
extern "C" __time32_t __cdecl _mkgmtime32(
	_Inout_ struct tm* _Tm
	)
{
	auto tbtemp = *_Tm;
	auto Time = _mkgmtime64(&tbtemp);
	if (Time == -1)
		return -1;

	// Check for illegal time_t value:
	if (Time > _MAX_TIME32 + _MAX_LOCAL_TIME)
	{
		errno = EINVAL;
		return -1;
	}

	*_Tm = tbtemp;

	return Time;
}

_LCRT_DEFINE_IAT_SYMBOL(_mkgmtime32);
#endif