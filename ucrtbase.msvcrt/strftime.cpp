
#include <time.h>
#include <internal_shared.h>
#include <vcruntime_internal.h>

#if WindowsTargetPlatformMinVersion >= WindowsTargetPlatformWindows6 && WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
//Vista以及以上版本才存在_wcsftime_l，因此只在Vista以上模式输出此函数
extern "C" size_t __cdecl _strftime_l(
	char* const string,
	size_t      const maxsize,
	char const* const format,
	tm const* const timeptr,
	_locale_t   const locale
)
{
	if (!locale)
		return strftime(string, maxsize, format, timeptr);

	unsigned int const lc_time_cp = locale->locinfo->_locale_lc_codepage;

	_VALIDATE_RETURN(string != nullptr, EINVAL, 0)
		_VALIDATE_RETURN(maxsize != 0, EINVAL, 0)
		* string = '\0';

	_VALIDATE_RETURN(format != nullptr, EINVAL, 0)
		_VALIDATE_RETURN(timeptr != nullptr, EINVAL, 0)


		auto cwFormat = MultiByteToWideChar(
			lc_time_cp,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			format,
			-1,
			nullptr,
			0
		);

	if (cwFormat == 0)
		return 0;

	__crt_scoped_stack_ptr<wchar_t> wformat(_malloca_crt_t(wchar_t, cwFormat));
	if (!wformat)
		return 0;

	if (MultiByteToWideChar(
		lc_time_cp,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		format,
		-1,
		wformat.get(),
		cwFormat
	) == 0)
	{
		errno = EINVAL;
		return 0;
	}



	__crt_scoped_stack_ptr<wchar_t> wstring(_malloca_crt_t(wchar_t, maxsize));

	if (!wstring)
		return 0;

	size_t const wcsftime_result = _wcsftime_l(wstring.get(), maxsize, wformat.get(), timeptr, locale);
	if (wcsftime_result == 0)
	{
		return 0;
	}


	auto string_used = WideCharToMultiByte(
		lc_time_cp,
		0,
		wstring.get(),
		-1,
		string,
		maxsize,
		nullptr,
		nullptr
	);

	//缓冲区不足或者缓冲区不足以容纳
	if (string_used == 0 || string_used > maxsize)
		return 0;


	// The WideCharToMultiByte result includes the null terminator; the strftime
	// function result does not:
	return string_used - 1;
}

_LCRT_DEFINE_IAT_SYMBOL(_strftime_l);

#endif
