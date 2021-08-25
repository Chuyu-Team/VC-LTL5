#pragma once

#define MakeMiniVersion(v1,v2) (unsigned)(v2|(v1<<16))

#ifndef UMDF_USING_NTSTATUS
#define UMDF_USING_NTSTATUS
#endif

#ifndef _NTDEF_
typedef _Return_type_success_(return >= 0) long NTSTATUS;
typedef NTSTATUS* PNTSTATUS;
#endif

#define _Disallow_YY_KM_Namespace
#include <km.h>

__forceinline unsigned __cdecl __LTL_GetOsMinVersion()
{
    PEB* pPeb = (NtCurrentTeb())->ProcessEnvironmentBlock;

    return MakeMiniVersion(pPeb->OSMajorVersion, pPeb->OSMinorVersion);
}
