#include <io.h>
#include <errno.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _access_s(
	_In_z_ char const* _FileName,
	_In_   int         _AccessMode
	)
{
	return _access(_FileName, _AccessMode) == 0 ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_access_s);
#endif


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _waccess_s(
	_In_z_ wchar_t const* _FileName,
	_In_   int            _AccessMode
	)
{
	return _waccess(_FileName, _AccessMode) == 0 ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_waccess_s);
#endif
