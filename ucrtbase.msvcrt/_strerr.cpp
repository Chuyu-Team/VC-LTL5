#include <internal_shared.h>


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl __wcserror_s(
	_Out_writes_opt_z_(_SizeInWords) wchar_t* _Buffer,
	_In_                             size_t         _SizeInWords,
	_In_z_                           wchar_t const* _ErrorMessage
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);
	_VALIDATE_RETURN_ERRCODE(_SizeInWords > 0, EINVAL);
	_Buffer[0] = '\0';


	auto szError = __wcserror(_ErrorMessage);

	if (!szError)
		return errno;

	return wcscpy_s(_Buffer, _SizeInWords, szError);
}

_LCRT_DEFINE_IAT_SYMBOL(__wcserror_s);
#endif


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _strerror_s(
	_Out_writes_z_(_SizeInBytes) char* _Buffer,
	_In_                         size_t      _SizeInBytes,
	_In_opt_z_                   char const* _ErrorMessage
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);
	_VALIDATE_RETURN_ERRCODE(_SizeInBytes > 0, EINVAL);
	_Buffer[0] = '\0';


	auto szError = _strerror(_ErrorMessage);

	if (!szError)
		return errno;

	return strcpy_s(_Buffer, _SizeInBytes, szError);
}

_LCRT_DEFINE_IAT_SYMBOL(_strerror_s);
#endif