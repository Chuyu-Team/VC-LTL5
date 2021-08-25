#include <float.h>
#include <errno.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _controlfp_s(
	_Out_opt_ unsigned int* _CurrentState,
	_In_      unsigned int  _NewValue,
	_In_      unsigned int  _Mask
	)
{
	_Mask &= 0xFFF7FFFF;

	if ((_NewValue&_Mask) & 0xFCF0FCE0)
	{
		if (_CurrentState)
			*_CurrentState = _control87(0, 0);

		errno = EINVAL;
		_invalid_parameter_noinfo();
		return EINVAL;
	}
	else
	{
		auto State = _control87(_NewValue, _Mask);
		if (_CurrentState)
			*_CurrentState = State;

		return 0;
	}
}

_LCRT_DEFINE_IAT_SYMBOL(_controlfp_s);
#endif