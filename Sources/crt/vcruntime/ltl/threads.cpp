#include <threads.h>

#include <internal_shared.h>

#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <crtdbg.h>
#include <errno.h>

#include <Windows.h>

#include "framework.h"

// 模仿 vctools\crt\vcruntime\src\internal\threads.cpp 实现

#pragma warning(disable: 28251)

namespace VC_LTL
{
    constexpr auto kDtorTableBufferLength = 0x400;

    class srwlock_shared_guard
    {
    private:
        SRWLOCK* pLock;

    public:
        srwlock_shared_guard(SRWLOCK& _Lock)
            : pLock(&_Lock)
        {
            AcquireSRWLockShared(pLock);
        }

        srwlock_shared_guard(const srwlock_shared_guard&) = delete;

        ~srwlock_shared_guard()
        {
            Reset();
        }

        void Reset()
        {
            if (pLock)
            {
                ReleaseSRWLockShared(pLock);
                pLock = nullptr;
            }
        }

        void Attach(SRWLOCK* _pLock)
        {
            if (pLock == _pLock)
                return;

            if (pLock)
                ReleaseSRWLockShared(pLock);

            pLock = _pLock;
            AcquireSRWLockShared(pLock);
        }

        void operator=(std::nullptr_t)
        {
            Reset();
        }

        void operator=(SRWLOCK& _Lock)
        {
            Attach(&_Lock);
        }
    };

    class srwlock_guard
    {
    private:
        SRWLOCK* pLock;

    public:
        srwlock_guard(SRWLOCK& _Lock)
            : pLock(&_Lock)
        {
            AcquireSRWLockExclusive(pLock);
        }

        srwlock_guard(const srwlock_guard&) = delete;

        ~srwlock_guard()
        {
            ReleaseSRWLockExclusive(pLock);
        }
    };

    struct tss_ptd
    {
        tss_ptd* next;
        tss_ptd* prev;
        void* data[kDtorTableBufferLength];
        bool tss_dtor_used;
    };

    class tss_global_data_t
    {
    private:
        // 0
        SRWLOCK lock = SRWLOCK_INIT;
        // 0x4
        DWORD tss_ptd_idx = TLS_OUT_OF_INDEXES;
        // 0x8
        tss_dtor_t* dtor_table = nullptr;
        // 0xC
        tss_ptd* ptd_list = nullptr;
        // 0x10
        uint32_t last_idx = 0;

    public:
        constexpr tss_global_data_t() = default;

        tss_global_data_t(const tss_global_data_t&) = delete;

        int create(tss_t* _Key, tss_dtor_t _Dtor)
        {
            if (!_Dtor)
                _Dtor = noop_dtor;

            int _iResult = thrd_success;
            AcquireSRWLockExclusive(&lock);
            do
            {
                if (!dtor_table)
                {
                    dtor_table = static_cast<tss_dtor_t*>(calloc(kDtorTableBufferLength, sizeof(tss_dtor_t)));
                    if (!dtor_table)
                    {
                        _iResult = thrd_error;
                        break;
                    }
                }

                if (tss_ptd_idx == TLS_OUT_OF_INDEXES)
                {
                    tss_ptd_idx = TlsAlloc();
                    if (tss_ptd_idx == TLS_OUT_OF_INDEXES)
                    {
                        free(dtor_table);
                        dtor_table = nullptr;
                        _iResult = thrd_error;
                        break;
                    }
                }

                for (; dtor_table[last_idx];)
                {
                    last_idx = (last_idx + 1) % kDtorTableBufferLength;
                    if (last_idx == 0)
                    {
                        // dtor_table已经满了
                        _iResult = thrd_error;
                        break;
                    }
                }

                dtor_table[last_idx] = _Dtor;
                _Key->_Idx = last_idx;
            } while (false);
            ReleaseSRWLockExclusive(&lock);
            return _iResult;
        }

        void tss_delete(tss_t _Key)
        {
            AcquireSRWLockExclusive(&lock);

            _ASSERT_EXPR(dtor_table, "tss dtor table not allocated.");
            _ASSERT_EXPR(dtor_table[_Key._Idx] != nullptr, "tss dtor table indicates delete called on nonexistant tss index.");

            dtor_table[_Key._Idx] = nullptr;

            for (auto _pItem = ptd_list; _pItem; _pItem = _pItem->next)
            {
                _pItem->data[_Key._Idx] = nullptr;
            }

            ReleaseSRWLockExclusive(&lock);
        }

