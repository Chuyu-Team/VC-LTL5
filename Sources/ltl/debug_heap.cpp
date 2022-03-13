
#ifdef _DEBUG

/*
这些调试函数仅仅用于编译通过处理，实际没有任何的功能。
如果需要真正的使用这些调试功能，那么请使用微软原版的 调试版！！！
*/

#include <crtdbg.h>
#include <stdlib.h>
#include <Windows.h>
#include <winnt.h>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Global Mutable State (Synchronized by the AppCRT Heap Lock)
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


extern "C" int* __cdecl __p__crtDbgFlag(void)
{
    static int crtDbgFlag = 0;
    return &crtDbgFlag;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__crtDbgFlag);

extern "C" long* __cdecl __p__crtBreakAlloc(void)
{
    static long crtBreakAlloc = 0;
    return &crtBreakAlloc;
}

_LCRT_DEFINE_IAT_SYMBOL(__p__crtBreakAlloc);



//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Public Debug Heap Allocation APIs
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


extern "C" __declspec(noinline) void* __cdecl _malloc_dbg(
    size_t      const size,
    int         const block_use,
    char const* const file_name,
    int         const line_number
    )
{
    return malloc(size);
}

_LCRT_DEFINE_IAT_SYMBOL(_malloc_dbg);

extern "C" __declspec(noinline) void* __cdecl _calloc_dbg(
    size_t      const count,
    size_t      const element_size,
    int         const block_use,
    char const* const file_name,
    int         const line_number
    )
{
    return calloc(count, element_size);
}

_LCRT_DEFINE_IAT_SYMBOL(_calloc_dbg);


extern "C" __declspec(noinline) void* __cdecl _realloc_dbg(
    void*       const block,
    size_t      const requested_size,
    int         const block_use,
    char const* const file_name,
    int         const line_number
    )
{
    return realloc(block, requested_size);
}

_LCRT_DEFINE_IAT_SYMBOL(_realloc_dbg);

extern "C" __declspec(noinline) void* __cdecl _recalloc_dbg(
    void*       const block,
    size_t      const count,
    size_t      const element_size,
    int         const block_use,
    char const* const file_name,
    int         const line_number
    )
{
    return _recalloc(block, count, element_size);
}

_LCRT_DEFINE_IAT_SYMBOL(_recalloc_dbg);

extern "C" __declspec(noinline) void* __cdecl _expand_dbg(
    void*       const block,
    size_t      const requested_size,
    int         const block_use,
    char const* const file_name,
    int         const line_number
    )
{
    return _expand(block, requested_size);
}

_LCRT_DEFINE_IAT_SYMBOL(_expand_dbg);

extern "C" __declspec(noinline) void __cdecl _free_dbg(void* const block, int const block_use)
{
    //UNREFERENCED_PARAMETER(block_use);

    return free(block);
}

_LCRT_DEFINE_IAT_SYMBOL(_free_dbg);

extern "C" __declspec(noinline) size_t __cdecl _msize_dbg(void* const block, int const block_use)
{
    return _msize(block);
}

_LCRT_DEFINE_IAT_SYMBOL(_msize_dbg);



//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Public Debug Heap Control and Status APIs
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


extern "C" long __cdecl _CrtSetBreakAlloc(
    _In_ long _NewValue
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetBreakAlloc);

extern "C" void __cdecl _CrtSetDbgBlockType(
    void* const block,
    int   const block_use
    )
{
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetDbgBlockType);

extern "C" _CRT_ALLOC_HOOK __cdecl _CrtGetAllocHook(void)
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtGetAllocHook);

extern "C" _CRT_ALLOC_HOOK __cdecl _CrtSetAllocHook(
    _In_opt_ _CRT_ALLOC_HOOK _PfnNewHook
    )
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetAllocHook);

extern "C" int __cdecl _CrtCheckMemory(void)
{
    return 1;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtCheckMemory);

extern "C" int __cdecl _CrtSetDbgFlag(
    _In_ int _NewFlag
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetDbgFlag);

extern "C" void __cdecl _CrtDoForAllClientObjects(
    _CrtDoForAllClientObjectsCallback const callback,
    void*                             const context
    )
{

}

_LCRT_DEFINE_IAT_SYMBOL(_CrtDoForAllClientObjects);

extern "C" int __cdecl _CrtIsValidPointer(
    void const*  const p,
    unsigned int const size_in_bytes,
    int          const read_write
    )
{
    UNREFERENCED_PARAMETER(size_in_bytes);
    UNREFERENCED_PARAMETER(read_write);

    return p != nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtIsValidPointer);

extern "C" int __cdecl _CrtIsValidHeapPointer(void const* const block)
{
    if (!block)
        return FALSE;

    return TRUE;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtIsValidHeapPointer);


