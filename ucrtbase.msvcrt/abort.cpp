

//此函数什么都不做，仅供编译通过处理，因此任何调用abort函数必定静默退出。
extern "C" unsigned int __cdecl _set_abort_behavior(
	_In_ unsigned int _Flags,
	_In_ unsigned int _Mask
)
{
	return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_set_abort_behavior);
