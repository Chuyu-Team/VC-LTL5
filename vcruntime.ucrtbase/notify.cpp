

#if defined _M_AMD64
extern "C" __declspec(dllimport) void __NLG_Dispatch2(void* p1, void* p2, int p3, long long);

extern "C" void _NLG_Notify(void* p1, void* p2, int p3)
{
	return __NLG_Dispatch2(p1, p2, p3, 0x19930520);
}
#endif