extern "C" int __cdecl _CrtIsMemoryBlock(
    void const* const block,
    unsigned    const size,
    long*       const request_number,
    char**      const file_name,
    int*        const line_number
    )
{
    if (request_number)
        *request_number = 0;

    if (file_name)
        *file_name = nullptr;

    if (line_number)
        *line_number = 0;

    if (!block)
        return FALSE;

    return TRUE;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtIsMemoryBlock);

extern "C" int _CrtReportBlockType(void const* const block)
{
    return -1;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtReportBlockType);

extern "C" _CRT_DUMP_CLIENT __cdecl _CrtGetDumpClient(void)
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtGetDumpClient);

extern "C" _CRT_DUMP_CLIENT __cdecl _CrtSetDumpClient(_CRT_DUMP_CLIENT const new_client)
{
    return nullptr;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtSetDumpClient);

extern "C" void __cdecl _CrtMemCheckpoint(_CrtMemState* const state)
{

}

_LCRT_DEFINE_IAT_SYMBOL(_CrtMemCheckpoint);

extern "C" int __cdecl _CrtMemDifference(
    _CrtMemState*       const state,
    _CrtMemState const* const old_state,
    _CrtMemState const* const new_state
    )
{
    return 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtMemDifference);


extern "C" void __cdecl _CrtMemDumpAllObjectsSince(_CrtMemState const* const state)
{
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtMemDumpAllObjectsSince);


extern "C" int __cdecl _CrtDumpMemoryLeaks(void)
{
    return FALSE;
}

_LCRT_DEFINE_IAT_SYMBOL(_CrtDumpMemoryLeaks);

extern "C" void __cdecl _CrtMemDumpStatistics(_CrtMemState const* const state)
{

}

_LCRT_DEFINE_IAT_SYMBOL(_CrtMemDumpStatistics);




//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Aligned Allocation
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// These functions are equivalent to the normal aligned allocation functions in
// alignment.cpp, but these functions (suffixed with _dbg instead of _base) utilize
// the debug heap and accept the file name and line number as arguments.  Consult
// alignment.cpp for more information on the behavior of these functions.


extern "C" void* __cdecl _aligned_malloc_dbg(
    size_t      const size,
    size_t      const alignment,
    char const* const file_name,
    int         const line_number
    )
{
    return _aligned_malloc(size, alignment);
}

_LCRT_DEFINE_IAT_SYMBOL(_aligned_malloc_dbg);

extern "C" void* __cdecl _aligned_offset_malloc_dbg(
    size_t      const size,
    size_t            alignment,
    size_t      const offset,
    char const* const file_name,
    int         const line_number
    )
{
    return _aligned_offset_malloc(size, alignment, offset);
}

_LCRT_DEFINE_IAT_SYMBOL(_aligned_offset_malloc_dbg);

extern "C" void* __cdecl _aligned_realloc_dbg(
    void*       const block,
    size_t      const size,
    size_t      const alignment,
    char const* const file_name,
    int         const line_number
    )
{
    return _aligned_realloc(block, size, alignment);
}

_LCRT_DEFINE_IAT_SYMBOL(_aligned_realloc_dbg);

extern "C" void* __cdecl _aligned_recalloc_dbg(
    void*       const block,
    size_t      const count,
    size_t      const size,
    size_t      const alignment,
    char const* const file_name,
    int         const line_number
    )
{
    return _aligned_recalloc(block, count, size, alignment);
}

_LCRT_DEFINE_IAT_SYMBOL(_aligned_recalloc_dbg);

extern "C" void* __cdecl _aligned_offset_realloc_dbg(
    void * block,
    size_t size,
    size_t alignment,
    size_t offset,
    const char * file_name,
    int line_number
    )
{
    return _aligned_offset_realloc(block, size, alignment, offset);
}

_LCRT_DEFINE_IAT_SYMBOL(_aligned_offset_realloc_dbg);

extern "C" size_t __cdecl _aligned_msize_dbg(
    void*  const block,
    size_t       alignment,
    size_t const offset
    )
{
    return _aligned_msize(block, alignment, offset);
}

_LCRT_DEFINE_IAT_SYMBOL(_aligned_msize_dbg);

extern "C" void* __cdecl _aligned_offset_recalloc_dbg(
    void*       const block,
    size_t      const count,
    size_t      const element_size,
    size_t      const alignment,
    size_t      const offset,
    char const* const file_name,
    int         const line_number
    )
{
    return _aligned_offset_recalloc(block, count, element_size, alignment, offset);
}

_LCRT_DEFINE_IAT_SYMBOL(_aligned_offset_recalloc_dbg);

extern "C" void __cdecl _aligned_free_dbg(void* const block)
{
    _aligned_free(block);
}

_LCRT_DEFINE_IAT_SYMBOL(_aligned_free_dbg);

#endif