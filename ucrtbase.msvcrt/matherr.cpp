

#include <internal_shared.h>
#include "msvcrt_loadhelper.h"

typedef int(__cdecl* _HANDLE_MATH_ERROR)(struct _exception*);

static _HANDLE_MATH_ERROR user_matherr;

extern "C" void __cdecl __setusermatherr(
	_HANDLE_MATH_ERROR pf
	);

__EXPAND_MSVCRT_FUN(__setusermatherr);

extern "C" void __cdecl __setusermatherr(
	_HANDLE_MATH_ERROR pf
	)
{
	//继续调用msvcrt版本是为了能接管 msvcrt的user_matherr，但是有一个问题，如果其他人调用了 __setusermatherr，这会覆盖msvcrt的user_matherr
	//二者会发生不一致……这类问题无法一种健全的解决方案，但是可以暂不处理
	if (auto p_setusermatherr_msvcrt = try_get___setusermatherr())
	{
		p_setusermatherr_msvcrt(pf);
	}

	user_matherr = __crt_fast_encode_pointer(pf);
}

_LCRT_DEFINE_IAT_SYMBOL(__setusermatherr);


extern "C" bool __cdecl __acrt_has_user_matherr()
{
	return user_matherr != nullptr && __crt_fast_decode_pointer(user_matherr);
}

extern "C" int __cdecl __acrt_invoke_user_matherr(struct _exception* ex)
{
	auto tmp = user_matherr;

	if (tmp)
	{
		tmp = __crt_fast_decode_pointer(tmp);

		if (tmp)
			return tmp(ex);
	}
	

	return 0;
}
