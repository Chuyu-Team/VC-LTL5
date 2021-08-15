
#include <stdlib.h>
#include <internal_shared.h>

__forceinline char* __cdecl _tgetenv(
	_In_z_ char const* _VarName
)
{
	return getenv(_VarName);
}

__forceinline wchar_t* __cdecl _tgetenv(
	_In_z_ wchar_t const* _VarName
)
{
	return _wgetenv(_VarName);
}

static __forceinline size_t __cdecl _tcslen(
	_In_z_ char const* _Str
	)
{
	return strlen(_Str);
}

static __forceinline size_t __cdecl _tcslen(
	_In_z_ wchar_t const* _String
	)
{
	return wcslen(_String);
}

 template<class TCHAR>
__forceinline errno_t __cdecl common_tdupenv_s_nolock(
	_Outptr_result_buffer_maybenull_(*_BufferCount) _Outptr_result_maybenull_z_ TCHAR * *_Buffer,
	_Out_opt_                                                                   size_t * _BufferCount,
	_In_z_                                                                      TCHAR const* _VarName
)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);
	*_Buffer = nullptr;

	if (_BufferCount != nullptr)
		*_BufferCount = 0;

	_VALIDATE_RETURN_ERRCODE(_VarName != nullptr, EINVAL);

	const auto value = _tgetenv(_VarName);
	if (value == nullptr)
		return 0;

	size_t const value_count = _tcslen(value) + 1;

	auto pBuffer = (TCHAR*)malloc(value_count * sizeof(TCHAR));

	_VALIDATE_RETURN_ERRCODE_NOEXC(pBuffer != nullptr, ENOMEM);


	memcpy(pBuffer, value, value_count * sizeof(TCHAR));

	*_Buffer = pBuffer;

	if (_BufferCount)
		*_BufferCount = value_count;

	return 0;
}

extern "C" __declspec(dllimport) void __cdecl _lock(
	int locknum
	);

extern "C" __declspec(dllimport) void __cdecl _unlock(
	int locknum
	);

template<class TCHAR>
__forceinline errno_t __cdecl common_tdupenv_s(
	_Outptr_result_buffer_maybenull_(*_BufferCount) _Outptr_result_maybenull_z_ TCHAR * *_Buffer,
	_Out_opt_                                                                   size_t * _BufferCount,
	_In_z_                                                                      TCHAR const* _VarName
)
{
	_lock(/*_ENV_LOCK*/7);

	auto result = common_tdupenv_s_nolock(_Buffer, _BufferCount, _VarName);

	_unlock(/*_ENV_LOCK*/7);

	return result;
}

extern "C" errno_t __cdecl _dupenv_s(
	_Outptr_result_buffer_maybenull_(*_BufferCount) _Outptr_result_maybenull_z_ char** _Buffer,
	_Out_opt_                                                                   size_t* _BufferCount,
	_In_z_                                                                      char const* _VarName
)
{
	return common_tdupenv_s(_Buffer, _BufferCount, _VarName);
}

_LCRT_DEFINE_IAT_SYMBOL(_dupenv_s);


extern "C" errno_t __cdecl _wdupenv_s(
	_Outptr_result_buffer_maybenull_(*_BufferCount) _Outptr_result_maybenull_z_ wchar_t** _Buffer,
	_Out_opt_                                                                   size_t* _BufferCount,
	_In_z_                                                                      wchar_t const* _VarName
)
{
	return common_tdupenv_s(_Buffer, _BufferCount, _VarName);
}

_LCRT_DEFINE_IAT_SYMBOL(_wdupenv_s);
