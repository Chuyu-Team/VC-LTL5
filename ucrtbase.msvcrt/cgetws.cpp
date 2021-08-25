#include <internal_shared.h>

EXTERN_C __declspec(dllimport) wchar_t* __cdecl _cgetws(_Inout_z_ wchar_t* const string);

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
EXTERN_C errno_t __cdecl _cgetws_s(
	_Out_writes_to_(_BufferCount, *_SizeRead) wchar_t* _Buffer,
	_In_                                      size_t   _BufferCount,
	_Out_                                     size_t* _SizeRead
	)
{
	_VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);
	_VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE(_BufferCount > 0, EINVAL);

	*_Buffer = 0;

	_VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE(_SizeRead != nullptr, EINVAL);

	*_SizeRead = 0;

	_BufferCount = max(_BufferCount, 0xFFFF);

	auto Tmp = (wchar_t*)_malloca((_BufferCount + 2) * sizeof(*_Buffer));
	if (!Tmp)
		return errno;

	*Tmp = _BufferCount;

	errno_t error = 0;

	if (_cgetws(Tmp))
	{
		memcpy(_Buffer, Tmp + 2, (*_SizeRead = Tmp[1]) + 1);
	}
	else
	{
		error = errno;
	}

	_freea(Tmp);

	return error;
}

_LCRT_DEFINE_IAT_SYMBOL(_cgetws_s);
#endif