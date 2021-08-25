#include <io.h>
#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _mktemp_s(
	_Inout_updates_z_(_Size) char*  _TemplateName,
	_In_                     size_t _Size
	)
{
	_VALIDATE_RETURN_ERRCODE(_TemplateName != nullptr && _Size > 0, EINVAL);

	if (strnlen(_TemplateName, _Size)>= _Size)
	{
		_VALIDATE_RETURN((L"String is not null terminated" && 0), EINVAL, EINVAL)
	}

	return _mktemp(_TemplateName) ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_mktemp_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _wmktemp_s(
	_Inout_updates_z_(_SizeInWords) wchar_t* _TemplateName,
	_In_                            size_t   _SizeInWords
	)
{
	_VALIDATE_RETURN_ERRCODE(_TemplateName != nullptr && _SizeInWords > 0, EINVAL);

	if (wcsnlen(_TemplateName, _SizeInWords) >= _SizeInWords)
	{
		_VALIDATE_RETURN((L"String is not null terminated" && 0), EINVAL, EINVAL)
	}

	return _wmktemp(_TemplateName) ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_wmktemp_s);
#endif