        void* get(tss_t _Key)
        {
            void* _pValue = nullptr;

            AcquireSRWLockShared(&lock);
            _ASSERT_EXPR(dtor_table, "tss dtor table not allocated.");
            _ASSERT_EXPR(dtor_table[_Key._Idx] != nullptr, "tss dtor table indicates delete called on nonexistant tss index.");

            if (tss_ptd_idx != TLS_OUT_OF_INDEXES)
            {
                auto _pTssData = (tss_ptd*)TlsGetValue(tss_ptd_idx);
                if (_pTssData)
                {
                    _pValue = _pTssData->data[_Key._Idx];
                }
            }

            ReleaseSRWLockShared(&lock);
            return _pValue;
        }

        int set(tss_t _Key, void* _Val)
        {
            {
                srwlock_shared_guard _AutoSharedLock(lock);
                _ASSERT_EXPR(tss_ptd_idx != TLS_OUT_OF_INDEXES, "tss_set called but the tss ptd index was never allcoated.");

                auto _pTssData = (tss_ptd*)TlsGetValue(tss_ptd_idx);
                _ASSERT_EXPR(GetLastError() == ERROR_SUCCESS, "tss_set could not get the value of the tss ptd.");

                if (_pTssData)
                {
                    if (dtor_table[_Key._Idx] != noop_dtor)
                    {
                        _pTssData->tss_dtor_used = true;
                    }

                    _pTssData->data[_Key._Idx] = _Val;
                    return thrd_success;
                }

                if (!_Val)
                {
                    return thrd_success;
                }
            }

            auto _pTssData = (tss_ptd*)calloc(1, sizeof(tss_ptd));
            if (!_pTssData)
                return thrd_error;

            TlsSetValue(tss_ptd_idx, _pTssData);
            {
                srwlock_guard _AutoExclusiveLock(lock);

                _pTssData->next = ptd_list;
                if (ptd_list)
                {
                    ptd_list->prev = _pTssData;
                }
                ptd_list = _pTssData;
            }

            srwlock_shared_guard _AutoSharedLock(lock);
            if (dtor_table[_Key._Idx] != noop_dtor)
            {
                _pTssData->tss_dtor_used = true;
            }

            _pTssData->data[_Key._Idx] = _Val;
            return thrd_success;
        }

        void cleanup(void)
        {
            tss_ptd* ptd = nullptr;
            {
                srwlock_shared_guard _AutoLock(lock);
                if (!dtor_table)
                {
                    return;
                }

                _ASSERTE((tss_ptd_idx != TLS_OUT_OF_INDEXES, "cleanup called but the tss ptd index was never allcoated."));

                ptd = (tss_ptd*)TlsGetValue(tss_ptd_idx);
                _ASSERTE((GetLastError() == ERROR_SUCCESS, "cleanup could not get the value of the tss ptd."));
                if (!ptd)
                {
                    return;
                }

                if (ptd->tss_dtor_used)
                {
                    for (int i = 0; i != kDtorTableBufferLength; ++i)
                    {
                        auto _pFun = dtor_table[i];
                        if (_pFun && _pFun != &tss_global_data_t::noop_dtor)
                        {
                            auto _pUserData = ptd->data[i];
                            ptd->data[i] = nullptr;
                            _AutoLock = nullptr;
                            _pFun(_pUserData);
                            _AutoLock = lock;
                        }
                    }
                }
            }

            srwlock_guard AutoLock(lock);
            if (ptd->prev)
            {
                ptd->prev = ptd->next;
            }
            else
            {
                _ASSERTE((ptd_list == ptd, "ptd list is malformed."));
                ptd_list = ptd->next;
            }
            if (ptd->next)
            {
                ptd->next->prev = ptd->next;
            }
            free(ptd);
        }

        void cleanup_global(void)
        {
            if (dtor_table)
            {
                free(dtor_table);
                dtor_table = reinterpret_cast<tss_dtor_t*>(-1);
            }

            if (tss_ptd_idx != TLS_OUT_OF_INDEXES)
            {
                const auto _bSuccess = TlsFree(tss_ptd_idx);
                tss_ptd_idx = TLS_OUT_OF_INDEXES - 1;
                _ASSERTE((_bSuccess, "failed to free ptd index."));
            }
        }

