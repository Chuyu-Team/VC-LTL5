#include <stdlib.h>
#include <internal_shared.h>


static __forceinline size_t __cdecl strlen_t(
	_In_z_ char const* _Str
	)
{
	return strlen(_Str);
}

static __forceinline size_t __cdecl strlen_t(
	_In_z_ wchar_t const* const _Str
	)
{
	return wcslen(_Str);
}


template<class _Fun, class ValueType,class CharType>
static errno_t __cdecl common_xtox(
								_Fun&& _xtox,
	_In_                         ValueType _Value,
	_Out_writes_z_(_BufferCount) CharType* _Buffer,
	_In_                         size_t    _BufferCount,
	_In_                         int       _Radix
	)
{
	_VALIDATE_RETURN_ERRCODE(_Buffer != nullptr, EINVAL);
	_VALIDATE_RETURN_ERRCODE(_BufferCount > 0, EINVAL);
	_VALIDATE_RETURN_ERRCODE(2 <= _Radix && _Radix <= 36, EINVAL);

	//最多保存33(32bit)/65(64bit)个,保证缓冲区充足
	CharType Temp[sizeof(_Value)* 8 + 1];

	if (_BufferCount >= _countof(Temp))
	{
		//缓冲区充足直接调用老版本
		_xtox(_Value, _Buffer, _Radix);

		return 0;
	}

	*Temp = CharType('\0');

	_xtox(_Value, Temp, _Radix);

	auto cTemp = strlen_t(Temp);

	if (cTemp >= _BufferCount)
	{
		*_Buffer = CharType('\0');
		_VALIDATE_RETURN_ERRCODE(cTemp < _BufferCount, ERANGE);
	}

	memcpy(_Buffer, Temp, (cTemp + 1) * sizeof(CharType));

	return 0;
}

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _i64toa_s(
	_In_                         __int64 _Value,
	_Out_writes_z_(_BufferCount) char*   _Buffer,
	_In_                         size_t  _BufferCount,
	_In_                         int     _Radix
	)
{
	return common_xtox(_i64toa,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_i64toa_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _i64tow_s(
	_In_                         __int64  _Value,
	_Out_writes_z_(_BufferCount) wchar_t* _Buffer,
	_In_                         size_t   _BufferCount,
	_In_                         int      _Radix
	)
{
	return common_xtox(_i64tow,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_i64tow_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _itoa_s(
	_In_                         int    _Value,
	_Out_writes_z_(_BufferCount) char*  _Buffer,
	_In_                         size_t _BufferCount,
	_In_                         int    _Radix
	)
{
	return common_xtox(_itoa,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_itoa_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _itow_s(
	_In_                         int      _Value,
	_Out_writes_z_(_BufferCount) wchar_t* _Buffer,
	_In_                         size_t   _BufferCount,
	_In_                         int      _Radix
	)
{
	return common_xtox(_itow,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_itow_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ltoa_s(
	_In_                         long   _Value,
	_Out_writes_z_(_BufferCount) char*  _Buffer,
	_In_                         size_t _BufferCount,
	_In_                         int    _Radix
	)
{
	return common_xtox(_ltoa,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_ltoa_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ltow_s(
	_In_                         long     _Value,
	_Out_writes_z_(_BufferCount) wchar_t* _Buffer,
	_In_                         size_t   _BufferCount,
	_In_                         int      _Radix
	)
{
	return common_xtox(_ltow,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_ltow_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ui64toa_s(
	_In_                         unsigned __int64 _Value,
	_Out_writes_z_(_BufferCount) char*            _Buffer,
	_In_                         size_t           _BufferCount,
	_In_                         int              _Radix
	)
{
	return common_xtox(_ui64toa,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_ui64toa_s);
#endif


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ui64tow_s(
	_In_                         unsigned __int64 _Value,
	_Out_writes_z_(_BufferCount) wchar_t*         _Buffer,
	_In_                         size_t           _BufferCount,
	_In_                         int              _Radix
	)
{
	return common_xtox(_ui64tow,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_ui64tow_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ultoa_s(
	_In_                         unsigned long _Value,
	_Out_writes_z_(_BufferCount) char*         _Buffer,
	_In_                         size_t        _BufferCount,
	_In_                         int           _Radix
	)
{
	return common_xtox(_ultoa,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_ultoa_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _ultow_s(
	_In_                         unsigned long _Value,
	_Out_writes_z_(_BufferCount) wchar_t*      _Buffer,
	_In_                         size_t        _BufferCount,
	_In_                         int           _Radix
	)
{
	return common_xtox(_ultow,_Value, _Buffer, _BufferCount, _Radix);
}

_LCRT_DEFINE_IAT_SYMBOL(_ultow_s);
#endif
