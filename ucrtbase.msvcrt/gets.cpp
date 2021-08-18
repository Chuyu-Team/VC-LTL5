
#include <stdio.h>
#include <internal_shared.h>





//不想额外写类，通过参数强制转换，规避下吧
__forceinline int _tgetc(char* in)
{
	return getc((FILE*)in);
}

__forceinline wint_t _tgetc(wchar_t* in)
{
	return getwc((FILE*)in);
}

template<class TCHAR>
__forceinline TCHAR * __cdecl common_gets_s_nolock(
	_Out_writes_z_(_Size) TCHAR * _Buffer,
	_In_                  rsize_t _Size,
	_In_                  bool const return_early_if_eof_is_first
)
{
	_VALIDATE_RETURN(_Buffer != nullptr, EINVAL, nullptr);
	_VALIDATE_RETURN(_Size > 0, EINVAL, nullptr);

	auto c = _tgetc((TCHAR*)_iob);
	if (c == EOF)
	{
		if(return_early_if_eof_is_first)
			return nullptr;
	}

	auto pBuffer = _Buffer;

	if (_Size == -1)
	{
		for (; c != '\n' && c != EOF; c = _tgetc((TCHAR*)_iob))
		{
			*pBuffer++ = c;
		}

		*pBuffer = '\0';

	}
	else
	{
		for (; c != '\n' && c != EOF; c = _tgetc((TCHAR*)_iob))
		{
			if (_Size)
			{
				--_Size;
				*pBuffer++ = c;
			}
		}

		// If we ran out of space, clear the buffer and return failure:
		if (_Size == 0)
		{
			*_Buffer = NULL;

			_VALIDATE_RETURN((L"Buffer is too small" && 0), ERANGE, nullptr);
		}

		*pBuffer = '\0';

	}

	return pBuffer;
}

extern "C" char* __cdecl gets_s(
	_Out_writes_z_(_Size) char* _Buffer,
	_In_                  rsize_t _Size
)
{
	_lock(0x10);

	auto result = common_gets_s_nolock(_Buffer, _Size, false);

	_unlock(0x10);
	return result;
}

_LCRT_DEFINE_IAT_SYMBOL(gets_s);

extern "C" wchar_t* __cdecl _getws_s(
	_Out_writes_z_(_BufferCount) wchar_t* _Buffer,
	_In_                         size_t   _BufferCount
)
{
	_lock(0x10);

	auto result = common_gets_s_nolock(_Buffer, _BufferCount, false);

	_unlock(0x10);
	return result;
}

_LCRT_DEFINE_IAT_SYMBOL(_getws_s);


// Reads a line of text from stdin and stores it in the result buffer.  This
// function assumes there is sufficient room in the buffer.  Do not use this
// function; use gets_s(), fgets(), _getws_s(), or fgetws() instead.  If EOF
// is encountered on the first read from the stream, the buffer is left
// unmodified and nullptr is returned.
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240 && defined(_M_ARM)
extern "C" char* __cdecl gets(char* const _Buffer)
{
	_lock(0x10);
	auto result = common_gets_s_nolock(_Buffer, (static_cast<size_t>(-1)), true);
	_unlock(0x10);
	return result;
}

_LCRT_DEFINE_IAT_SYMBOL(gets);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240 && defined(_M_ARM)
extern "C" wchar_t* __cdecl _getws(wchar_t* const _Buffer)
{
	_lock(0x10);
	auto result = common_gets_s_nolock(_Buffer, (static_cast<size_t>(-1)), true);
	_unlock(0x10);
	return result;
}

_LCRT_DEFINE_IAT_SYMBOL(_getws);
#endif