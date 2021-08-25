
#include <internal_shared.h>
#include <io.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _chsize_s(int const fh, __int64 const size)
{
	//XP中 _chsize_s 的 size参数不允许大于 INT_MAX
	_VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE((size >= 0 && size <= INT_MAX), EINVAL);

	return _chsize(fh, (long)size) ? errno : 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_chsize_s);
#endif