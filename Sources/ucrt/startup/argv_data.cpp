//
// argv_data.cpp
//
//      Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This file defines the global data that stores the command line with which the
// program was executed, along with the parsed arguments (if the arguments were
// parsed), and the accessors for the global data.
//
#include <corecrt_internal.h>

#undef __argc
#undef __argv
#undef __wargv
#undef _pgmptr
#undef _wpgmptr
#undef _acmdln
#undef _wcmdln

extern "C" {


// Note:  In general, either the narrow or wide string variables will be set,
// but not both.  These get initialized by the CRT startup sequence before any
// user code is executed.  There are cases where any or all of the pointers may
// be null during execution.  Do not assume that they are non-null.

__declspec(dllimport) extern int       __argc;   // The number of arguments in __argv or __wargv
__declspec(dllimport) extern  char**    __argv;  // The arguments as narrow strings
__declspec(dllimport) extern  wchar_t** __wargv; // The arguments as wide strings
__declspec(dllimport) extern  char*     _pgmptr; // The name of the program as a narrow string
__declspec(dllimport) extern  wchar_t*  _wpgmptr;// The name of the program as a wide string
__declspec(dllimport) extern  char*     _acmdln; // The raw command line as a narrow string
__declspec(dllimport) extern  wchar_t*  _wcmdln; // The raw command line as a wide string

_BEGIN_SECURE_CRT_DEPRECATION_DISABLE

#if defined _M_AMD64 || defined _M_ARM64 || defined _M_ARM

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
int*       __cdecl __p___argc()   { return &__argc;   }
_LCRT_DEFINE_IAT_SYMBOL(__p___argc);

char***    __cdecl __p___argv()   { return &__argv;   }
_LCRT_DEFINE_IAT_SYMBOL(__p___argv);

wchar_t*** __cdecl __p___wargv()  { return &__wargv;  }
_LCRT_DEFINE_IAT_SYMBOL(__p___wargv);

char**     __cdecl __p__pgmptr()  { return &_pgmptr;  }
_LCRT_DEFINE_IAT_SYMBOL(__p__pgmptr);

wchar_t**  __cdecl __p__wpgmptr() { return &_wpgmptr; }
_LCRT_DEFINE_IAT_SYMBOL(__p__wpgmptr);

char**     __cdecl __p__acmdln()  { return &_acmdln;  }
_LCRT_DEFINE_IAT_SYMBOL(__p__acmdln);

wchar_t**  __cdecl __p__wcmdln()  { return &_wcmdln;  }
_LCRT_DEFINE_IAT_SYMBOL(__p__wcmdln);


#endif

#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6 || defined _M_ARM64 || defined _M_ARM
errno_t __cdecl _get_wpgmptr(wchar_t** const result)
{
    _VALIDATE_RETURN_ERRCODE(result   != nullptr, EINVAL);
    _VALIDATE_RETURN_ERRCODE(_wpgmptr != nullptr, EINVAL);

    *result = _wpgmptr;
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_wpgmptr);

errno_t __cdecl _get_pgmptr(char** const result)
{
    _VALIDATE_RETURN_ERRCODE(result  != nullptr, EINVAL);
    _VALIDATE_RETURN_ERRCODE(_pgmptr != nullptr, EINVAL);
    *result = _pgmptr;
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_pgmptr);

#endif

_END_SECURE_CRT_DEPRECATION_DISABLE


#if 0
bool __cdecl __acrt_initialize_command_line()
{
    _acmdln = GetCommandLineA();
    _wcmdln = GetCommandLineW();
    return true;
}

bool __cdecl __acrt_uninitialize_command_line(bool const /* terminating */)
{
    return true;
}
#endif


} // extern "C"
