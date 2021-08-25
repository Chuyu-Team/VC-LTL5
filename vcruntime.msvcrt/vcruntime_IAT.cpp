

_LCRT_DEFINE_IAT_SYMBOL_EXTERN(__NLG_Dispatch2);
_LCRT_DEFINE_IAT_SYMBOL_EXTERN(__NLG_Return2);

#ifdef _M_IX86
_LCRT_DEFINE_IAT_SYMBOL_EXTERN(_NLG_Return);
#endif


#if defined(_M_IX86) && WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
_LCRT_DEFINE_IAT_SYMBOL_EXTERN(_local_unwind4);

#ifdef _DLL
extern "C" void __stdcall _seh_longjmp_unwind4(void*);
__pragma(warning(suppress:4483))                                                                 
extern "C" __declspec(selectany) void const* const __identifier("_imp___seh_longjmp_unwind4@4") = reinterpret_cast<void const*>(&_seh_longjmp_unwind4);
#endif

#endif