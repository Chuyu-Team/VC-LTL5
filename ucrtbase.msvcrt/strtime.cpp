#include <time.h>
#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _strtime_s(
	_Out_writes_(_SizeInBytes) _When_(_SizeInBytes >= 9, _Post_readable_size_(9)) char*  _Buffer,
	_In_                                                                         size_t _SizeInBytes
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr && _SizeInBytes > 0, EINVAL);
	*_Buffer = 0;
	_VALIDATE_RETURN_ERRCODE(_SizeInBytes >= 9, ERANGE);

	return _strtime(_Buffer) ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_strtime_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _wstrtime_s(
	_Out_writes_z_(_SizeInWords) _When_(_SizeInWords >= 9, _Post_readable_size_(9)) wchar_t* _Buffer,
	_In_                                                                           size_t   _SizeInWords
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr && _SizeInWords > 0, EINVAL);
	*_Buffer = 0;
	_VALIDATE_RETURN_ERRCODE(_SizeInWords >= 9, ERANGE);

	return _wstrtime(_Buffer) ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_wstrtime_s);
#endif