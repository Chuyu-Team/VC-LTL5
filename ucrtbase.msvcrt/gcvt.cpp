#include <stdlib.h>
#include <internal_shared.h>


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _gcvt_s(
	_Out_writes_z_(_BufferCount) char*  _Buffer,
	_In_                         size_t _BufferCount,
	_In_                         double _Value,
	_In_                         int    _DigitCount
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer && _BufferCount >0, EINVAL);

	*_Buffer = '\0';

	_VALIDATE_RETURN_ERRCODE(_BufferCount>_DigitCount, ERANGE);

	char buffer[_CVTBUFSIZE];

	_gcvt(_Value, _DigitCount, buffer);

	return strcpy_s(_Buffer, _BufferCount, buffer);
}

_LCRT_DEFINE_IAT_SYMBOL(_gcvt_s);
#endif