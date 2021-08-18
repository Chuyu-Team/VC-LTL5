//
// Copyright (C) Microsoft Corporation
// All rights reserved.
//

#include "pch.h"
#pragma hdrstop

namespace Platform { namespace Details {

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
	VCCORLIB_API void ControlBlock::ReleaseTarget()
	{
		// called by __abi_release after calling the destructor of object
		if (!__bSingleAllocation && __strongRefCount < 0 && __target != nullptr)
		{
			if (__bAlignedAllocation)
			{
				if (__bExceptionAllocation)
				{
					::Platform::Details::Heap::AlignedFreeException(__target);
				}
				else
				{
					::Platform::Details::Heap::AlignedFree(__target);
				}
			}
			else
			{
				if (__bExceptionAllocation)
				{
					::Platform::Details::Heap::FreeException(__target);
				}
				else
				{
					::Platform::Details::Heap::Free(__target);
				}
			}
			__target = nullptr;
		}
	}

	_LCRT_DEFINE_IAT_RAW_SYMBOL(
		?ReleaseTarget@ControlBlock@Details@Platform@@AAEXXZ,
		?ReleaseTarget@ControlBlock@Details@Platform@@AEAAXXZ,
		?ReleaseTarget@ControlBlock@Details@Platform@@AAAXXZ,
		?ReleaseTarget@ControlBlock@Details@Platform@@AEAAXXZ
		);

	VCCORLIB_API long __stdcall ControlBlock::__abi_Resolve(::Platform::Guid& __riid, __abi_IInspectable ** __objectReference)
	{
		for(;;)
		{
			long __ref = __strongRefCount;
			if (__ref <= 0)
			{
				return 0;
			}

			// InterlockedCompareExchange calls _InterlockedCompareExchange intrinsic thus we call directly _InterlockedCompareExchange to save the call
			if (::_InterlockedCompareExchange(&__strongRefCount, __ref + 1, __ref) == __ref)
			{
				break;
			}
		}

		long __hr = __target->__abi_QueryInterface(__riid, reinterpret_cast<void**>(__objectReference));

		// Undo the "addref" that happened above during the _InterlockedCompareExchange call
		__target->__abi_Release();

		return __hr;
	}
	
	_LCRT_DEFINE_IAT_RAW_SYMBOL(
		?__abi_Resolve@ControlBlock@Details@Platform@@UAGJAAVGuid@3@PAPAU__abi_IInspectable@@@Z,
		?__abi_Resolve@ControlBlock@Details@Platform@@UEAAJAEAVGuid@3@PEAPEAU__abi_IInspectable@@@Z,
		?__abi_Resolve@ControlBlock@Details@Platform@@UAAJAAVGuid@3@PAPAU__abi_IInspectable@@@Z,
		?__abi_Resolve@ControlBlock@Details@Platform@@UEAAJAEAVGuid@3@PEAPEAU__abi_IInspectable@@@Z
		);

	// Object has a reference to control block
	VCCORLIB_API void ControlBlock::InitControlBlock(void* __object, bool __bSingleAllocationParam, bool __bAlignedAllocationParam, bool __bExceptionAllocationParam)
	{
		__vtable_initialize(ControlBlock);
		__weakRefCount = 1;
		__strongRefCount = 1;
		__target = reinterpret_cast<__abi_IUnknown*>(__object);
		__bSingleAllocation = __bSingleAllocationParam;
		__bAlignedAllocation = __bAlignedAllocationParam;
		__bExceptionAllocation = __bExceptionAllocationParam;
	}

	_LCRT_DEFINE_IAT_RAW_SYMBOL(
		?InitControlBlock@ControlBlock@Details@Platform@@AAEXPAX_N11@Z,
		?InitControlBlock@ControlBlock@Details@Platform@@AEAAXPEAX_N11@Z,
		?InitControlBlock@ControlBlock@Details@Platform@@AAAXPAX_N11@Z,
		?InitControlBlock@ControlBlock@Details@Platform@@AEAAXPEAX_N11@Z
		);
#endif

#if 0
#pragma warning(push)
#pragma warning(disable: 4400)
	VCCORLIB_API __abi_IUnknown* __stdcall GetWeakReference(const volatile Platform::Object^ const other)
#pragma warning(pop)
	{
		if (other == nullptr)
		{
			throw ref new ::Platform::InvalidArgumentException();
		}

		Microsoft::WRL::WeakRef ref;
		HRESULT hr = Microsoft::WRL::AsWeak(reinterpret_cast<IUnknown*>(const_cast<Platform::Object^>(other)), &ref);
		__abi_ThrowIfFailed(hr);

		return reinterpret_cast<__abi_IUnknown*>(ref.Detach());
	}

	VCCORLIB_API
		__declspec(no_refcount)
		::Platform::Object^ __stdcall ResolveWeakReference(const ::_GUID& guid, __abi_IUnknown** weakRef)
	{
		::Platform::Object ^strongRef = nullptr;

		if (*weakRef != nullptr)
		{
			Microsoft::WRL::WeakRef *ref = reinterpret_cast<Microsoft::WRL::WeakRef*>(weakRef);
			HRESULT hr = ref->CopyTo(guid, reinterpret_cast<IInspectable**>(&strongRef));
			__abi_ThrowIfFailed(hr);
		}

		return strongRef;
	}
#endif

} } // namesapce Platform::Details
