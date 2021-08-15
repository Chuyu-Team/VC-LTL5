
extern "C" __declspec(dllimport) extern unsigned char _mbctype[];
extern "C" __declspec(dllimport) extern unsigned char _mbcasemap[];

#if defined _M_AMD64 || defined _M_ARM64 || defined _M_ARM

extern "C" unsigned char* __cdecl __p__mbctype()
{
    return _mbctype;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__mbctype);

extern "C" unsigned char* __cdecl __p__mbcasemap()
{
    return _mbcasemap;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__mbcasemap);

#endif
