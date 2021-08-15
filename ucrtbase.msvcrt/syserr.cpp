
#ifdef _sys_nerr
#undef _sys_nerr
#endif


#ifdef _sys_errlist
#undef _sys_errlist
#endif

extern "C" __declspec(dllimport) extern char const* const _sys_errlist[];
extern "C" __declspec(dllimport) extern int const _sys_nerr;

extern "C" int* __cdecl __sys_nerr()
{
	return const_cast<int*>(&_sys_nerr);
}

_LCRT_DEFINE_IAT_SYMBOL(__sys_nerr);

extern "C" char** __cdecl __sys_errlist()
{
	return const_cast<char**>(_sys_errlist);
}

_LCRT_DEFINE_IAT_SYMBOL(__sys_errlist);
