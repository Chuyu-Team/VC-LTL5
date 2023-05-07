#include <stdlib.h>
#include <internal_shared.h>

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

__forceinline int __cdecl _tputenv(
	_In_z_ wchar_t const* _EnvString
)
{
	return _wputenv(_EnvString);
}

__forceinline int __cdecl _tputenv(
	_In_z_ char const* _EnvString
)
{
	return _putenv(_EnvString);
}

template<typename TCAHR> __forceinline errno_t __cdecl common_putenv_s(
	_In_z_ TCAHR const* _Name,
	_In_z_ TCAHR const* _Value
)
{
	_VALIDATE_RETURN_ERRCODE(_Value != nullptr, EINVAL);
	_VALIDATE_RETURN_ERRCODE(_Name != nullptr, EINVAL);

	auto cName = _tcslen(_Name);
	auto cValue = _tcslen(_Value);

	// 我们需要额外添加二个字符，一个用于存储连接符 '='，另外一个用于字符串终止符 '\0'。
	auto _EnvString = (TCAHR*)_malloca((cName + cValue + 2) * sizeof(TCAHR));
	if (!_EnvString)
		return errno;

	memcpy(_EnvString, _Name, cName * sizeof(TCAHR));
	_EnvString[cName] = '=';
	memcpy(_EnvString + cName + 1, _Value, (cValue + 1) * sizeof(TCAHR));

	auto result = _tputenv(_EnvString) == 0 ? 0 : errno;

	_freea(_EnvString);

	return result;
}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _putenv_s(
	_In_z_ char const* _Name,
	_In_z_ char const* _Value
)
{
	return common_putenv_s(_Name, _Value);
}

_LCRT_DEFINE_IAT_SYMBOL(_putenv_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wputenv_s(
	_In_z_ wchar_t const* _Name,
	_In_z_ wchar_t const* _Value
)
{
	return common_putenv_s(_Name, _Value);
}

_LCRT_DEFINE_IAT_SYMBOL(_wputenv_s);
#endif