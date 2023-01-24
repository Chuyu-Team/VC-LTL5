// Copyright (c) Microsoft Corporation. All rights reserved.
//
// C11 atomic support routines: extern inline defitions
#include <vcruntime_c11_atomic_support.h>

extern inline void _Check_memory_order(const unsigned int _Order);
extern inline void _Atomic_thread_fence(const unsigned int _Order);

extern inline void _Atomic_store8(volatile char* _Ptr, char _Desired, int _Order);
extern inline void _Atomic_store16(volatile short* _Ptr, short _Desired, int _Order);
extern inline void _Atomic_store32(volatile int* _Ptr, int _Desired, int _Order);
extern inline void _Atomic_store64(volatile long long* _Ptr, long long _Desired, int _Order);

extern inline char _Atomic_load8(const volatile char* _Ptr, int _Order);
extern inline short _Atomic_load16(const volatile short* _Ptr, int _Order);
extern inline int _Atomic_load32(const volatile int* _Ptr, int _Order);
extern inline long long _Atomic_load64(const volatile long long* _Ptr, int _Order);

extern inline _Bool _Atomic_compare_exchange_strong8(volatile char* _Ptr, char* _Expected, char _Desired, int _Order);
extern inline _Bool _Atomic_compare_exchange_strong16(
    volatile short* _Ptr, short* _Expected, short _Desired, int _Order);
extern inline _Bool _Atomic_compare_exchange_strong32(volatile int* _Ptr, int* _Expected, int _Desired, int _Order);
extern inline _Bool _Atomic_compare_exchange_strong64(
    volatile long long* _Ptr, long long* _Expected, long long _Desired, int _Order);
