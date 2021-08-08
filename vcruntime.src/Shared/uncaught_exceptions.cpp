
#include "framework.h"
#include "vcruntime_internal.h"

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 14393)

extern "C" int __cdecl __uncaught_exceptions()
{
	auto ptd = __vcrt_getptd_noinit();

	return ptd ? ptd->_ProcessingThrow : 0;
}

_LCRT_DEFINE_IAT_SYMBOL(__uncaught_exceptions);

#endif
