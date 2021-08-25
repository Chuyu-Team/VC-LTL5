#include <float.h>
#include <intrin.h>

#ifdef _M_IX86
extern "C" extern int __isa_available;

extern "C" unsigned int __cdecl __get_fpsr_sse2();

static __forceinline unsigned int __cdecl __statusfp_sse2()
{
	auto fpsr_sse2 = __get_fpsr_sse2();

	unsigned int result = 0;

	if (fpsr_sse2 & 0x3F)
	{
		if (fpsr_sse2 & 0x01)
			result |= 0x10;

		if (fpsr_sse2 & 0x02)
			result |= 0x80000;

		if (fpsr_sse2 & 0x04)
			result |= 0x8;

		if (fpsr_sse2 & 0x08)
			result |= 0x4;

		if (fpsr_sse2 & 0x10)
			result |= 0x2;

		if (fpsr_sse2 & 0x20)
			result |= 0x1;
	}

	return result;
}

extern "C" void __cdecl _statusfp2(
	_Out_opt_ unsigned int* _X86Status,
	_Out_opt_ unsigned int* _SSE2Status
)
{
	if (_X86Status)
	{
		unsigned short FpuStatus;

		__asm { fstsw FpuStatus };

		unsigned int dwX86Status = 0;

		if (FpuStatus & 0x3F)
		{
			if (FpuStatus & 0x01)
				dwX86Status |= 0x10;

			if (FpuStatus & 0x02)
				dwX86Status |= 0x80000;

			if (FpuStatus & 0x04)
				dwX86Status |= 0x08;

			if (FpuStatus & 0x08)
				dwX86Status |= 0x04;

			if (FpuStatus & 0x10)
				dwX86Status |= 0x02;

			if (FpuStatus & 0x20)
				dwX86Status |= 0x01;
		}

		*_X86Status = dwX86Status;
	}

	if (_SSE2Status)
		*_SSE2Status = __statusfp_sse2();
}

_LCRT_DEFINE_IAT_SYMBOL(_statusfp2);

static unsigned short __cdecl _hw_cw(unsigned int  _NewValue)
{
	unsigned int result = 0;

	if (_NewValue & 0x10)
		result = 0x1;

	if (_NewValue & 0x8)
		result |= 0x4;

	if (_NewValue & 0x4)
		result |= 0x8;

	if (_NewValue & 0x2)
		result |= 0x10;

	if (_NewValue & 0x1)
		result |= 0x20;

	if (_NewValue & 0x80000)
		result |= 0x2;

	if (_NewValue & 0x100)
		result |= 0x400;

	if (_NewValue & 0x200)
		result |= 0x800;

	if ((_NewValue & 0x30000) == 0x10000)
		result |= 0x200;
	else
		result |= 0x300;

	if (_NewValue & 0x40000)
		result |= 0x1000;

	return result;
}

static unsigned int __cdecl __hw_cw_sse2(unsigned int Sse2ControlWord)
{
	unsigned int result = 0;

	if (Sse2ControlWord & 0x10)
		result = 0x80;
	if (Sse2ControlWord & 0x08)
		result |= 0x200u;
	if (Sse2ControlWord & 0x04)
		result |= 0x400u;
	if (Sse2ControlWord & 0x02)
		result |= 0x800u;
	if (Sse2ControlWord & 0x01)
		result |= 0x1000u;
	if (Sse2ControlWord & 0x80000)
		result |= 0x100u;

	if (Sse2ControlWord & 0x100)
		result |= 0x2000u;

	if (Sse2ControlWord & 0x200)
		result |= 0x4000u;


	switch (Sse2ControlWord & 0x3000000)
	{
	case 0x1000000:
		result |= 0x8040u;
		break;
	case 0x2000000:
		result |= 0x40u;
		break;
	case 0x3000000:
		result |= 0x8000u;
		break;
	default:
		break;
	}

	return result;
}

static unsigned int __fastcall FpuStatusToX86ControlWord(unsigned short FpuStatus)
{
	unsigned int CurrentX86ControlWord = 0;

	if (FpuStatus & 0x1)
		CurrentX86ControlWord |= 0x10;

	if (FpuStatus & 0x2)
		CurrentX86ControlWord |= 0x80000;

	if (FpuStatus & 0x4)
		CurrentX86ControlWord |= 0x8;

	if (FpuStatus & 0x8)
		CurrentX86ControlWord |= 0x4;

	if (FpuStatus & 0x10)
		CurrentX86ControlWord |= 0x2;

	if (FpuStatus & 0x20)
		CurrentX86ControlWord |= 0x1;


	if (FpuStatus & 0x400)
		CurrentX86ControlWord |= 0x100;

	if (FpuStatus & 0x800)
		CurrentX86ControlWord |= 0x200;

	if ((FpuStatus & 0x300) == 0x200)
	{
		CurrentX86ControlWord |= 0x10000;
	}
	else
	{
		CurrentX86ControlWord |= 0x20000;
	}

	if (FpuStatus & 0x1000)
	{
		CurrentX86ControlWord |= 0x40000;
	}

	return CurrentX86ControlWord;
}

