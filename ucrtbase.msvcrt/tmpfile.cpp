#include <stdio.h>
#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl tmpnam_s(
	_Out_writes_z_(_Size) char* _Buffer,
	_In_                  rsize_t _Size
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);

	auto FileName = tmpnam(nullptr);
	if (!FileName)
		return errno;

	auto cName = strlen(FileName) + 1;

	if (cName > _Size)
		return ERANGE;

	memcpy(_Buffer, FileName, cName * sizeof(*FileName));
	*FileName = NULL;

	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(tmpnam_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wtmpnam_s(
	_Out_writes_z_(_BufferCount) wchar_t* _Buffer,
	_In_                         size_t   _BufferCount
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);

	auto FileName = _wtmpnam(nullptr);
	if (!FileName)
		return errno;

	auto cName = wcslen(FileName) + 1;

	if (cName > _BufferCount)
		return ERANGE;

	memcpy(_Buffer, FileName, cName * sizeof(*FileName));
	*FileName = NULL;

	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_wtmpnam_s);
#endif


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl tmpfile_s(
	_Out_opt_ _Deref_post_valid_ FILE** _Stream
	)
{
	_VALIDATE_RETURN_ERRCODE(_Stream != nullptr, EINVAL);

	return (*_Stream = tmpfile()) ? 0 : errno;
}

_LCRT_DEFINE_IAT_SYMBOL(tmpfile_s);
#endif
