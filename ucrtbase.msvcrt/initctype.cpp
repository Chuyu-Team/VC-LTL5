#include <locale.h>
#include <corecrt_internal.h>
#include <vcruntime_internal.h>

EXTERN_C int __cdecl ___mb_cur_max_l_func(_locale_t locale)
{
	return locale == nullptr
		? ___mb_cur_max_func()
		: locale->locinfo->_locale_mb_cur_max;

}

_LCRT_DEFINE_IAT_SYMBOL(___mb_cur_max_l_func);



struct lc_locale_name_buffer
{
	wchar_t* locale_name[6];
	wchar_t locale_name_buffer[6][LOCALE_NAME_MAX_LENGTH];
	unsigned long lc_handle[6];
};

EXTERN_C wchar_t** __cdecl ___lc_locale_name_func(void)
{
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
	//XP系统 thread_local 存在限制，所以我们需要在本地弄一份很大的数组，对于这个场景很显然我们不需要太强大散列支持
	static lc_locale_name_buffer ThreadBuffer[0x4000];

	auto &Current = ThreadBuffer[(GetCurrentThreadId() >> 1) % __crt_countof(ThreadBuffer)]
#else
	static thread_local lc_locale_name_buffer Current;
#endif
		

	auto lc_handle = ___lc_handle_func();

	for (int i = 0; i != _countof(Current.locale_name_buffer); ++i)
	{
		Current.locale_name[i] = Current.locale_name_buffer[i];
	}

	for (int i = 0; i != _countof(Current.lc_handle); ++i)
	{
		if (Current.lc_handle[i] != lc_handle[i])
		{
			Current.locale_name_buffer[i][0] = L'\0';
			//不一致，重新缓存
			LCIDToLocaleName(lc_handle[i], Current.locale_name_buffer[i], _countof(Current.locale_name_buffer[i]), 0);
			Current.lc_handle[i] = lc_handle[i];
		}
	}

	//返回新数据
	return Current.locale_name;
}

_LCRT_DEFINE_IAT_SYMBOL(___lc_locale_name_func);
