#include <stdlib.h>
#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _ecvt_s(
	_Out_writes_z_(_BufferCount) char* _Buffer,
	_In_  size_t                       _BufferCount,
	_In_  double                       _Value,
	_In_  int                          _DigitCount,
	_Out_ int*                         _PtDec,
	_Out_ int*                         _PtSign
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer && _BufferCount >0, EINVAL);

	*_Buffer = '\0';

	_VALIDATE_RETURN_ERRCODE(_PtDec&&_PtSign, EINVAL);

	return strcpy_s(_Buffer,
		_BufferCount,
		_ecvt(_Value, _DigitCount, _PtDec, _PtSign));
}

_LCRT_DEFINE_IAT_SYMBOL(_ecvt_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _fcvt_s(
	_Out_writes_z_(_BufferCount) char*  _Buffer,
	_In_                         size_t _BufferCount,
	_In_                         double _Value,
	_In_                         int    _FractionalDigitCount,
	_Out_                        int*   _PtDec,
	_Out_                        int*   _PtSign
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);
	_VALIDATE_RETURN_ERRCODE(_BufferCount > 0, EINVAL);

	*_Buffer = '\0';

	_VALIDATE_RETURN_ERRCODE(_PtDec != nullptr, EINVAL);
	_VALIDATE_RETURN_ERRCODE(_PtSign != nullptr, EINVAL);

	auto szValue = _fcvt(_Value, _FractionalDigitCount, _PtDec, _PtSign);

	if (!szValue)
		return errno;

	return strcpy_s(_Buffer, _BufferCount, szValue);
}

_LCRT_DEFINE_IAT_SYMBOL(_fcvt_s);
#endif