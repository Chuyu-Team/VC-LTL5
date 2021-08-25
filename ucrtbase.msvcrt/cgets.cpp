#include <internal_shared.h>

EXTERN_C __declspec(dllimport) char* __cdecl _cgets(_Inout_z_ char* const string);

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _cgets_s(
	_Out_writes_z_(_BufferCount) char*   _Buffer,
	_In_                         size_t  _BufferCount,
	_Out_                        size_t* _SizeRead
	)
{
	_VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);
	_VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE(_BufferCount > 0, EINVAL);

	*_Buffer = 0;

	_VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE(_SizeRead != nullptr, EINVAL);

	*_SizeRead = 0;

	_BufferCount = min(0xFF, _BufferCount);

	char TempBuffer[0xFF + 2];
	*TempBuffer = _BufferCount;

	if (!_cgets((char*)TempBuffer))
	{
		return errno;
	}

	memcpy(_Buffer, TempBuffer + 2, *_SizeRead = TempBuffer[2]);

	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_cgets_s);
#endif