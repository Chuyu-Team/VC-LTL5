#include "pch.h"
#include "CppUnitTest.h"

#include <threads.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace C11Threads
{
	constexpr auto kReturnCode = 8848;

	TEST_CLASS(Threads)
	{
	public:
		TEST_METHOD(thrd_create与thrd_detach)
		{
			thrd_t _Thrd;
			auto _iResult = ::thrd_create(&_Thrd, [](void* _pUserData) -> int
				{
					return kReturnCode;
				}, nullptr);

			Assert::AreEqual(_iResult, 0);

			_iResult = thrd_detach(_Thrd);
			Assert::AreEqual(_iResult, 0);
		}

		TEST_METHOD(thrd_current)
		{
			auto _Result = ::thrd_current();
			Assert::AreEqual(_Result._Tid, static_cast<uint32_t>(GetCurrentThreadId()));
		}

		TEST_METHOD(thrd_join)
		{
			int _iOut = 0;
			thrd_t _Thrd;
			auto _iResult = ::thrd_create(&_Thrd, [](void* _pUserData) -> int
				{
					*(int*)_pUserData = kReturnCode;
					Sleep(500);
					return kReturnCode;
				}, &_iOut);
			Assert::AreEqual(_iResult, 0);

			int _iExitCode = 0;
			_iResult = ::thrd_join(_Thrd, &_iExitCode);
			Assert::AreEqual(_iResult, 0);
			Assert::AreEqual(_iExitCode, kReturnCode);
			Assert::AreEqual(_iOut, kReturnCode);
		}

		TEST_METHOD(thrd_equal)
		{
			{
				thrd_t _Left = { reinterpret_cast<void*>(888), 111 };
				thrd_t _Right = { reinterpret_cast<void*>(5), 111 };;

				auto _bResult = ::thrd_equal(_Left, _Right);
				Assert::IsTrue(_bResult);
			}

			{
				thrd_t _Left = { reinterpret_cast<void*>(888), 111 };
				thrd_t _Right = { reinterpret_cast<void*>(888), 22 };;

				auto _bResult = ::thrd_equal(_Left, _Right);
				Assert::IsFalse(_bResult);
			}
		}

		TEST_METHOD(thrd_exit)
		{
			thrd_t _Thrd;
			auto _iResult = ::thrd_create(&_Thrd, [](void*) -> int
				{
					::thrd_exit(kReturnCode);
					return 1;
				}, nullptr);
			Assert::AreEqual(_iResult, 0);

			int _iExitCode = 0;
			_iResult = ::thrd_join(_Thrd, &_iExitCode);
			Assert::AreEqual(_iResult, 0);
			Assert::AreEqual(_iExitCode, kReturnCode);
		}

		TEST_METHOD(_thrd_sleep64)
		{
			// 等待500ms
			{
				_timespec64 _Duration = { 0, 1000000 * 500 };

				const auto _uStart = GetTickCount64();
				auto _iResult = ::_thrd_sleep64(&_Duration, nullptr);
				const auto _uTick = GetTickCount64() - _uStart;
				Assert::AreEqual(_iResult, 0);
				Assert::IsTrue(_uTick >= 400 && _uTick <= 700);
			}

			// 等待 1.5秒
			{
				_timespec64 _Duration = { 1, 1000000 * 500 };

				const auto _uStart = GetTickCount64();
				auto _iResult = ::_thrd_sleep64(&_Duration, nullptr);
				const auto _uTick = GetTickCount64() - _uStart;
				Assert::AreEqual(_iResult, 0);
				Assert::IsTrue(_uTick >= 1400 && _uTick <= 1700);
			}

			// 超大等待时间，返回失败
			{
				_timespec64 _Duration = { UINT32_MAX, 0 };
				auto _iResult = ::_thrd_sleep64(&_Duration, nullptr);
				Assert::AreNotEqual(_iResult, 0);
			}
		}
	};

	static void* g_pClearupData = 0;

	TEST_CLASS(ThreadSpecificStorage)
	{
	public:
		TEST_METHOD(tss_create与tss_delete)
		{
			tss_t _Key;
			auto _iResult = ::tss_create(&_Key, nullptr);
			Assert::AreEqual(_iResult, 0);

			::tss_delete(_Key);
		}
		
		TEST_METHOD(Tss清理机制)
		{
			g_pClearupData = nullptr;
			tss_t _Key;
			auto _iResult = ::tss_create(&_Key, [](void* _pData)
				{
					g_pClearupData = _pData;
				});
			Assert::AreEqual(_iResult, 0);

			auto _hThread = (HANDLE)_beginthreadex(
				0,
				0,
				[](void* _pUserData) -> unsigned
				{
					auto& _Key = *static_cast<tss_t*>(_pUserData);
					auto _iResult = ::tss_set(_Key, (void*)7);
					Assert::AreEqual(_iResult, 0);
					return 0;
				}, &_Key, 0, nullptr);

			Assert::IsNotNull(_hThread);

			Assert::AreEqual(WaitForSingleObject(_hThread, 5 * 1000), (DWORD)WAIT_OBJECT_0);

			CloseHandle(_hThread);

			Assert::AreEqual(g_pClearupData, (void*)7);

			::tss_delete(_Key);
		}

		TEST_METHOD(单线程tss_get与tss_set)
		{
			tss_t _Key;
			auto _iResult = ::tss_create(&_Key, nullptr);
			Assert::AreEqual(_iResult, 0);

			Assert::AreEqual(::tss_get(_Key), (void*)NULL);

			_iResult = ::tss_set(_Key, (void*)5);
			Assert::AreEqual(_iResult, 0);
			Assert::AreEqual(::tss_get(_Key), (void*)5);

			_iResult = ::tss_set(_Key, (void*)7);
			Assert::AreEqual(_iResult, 0);
			Assert::AreEqual(::tss_get(_Key), (void*)7);

			::tss_delete(_Key);
		}

		TEST_METHOD(多线程tss_get与tss_set)
		{
			tss_t _Key;
			auto _iResult = ::tss_create(&_Key, nullptr);
			_iResult = ::tss_set(_Key, (void*)5);
			Assert::AreEqual(_iResult, 0);
			Assert::AreEqual(::tss_get(_Key), (void*)5);


			auto _hThread = (HANDLE)_beginthreadex(
				0,
				0,
				[](void* _pUserData) -> unsigned
				{
					auto& _Key = *static_cast<tss_t*>(_pUserData);
					Assert::AreEqual(::tss_get(_Key), (void*)0);
					auto _iResult = ::tss_set(_Key, (void*)7);
					Assert::AreEqual(_iResult, 0);
					Assert::AreEqual(::tss_get(_Key), (void*)7);
					return 0;
				}, &_Key, 0, nullptr);

			Assert::IsNotNull(_hThread);

			Assert::AreEqual(WaitForSingleObject(_hThread, 5 * 1000), (DWORD)WAIT_OBJECT_0);

			CloseHandle(_hThread);

			Assert::AreEqual(::tss_get(_Key), (void*)5);
		}
	};

	TEST_CLASS(Mutexes)
	{
	public:
		TEST_METHOD(mtx_init)
		{
			{
				mtx_t _Mtx;
				auto _iResult = ::mtx_init(&_Mtx, 0);
				Assert::AreEqual(_iResult, 0);
				mtx_destroy(&_Mtx);
			}

			{
				mtx_t _Mtx;
				auto _iResult = ::mtx_init(&_Mtx, mtx_recursive);
				Assert::AreEqual(_iResult, 0);
				mtx_destroy(&_Mtx);
			}
		}

		TEST_METHOD(mtx_lock与mtx_unlock)
		{
			// 锁定后其他线程会等待
			{
				mtx_t _Mtx;
				::mtx_init(&_Mtx, 0);

				auto _iResult = ::mtx_lock(&_Mtx);
				Assert::AreEqual(_iResult, 0);

				auto _hThread = (HANDLE)_beginthreadex(
					0,
					0,
					[](void* _pUserData) -> unsigned
					{
						auto& _Mtx = *static_cast<mtx_t*>(_pUserData);

						auto _uStart = GetTickCount64();
						auto _iResult = ::mtx_lock(&_Mtx);
						auto _uTick = GetTickCount64() - _uStart;

						_iResult = ::mtx_unlock(&_Mtx);
						Assert::AreEqual(_iResult, 0);
						return static_cast<unsigned>(_uTick);
					}, &_Mtx, 0, nullptr);

				Assert::IsNotNull(_hThread);

				Sleep(500);
				_iResult = ::mtx_unlock(&_Mtx);
				Assert::AreEqual(_iResult, 0);

				Assert::AreEqual(WaitForSingleObject(_hThread, 5 * 1000), (DWORD)WAIT_OBJECT_0);

				DWORD _uCode = 0;
				GetExitCodeThread(_hThread, &_uCode);
				CloseHandle(_hThread);

				Assert::IsTrue(_uCode <= 700 && _uCode >= 400);
			}

			// 递归时多次加锁解锁不会有事情
			{
				mtx_t _Mtx;
				::mtx_init(&_Mtx, mtx_recursive);

				auto _iResult = ::mtx_lock(&_Mtx);
				Assert::AreEqual(_iResult, 0);
				_iResult = ::mtx_lock(&_Mtx);
				Assert::AreEqual(_iResult, 0);
				_iResult = ::mtx_unlock(&_Mtx);
				Assert::AreEqual(_iResult, 0);
				_iResult = ::mtx_unlock(&_Mtx);
				Assert::AreEqual(_iResult, 0);
			}
		}

		TEST_METHOD(mtx_trylock)
		{
			{
				mtx_t _Mtx;
				::mtx_init(&_Mtx, 0);

				auto _iResult = ::mtx_trylock(&_Mtx);
				Assert::AreEqual(_iResult, 0);

				_iResult = ::mtx_trylock(&_Mtx);
				Assert::AreNotEqual(_iResult, 0);

				_iResult = ::mtx_unlock(&_Mtx);
				Assert::AreEqual(_iResult, 0);

				_iResult = ::mtx_trylock(&_Mtx);
				Assert::AreEqual(_iResult, 0);
			}

			{
				mtx_t _Mtx;
				auto _iResult = ::mtx_init(&_Mtx, mtx_recursive);
				Assert::AreEqual(_iResult, 0);

				_iResult = ::mtx_trylock(&_Mtx);
				Assert::AreEqual(_iResult, 0);

				_iResult = ::mtx_trylock(&_Mtx);
				Assert::AreEqual(_iResult, 0);
			}
		}

		TEST_METHOD(_mtx_timedlock64)
		{
			mtx_t _Mtx;
			auto _iResult = ::mtx_init(&_Mtx, 0);
			Assert::AreEqual(_iResult, 0);

			_timespec64 _Now;
			_iResult = _timespec64_get(&_Now, TIME_UTC);
			Assert::AreEqual(_iResult, TIME_UTC);

			_iResult = ::_mtx_timedlock64(&_Mtx, &_Now);
			Assert::AreEqual(_iResult, 0);

			auto _hThread = (HANDLE)_beginthreadex(
				0,
				0,
				[](void* _pUserData) -> unsigned
				{
					auto& _Mtx = *static_cast<mtx_t*>(_pUserData);
					auto _uStart = GetTickCount64();

					_timespec64 _Now;
					auto _iResult = _timespec64_get(&_Now, TIME_UTC);
					Assert::AreEqual(_iResult, TIME_UTC);
					_Now.tv_sec += 1;

					_iResult = ::_mtx_timedlock64(&_Mtx, &_Now);
					Assert::AreEqual(_iResult, 2);
					auto _uTick = GetTickCount64() - _uStart;
					return static_cast<unsigned>(_uTick);
				}, &_Mtx, 0, nullptr);

			Assert::IsNotNull(_hThread);
			Assert::AreEqual(WaitForSingleObject(_hThread, 5 * 1000), (DWORD)WAIT_OBJECT_0);

			DWORD _uCode = 0;
			GetExitCodeThread(_hThread, &_uCode);
			CloseHandle(_hThread);

			Assert::IsTrue(_uCode <= 1200 && _uCode >= 900);
		}
	};

	TEST_CLASS(ConditionVariables)
	{
	public:
		TEST_METHOD(cnd_init)
		{
			cnd_t _Cond;
			auto _iResult = ::cnd_init(&_Cond);
			Assert::AreEqual(_iResult, 0);
		}

		TEST_METHOD(cnd_signal与cnd_broadcast与cnd_wait)
		{
			struct ThreadData
			{
				mtx_t Mtx;
				cnd_t Cond;
			};

			ThreadData _ThreadData;
			
			auto _iResult = ::mtx_init(&_ThreadData.Mtx, 0);
			Assert::AreEqual(_iResult, 0);

			_iResult = ::cnd_init(&_ThreadData.Cond);
			Assert::AreEqual(_iResult, 0);

			HANDLE _hThreads[5];
			for (auto& _hThread : _hThreads)
			{
				_hThread = (HANDLE)_beginthreadex(
					0,
					0,
					[](void* _pUserData) -> unsigned
					{
						auto& _ThreadData = *static_cast<ThreadData*>(_pUserData);
						auto _iResult = ::mtx_lock(&_ThreadData.Mtx);
						Assert::AreEqual(_iResult, 0);
						::cnd_wait(&_ThreadData.Cond, &_ThreadData.Mtx);
						_iResult = ::mtx_unlock(&_ThreadData.Mtx);
						Assert::AreEqual(_iResult, 0);
						return 1;
					}, &_ThreadData, 0, nullptr);

				Assert::IsNotNull(_hThread);
			}

			Sleep(100);

			_iResult = cnd_signal(&_ThreadData.Cond);
			Assert::AreEqual(_iResult, 0);

			uint32_t _uSignalCount = 0;

			for (auto& _hThread : _hThreads)
			{
				if (!_hThread)
					continue;
				if (WaitForSingleObject(_hThread, 100) == WAIT_OBJECT_0)
				{
					++_uSignalCount;
					CloseHandle(_hThread);
					_hThread = nullptr;
				}
			}
			
			Assert::AreEqual(_uSignalCount, 1u);
			_iResult = cnd_signal(&_ThreadData.Cond);
			Assert::AreEqual(_iResult, 0);

			for (auto& _hThread : _hThreads)
			{
				if (!_hThread)
					continue;
				if (WaitForSingleObject(_hThread, 100) == WAIT_OBJECT_0)
				{
					++_uSignalCount;

					CloseHandle(_hThread);
					_hThread = nullptr;
				}
			}
			Assert::AreEqual(_uSignalCount, 2u);

			_iResult = cnd_broadcast(&_ThreadData.Cond);
			Assert::AreEqual(_iResult, 0);

			for (auto& _hThread : _hThreads)
			{
				if (!_hThread)
					continue;
				if (WaitForSingleObject(_hThread, 100) == WAIT_OBJECT_0)
				{
					++_uSignalCount;

					CloseHandle(_hThread);
					_hThread = nullptr;
				}
			}

			Assert::AreEqual(_uSignalCount, (uint32_t)std::size(_hThreads));
		}

		TEST_METHOD(_cnd_timedwait64)
		{
			struct ThreadData
			{
				mtx_t Mtx;
				cnd_t Cond;
			};

			ThreadData _ThreadData;

			auto _iResult = ::mtx_init(&_ThreadData.Mtx, 0);
			Assert::AreEqual(_iResult, 0);

			_iResult = ::cnd_init(&_ThreadData.Cond);
			Assert::AreEqual(_iResult, 0);

			_iResult = ::mtx_lock(&_ThreadData.Mtx);
			Assert::AreEqual(_iResult, 0);

			auto _hThread = (HANDLE)_beginthreadex(
				0,
				0,
				[](void* _pUserData) -> unsigned
				{
					Sleep(5 * 1000);
					auto& _ThreadData = *static_cast<ThreadData*>(_pUserData);
					auto _iResult = cnd_signal(&_ThreadData.Cond);
					Assert::AreEqual(_iResult, 0);
					return 1;
				}, &_ThreadData, 0, nullptr);

			Assert::IsNotNull(_hThread);
			_timespec64 _Now;

			const auto _uStart0 = GetTickCount64();

			_iResult = _timespec64_get(&_Now, TIME_UTC);
			Assert::AreEqual(_iResult, TIME_UTC);

			_Now.tv_sec += 1;
			_iResult = ::_cnd_timedwait64(&_ThreadData.Cond, &_ThreadData.Mtx, &_Now);
			const auto _uTick1 = GetTickCount64() - _uStart0;
			Assert::AreEqual(_iResult, (int)thrd_timedout);
			Assert::IsTrue(_uTick1 >= 900 && _uTick1 <= 1200);

			_Now.tv_sec += 5;
			_iResult = ::_cnd_timedwait64(&_ThreadData.Cond, &_ThreadData.Mtx, &_Now);
			const auto _uTick2 = GetTickCount64() - _uStart0;
			Assert::AreEqual(_iResult, (int)thrd_success);
			Assert::IsTrue(_uTick2 >= 4800 && _uTick2 <= 5300);

			::mtx_unlock(&_ThreadData.Mtx);
		}
	};
}
