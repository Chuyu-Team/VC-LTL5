

extern "C" __declspec(dllimport) void __cdecl _assert(
	_In_z_ char const* _Message,
	_In_z_ char const* _File,
	_In_   unsigned       _Line
	);


//WinXP不支持_wassert，因此我们通过字符串转换再调用_assert实现。
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" void __cdecl _wassert(
	_In_z_ wchar_t const* _Message,
	_In_z_ wchar_t const* _File,
	_In_   unsigned       _Line
	)
{
	//VC-LTL只针提供Release编译，因此此函数单纯做中断程序处理。
	_assert(nullptr, nullptr, _Line);
}

_LCRT_DEFINE_IAT_SYMBOL(_wassert);
#endif