    private:
        static void __cdecl noop_dtor(void*)
        {
            // 什么也不用做
        }
    };
}

using namespace VC_LTL;

extern "C" void __cdecl mtx_destroy(mtx_t * _Mtx)
{
    // 什么也不做
}

_LCRT_DEFINE_IAT_SYMBOL(mtx_destroy);

extern "C" int __cdecl mtx_init(_Out_ mtx_t* _Mtx, int _Type)
{
    _Mtx->_Type = _Type;
    _Mtx->_Ptr = nullptr;
    _Mtx->_Cv = nullptr;
    _Mtx->_Owner = 0;
    _Mtx->_Cnt = 0;
    return thrd_success;
}

_LCRT_DEFINE_IAT_SYMBOL(mtx_init);

static int __cdecl mtx_dolock(mtx_t * _Mtx)
{
    const auto _uCurrentThreadId = GetCurrentThreadId();
    if (_uCurrentThreadId == _Mtx->_Owner)
    {
        if ((_Mtx->_Type & mtx_recursive) == 0)
        {
            abort();
        }
        _Mtx->_Cnt++;
        return thrd_success;
    }

    for(; _Mtx->_Owner;)
    {
        if (!SleepConditionVariableSRW(
            reinterpret_cast<PCONDITION_VARIABLE>(&_Mtx->_Cv),
            reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr),
            INFINITE, 0))
        {
            return thrd_error;
        }
    }

    _Mtx->_Owner = _uCurrentThreadId;
    _Mtx->_Cnt = 1;
    return thrd_success;
}

extern "C" int __cdecl mtx_lock(mtx_t* _Mtx)
{
    AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(& _Mtx->_Ptr));
    auto _iResult = mtx_dolock(_Mtx);
    ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));
    return _iResult;
}

_LCRT_DEFINE_IAT_SYMBOL(mtx_lock);

template<typename timespec>
static int64_t __fastcall duration_timespec_to_ms(const timespec& _Duration)
{
    return _Duration.tv_sec * 1000ll + _Duration.tv_nsec / 1000000;
}

static int64_t __fastcall abstime_timespec_to_duration_ms(const struct _timespec32* __restrict _Ts)
{
    _timespec32 _Now;
    if (_timespec32_get(&_Now, TIME_UTC) == 0)
    {
        return INT64_MAX;
    }

    _Now.tv_sec = _Ts->tv_sec - _Now.tv_sec;
    _Now.tv_nsec = _Ts->tv_nsec - _Now.tv_nsec;
    return duration_timespec_to_ms(_Now);
}

static int64_t __fastcall abstime_timespec_to_duration_ms(const struct _timespec64* __restrict _Ts)
{
    _timespec64 _Now;
    if (_timespec64_get(&_Now, TIME_UTC) == 0)
    {
        return INT64_MAX;
    }

    _Now.tv_sec = _Ts->tv_sec - _Now.tv_sec;
    _Now.tv_nsec = _Ts->tv_nsec - _Now.tv_nsec;
    return duration_timespec_to_ms(_Now);
}

template<typename timespec>
static int __cdecl common_mtx_timedlock(
    mtx_t* __restrict _Mtx, const timespec* __restrict _Ts)
{
    int _iResult = 0;
    const auto _uCurrentThreadId = GetCurrentThreadId();

    AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));

    do
    {
        const auto _uCurrentThreadId = GetCurrentThreadId();
        if (_uCurrentThreadId == _Mtx->_Owner)
        {
            if ((_Mtx->_Type & mtx_recursive) == 0)
            {
                abort();
            }
            _Mtx->_Cnt++;
            _iResult = thrd_success;
            break;
        }

        for (;;)
        {
            if (_Mtx->_Owner == 0)
            {
                _Mtx->_Owner = _uCurrentThreadId;
                _Mtx->_Cnt = 1;
                _iResult = thrd_success;
                break;
            }
            const auto _iDuration = abstime_timespec_to_duration_ms(_Ts);
            if (_iDuration < 0ll)
            {
                _iResult = thrd_timedout;
                break;
            }
            else if (_iDuration > int64_t(UINT32_MAX))
            {
                _iResult = thrd_error;
                break;
            } 

            if (SleepConditionVariableSRW(
                reinterpret_cast<PCONDITION_VARIABLE>(&_Mtx->_Cv),
                reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr),
                static_cast<DWORD>(_iDuration), 0))
            {
                continue;
            }

            if (GetLastError() != ERROR_TIMEOUT)
            {
                _iResult = thrd_error;
                break;
            }
        }

        break;
    } while (false);

    ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));
    return _iResult;
}

