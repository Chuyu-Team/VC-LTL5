#include <Windows.h>


HMODULE __fastcall __LTL_GetMSVCRTModule()
{
	static HMODULE MSVCRTModuleCache;


	if (MSVCRTModuleCache)
	{
		if (MSVCRTModuleCache == (HMODULE)INVALID_HANDLE_VALUE)
		{
			return NULL;
		}
		return MSVCRTModuleCache;
	}


	auto hMSVCRT = GetModuleHandleW(L"msvcrt.dll");

	MSVCRTModuleCache = hMSVCRT ? hMSVCRT : (HMODULE)INVALID_HANDLE_VALUE;

	return hMSVCRT;
}