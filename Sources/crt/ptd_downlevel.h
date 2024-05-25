

#ifndef _LTL_ptd_downlevel

#define _LTL_ptd_downlevel

struct ptd_downlevel
{
    uintptr_t bInit;
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
	void(__cdecl* _thread_local_iph)(
		wchar_t const*,
		wchar_t const*,
		wchar_t const*,
		unsigned int,
		uintptr_t
		);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindowsXP
    int         _ProcessingThrow; /* for uncaught_exception */
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
    int _cxxReThrow;
#endif

#if defined (_M_IA64) || defined (_M_AMD64) || defined (_M_ARM64) || defined (_M_ARM) || defined _M_HYBRID
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
	uintptr_t   _ImageBase;
	uintptr_t _ThrowImageBase;
    void* _pForeignException;
#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 19041)
    int _CatchStateInParent;
#endif

#endif // defined (_M_IA64) || defined (_M_AMD64) || defined (_M_ARM64) || defined (_M_ARM)

#if defined(_M_IX86)

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
	void* _pFrameInfoChain;
#endif
#endif // defined(_M_IX86)
};

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 19041)
__declspec(noinline) inline ptd_downlevel* __fastcall __LTL_get_ptd_downlevel()
{
    static thread_local ptd_downlevel s_ptd_downlevel;
    if (!s_ptd_downlevel.bInit)
    {
        s_ptd_downlevel.bInit = 1;
        __if_exists(ptd_downlevel::_CatchStateInParent)
        {
            s_ptd_downlevel._CatchStateInParent = -2;//INVALID_CATCH_SPECIFIC_STATE
        }
    }

    return &s_ptd_downlevel;
}
#endif
#endif // _LTL_ptd_downlevel