extern "C" int __cdecl _mtx_timedlock32(
    mtx_t * __restrict _Mtx, const struct _timespec32* __restrict _Ts)
{
    return common_mtx_timedlock(_Mtx, _Ts);
}

_LCRT_DEFINE_IAT_SYMBOL(_mtx_timedlock32);

extern "C" int __cdecl _mtx_timedlock64(
    mtx_t* __restrict _Mtx, const struct _timespec64* __restrict _Ts)
{
    return common_mtx_timedlock(_Mtx, _Ts);
}

_LCRT_DEFINE_IAT_SYMBOL(_mtx_timedlock64);

extern "C" int __cdecl mtx_trylock(mtx_t* _Mtx)
{
    int _iResult = thrd_success;
    const auto _uCurrentThreadId = GetCurrentThreadId();

    AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));
    
    if (_Mtx->_Owner == 0)
    {
        _Mtx->_Owner = _uCurrentThreadId;
        _Mtx->_Cnt = 1;
        _iResult = thrd_success;
    }
    else if (_uCurrentThreadId == _Mtx->_Owner)
    {
        if ((_Mtx->_Type & mtx_recursive) == 0)
        {
            _iResult = thrd_busy;
        }
        else
        {
            _Mtx->_Cnt++;
            _iResult = thrd_success;
        }
    }
    else
    {
        _iResult = thrd_busy;
    }

    ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));
    return _iResult;
}

_LCRT_DEFINE_IAT_SYMBOL(mtx_trylock);

static int __cdecl mtx_dounlock(mtx_t* _Mtx)
{
    if (GetCurrentThreadId() != _Mtx->_Owner)
    {
        abort();
    }

    if (--(_Mtx->_Cnt) == 0)
    {
        _Mtx->_Owner = 0;
    }
    WakeConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(&_Mtx->_Cv));
    return thrd_success;
}

extern "C" int __cdecl mtx_unlock(mtx_t* _Mtx)
{
    AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));
    const auto _iResult = mtx_dounlock(_Mtx);
    ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));
    return _iResult;
}

_LCRT_DEFINE_IAT_SYMBOL(mtx_unlock);

extern "C" int __cdecl cnd_broadcast(cnd_t * _Cond)
{
    WakeAllConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(&_Cond->_Ptr));
    return thrd_success;
}

_LCRT_DEFINE_IAT_SYMBOL(cnd_broadcast);

extern "C" void __cdecl cnd_destroy(cnd_t * _Cond)
{
    // 什么也不做
}

_LCRT_DEFINE_IAT_SYMBOL(cnd_destroy);

extern "C" int __cdecl cnd_init(cnd_t * _Cond)
{
    InitializeConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(&_Cond->_Ptr));
    return thrd_success;
}

_LCRT_DEFINE_IAT_SYMBOL(cnd_init);

extern "C" int __cdecl cnd_signal(cnd_t * _Cond)
{
    WakeConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(&_Cond->_Ptr));
    return thrd_success;
}

_LCRT_DEFINE_IAT_SYMBOL(cnd_signal);

template<typename timespec>
static int __cdecl common_cnd_timedwait(cnd_t* _Cond, mtx_t* _Mtx, const timespec* _Ts)
{
    int _iResult = thrd_success;
    AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));

    do
    {
        if (mtx_dounlock(_Mtx) != thrd_success)
        {
            _iResult = thrd_error;
            break;
        }

        for (;;)
        {
            const auto _iDuration = abstime_timespec_to_duration_ms(_Ts);
            if (_iDuration <= 0ll)
            {
                _iResult = thrd_timedout;
                break;
            }
            else if (_iDuration > int64_t(UINT32_MAX))
            {
                _iResult = thrd_error;
                break;
            }

            if (SleepConditionVariableSRW(reinterpret_cast<PCONDITION_VARIABLE>(&_Cond->_Ptr), reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr), static_cast<DWORD>(_iDuration), 0))
            {
                _iResult = thrd_success;
                break;
            }
        }

        if (mtx_dolock(_Mtx) != thrd_success)
        {
            _iResult = thrd_error;
            break;
        }
    } while (false);


    ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));
    return _iResult;
}

