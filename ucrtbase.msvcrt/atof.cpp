
#include <stdlib.h>
#include <internal_shared.h>
#include <corecrt_math.h>


//msvcrt没有_atoflt，但是我们可以使用atof转换
extern "C" int __cdecl _atoflt(_Out_ _CRT_FLOAT * _Result, _In_z_ char const* _String)
{
	_VALIDATE_RETURN(_Result != nullptr, EINVAL, _DOMAIN);

	_Result->f = atof(_String);

	switch (fpclassify(_Result->f))
	{
	case FP_SUBNORMAL:
		//如果为太小，则设置为下溢
		return _UNDERFLOW;
		break;
	case FP_INFINITE:
		//如果为无穷，则设置为上溢
		return _OVERFLOW;
		break;
	default:
		//其他情况设置为0
		return 0;
		break;
	}
}

_LCRT_DEFINE_IAT_SYMBOL(_atoflt);
