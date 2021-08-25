#include <stdio.h>
#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl fopen_s(
	_Outptr_result_maybenull_ FILE** _Stream,
	_In_z_                    char const* _FileName,
	_In_z_                    char const* _Mode
	)
{
	_VALIDATE_RETURN_ERRCODE(_Stream != nullptr, EINVAL);

	return (*_Stream = fopen(_FileName, _Mode)) ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(fopen_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wfopen_s(
	_Outptr_result_maybenull_ FILE** _Stream,
	_In_z_                    wchar_t const* _FileName,
	_In_z_                    wchar_t const* _Mode
	)
{
	_VALIDATE_RETURN_ERRCODE(_Stream != nullptr, EINVAL);

	return (*_Stream = _wfopen(_FileName, _Mode)) ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_wfopen_s);
#endif
