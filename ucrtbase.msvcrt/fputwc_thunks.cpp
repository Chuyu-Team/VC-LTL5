#include <Windows.h>
#include <corecrt_wstdio.h>
#include <stdio.h>
#include <corecrt_internal_stdio.h>
#include "msvcrt_loadhelper.h"

extern "C" __declspec(dllimport) extern _iobuf _iob[20];

__EXPAND_MSVCRT_FUN(fputc);

static int __cdecl fputc_msvcrt(int const c, FILE* const stream)
{
	if (auto pfputc_msvcrt = try_get_fputc())
	{
		return pfputc_msvcrt(c, stream);
	}
	
	return EOF;
}

__EXPAND_MSVCRT_FUN(fputwc);

static wint_t __cdecl fputwc_msvcrt(
	wchar_t _Character,
	FILE* _Stream)
{
	if (auto pfputwc_msvcrt = try_get_fputwc())
	{
		return pfputwc_msvcrt(_Character, _Stream);
	}

	return WEOF;
}

__EXPAND_MSVCRT_FUN(fwrite);

static size_t __cdecl fwrite_msvcrt(
    void const* const buffer,
    size_t      const size,
    size_t      const count,
    FILE*       const stream
    )
{
	if (auto pfwrite_msvcrt = try_get_fwrite())
	{
		return pfwrite_msvcrt(buffer, size, count, stream);
	}

	
	return 0;
}


extern "C" wint_t __cdecl fputwc(
	wchar_t _Character,
	FILE* _Stream)
{
	if ((_Character & (~0x7F))
		&&(_Stream == /*stdout*/&_iob[1] || _Stream == /*stderr*/&_iob[2]))
	{
		_lock_file(_Stream);
		auto Success = __acrt_stdio_begin_temporary_buffering_nolock(_Stream);
		wint_t result = fputwc_msvcrt(_Character, _Stream);
		__acrt_stdio_end_temporary_buffering_nolock(Success, _Stream);
		_unlock_file(_Stream);

		return result;
	}
	
	return fputwc_msvcrt(_Character, _Stream);
}

_LCRT_DEFINE_IAT_SYMBOL(fputwc);

extern "C" wint_t __cdecl putwc(
        wchar_t _Character,
        FILE*   _Stream
        )
{
	return fputwc(_Character, _Stream);
}

_LCRT_DEFINE_IAT_SYMBOL(putwc);

extern "C" wint_t __cdecl _fputwchar(
	wchar_t _Character
	)
{
	return fputwc(_Character,/*stdout*/(FILE*)&_iob[1]);
}

_LCRT_DEFINE_IAT_SYMBOL(_fputwchar);

extern "C"  wint_t __cdecl putwchar(
        wchar_t _Character
        )
 {
	return _fputwchar(_Character);
 }

_LCRT_DEFINE_IAT_SYMBOL(putwchar);


extern "C" size_t __cdecl fwrite(
    void const* const buffer,
    size_t      const size,
    size_t      const count,
    FILE*       const stream
    )
{
	if (stream == /*stdout*/&_iob[1] || stream == /*stderr*/&_iob[2])
	{
		_lock_file(stream);
		auto Success = __acrt_stdio_begin_temporary_buffering_nolock(stream);
		auto result = fwrite_msvcrt(buffer, size, count, stream);
		__acrt_stdio_end_temporary_buffering_nolock(Success, stream);
		_unlock_file(stream);

		return result;
	}
	else
	{
		return fwrite_msvcrt(buffer, size, count, stream);
	}
}

_LCRT_DEFINE_IAT_SYMBOL(fwrite);

extern "C" int __cdecl fputc(int const c, FILE* const stream)
{
	if ((c & 0x80)
		&&(stream == /*stdout*/&_iob[1] || stream == /*stderr*/&_iob[2]))
	{
		_lock_file(stream);
		static bool stdout_buffer_used = false;
		static bool stderr_buffer_used = false;

		static char stdout_buffer[2];
		
		static char stderr_buffer[2];


		bool* buffer_used;
		char* buffer;
		if (stream == /*stdout*/&_iob[1])
		{
			buffer_used = &stdout_buffer_used;
			buffer = stdout_buffer;
		}
		else
		{
			buffer_used = &stderr_buffer_used;
			buffer = stderr_buffer;
		}

		int nRet;
		if (*buffer_used)
		{
			buffer[1] = c;
			auto Success = __acrt_stdio_begin_temporary_buffering_nolock(stream);
			auto result = fwrite_msvcrt(buffer, 1, 2, stream);
			__acrt_stdio_end_temporary_buffering_nolock(Success, stream);

			*buffer_used = false;


			nRet = result ? c : EOF;
		}
		else if (isleadbyte(c))
		{
			*buffer_used = true;
			buffer[0] = c;
			nRet = c;
		}
		else
		{
			nRet = fputc_msvcrt(c, stream);
		}

		_unlock_file(stream);

		return nRet;
	}

	return fputc_msvcrt(c, stream);
}

_LCRT_DEFINE_IAT_SYMBOL(fputc);

extern "C" int __cdecl putc(
        int   _Character,
        FILE* _Stream
        )
{
	return fputc(_Character, _Stream);
}

_LCRT_DEFINE_IAT_SYMBOL(putc);

extern "C" int __cdecl _fputchar(
        int _Character
        )
{
	return fputc(_Character, /*stdout*/&_iob[1]);
}

_LCRT_DEFINE_IAT_SYMBOL(_fputchar);

extern "C" int __cdecl putchar(
        int _Character
        )
{
	return _fputchar(_Character);
}

_LCRT_DEFINE_IAT_SYMBOL(putchar);
