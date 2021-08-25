#include <io.h>
#include <sys/stat.h>
#include <internal_shared.h>

//行为略微跟原版不同，当参数不合法时，不会返回 _OldMode
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _umask_s(
	_In_  int  _NewMode,
	_Out_ int* _OldMode
	)
{
	_VALIDATE_RETURN_ERRCODE(_OldMode != nullptr, EINVAL);
	_VALIDATE_RETURN_ERRCODE((_NewMode & ~(_S_IREAD | _S_IWRITE)) == 0, EINVAL);

	*_OldMode = _umask(_NewMode);

	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_umask_s);
#endif
