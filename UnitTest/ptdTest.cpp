#include "pch.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	extern "C" void* __cdecl __acrt_getptd_noexit();

	TEST_CLASS(PTD测试)
	{
	public:
		TEST_METHOD(主线程PTD测试)
		{
			Assert::IsNotNull(__acrt_getptd_noexit());
		}

		TEST_METHOD(_beginthread_PTD测试)
		{
			HANDLE _hEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);

            _beginthread(
				[](void* _hEvent)
                {
                    Assert::IsNotNull(__acrt_getptd_noexit());

                    SetEvent(_hEvent);

                }, 0, _hEvent);


			Assert::AreEqual(WaitForSingleObject(_hEvent, 5 * 1000), (DWORD)WAIT_OBJECT_0);

			CloseHandle(_hEvent);
		}

		TEST_METHOD(_beginthreadex_PTD测试)
		{
			auto _hThread = (HANDLE)_beginthreadex(
				0,
				0,
				[](void*) -> unsigned
				{
					Assert::IsNotNull(__acrt_getptd_noexit());
					return 0;
				}, 0, 0, nullptr);

			Assert::IsNotNull(_hThread);

			Assert::AreEqual(WaitForSingleObject(_hThread, 5 * 1000), (DWORD)WAIT_OBJECT_0);

			CloseHandle(_hThread);
		}

		TEST_METHOD(CreateThread_PTD测试)
		{
			auto _hThread = (HANDLE)CreateThread(
				0,
				0,
				[](void*) -> DWORD
				{
					Assert::IsNotNull(__acrt_getptd_noexit());
					return 0;
				}, 0, 0, nullptr);

			Assert::IsNotNull(_hThread);

			Assert::AreEqual(WaitForSingleObject(_hThread, 5 * 1000), (DWORD)WAIT_OBJECT_0);

			CloseHandle(_hThread);
		}
	};
}
