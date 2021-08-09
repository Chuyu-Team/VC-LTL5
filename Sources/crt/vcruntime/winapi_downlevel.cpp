#include "framework.h"

#include <Windows.h>

 extern "C" BOOL __cdecl __vcrt_InitializeCriticalSectionEx(
        LPCRITICAL_SECTION const critical_section,
        DWORD              const spin_count,
        DWORD              const flags
        )
{
     //考虑到XP支持，直接使用它
    return InitializeCriticalSectionAndSpinCount(critical_section, spin_count);
}


 _LCRT_DEFINE_IAT_SYMBOL(__vcrt_InitializeCriticalSectionEx);
