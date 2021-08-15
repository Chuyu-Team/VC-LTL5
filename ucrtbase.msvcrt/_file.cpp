#include <corecrt_internal_stdio.h>

#undef _IOB_ENTRIES
#define _IOB_ENTRIES 20

extern "C" __declspec(dllimport) extern _iobuf _iob[_IOB_ENTRIES];

extern "C" FILE * __cdecl __acrt_iob_func(unsigned const id)
{
    return (FILE*)&_iob[id];
}

_LCRT_DEFINE_IAT_SYMBOL(__acrt_iob_func);



extern "C" __declspec(dllimport) void __cdecl _lock(
	int locknum
);

extern "C" __declspec(dllimport) void __cdecl _unlock(
	int locknum
);


static __inline bool IsInternalStream(_iobuf* const stream)
{
	return ((byte*)stream >= (byte*)_iob) && ((byte*)stream <= (byte*)(_iob + _IOB_ENTRIES - 1));
}


extern "C" void __cdecl _lock_file(FILE* const stream)
{
	if (IsInternalStream(stream))
		_lock(((_iobuf*)stream - _iob) + 0x10);
	else
		EnterCriticalSection(&((__crt_stdio_stream_data*)stream)->_lock);
}

_LCRT_DEFINE_IAT_SYMBOL(_lock_file);


extern "C" void __cdecl _unlock_file(FILE* const stream)
{
	if (IsInternalStream(stream))
		_unlock(((_iobuf*)stream - _iob) + 0x10);
	else
		LeaveCriticalSection(&((__crt_stdio_stream_data*)stream)->_lock);
}


_LCRT_DEFINE_IAT_SYMBOL(_unlock_file);


extern "C" errno_t __cdecl _get_stream_buffer_pointers(
    FILE*   const public_stream,
    char*** const base,
    char*** const ptr,
    int**   const count
    )
{
    _VALIDATE_RETURN_ERRCODE(public_stream != nullptr, EINVAL);

    //__crt_stdio_stream const stream(public_stream);
    if (base)
    {
        *base = &public_stream->_base;
    }

    if (ptr)
    {
        *ptr = &public_stream->_ptr;
    }

    if (count)
    {
        *count = &public_stream->_cnt;
    }

    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_stream_buffer_pointers);
