#include "pch.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// 对于某些RC文件来说，某些语法是不支持的。
// 这项测试主要用于测试RC文件的容错性。
namespace UnitTest
{
	TEST_CLASS(RC编译检测)
	{
	public:

		TEST_METHOD(RC常规检测)
		{
			constexpr LPCWSTR szWindowsTargetPlatformMinVersions[] = {L"5.2.3790.0", L"6.0.6000.0", L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0"};

			for (auto szWindowsTargetPlatformMinVersion : szWindowsTargetPlatformMinVersions)
			{
				RunRCTest(szWindowsTargetPlatformMinVersion, nullptr);
			}
		}

		static HRESULT RunRCTest(
			LPCWSTR szWindowsTargetPlatformMinVersion,
			CStringW* pTempRoot = nullptr
			)
		{
			CString SymbolsTestCppRootPath = SymbolBuildTestPath L"..\\RCBuildTest\\";
			SymbolsTestCppRootPath += L".test\\";

			CreateDirectoryW(SymbolsTestCppRootPath, nullptr);

			SymbolsTestCppRootPath += L"RC.Test";
			SymbolsTestCppRootPath += L'.';
			SymbolsTestCppRootPath += szWindowsTargetPlatformMinVersion;

			SymbolsTestCppRootPath += L'\\';


			if (pTempRoot)
				*pTempRoot = SymbolsTestCppRootPath;


			CreateDirectoryW(SymbolsTestCppRootPath, nullptr);

			CString Cmd;

			auto szConfiguration = L"Release";
			auto szPlatform = L"Win32";

			Cmd.Format(L"\"%s\" \"%s\" -t:Rebuild \"-p:Configuration=%s;Platform=%s;WindowsTargetPlatformMinVersion=%s;SymbolsTestCppRootPath=%s\\\"",
				MSBuildBinPath LR"(MSBuild.exe)",
				SymbolBuildTestPath LR"(..\RCBuildTest\RCBuildTest.vcxproj)",
				szConfiguration,
				szPlatform,
				szWindowsTargetPlatformMinVersion,
				SymbolsTestCppRootPath.GetString());

			CString OutString;

			auto lStatus = RunCmd(nullptr, Cmd, &OutString);



			OutString.Insert(0, (wchar_t)0xfeff);


			auto BuildLog = SymbolsTestCppRootPath + L"Build.log";

			CreateFileByData(BuildLog, OutString.GetString(), OutString.GetLength() * sizeof(OutString[0]));

			Assert::AreEqual(lStatus, ERROR_SUCCESS, BuildLog);

			// 把这些信息拉黑处理
			constexpr static LPCWSTR LinkWarnings[] =
			{
				L" RC4011:", //RC中不支持宏嵌套
			};

			for (auto Warning : LinkWarnings)
			{
				Assert::AreEqual(OutString.Find(Warning), -1, CString(L"出现了") + Warning + L"\r\n" + SymbolsTestCppRootPath + L"Build.log");
			}

			return lStatus;
		}
	};
}


