#pragma once

#define _FILE_DEFINED
typedef struct _iobuf
{
    char* _ptr;
    int   _cnt;
    char* _base;
    long   _flags;
    long   _file;
    int   _charbuf;
    int   _bufsiz;
    char* _tmpfname;
} _iobuf, FILE;

#ifdef __cplusplus
extern "C" {
#endif
 __declspec(dllimport) void __cdecl _lock(
    int locknum
);
__declspec(dllimport) void __cdecl _unlock(
    int locknum
);

__declspec(dllimport) extern _iobuf _iob[20];


//LCID* __cdecl ___lc_handle_func(void);
__declspec(dllimport) unsigned long* __cdecl ___lc_handle_func(void);

__declspec(dllimport) extern  char* _acmdln;
__declspec(dllimport) extern  wchar_t* _wcmdln;




#ifdef __cplusplus
}
#endif

#define _DCRTIMP
#define _CORECRT_BUILD
#define _CRT_SECURE_NO_WARNINGS
#define _MBCS