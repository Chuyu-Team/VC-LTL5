#include <setjmp.h>

namespace DLL
{
    extern "C"
    __declspec(dllimport)
    int
    __cdecl
    __intrinsic_setjmp(
        _Out_ jmp_buf _Buf
        );
}


extern "C"
int
__cdecl
__intrinsic_setjmp(
    _Out_ jmp_buf _Buf
    )
{
    return DLL::__intrinsic_setjmp(_Buf);
}