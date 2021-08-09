#include <corecrt_terminate.h>
#include <ehdata.h>
#include <framework.h>

#if defined _M_AMD64 || defined _M_ARM || defined _M_ARM64

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 17134)

extern "C" EXCEPTION_DISPOSITION __cdecl __C_specific_handler_noexcept(
	_In_    struct _EXCEPTION_RECORD* ExceptionRecord,
	_In_    void* EstablisherFrame,
	_Inout_ struct _CONTEXT* ContextRecord,
	_Inout_ struct _DISPATCHER_CONTEXT* DispatcherContext
)
{
	auto Excption = __C_specific_handler(ExceptionRecord, EstablisherFrame, ContextRecord, DispatcherContext);

	if (IS_DISPATCHING(ExceptionRecord->ExceptionFlags)
		&& ExceptionRecord->ExceptionCode == EH_EXCEPTION_NUMBER
		&& Excption == ExceptionContinueSearch)
	{
		//结束进程
		terminate();
	}

	return Excption;
}

_LCRT_DEFINE_IAT_SYMBOL(__C_specific_handler_noexcept);

#endif

#endif