static unsigned int __fastcall Sse2StatusToSse2ControlWord(unsigned int Sse2Status)
{
	unsigned int Sse2ControlWord = 0;

	if (Sse2Status & 0x8000)
		Sse2ControlWord = 0x10;

	if (Sse2Status & 0x200)
		Sse2ControlWord |= 0x8;

	if (Sse2Status & 0x400)
		Sse2ControlWord |= 0x4;

	if (Sse2Status & 0x800)
		Sse2ControlWord |= 0x2;

	if (Sse2Status & 0x1000)
		Sse2ControlWord |= 0x1;

	if (Sse2Status & 0x100)
		Sse2ControlWord |= 0x80000;

	if (Sse2Status & 0x2000)
		Sse2ControlWord |= 0x100;

	if (Sse2Status & 0x4000)
		Sse2ControlWord |= 0x200;


	switch (Sse2Status & 0x8040)
	{
	case 0x40:
		Sse2ControlWord |= 0x2000000;
		break;
	case 0x8000:
		Sse2ControlWord |= 0x3000000;
		break;
	case 0x8040:
		Sse2ControlWord |= 0x1000000;
		break;
	default:
		break;
	}

	return Sse2ControlWord;
}

extern "C" void __cdecl __set_fpsr_sse2(unsigned int);

extern "C" int __cdecl __control87_2(
	_In_      unsigned int  _NewValue,
	_In_      unsigned int  _Mask,
	_Out_opt_ unsigned int* _X86ControlWord,
	_Out_opt_ unsigned int* _Sse2ControlWord
)
{
	_NewValue &= _Mask;

	if (_X86ControlWord)
	{
		unsigned short FpuStatus;

		__asm { fstcw FpuStatus };

		unsigned int CurrentX86ControlWord = FpuStatusToX86ControlWord(FpuStatus);


		auto NewX86ControlWord = (_NewValue) | (CurrentX86ControlWord & ~_Mask);


		if (NewX86ControlWord != CurrentX86ControlWord)
		{
			FpuStatus = _hw_cw(NewX86ControlWord);

			__asm
			{
				fldcw FpuStatus
				fstcw FpuStatus
			}

			NewX86ControlWord = FpuStatusToX86ControlWord(FpuStatus);
		}

		*_X86ControlWord = NewX86ControlWord;
	}


	if (_Sse2ControlWord)
	{
		if (__isa_available < 1)
		{
			*_Sse2ControlWord = 0;
		}
		else
		{
			auto CurrentSse2ControlWord = Sse2StatusToSse2ControlWord(_mm_getcsr());

			auto NewSse2ControlWord = (_NewValue & 0x308031F) | (CurrentSse2ControlWord & ~(_Mask & 0x308031F));

			if (NewSse2ControlWord != CurrentSse2ControlWord)
			{
				__set_fpsr_sse2(__hw_cw_sse2(NewSse2ControlWord));

				CurrentSse2ControlWord = Sse2StatusToSse2ControlWord(_mm_getcsr());
			}

			*_Sse2ControlWord = CurrentSse2ControlWord;
		}
	}

	return 1;
}

_LCRT_DEFINE_IAT_SYMBOL(__control87_2);

#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" void __cdecl _set_controlfp(
	_In_ unsigned int _NewValue,
	_In_ unsigned int _Mask
	)
{
	if (_NewValue == 0x9001F
		|| _Mask==0xFFFFFFFF)
	{
		return;
	}

#ifdef _M_IX86
	unsigned short currentState;

	__asm { fstcw currentState }

	if ((currentState&0x1F3D)== 0x23D 
		&& (__isa_available < 1 || (_mm_getcsr() & 0xFEC0) == 0x1E80))
	{
		return;
	}
#elif defined (_M_AMD64)
	if ((_mm_getcsr() & 0xFEC0) == 0x1E80)
		return;
#else
#error "不支持此体系"
#endif

	_controlfp(_NewValue, _Mask &0xFFF7FFFF);
}

_LCRT_DEFINE_IAT_SYMBOL(_set_controlfp);
#endif