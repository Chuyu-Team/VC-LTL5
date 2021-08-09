//
// std_type_info.cpp
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
// Definitions of the std::type_info implementation functions, used for
// Run-Time Type Information (RTTI).
//
#include <vcruntime_internal.h>
#include <vcruntime_string.h>
//#include <vcruntime_typeinfo.h>
//#include <undname.h>
#include <framework.h>


struct __std_type_info_data
{
    const char* _UndecoratedName;
    const char   _DecoratedName[1];
    __std_type_info_data() = delete;
    __std_type_info_data(const __std_type_info_data&) = delete;
    __std_type_info_data(__std_type_info_data&&) = delete;

    __std_type_info_data& operator=(const __std_type_info_data&) = delete;
    __std_type_info_data& operator=(__std_type_info_data&&) = delete;
};

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" int __cdecl __std_type_info_compare(
    __std_type_info_data const* const lhs,
    __std_type_info_data const* const rhs
    )
{
    if (lhs == rhs)
    {
        return 0;
    }

    return strcmp(lhs->_DecoratedName + 1, rhs->_DecoratedName + 1);
}

_LCRT_DEFINE_IAT_SYMBOL(__std_type_info_compare);

#endif

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" size_t __cdecl __std_type_info_hash(
    __std_type_info_data const* const data
    )
{
    // FNV-1a hash function for the undecorated name

    #ifdef _WIN64
    static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
    size_t const fnv_offset_basis = 14695981039346656037ULL;
    size_t const fnv_prime        = 1099511628211ULL;
    #else
    static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
    size_t const fnv_offset_basis = 2166136261U;
    size_t const fnv_prime        = 16777619U;
    #endif

    size_t value = fnv_offset_basis;
    for (char const* it = data->_DecoratedName + 1; *it != '\0'; ++it)
    {
        value ^= static_cast<size_t>(static_cast<unsigned char>(*it));
        value *= fnv_prime;
    }

    #ifdef _WIN64
    static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
    value ^= value >> 32;
    #else
    static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
    #endif

    return value;
}

_LCRT_DEFINE_IAT_SYMBOL(__std_type_info_hash);

#endif

//重新声明出msvcrt 的名称
class __declspec(dllimport) type_info {
public:
    const char* name() const noexcept;
    virtual ~type_info() noexcept;
//private:
    void* _m_data;
    char _m_d_name[1];
};

#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" char const* __cdecl __std_type_info_name(
    __std_type_info_data* const data,
    __type_info_node*     const root_node
    )
{
    //调用 msvcrt中的 type_info::name() 即可

    auto pMsvcrtTypeInfo = (::type_info*)((char*)data - (unsigned)&((type_info*)NULL)->_m_data);

    return pMsvcrtTypeInfo->name();
}

_LCRT_DEFINE_IAT_SYMBOL(__std_type_info_name);

#endif

// This function is called during module unload to clean up all of the undecorated
// name strings that were allocated by calls to name().
#if WindowsTargetPlatformMinVersion < __MakeVersion(10, 0, 10240)
extern "C" void __cdecl __std_type_info_destroy_list(
    __type_info_node* const root_node
    )
{
    PSLIST_ENTRY current_node = InterlockedFlushSList(&root_node->_Header);
    while (current_node)
    {
        PSLIST_ENTRY const next_node = current_node->Next;
        _free_crt(current_node);
        current_node = next_node;
    }
}

_LCRT_DEFINE_IAT_SYMBOL(__std_type_info_destroy_list);

#endif
