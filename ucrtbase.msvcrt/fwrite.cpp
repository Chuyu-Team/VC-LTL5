#include <Windows.h>
#include <corecrt_wstdio.h>
#include <stdio.h>
#include <corecrt_internal_stdio.h>
#include "msvcrt_loadhelper.h"

__EXPAND_MSVCRT_FUN(fwrite);

static size_t __cdecl fwrite_msvcrt(
    void const* const buffer,
    size_t      const size,
    size_t      const count,
    FILE* const stream
    )
{
    if (auto pfwrite_msvcrt = try_get_fwrite())
    {
        return pfwrite_msvcrt(buffer, size, count, stream);
    }


    return 0;
}

extern "C" size_t __cdecl fwrite(
    void const* const buffer,
    size_t      const size,
    size_t      const count,
    FILE* const stream
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

extern "C" size_t __cdecl _fwrite_nolock(
    void const* const buffer,
    size_t      const size,
    size_t      const count,
    FILE* const stream
)
{
    if (stream == /*stdout*/&_iob[1] || stream == /*stderr*/&_iob[2])
    {
        auto Success = __acrt_stdio_begin_temporary_buffering_nolock(stream);
        auto result = fwrite_msvcrt(buffer, size, count, stream);
        __acrt_stdio_end_temporary_buffering_nolock(Success, stream);
        return result;
    }
    else
    {
        return fwrite_msvcrt(buffer, size, count, stream);
    }
}

_LCRT_DEFINE_IAT_SYMBOL(_fwrite_nolock);
