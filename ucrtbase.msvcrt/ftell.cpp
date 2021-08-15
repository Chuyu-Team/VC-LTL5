
#include <stdio.h>

//msvcrt没有_ftelli64，不过好在有fgetpos
extern "C" __int64 __cdecl _ftelli64(
	_Inout_ FILE* _Stream
)
{
	fpos_t _Position;

	if (fgetpos(_Stream, &_Position) != 0)
	{
		return -1;
	}
	else
	{
		return _Position;
	}
}

_LCRT_DEFINE_IAT_SYMBOL(_ftelli64);
