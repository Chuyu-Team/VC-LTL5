#ifdef _DEBUG

#include <crtdbg.h>


extern "C" size_t __cdecl _CrtSetDebugFillThreshold(size_t const new_threshold)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetDebugFillThreshold);

extern "C" size_t __cdecl _CrtGetDebugFillThreshold(void)
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtGetDebugFillThreshold);

#endif



























