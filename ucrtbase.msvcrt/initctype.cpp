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
    // 由YY-Thunks 为Windows XP提供完整的TLS支持
	static thread_local lc_locale_name_buffer Current;		

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


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows2003
EXTERN_C __declspec(dllimport) extern unsigned int __lc_collate_cp;

EXTERN_C unsigned int __cdecl ___lc_collate_cp_func(void)
{
	return __lc_collate_cp;
}

_LCRT_DEFINE_IAT_SYMBOL(___lc_collate_cp_func);

#endif