extern "C" int __cdecl _cnd_timedwait32(cnd_t * _Cond, mtx_t * _Mtx, const struct _timespec32* _Ts)
{
    return common_cnd_timedwait(_Cond, _Mtx, _Ts);
}

_LCRT_DEFINE_IAT_SYMBOL(_cnd_timedwait32);

extern "C" int __cdecl _cnd_timedwait64(cnd_t* _Cond, mtx_t* _Mtx, const struct _timespec64* _Ts)
{
    return common_cnd_timedwait(_Cond, _Mtx, _Ts);
}

_LCRT_DEFINE_IAT_SYMBOL(_cnd_timedwait64);

extern "C" int cnd_wait(cnd_t* _Cond, mtx_t* _Mtx)
{
    int _iResult = thrd_success;
    AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));

    do
    {
        if (mtx_dounlock(_Mtx) != thrd_success)
        {
            _iResult = thrd_error;
            break;
        }

        if (!SleepConditionVariableSRW(reinterpret_cast<PCONDITION_VARIABLE>(&_Cond->_Ptr), reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr), INFINITE, 0))
        {
            _iResult = thrd_error;
        }

        if (mtx_dolock(_Mtx) != thrd_success)
        {
            _iResult = thrd_error;
            break;
        }
    } while (false);

    ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&_Mtx->_Ptr));
    return _iResult;
}

_LCRT_DEFINE_IAT_SYMBOL(cnd_wait);

extern "C" int __cdecl thrd_create(thrd_t * _Thr, thrd_start_t _Func, void* _Arg)
{
#if defined(_X86_)
    // x86平台 __cdecl 与 __stdcall是不同的调用约定
    struct thrd_start_data
    {
        thrd_start_t Func;
        void* Arg;
    };

    auto _pThreadData = (thrd_start_data*)malloc(sizeof(thrd_start_data));
    if (!_pThreadData)
        return thrd_nomem;

    _pThreadData->Func = _Func;
    _pThreadData->Arg = _Arg;

    errno = 0;
    _Thr->_Handle = reinterpret_cast<void*>(_beginthreadex(nullptr, 0,
        [](void* _pUserData) -> unsigned
        {
            auto _pStartData = static_cast<thrd_start_data*>(_pUserData);
            auto _uResult = _pStartData->Func(_pStartData->Arg);
            free(_pStartData);
            return _uResult;
        }, _pThreadData, 0, &_Thr->_Tid));

    if (_Thr->_Handle)
    {
        return thrd_success;
    }

    // 微软原版存在一点瑕疵，应该先取错误代码，然后再释放内存
    // 避免free时 errno 修改风险。
    const auto _iResult = (errno == 0 || errno == ENOMEM) ? thrd_nomem : thrd_error;
    free(_pThreadData);
    return _iResult;
#else
    // 其他平台 __cdecl 与 __stdcall等价，所以不用额外跳转直接就可以用。
    errno = 0;
    _Thr->_Handle = reinterpret_cast<void*>(_beginthreadex(nullptr, 0, reinterpret_cast<_beginthreadex_proc_type>(_Func), _Arg, 0, &_Thr->_Tid));
    if (_Thr->_Handle)
    {
        return thrd_success;
    }

    return (errno == 0 || errno == ENOMEM) ? thrd_nomem : thrd_error;
#endif
}

_LCRT_DEFINE_IAT_SYMBOL(thrd_create);

extern "C" thrd_t __cdecl thrd_current(void)
{
    thrd_t _Result;
    _Result._Handle = nullptr;
    _Result._Tid = GetCurrentThreadId();
    return _Result;
}

_LCRT_DEFINE_IAT_SYMBOL(thrd_current);

extern "C" int __cdecl thrd_detach(thrd_t _Thr)
{
    return CloseHandle(_Thr._Handle) ? thrd_success : thrd_error;
}

_LCRT_DEFINE_IAT_SYMBOL(thrd_detach);

