#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(HelperForPowerShellUnitTest)
	{
	public:
		CString szPSRootPath;
		CString szTestDllPath;

		HelperForPowerShellUnitTest()
		{
			szPSRootPath = _CRT_WIDE(__FILE__);
			szPSRootPath.ReleaseBufferSetLength(szPSRootPath.ReverseFind(L'\\') + 1);

			szTestDllPath.ReleaseBufferSetLength(GetModuleFileNameW((HMODULE)&__ImageBase, szTestDllPath.GetBuffer(512), 512));
			szTestDllPath.ReleaseBufferSetLength(szTestDllPath.ReverseFind(L'\\') + 1);

		}

		TEST_METHOD(WindowsTargetPlatformMinVersion适配性检查)
		{
			struct CheckConfig
			{
				LPCWSTR szInput;
				LPCWSTR szOutput;
			};

			static const CheckConfig _oConfigList[] =
			{
				// 如果设置 5.0.0.0，预期应该变成5.1.2600.0，因为最低仅支持5.1.2600.0
				{ L"5.0.0.0", L"5.1.2600.0"},
				// 版本完全支持，那么就应该维持现状
				{ L"5.1.2600.0", L"5.1.2600.0" },
				{ L"6.0.6000.0", L"6.0.6000.0"},
				{ L"10.0.19041.0", L"10.0.19041.0"},
				// 输入略大于 5.1.2600.0的版本，应该就近匹配，所以是 5.1.2600.0
				{ L"5.1.2700.0", L"5.1.2600.0" },
				// 输入超大版本号，自动匹配到最高支持版本
				{L"99.0.19041.0", L"10.0.19041.0"},
			};
			for (auto& _oConfig : _oConfigList)
			{
				CString _szCmd;
				_szCmd.Format(LR"(powershell.exe -File "%wsPowerShellInitUnitTest.ps1" -arch x86 -WindowsTargetPlatformMinVersion "%ws")", szPSRootPath.GetString(), _oConfig.szInput);
				CString _szOut;
				auto _lStatus = RunCmd(nullptr, _szCmd, &_szOut);
				Assert::AreEqual(_lStatus, (LSTATUS)ERROR_SUCCESS);

				auto _nIndex = _szOut.Find(CString(L"WindowsTargetPlatformMinVersion : ") + _oConfig.szOutput);
				Assert::AreNotEqual(_nIndex, -1, CString(L"未找到：") + _oConfig.szOutput);
			}
		}

		TEST_METHOD(可链接性检查)
		{
			struct CheckConfig
			{
				LPCWSTR szPlatform;
				LPCWSTR szTargetPlatform;
				std::vector<LPCSTR> IncludeDllNames;
				std::vector<LPCSTR> ExcludeDllNames;
			};

			static const CheckConfig _oConfigList[] =
			{
				{L"x86", L"Win32", {"msvcrt.dll"}, {"vcruntime140.dll"}},
				{L"amd64", L"x64", {"msvcrt.dll"}, {"vcruntime140.dll"}},
			};

			for (auto& _oConfig : _oConfigList)
			{
				CString _szOutDir = szTestDllPath;
				_szOutDir += LR"(out\HelperForPowerShellUnitTest\)";
				_szOutDir += _oConfig.szPlatform;

				CString _szCmd;
				_szCmd.Format(LR"(powershell.exe -File "%wsPowerShellBuildUnitTest.ps1" -arch %ws -OutDir "%ws")", szPSRootPath.GetString(), _oConfig.szPlatform, _szOutDir.GetString());
				CString _szOut;
				auto _lStatus = RunCmd(nullptr, _szCmd, &_szOut);
				Assert::AreEqual(_lStatus, (LSTATUS)ERROR_SUCCESS);

				auto _nIndex = _szOut.Find(CString(L"Platform : ") + _oConfig.szTargetPlatform);
				Assert::AreNotEqual(_nIndex, -1, CString(L"未找到：") + _oConfig.szTargetPlatform);

				auto _oImportInfo = GetDllImportInfo(_szOutDir + LR"(\NMakeExample.exe)");


				for (auto _szIncludeDllName : _oConfig.IncludeDllNames)
				{
					auto _pInfo = FindDllImport(_oImportInfo, _szIncludeDllName);
					Assert::IsNotNull(_pInfo, CStringW(_oConfig.szPlatform) + L" Include:" + _szIncludeDllName);
				}

				for (auto _szExcludeDllName : _oConfig.ExcludeDllNames)
				{
					auto _pInfo = FindDllImport(_oImportInfo, _szExcludeDllName);
					Assert::IsNull(_pInfo, CStringW(_oConfig.szPlatform) + L" Exclude:" + _szExcludeDllName);
				}
			}
		}

		TEST_METHOD(CleanImport生效检查)
		{
			struct CheckConfig
			{
				LPCWSTR szPlatform;
				LPCWSTR szAppend;
				std::vector<LPCSTR> IncludeDllNames;
				std::vector<LPCSTR> ExcludeDllNames;
			};

			static const CheckConfig _oConfigList[] =
			{
				// 默认情况不开启 CleanImport，会依赖 api-ms-win-crt-stdio
				{L"x86", L" -WindowsTargetPlatformMinVersion 10.0.10240.0", {"ucrtbase.dll", "api-ms-win-crt-stdio-l1-1-0.dll"}, {"vcruntime140.dll"}},
				// 显式指定关闭 CleanImport，会依赖 api-ms-win-crt-stdio
				{L"x86", L" -WindowsTargetPlatformMinVersion 10.0.10240.0 -CleanImport false", {"ucrtbase.dll", "api-ms-win-crt-stdio-l1-1-0.dll"}, {"vcruntime140.dll"}},
				// 开启时不依赖 api-ms-win-crt-stdio
				{L"x86", L" -WindowsTargetPlatformMinVersion 10.0.10240.0 -CleanImport true", {"ucrtbase.dll"}, {"vcruntime140.dll", "api-ms-win-crt-stdio-l1-1-0.dll"}},
			};

			for (auto& _oConfig : _oConfigList)
			{
				CString _szOutDir = szTestDllPath;
				_szOutDir += LR"(out\HelperForPowerShellCleanImportUnitTest\)";
				_szOutDir += _oConfig.szPlatform;

				CString _szCmd;
				_szCmd.Format(LR"(powershell.exe -File "%wsPowerShellBuildUnitTest.ps1" -arch %ws -OutDir "%ws")", szPSRootPath.GetString(), _oConfig.szPlatform, _szOutDir.GetString());
				
				_szCmd += _oConfig.szAppend;
				
				CString _szOut;
				auto _lStatus = RunCmd(nullptr, _szCmd, &_szOut);
				Assert::AreEqual(_lStatus, (LSTATUS)ERROR_SUCCESS);

				auto _oImportInfo = GetDllImportInfo(_szOutDir + LR"(\NMakeExample.exe)");


				for (auto _szIncludeDllName : _oConfig.IncludeDllNames)
				{
					auto _pInfo = FindDllImport(_oImportInfo, _szIncludeDllName);
					Assert::IsNotNull(_pInfo, CStringW(_oConfig.szPlatform) + L" Include:" + _szIncludeDllName);
				}

				for (auto _szExcludeDllName : _oConfig.ExcludeDllNames)
				{
					auto _pInfo = FindDllImport(_oImportInfo, _szExcludeDllName);
					Assert::IsNull(_pInfo, CStringW(_oConfig.szPlatform) + L" Exclude:" + _szExcludeDllName);
				}
			}
		}
	};
}
