#include <time.h>
#include <internal_shared.h>

__forceinline void __cdecl _ctime_t(
	_In_ __time32_t const* _Time,
	char*& szTime
)
{
	szTime = _ctime32(_Time);
}

__forceinline void __cdecl _ctime_t(
	_In_ __time64_t const* _Time,
	char*& szTime
)
{
	szTime = _ctime64(_Time);
}

__forceinline void __cdecl _ctime_t(
	_In_ __time32_t const* _Time,
	wchar_t*& szTime
)
{
	szTime = _wctime32(_Time);
}

__forceinline void __cdecl _ctime_t(
	_In_ __time64_t const* _Time,
	wchar_t*& szTime
)
{
	szTime = _wctime64(_Time);
}


static __forceinline errno_t __cdecl _tcscpy_s(
	_Out_writes_z_(_SizeInBytes) char*       _Destination,
	_In_                         rsize_t     _SizeInBytes,
	_In_z_                       char const* _Source
	)
{
	return strcpy_s(_Destination, _SizeInBytes, _Source);
}

static __forceinline errno_t __cdecl _tcscpy_s(
	_Out_writes_z_(_SizeInBytes) wchar_t*       _Destination,
	_In_                         rsize_t     _SizeInBytes,
	_In_z_                       wchar_t const* _Source
	)
{
	return wcscpy_s(_Destination, _SizeInBytes, _Source);
}

template<typename time_t, typename TCHAR>
__forceinline errno_t __cdecl common_ctime_s(
	_Out_writes_(_SizeInBytes) _Post_readable_size_(26) TCHAR * _Buffer,
	_In_range_(>= , 26)                                   size_t            _Size,
	_In_                                                time_t const* _Time
)
{
	_VALIDATE_RETURN_ERRCODE(
		_Buffer != nullptr && _Size > 0,
		EINVAL
	);

	*_Buffer = NULL;

	_VALIDATE_RETURN_ERRCODE(_Size >= 26, EINVAL);

	TCHAR* szTime;

	_ctime_t(_Time, szTime);
	if (!szTime)
		return errno;

	return _tcscpy_s(_Buffer, _Size, szTime);
}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ctime32_s(
	_Out_writes_(_SizeInBytes) _Post_readable_size_(26) char* _Buffer,
	_In_range_(>= , 26)                                   size_t            _SizeInBytes,
	_In_                                                __time32_t const* _Time
)
{
	return common_ctime_s(_Buffer, _SizeInBytes, _Time);
}

_LCRT_DEFINE_IAT_SYMBOL(_ctime32_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ctime64_s(
	_Out_writes_z_(_SizeInBytes) _Post_readable_size_(26) char* _Buffer,
	_In_range_(>= , 26)                                     size_t            _SizeInBytes,
	_In_                                                  __time64_t const* _Time
)
{
	return common_ctime_s(_Buffer, _SizeInBytes, _Time);
}

_LCRT_DEFINE_IAT_SYMBOL(_ctime64_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wctime32_s(
	_Out_writes_z_(_SizeInWords) _Post_readable_size_(26) wchar_t* _Buffer,
	_In_  _In_range_(>= , 26)                              size_t            _SizeInWords,
	_In_                                                  __time32_t const* _Time
)
{
	return common_ctime_s(_Buffer, _SizeInWords, _Time);
}

_LCRT_DEFINE_IAT_SYMBOL(_wctime32_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wctime64_s(
	_Out_writes_z_(_SizeInWords) _Post_readable_size_(26) wchar_t* _Buffer,
	_In_  _In_range_(>= , 26)                              size_t            _SizeInWords,
	_In_                                                  __time64_t const* _Time
)
{
	return common_ctime_s(_Buffer, _SizeInWords, _Time);
}

_LCRT_DEFINE_IAT_SYMBOL(_wctime64_s);
#endif
