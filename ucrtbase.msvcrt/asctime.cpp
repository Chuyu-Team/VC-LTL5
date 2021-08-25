#include <time.h>
#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wasctime_s(
	_Out_writes_z_(_SizeInWords) _Post_readable_size_(26) wchar_t* _Buffer,
	_In_range_(>= , 26)                                     size_t           _SizeInWords,
	_In_                                                  struct tm const* _Tm
	)
{
	_VALIDATE_RETURN_ERRCODE(
		_Buffer != nullptr && _SizeInWords > 0,
		EINVAL
	)

		* _Buffer = NULL;

	_VALIDATE_RETURN_ERRCODE(_SizeInWords >= 26, EINVAL)

		auto szTime = _wasctime(_Tm);
	if (!szTime)
		return errno;

	return wcscpy_s(_Buffer, _SizeInWords, szTime);
}

_LCRT_DEFINE_IAT_SYMBOL(_wasctime_s);
#endif


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl asctime_s(
	_Out_writes_(_SizeInBytes) _Post_readable_size_(26) char* _Buffer,
	_In_range_(>= , 26)                                   size_t           _SizeInBytes,
	_In_                                                struct tm const* _Tm
	)
{
	_VALIDATE_RETURN_ERRCODE(
		_Buffer != nullptr && _SizeInBytes > 0,
		EINVAL
	)

		* _Buffer = NULL;

	_VALIDATE_RETURN_ERRCODE(_SizeInBytes >= 26, EINVAL)

		auto szTime = asctime(_Tm);
	if (!szTime)
		return errno;

	return strcpy_s(_Buffer, _SizeInBytes, szTime);
}

_LCRT_DEFINE_IAT_SYMBOL(asctime_s);
#endif
