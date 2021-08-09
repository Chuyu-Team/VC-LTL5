
#include "framework.h"
#include "vcruntime_internal.h"

extern "C" int* __cdecl __processing_throw();

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 14393)

extern "C" int __cdecl __uncaught_exceptions()
{
#ifdef __BuildWithMSVCRT
	return *__processing_throw();
#else
	auto ptd = __vcrt_getptd_noinit();

	return ptd ? ptd->_ProcessingThrow : 0;
#endif
}

_LCRT_DEFINE_IAT_SYMBOL(__uncaught_exceptions);

#endif
