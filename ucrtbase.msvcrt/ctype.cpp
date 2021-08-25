#include <corecrt.h>

#undef _pwctype

__declspec(dllimport) extern const wctype_t* _pwctype;

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
extern "C" const wctype_t * __cdecl __pwctype_func(void)
{
	return _pwctype;
}

_LCRT_DEFINE_IAT_SYMBOL(__pwctype_func);
#endif