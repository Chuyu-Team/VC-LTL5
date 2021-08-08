#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#include <Windows.h>

#if defined(_X86_)  && !defined(_DLL)

typedef void(__fastcall *PCOOKIE_CHECK)(UINT_PTR);

EXTERN_C __declspec(dllimport) EXCEPTION_DISPOSITION __cdecl _except_handler4_common(
	IN PUINT_PTR                        CookiePointer,
	IN PCOOKIE_CHECK                    CookieCheckFunction,
	IN PEXCEPTION_RECORD                ExceptionRecord,
	IN PEXCEPTION_REGISTRATION_RECORD   EstablisherFrame,
	IN OUT PCONTEXT                     ContextRecord,
	IN OUT PVOID                        DispatcherContext
	);

EXTERN_C
DECLSPEC_GUARD_SUPPRESS
EXCEPTION_DISPOSITION
__cdecl
_except_handler4(
	IN PEXCEPTION_RECORD                ExceptionRecord,
	IN PEXCEPTION_REGISTRATION_RECORD   EstablisherFrame,
	IN OUT PCONTEXT                     ContextRecord,
	IN OUT PVOID                        DispatcherContext
	)
{
	return _except_handler4_common(&__security_cookie, &__security_check_cookie, ExceptionRecord, EstablisherFrame, ContextRecord, DispatcherContext);
}


#endif