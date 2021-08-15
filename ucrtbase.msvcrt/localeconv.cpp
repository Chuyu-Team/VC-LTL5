
#include <locale.h>
#include "msvcrt_loadhelper.h"

struct lconv_Win7
{
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
    char* int_curr_symbol;
    char* currency_symbol;
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char     int_frac_digits;
    char     frac_digits;
    char     p_cs_precedes;
    char     p_sep_by_space;
    char     n_cs_precedes;
    char     n_sep_by_space;
    char     p_sign_posn;
    char     n_sign_posn;
    wchar_t* _W_decimal_point;
    wchar_t* _W_thousands_sep;
    wchar_t* _W_int_curr_symbol;
    wchar_t* _W_currency_symbol;
    wchar_t* _W_mon_decimal_point;
    wchar_t* _W_mon_thousands_sep;
    wchar_t* _W_positive_sign;
    wchar_t* _W_negative_sign;
};


static_assert(sizeof(lconv_Win7) == sizeof(lconv), "lconv 与 lconv_Win7大小不一致");

struct lconv_with_static_buffer : public lconv
{
    wchar_t _W_decimal_point_buffer[2];
    wchar_t _W_thousands_sep_buffer[2];
    wchar_t _W_int_curr_symbol_buffer[4];
    wchar_t _W_currency_symbol_buffer[128];
    wchar_t _W_mon_decimal_point_buffer[128];
    wchar_t _W_mon_thousands_sep_buffer[128];
    wchar_t _W_positive_sign_buffer[128];
    wchar_t _W_negative_sign_buffer[128];
};

static wchar_t* __fastcall MultiByteToWideCharHelper(UINT CodePage, LPCCH lpMultiByteStr, LPWSTR lpWideCharStaticBuffer, DWORD cchStaticBufferSize)
{
    if (lpMultiByteStr == nullptr)
        return nullptr;

    if (lpMultiByteStr[0] == '\0')
        return L"";

    auto Count = MultiByteToWideChar(CodePage, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, lpMultiByteStr, -1, lpWideCharStaticBuffer, cchStaticBufferSize - 1);

    //保证 0 结尾
    lpWideCharStaticBuffer[Count] = L'\0';

    return lpWideCharStaticBuffer;
}

//
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows7
extern "C" lconv* __cdecl localeconv(void)
{
    __EXPAND_MSVCRT_FUN(localeconv);

    if (pMSVCRT_localeconv == nullptr)
    {
        //正常不应该为空！！
        return nullptr;
    }

    auto pOrglconv = pMSVCRT_localeconv();

    if (__LTL_GetOsMinVersion() >= MakeMiniVersion(6, 1))
    {
        //如果是Windows 7，以及更高版本，那么直接返回指针本身即可
        return pOrglconv;
    }

    auto _Page = ___lc_codepage_func();

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
    //低于Vista，那么无法安全的使用 thread_local
    static lconv_with_static_buffer ThreadBuffer[0x4000];

    auto& Current = ThreadBuffer[(GetCurrentThreadId() >> 1) % __crt_countof(ThreadBuffer)];
#else
    static thread_local lconv_with_static_buffer Current;
#endif

    if (Current.decimal_point != pOrglconv->decimal_point)
    {
        Current.decimal_point = pOrglconv->decimal_point;
        Current._W_decimal_point = MultiByteToWideCharHelper(_Page, pOrglconv->decimal_point, Current._W_decimal_point_buffer, __crt_countof(Current._W_decimal_point_buffer));
    }

    if (Current.thousands_sep != pOrglconv->thousands_sep)
    {
        Current.thousands_sep = pOrglconv->thousands_sep;
        Current._W_thousands_sep = MultiByteToWideCharHelper(_Page, pOrglconv->thousands_sep, Current._W_thousands_sep_buffer, __crt_countof(Current._W_thousands_sep_buffer));
    }

    Current.grouping = pOrglconv->grouping;

    if (Current.int_curr_symbol != pOrglconv->int_curr_symbol)
    {
        Current.int_curr_symbol = pOrglconv->int_curr_symbol;

        if (pOrglconv->int_curr_symbol)
        {
            Current._W_int_curr_symbol = Current._W_int_curr_symbol_buffer;

            //这个货币符号必然为字母，而且最多3个字符
            int i = 0;
            for (; i != __crt_countof(Current._W_int_curr_symbol_buffer) - 1 && pOrglconv->int_curr_symbol[i] != '\0'; ++i)
            {
                Current._W_int_curr_symbol_buffer[i] = pOrglconv->int_curr_symbol[i];
            }

            Current._W_int_curr_symbol_buffer[i] = L'\0';
        }
        else
        {
            Current._W_int_curr_symbol = nullptr;
        }
    }
    
    if (Current.currency_symbol != pOrglconv->currency_symbol)
    {
        Current.currency_symbol = pOrglconv->currency_symbol;
        Current._W_currency_symbol = MultiByteToWideCharHelper(_Page, pOrglconv->currency_symbol, Current._W_currency_symbol_buffer, __crt_countof(Current._W_currency_symbol_buffer));
    }
    
    if (Current.mon_decimal_point != pOrglconv->mon_decimal_point)
    {
        Current.mon_decimal_point = pOrglconv->mon_decimal_point;
        Current._W_mon_decimal_point = MultiByteToWideCharHelper(_Page, pOrglconv->mon_decimal_point, Current._W_mon_decimal_point_buffer, __crt_countof(Current._W_mon_decimal_point_buffer));
    }

    if (Current.mon_thousands_sep != pOrglconv->mon_thousands_sep)
    {
        Current.mon_thousands_sep = pOrglconv->mon_thousands_sep;
        Current._W_mon_thousands_sep = MultiByteToWideCharHelper(_Page, pOrglconv->mon_thousands_sep, Current._W_mon_thousands_sep_buffer, __crt_countof(Current._W_mon_thousands_sep_buffer));
    }

    Current.mon_grouping = pOrglconv->mon_grouping;

    if (Current.positive_sign != pOrglconv->positive_sign)
    {
        Current.positive_sign = pOrglconv->positive_sign;
        Current._W_positive_sign = MultiByteToWideCharHelper(_Page, pOrglconv->positive_sign, Current._W_positive_sign_buffer, __crt_countof(Current._W_positive_sign_buffer));
    }
    
    if (Current.negative_sign != pOrglconv->negative_sign)
    {
        Current.negative_sign = pOrglconv->negative_sign;
        Current._W_negative_sign = MultiByteToWideCharHelper(_Page, pOrglconv->negative_sign, Current._W_negative_sign_buffer, __crt_countof(Current._W_negative_sign_buffer));
    }

    Current.int_frac_digits = pOrglconv->int_frac_digits;
    Current.frac_digits = pOrglconv->frac_digits;
    Current.p_cs_precedes = pOrglconv->p_cs_precedes;
    Current.p_sep_by_space = pOrglconv->p_sep_by_space;
    Current.n_cs_precedes = pOrglconv->n_cs_precedes;
    Current.n_sep_by_space = pOrglconv->n_sep_by_space;
    Current.p_sign_posn = pOrglconv->p_sign_posn;
    Current.n_sign_posn = pOrglconv->n_sign_posn;
    return &Current;
}

_LCRT_DEFINE_IAT_SYMBOL(localeconv);
#endif