#include <excpt.h>

typedef struct _EXCEPTION_POINTERS EXCEPTION_POINTERS, * PEXCEPTION_POINTERS;

extern "C" __declspec(dllimport) int __cdecl _XcptFilter(
	unsigned long       const xcptnum,
	PEXCEPTION_POINTERS const pxcptinfoptrs
	);

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
extern "C" int __cdecl _seh_filter_dll(
	unsigned long       const xcptnum,
	PEXCEPTION_POINTERS const pxcptinfoptrs
	)
{
	if (xcptnum != ('msc' | 0xE0000000))
		return EXCEPTION_CONTINUE_SEARCH;

	return _XcptFilter(xcptnum, pxcptinfoptrs);
}

_LCRT_DEFINE_IAT_SYMBOL(_seh_filter_dll);
#endif