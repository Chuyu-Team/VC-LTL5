#include <corecrt.h>

#undef _pwctype

extern "C" __declspec(dllimport) extern const wctype_t * _pwctype;

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
extern "C" const wctype_t * __cdecl __pwctype_func(void)
{
	return _pwctype;
}

_LCRT_DEFINE_IAT_SYMBOL(__pwctype_func);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
extern "C" const wctype_t * _wctype = _pwctype - 1;
extern "C" __declspec(selectany) void const* const _LCRT_DEFINE_IAT_SYMBOL_MAKE_NAME(_wctype) = &_wctype;
#endif