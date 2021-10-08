
#include <Windows.h>
#include "ptd_downlevel.h"

static ptd_downlevel ___ptd_downlevel_data[0x4000];


EXTERN_C ptd_downlevel * __fastcall __LTL_get_ptd_downlevel(BOOL bCanAlloc)
{
	const auto dwCurrentThreadId = GetCurrentThreadId();

	//我们做一个桶，分成128组，每组128个元素
	auto pPTD_downlevelGroup = &___ptd_downlevel_data[(dwCurrentThreadId >> 1) & 0x7F];

	for (int i = 0; i != 128; ++i)
	{
		//搜索是线程安全的，因为不可能二个线程ID是一样的
		if (pPTD_downlevelGroup[i].dwThreadId == dwCurrentThreadId)
		{
			return &pPTD_downlevelGroup[i];
		}
	}

	if (bCanAlloc)
	{
		for (int i = 0; i != 128; ++i)
		{
			//尝试搜索一个空闲的位置
			auto dwOrgThreadId = InterlockedCompareExchange(&pPTD_downlevelGroup[i].dwThreadId, dwCurrentThreadId, 0);

			if (dwOrgThreadId == 0)
			{
				//成功锁定了一个新的 ptd 快，我们先将数据初始化为 0
				memset(
					&pPTD_downlevelGroup[i]._thread_local_iph,
					0,
					sizeof(pPTD_downlevelGroup[i]) - size_t(&((ptd_downlevel*)NULL)->_thread_local_iph));


				__if_exists(ptd_downlevel::_CatchStateInParent)
				{
					pPTD_downlevelGroup[i]._CatchStateInParent = -2;//INVALID_CATCH_SPECIFIC_STATE
				}

				return &pPTD_downlevelGroup[i];
			}
		}
	}

	return nullptr;
}

static VOID NTAPI ptd_downlevel_TLS_CALLBACK(
	PVOID DllHandle,
	DWORD Reason,
	PVOID Reserved
)
{
	if (DLL_THREAD_DETACH == Reason)
	{
		auto ptd = __LTL_get_ptd_downlevel(FALSE);

		//释放ptd
		if (ptd)
			InterlockedExchange(&ptd->dwThreadId, 0);
	}
}

#pragma section(".CRT$XLB",    long, read) // LTL Loader TLS Callback
__declspec(allocate(".CRT$XLB")) static PIMAGE_TLS_CALLBACK __LTL_TLS_CALLBACKs = ptd_downlevel_TLS_CALLBACK;

#if defined(_X86_)
#pragma comment(linker, "/INCLUDE:__tls_used")
#else
#pragma comment(linker, "/INCLUDE:_tls_used")
#endif
