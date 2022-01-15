#include <setjmpex.h>


namespace DLL
{
    extern "C"
    __declspec(dllimport)
    int
    __cdecl
    __intrinsic_setjmpex(
        _Out_ jmp_buf _Buf
        );
}


extern "C"
int
__cdecl
__intrinsic_setjmpex(
    _Out_ jmp_buf _Buf
    )
{
    return DLL::__intrinsic_setjmpex(_Buf);
}