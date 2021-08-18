
#include <Windows.h>
#include <internal_shared.h>
#include <time.h>

#undef _daylight
__declspec(dllimport) extern int _daylight;

#if !defined _ARM64_ && !defined _ARM_
#undef _dstbias
__declspec(dllimport) extern long _dstbias;
#endif

#undef _timezone
__declspec(dllimport) extern long _timezone;

#undef _tzname
__declspec(dllimport) extern char* _tzname[2];

extern "C" errno_t __cdecl _get_daylight(
	_Out_ int* _Daylight
)
{
	_VALIDATE_RETURN_ERRCODE(_Daylight != nullptr, EINVAL);

	*_Daylight = _daylight;

	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_daylight);

extern "C" errno_t __cdecl _get_dstbias(
	_Out_ long* _DaylightSavingsBias
)
{
	_VALIDATE_RETURN_ERRCODE(_DaylightSavingsBias != nullptr, EINVAL);

	*_DaylightSavingsBias = _dstbias;
	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_dstbias);

extern "C" errno_t __cdecl _get_timezone(
	_Out_ long* _TimeZone
)
{
	_VALIDATE_RETURN_ERRCODE(_TimeZone != nullptr, EINVAL);

	*_TimeZone = _timezone;
	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_timezone);

extern "C" errno_t __cdecl _get_tzname(
	_Out_                        size_t* _ReturnValue,
	_Out_writes_z_(_SizeInBytes) char* _Buffer,
	_In_                         size_t  _SizeInBytes,
	_In_                         int     _Index
)
{
	_VALIDATE_RETURN_ERRCODE(
		(_Buffer != nullptr && _SizeInBytes > 0) ||
		(_Buffer == nullptr && _SizeInBytes == 0),
		EINVAL);

	if (_Buffer != nullptr)
		_Buffer[0] = '\0';

	_VALIDATE_RETURN_ERRCODE(_ReturnValue != nullptr, EINVAL);
	_VALIDATE_RETURN_ERRCODE(_Index == 0 || _Index == 1, EINVAL);

	auto& _TmpName = _tzname[_Index];

	// _tzname is correctly inited at startup, so no need to check if
	// CRT init finished.
	const auto cchBuffRerrequest = strlen(_TmpName) + 1;

	*_ReturnValue = cchBuffRerrequest;

	// If the buffer pointer is null, the caller is interested only in the size
	// of the string, not in the actual value of the string:
	if (_Buffer == nullptr)
		return 0;

	if (cchBuffRerrequest > _SizeInBytes)
		return ERANGE;

	memcpy(_Buffer, _TmpName, cchBuffRerrequest * sizeof(_TmpName[0]));
	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_tzname);

#if defined _M_AMD64 && WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6

extern "C" int* __cdecl __daylight()
{
	return &_daylight;
}

_LCRT_DEFINE_IAT_SYMBOL(__daylight);


extern "C" long* __cdecl __dstbias()
{
	return &_dstbias;
}
_LCRT_DEFINE_IAT_SYMBOL(__dstbias);

#endif

#if defined _M_AMD64 || defined _M_ARM || defined _M_ARM64

extern "C" long* __cdecl __timezone()
{
	return &_timezone;
}
_LCRT_DEFINE_IAT_SYMBOL(__timezone);

extern "C" char** __cdecl __tzname()
{
	return _tzname;
}
_LCRT_DEFINE_IAT_SYMBOL(__tzname);

#endif