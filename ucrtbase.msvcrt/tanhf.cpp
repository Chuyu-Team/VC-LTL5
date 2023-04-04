#include <math.h>

#pragma warning(disable : 28251)

#if defined _M_X64 || defined _M_ARM || defined _M_ARM64 || defined _M_HYBRID_X86_ARM64 || defined _M_ARM64EC
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
#pragma function(tanhf)

extern "C" float __CRTDECL tanhf(float _X)
{
    // 2003的DLL有 double版，凑合一下吧。
    return (float)tanh(_X);
}

_LCRT_DEFINE_IAT_SYMBOL(tanhf);

#endif // WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
#endif // defined _M_X64 || defined _M_ARM || defined _M_ARM64 || defined _M_HYBRID_X86_ARM64 || defined _M_ARM64EC
