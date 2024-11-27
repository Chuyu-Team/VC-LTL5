#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(detect_mismatch检测)
	{
	public:

		TEST_METHOD(_CRT_STDIO_ISO_WIDE_SPECIFIERS)
		{
			constexpr static const char TestCode[] = R"(

#define _CRT_STDIO_ISO_WIDE_SPECIFIERS

#include <stdio.h>

)";
			RunTest(TestCode, sizeof(TestCode), L"_CRT_STDIO_ISO_WIDE_SPECIFIERS");
			
		}

		TEST_METHOD(_CRT_STDIO_LEGACY_WIDE_SPECIFIERS)
		{
			constexpr static const char TestCode[] = R"(

#define _CRT_STDIO_LEGACY_WIDE_SPECIFIERS

#include <stdio.h>

)";
			RunTest(TestCode, sizeof(TestCode), L"_CRT_STDIO_LEGACY_WIDE_SPECIFIERS");

		}

	private:
		static void RunTest(
			const void* pTestCode,
			DWORD cbTestCode,
			LPCWSTR DetectName,
			LPCWSTR BuildProperty = nullptr
			)
		{
			RunTest(
				pTestCode,
				cbTestCode,
				DetectName,
				L"Win32",
				{ L"Dynamic", L"Static", L"DDynamic", L"DStatic" },
				{ L"5.1.2600.0", L"6.0.6000.0", L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" },
				BuildProperty
			);

			RunTest(
				pTestCode,
				cbTestCode,
				DetectName,
				L"x64",
				{ L"Dynamic", L"Static", L"DDynamic", L"DStatic" },
				{ L"5.2.3790.0", L"6.0.6000.0", L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" },
				BuildProperty
			);

			//RunTest(
			//	pTestCode,
			//	cbTestCode,
			//	DetectName,
			//	L"arm",
			//	{ L"Dynamic", L"Static", L"DDynamic", L"DStatic" },
			//	{ L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" },
			//	BuildProperty
			//);

			RunTest(
				pTestCode,
				cbTestCode,
				DetectName,
				L"arm64",
				{ L"Dynamic", L"Static", L"DDynamic", L"DStatic" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				BuildProperty
			);
		}
		
		static void RunTest(
			const void* pTestCode,
			DWORD cbTestCode,
			LPCWSTR DetectName,
			LPCWSTR szPlatform,
			std::initializer_list<LPCWSTR> szConfigurations,
			std::initializer_list<LPCWSTR> szWindowsTargetPlatformMinVersions,
			LPCWSTR BuildProperty = nullptr
			)
		{
			for (auto szConfiguration : szConfigurations)
			{
				for (auto szWindowsTargetPlatformMinVersion : szWindowsTargetPlatformMinVersions)
				{
					RunMSBuildTest(pTestCode, cbTestCode, DetectName, szPlatform, szConfiguration, szWindowsTargetPlatformMinVersion, BuildProperty);
				}
			}
		}
	};
}
