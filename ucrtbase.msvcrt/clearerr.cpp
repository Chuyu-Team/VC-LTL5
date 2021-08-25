#include <stdio.h>
#include <errno.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl clearerr_s(
	_Inout_ FILE* _Stream
	)
{
	errno = 0;
	clearerr(_Stream);
	return errno;
}

_LCRT_DEFINE_IAT_SYMBOL(clearerr_s);
#endif
