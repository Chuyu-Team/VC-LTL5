#include <stdlib.h>
#include <Windows.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _searchenv_s(
	_In_z_                       char const* _Filename,
	_In_z_                       char const* _VarName,
	_Out_writes_z_(_BufferCount) char*       _Buffer,
	_In_                         size_t      _BufferCount
	)
{
	if (_BufferCount >= MAX_PATH)
	{
		_searchenv(_Filename, _VarName, _Buffer);

		return errno;
	}
	else
	{
		char Temp[MAX_PATH];

		_searchenv(_Filename, _VarName, Temp);

		auto err = errno;

		if (err ==0)
		{
			return strcpy_s(_Buffer, _BufferCount, Temp);
		}
		else
		{
			return err;
		}
	}
}

_LCRT_DEFINE_IAT_SYMBOL(_searchenv_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _wsearchenv_s(
	_In_z_                       wchar_t const* _Filename,
	_In_z_                       wchar_t const* _VarName,
	_Out_writes_z_(_BufferCount) wchar_t*       _Buffer,
	_In_                         size_t         _BufferCount
	)
{
	if (_BufferCount >= MAX_PATH)
	{
		_wsearchenv(_Filename, _VarName, _Buffer);

		return errno;
	}
	else
	{
		wchar_t Temp[MAX_PATH];

		_wsearchenv(_Filename, _VarName, Temp);

		auto err = errno;

		if (err == 0)
		{
			return wcscpy_s(_Buffer, _BufferCount, Temp);
		}
		else
		{
			return err;
		}
	}
}

_LCRT_DEFINE_IAT_SYMBOL(_wsearchenv_s);
#endif