

#ifndef _LTL_ptd_downlevel

#define _LTL_ptd_downlevel

struct ptd_downlevel
{
	//所需线程ID
	unsigned dwThreadId;
	void(__cdecl* _thread_local_iph)(
		wchar_t const*,
		wchar_t const*,
		wchar_t const*,
		unsigned int,
		uintptr_t
		);
#if defined (_M_IA64) || defined (_M_AMD64) || defined (_M_ARM64) || defined (_M_ARM)
	void* _pForeignException;
	uintptr_t   _ImageBase;
	uintptr_t _ThrowImageBase;
#endif
	int _cxxReThrow;
#if defined _M_X64 || defined _M_ARM || defined _M_ARM64 || defined _M_HYBRID
	int _CatchStateInParent;
#endif
#if defined(_M_IX86)
	int         _ProcessingThrow; /* for uncaught_exception */
	void* _pFrameInfoChain;
#endif
};

EXTERN_C ptd_downlevel* __fastcall __LTL_get_ptd_downlevel(BOOL bCanAlloc);

#endif