extern "C" int __cdecl thrd_equal(thrd_t _Thr0, thrd_t _Thr1)
{
    return _Thr0._Tid == _Thr1._Tid;
}

_LCRT_DEFINE_IAT_SYMBOL(thrd_equal);

extern "C" void __cdecl thrd_exit(int _Res)
{
    ExitThread(static_cast<DWORD>(_Res));
}

_LCRT_DEFINE_IAT_SYMBOL(thrd_exit);

extern "C" int __cdecl thrd_join(thrd_t _Thr, int* _Res)
{
    DWORD _uExitCode;
    if (WaitForSingleObject(_Thr._Handle, INFINITE) != /*WAIT_OBJECT_0*/ 0 || GetExitCodeThread(_Thr._Handle, &_uExitCode) == FALSE)
    {
        return thrd_error;
    }

    if (_Res)
    {
        *_Res = static_cast<int>(_uExitCode);
    }
    return CloseHandle(_Thr._Handle) ? thrd_success : thrd_error;
}

_LCRT_DEFINE_IAT_SYMBOL(thrd_join);

extern "C" int __cdecl _thrd_sleep32(
    const struct _timespec32* duration, struct _timespec32* remaining)
{
    const auto _iDuration = duration_timespec_to_ms(*duration);
    if (_iDuration >= 0ll && _iDuration <= int64_t(UINT32_MAX))
    {
        Sleep(static_cast<DWORD>(_iDuration));
        return 0;
    }
    else
    {
        return -2;
    }
}

_LCRT_DEFINE_IAT_SYMBOL(_thrd_sleep32);

extern "C" int __cdecl _thrd_sleep64(
    const struct _timespec64* duration, struct _timespec64* remaining)
{
    const auto _iDuration = duration_timespec_to_ms(*duration);
    if (_iDuration >= 0ll && _iDuration <= int64_t(UINT32_MAX))
    {
        Sleep(static_cast<DWORD>(_iDuration));
        return 0;
    }
    else
    {
        return -2;
    }
}

_LCRT_DEFINE_IAT_SYMBOL(_thrd_sleep64);

extern "C" void __cdecl thrd_yield(void)
{
    SwitchToThread();
}

_LCRT_DEFINE_IAT_SYMBOL(thrd_yield);

static tss_global_data_t tss_global_data;

extern "C" int __cdecl tss_create(tss_t * _Key, tss_dtor_t _Dtor)
{
    return tss_global_data.create(_Key, _Dtor);
}

_LCRT_DEFINE_IAT_SYMBOL(tss_create);

extern "C" void __cdecl tss_delete(tss_t _Key)
{
    tss_global_data.tss_delete(_Key);
}

_LCRT_DEFINE_IAT_SYMBOL(tss_delete);

extern "C" void* __cdecl tss_get(tss_t _Key)
{
    return tss_global_data.get(_Key);
}

_LCRT_DEFINE_IAT_SYMBOL(tss_get);

extern "C" int __cdecl tss_set(tss_t _Key, void* _Val)
{
    return tss_global_data.set(_Key, _Val);
}

_LCRT_DEFINE_IAT_SYMBOL(tss_set);

extern "C" void __cdecl call_once(once_flag* _Flag, void(*_Func)(void))
{
    InitOnceExecuteOnce(reinterpret_cast<PINIT_ONCE>(_Flag),
        [](_In_ PINIT_ONCE InitOnce,
            _In_ PVOID Parameter,
            _In_opt_ PVOID* Context) ->BOOL
        {
            auto _Func = static_cast<void(*)(void)>(Parameter);
            _Func();
            return TRUE;
        },
        _Func,
        nullptr);
}

_LCRT_DEFINE_IAT_SYMBOL(call_once);

#pragma section(".CRT$XLE", long, read)

static void NTAPI ClearTss(
    PVOID DllHandle,
    DWORD Reason,
    PVOID Reserved)
{
    switch (Reason)
    {
    case DLL_PROCESS_DETACH:
        tss_global_data.cleanup_global();
        break;
    case DLL_THREAD_DETACH:
        tss_global_data.cleanup();
        break;
    }
}

static _CRTALLOC(".CRT$XLE") PIMAGE_TLS_CALLBACK volmd = &ClearTss;
