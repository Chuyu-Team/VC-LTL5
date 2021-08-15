

#include <stdlib.h>
#include <corecrt_internal.h>
#include <locale.h>


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindowsBlue
//msvrct仅支持_Strftime，我们可以将通过字符串转换，得到_Wcsftime
extern "C" size_t __cdecl _Wcsftime(
	wchar_t* const buffer,
	size_t         const max_size,
	wchar_t const* const format,
	tm const* const timeptr,
	void* const lc_time_arg
)
{
	_VALIDATE_RETURN(buffer != nullptr, EINVAL, 0)
		_VALIDATE_RETURN(max_size != 0, EINVAL, 0)
		* buffer = '\0';

	_VALIDATE_RETURN(format != nullptr, EINVAL, 0)
		_VALIDATE_RETURN(timeptr != nullptr, EINVAL, 0)

		size_t Count = 0;

	unsigned int const lc_time_cp = ___lc_codepage_func();

	auto ch_format = WideCharToMultiByte(lc_time_cp, 0, format, -1, 0, 0, nullptr, nullptr);
	if (ch_format == 0)
	{
		return 0;
	}

	auto formatA = (char*)malloc(ch_format);
	//开辟2倍缓冲区
	auto BufferA = (char*)malloc(max_size * 2);

	if (formatA == nullptr || BufferA == nullptr)
	{
		//内存不足
		goto __Error;
	}

	if (WideCharToMultiByte(lc_time_cp, 0, format, -1, formatA, ch_format, nullptr, nullptr))
	{
		//转换失败
		goto __Error;
	}


	Count = _Strftime(BufferA, max_size * 2, formatA, timeptr, lc_time_arg);

	if (Count)
	{
		//
		Count = MultiByteToWideChar(lc_time_cp, 0, BufferA, Count, buffer, max_size);

		//改函数长度并不包含null，因为长度减一
		if (Count)
			--Count;
	}



__Error:
	if (formatA)
		free(formatA);
	if (BufferA)
		free(BufferA);
	return Count;
}

_LCRT_DEFINE_IAT_SYMBOL(_Wcsftime);

#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindowsBlue
//msvrct仅支持_Getdays，我们可以将通过字符串转换，得到_W_Getdays
extern "C" wchar_t* __cdecl _W_Getdays(void)
{
	auto szDays = _Getdays();
	if (!szDays)
		return nullptr;

	unsigned int const lc_time_cp = ___lc_codepage_func();

	auto ch_Days = MultiByteToWideChar(lc_time_cp, 0, szDays, -1, 0, 0);
	if (ch_Days == 0)
	{
		return nullptr;
	}

	//内存申请失败
	auto szDaysW = (wchar_t*)malloc(ch_Days * sizeof(wchar_t));
	if (!szDaysW)
		return nullptr;

	ch_Days = MultiByteToWideChar(lc_time_cp, 0, szDays, -1, szDaysW, ch_Days);

	if (ch_Days)
	{
		return szDaysW;
	}
	else
	{
		free(szDaysW);
		return nullptr;
	}
}

_LCRT_DEFINE_IAT_SYMBOL(_W_Getdays);

#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindowsBlue
//msvrct仅支持_Getmonths，我们可以将通过字符串转换，得到_W_Getmonths
extern "C" wchar_t* __cdecl _W_Getmonths(void)
{
	auto szMonths = _Getmonths();
	if (!szMonths)
		return nullptr;

	unsigned int const lc_time_cp = ___lc_codepage_func();

	auto ch_Months = MultiByteToWideChar(lc_time_cp, 0, szMonths, -1, 0, 0);
	if (ch_Months == 0)
	{
		return nullptr;
	}

	//内存申请失败
	auto szMonthsW = (wchar_t*)malloc(ch_Months * sizeof(wchar_t));
	if (!szMonthsW)
		return nullptr;

	ch_Months = MultiByteToWideChar(lc_time_cp, 0, szMonths, -1, szMonthsW, ch_Months);

	if (ch_Months)
	{
		return szMonthsW;
	}
	else
	{
		free(szMonthsW);
		return nullptr;
	}
}

_LCRT_DEFINE_IAT_SYMBOL(_W_Getmonths);

#endif
