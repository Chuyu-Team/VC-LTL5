#include "pch.h"
#include "CppUnitTest.h"
#include <winnt.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void EndianChange(const byte* Src, byte* Dst, size_t cData)
{
	for (size_t i = 0; i != cData; ++i)
	{
		Dst[cData - i - 1] = Src[i];
	}
}


template<class Type>
Type EndianChange(const Type Src)
{
	Type Tmp;

	EndianChange((const byte*)&Src, (byte*)&Tmp, sizeof(Type));

	return Tmp;
}


typedef struct SectionHeader : public IMAGE_ARCHIVE_MEMBER_HEADER
{
	unsigned int get_Size()
	{
		return strtoul((char*)Size, nullptr, 10);
	}

	byte* get_Data()
	{
		auto Start = EndHeader;

		for (; *Start != '`' || *Start == '\n'; ++Start);

		return (byte*)Start + 2;
	}

	SectionHeader* get_NextHeader()
	{
		return (SectionHeader*)(get_Data() + get_Size());
	}
};


typedef struct
{
	unsigned long SymbolNum; // 库中符号的数量

	unsigned long get_SymbolNum()
	{
		return EndianChange(SymbolNum);
	}

	unsigned long SymbolOffset[0/*SymbolNum*/]; // 符号所在目标节的偏移

	unsigned long* get_SymbolOffset()
	{
		return SymbolOffset;
	}

	//char StrTable[SectionHeader.Size-(SymbolNum + 1) * 4]; // 符号名称字符串表

	LPCSTR get_StrTable()
	{
		return (LPCSTR)(SymbolOffset + get_SymbolNum());
	}
}FirstSec;

typedef struct
{
	unsigned long ObjNum; // Obj Sec的数量

	unsigned long get_ObjNum()
	{
		return ObjNum;
	}

	unsigned long ObjOffset[0/*ObjNum*/]; // 每一个Obj Sec的偏移

	unsigned long* get_ObjOffset()
	{
		return ObjOffset;
	}

	//unsigned long SymbolNum; // 库中符号的数量
	unsigned long get_SymbolNum()
	{
		return *(unsigned long*)(ObjOffset + get_ObjNum());
	}


	//unsigned short SymbolIdx[SymbolNum]; // 符号在ObjOffset表中的索引
	unsigned short* get_SymbolIdx()
	{
		return (unsigned short*)(ObjOffset + get_ObjNum() + 1);
	}

	//char StrTable[m]; // 符号名称字符串表
	LPCSTR get_StrTable()
	{
		return (LPCSTR)(get_SymbolIdx() + get_SymbolNum());
	}

}SecondSec;




namespace UnitTest
{
	TEST_CLASS(符号保证)
	{
	public:
		
		TEST_METHOD(vcruntime所有符号可以被链接)
		{
			TestLib(
				VCToolsInstallDir LR"(lib\x86\vcruntime.lib)",
				L"vcruntime",
				L"Win32",
				{ L"Dynamic", L"Static" },
				{ L"5.1.2600.0", L"6.0.6000.0", L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" });

			TestLib(
				VCToolsInstallDir LR"(lib\x64\vcruntime.lib)",
				L"vcruntime",
				L"x64",
				{ L"Dynamic", L"Static" },
				{ L"5.2.3790.0", L"6.0.6000.0", L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" });
			
			TestLib(
				VCToolsInstallDir LR"(lib\arm\vcruntime.lib)",
				L"vcruntime",
				L"arm",
				{ L"Dynamic", L"Static" },
				{ L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" });

			TestLib(
				VCToolsInstallDir LR"(lib\arm64\vcruntime.lib)",
				L"vcruntime",
				L"arm64",
				{ L"Dynamic", L"Static" },
				{ L"10.0.10240.0", L"10.0.19041.0" });
		}

		TEST_METHOD(ucrt所有符号可以被链接)
		{
			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x86\ucrt.lib)",
				L"ucrt",
				L"Win32",
				{ L"Dynamic", L"Static" },
				{ L"5.1.2600.0", L"6.0.6000.0", L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" });

			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x64\ucrt.lib)",
				L"ucrt",
				L"x64",
				{ L"Dynamic", L"Static" },
				{ L"5.2.3790.0", L"6.0.6000.0", L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" });

			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\arm\ucrt.lib)",
				L"ucrt",
				L"arm",
				{ L"Dynamic", L"Static" },
				{ L"6.2.9200.0", L"10.0.10240.0", L"10.0.19041.0" });

			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\arm64\ucrt.lib)",
				L"ucrt",
				L"arm64",
				{ L"Dynamic", L"Static" },
				{ L"10.0.10240.0", L"10.0.19041.0" });
		}
	private:
		static LSTATUS TestLib(LPCWSTR SrcLibPath, LPCWSTR DstLibName, LPCWSTR szPlatform, std::initializer_list<LPCWSTR> szConfigurations, std::initializer_list<LPCWSTR> szWindowsTargetPlatformMinVersions)
		{
			auto&& Symbols = GetALlSymbols(SrcLibPath);

			for (auto szConfiguration : szConfigurations)
			{
				for (auto szWindowsTargetPlatformMinVersion : szWindowsTargetPlatformMinVersions)
				{
					auto&& ExclusionSymbols = GetIgnoreSymbolsList(DstLibName, szPlatform, szConfiguration, szWindowsTargetPlatformMinVersion);

					TestSymbols(Symbols, ExclusionSymbols, DstLibName, szPlatform, szConfiguration, szWindowsTargetPlatformMinVersion);
				}
			}

			return ERROR_SUCCESS;
		}

		static LSTATUS TestSymbols(const std::vector<CStringA>& TestSymbols, const std::set<CStringA>& ExclusionSymbols, LPCWSTR DstLibName, LPCWSTR szPlatform, LPCWSTR szConfiguration, LPCWSTR szWindowsTargetPlatformMinVersion)
		{
			Assert::AreNotEqual(TestSymbols.size(), size_t(0));

			WORD Machine;

			if (wcsicmp(szPlatform, L"Win32") == 0)
			{
				Machine = IMAGE_FILE_MACHINE_I386;
			}
			else if (wcsicmp(szPlatform, L"x64") == 0)
			{
				Machine = IMAGE_FILE_MACHINE_AMD64;
			}
			else if (wcsicmp(szPlatform, L"arm") == 0)
			{
				Machine = IMAGE_FILE_MACHINE_ARMNT;
			}
			else if (wcsicmp(szPlatform, L"arm64") == 0)
			{
				Machine = IMAGE_FILE_MACHINE_ARM64;
			}
			else
			{
				Assert::Fail(szPlatform);
				return ERROR_BAD_FORMAT;
			}


			auto SymbolsTest = BuildRefCPPCode(TestSymbols, Machine, ExclusionSymbols, wcsicmp(szConfiguration, L"Static") == 0);

			CString SymbolsTestCppRootPath = SymbolBuildTestPath;
			SymbolsTestCppRootPath += L".test\\";

			CreateDirectoryW(SymbolsTestCppRootPath, nullptr);

			SymbolsTestCppRootPath += DstLibName;
			SymbolsTestCppRootPath += L'.';
			SymbolsTestCppRootPath += szWindowsTargetPlatformMinVersion;
			SymbolsTestCppRootPath += L'.';
			SymbolsTestCppRootPath += szPlatform;
			SymbolsTestCppRootPath += L'.';
			SymbolsTestCppRootPath += szConfiguration;
			SymbolsTestCppRootPath += L'\\';

			CreateDirectoryW(SymbolsTestCppRootPath, nullptr);

			;

			auto lStatus = CreateFileByData(SymbolsTestCppRootPath + L"SymbolsTest.cpp", SymbolsTest.GetString(), SymbolsTest.GetLength());

			Assert::AreEqual(lStatus, ERROR_SUCCESS, SymbolsTestCppRootPath + L"无法创建");


			CString Cmd;

			Cmd.Format(L"\"%s\" \"%s\" -t:Rebuild \"-p:Configuration=%s;Platform=%s;WindowsTargetPlatformMinVersion=%s;SymbolsTestCppRootPath=%s\\\"",
				MSBuildBinPath LR"(MSBuild.exe)",
				SymbolBuildTestPath LR"(SymbolBuildTest.vcxproj)",
				szConfiguration,
				szPlatform,
				szWindowsTargetPlatformMinVersion,
				SymbolsTestCppRootPath.GetString());

			CString OutString;

			lStatus = RunCmd(nullptr, Cmd, &OutString);

			

			OutString.Insert(0, (wchar_t)0xfeff);


			auto BuildLog = SymbolsTestCppRootPath + L"Build.log";
			CreateFileByData(BuildLog, OutString.GetString(), OutString.GetLength() * sizeof(OutString[0]));

			Assert::AreEqual(lStatus, ERROR_SUCCESS, BuildLog);

			//把这些链接信息拉黑处理
			constexpr static LPCWSTR LinkWarnings[] =
			{
				L" LNK4286:", //一般是缺少 IAT符号所致
			};

			for (auto Warning : LinkWarnings)
			{
				Assert::AreEqual(OutString.Find(Warning), -1, CString(L"出现了") + Warning + L"\r\n" + BuildLog);
			}



			return lStatus;
		}

		static std::vector<CStringA> GetALlSymbols(LPCWSTR szLibFile)
		{
			std::vector<CStringA> Symbols;
			auto pData = ReadFileData(szLibFile);

			if (memcmp(pData, IMAGE_ARCHIVE_START, IMAGE_ARCHIVE_START_SIZE) != 0)
				return Symbols;

			auto pHeader = (SectionHeader*)(pData + IMAGE_ARCHIVE_START_SIZE);

			auto pFirstSec = (FirstSec*)(pHeader->get_Data());

			auto SymbolNum = pFirstSec->get_SymbolNum();
			auto SymbolOffset = pFirstSec->get_SymbolOffset();
			auto StrTable = pFirstSec->get_StrTable();


			for (int i = 0; i != SymbolNum; ++i)
			{
				CStringA Symbol = StrTable;
				StrTable += Symbol.GetLength() + 1;

				if (Symbol.GetLength() == 0)
					continue;

				//_NULL_THUNK_DATA 之类的全部跳过
				if (Symbol[0] == 127
					|| stricmp(Symbol, "__NULL_IMPORT_DESCRIPTOR") == 0
					|| strnicmp(Symbol, "__IMPORT_DESCRIPTOR_", 20) == 0)
				{
					continue;
				}
				
				//跳过所有 ECSYMBOLS
				if (strnicmp(Symbol, "__auximpcopy_", 13) == 0
					|| strnicmp(Symbol, "__imp_aux_", 10) == 0)
				{
					continue;
				}
				Symbols.push_back(Symbol);
			}

			return Symbols;
		}

		static byte* ReadFileData(LPCWSTR szFile)
		{
			auto hFile = CreateFileW(szFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, 0, nullptr);
			if (hFile == INVALID_HANDLE_VALUE)
				return nullptr;

			DWORD cbData = GetFileSize(hFile, nullptr);
			auto pData = (byte*)malloc(cbData);

			ReadFile(hFile, pData, cbData, &cbData, nullptr);

			return pData;
		}

		static CStringA BuildRefCPPCode(const std::vector<CStringA>& Symbols, WORD Machine, const std::set<CStringA>& ExclusionSymbols, bool IgnoreIAT)
		{
			CStringA CCode;

			CCode += "#pragma warning(disable : 4483)\r\n"
				     "namespace SymbolsTest {\r\n";



			for (auto& Symbol : Symbols)
			{
				//忽略所有 IAT
				if (IgnoreIAT && strncmp(Symbol, "__imp_", 5) == 0)
					continue;

				if (ExclusionSymbols.find(Symbol) != ExclusionSymbols.end())
					continue;

				//首先展开所有的声明
				//extern void* __identifier(_CRT_STRINGIZE_(NAME));

				CCode += "extern \"C\" extern void* __identifier(\"";

				auto szSymbol = Symbol.GetString();

				if (Machine == IMAGE_FILE_MACHINE_I386 && szSymbol[0] == '_')
				{
					++szSymbol;
				}
				
				CCode += szSymbol;

				CCode += "\");\r\n";
			}

			CCode += "\r\n\r\n"
				     "extern \"C\" void SymbolsTest()\r\n"
				     "{\r\n";

			for (auto& Symbol : Symbols)
			{
				//忽略所有 IAT
				if (IgnoreIAT && strncmp(Symbol, "__imp_", 5) == 0)
					continue;

				if (ExclusionSymbols.find(Symbol) != ExclusionSymbols.end())
					continue;
				
				//&__identifier(_CRT_STRINGIZE_(NAME));

				CCode += "&__identifier(\"";
				auto szSymbol = Symbol.GetString();

				if (Machine == IMAGE_FILE_MACHINE_I386 && szSymbol[0] == '_')
				{
					++szSymbol;
				}

				CCode += szSymbol;

				CCode += "\");\r\n";
			}

			CCode += "}"
			         "}";
			return CCode;
		}

		static LSTATUS CreateFileByData(LPCWSTR FilePath, const void* Data, DWORD ccbData)
		{
			if (Data == NULL)
				return ERROR_DATABASE_FULL;

			DWORD FileAttr = GetFileAttributes(FilePath);

			if (FileAttr != INVALID_FILE_ATTRIBUTES && (FileAttr & FILE_ATTRIBUTE_READONLY))
			{
				SetFileAttributes(FilePath, FileAttr & (-1 ^ FILE_ATTRIBUTE_READONLY));
			}

			LSTATUS lStatus = ERROR_SUCCESS;

			auto thFile = CreateFile(FilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, 0);

			if (thFile != INVALID_HANDLE_VALUE)
			{
				if (!WriteFile(thFile, Data, ccbData, &ccbData, NULL))
				{
					lStatus = GetLastError();
				}

				CloseHandle(thFile);
			}
			else
			{
				lStatus = GetLastError();
			}

			if (FileAttr != INVALID_FILE_ATTRIBUTES)
			{
				SetFileAttributes(FilePath, FileAttr);
			}


			return lStatus;
		}


		static LSTATUS RunCmd(LPCWSTR FilePath, CString CmdString, CString* pOutString)
		{
			SECURITY_ATTRIBUTES sa;
			HANDLE hRead, hWrite;

			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;
			if (!CreatePipe(&hRead, &hWrite, &sa, 0))
			{
				return GetLastError();
			}

			STARTUPINFO si = { sizeof(STARTUPINFO) };
			PROCESS_INFORMATION pi;

			GetStartupInfo(&si);
			si.hStdError = hWrite;
			si.hStdOutput = hWrite;
			si.wShowWindow = SW_HIDE;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			//关键步骤，CreateProcess函数参数意义请查阅MSDN
			//auto TT= EXECDOSCMD.GetBuffer();

			wchar_t SystempPath[MAX_PATH + 1];

			GetSystemDirectoryW(SystempPath, _countof(SystempPath));

			if (!CreateProcessW(FilePath, CmdString.GetBuffer(), NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT, NULL, SystempPath, &si, &pi))
			{
				return GetLastError();
			}

			CloseHandle(hWrite);

			DWORD bytesRead;

			CStringA OutString;
			//OutString.reserve(1024);

			//OutString.GetBuffer(1024);
			while (ReadFile(hRead, OutString.GetBuffer(OutString.GetLength() + 1024) + OutString.GetLength(), 1024, &bytesRead, NULL) && bytesRead)
			{
				OutString.ReleaseBufferSetLength(OutString.GetLength() + bytesRead);

				//OutString._Mylast() += bytesRead;
				//OutString.reserve(OutString.size() + 1024);


				//buffer中就是执行的结果，可以保存到文本，也可以直接输出
				//TRACE(buffer);
				//等待10毫秒

				Sleep(5);

			}

			CloseHandle(hRead);

			WaitForSingleObject(pi.hProcess, INFINITE);

			LSTATUS lStatus = ERROR_INVALID_FUNCTION;

			GetExitCodeProcess(pi.hProcess, (LPDWORD)&lStatus);


			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			//*OutString._Mylast() = NULL;
			if (pOutString)
			{
				*pOutString = OutString;
			}


			//EXECDOSCMD.
			return lStatus;
		}

		static std::set<CStringA> GetIgnoreSymbolsList(LPCWSTR DstLibName, LPCWSTR szPlatform, LPCWSTR szConfiguration, LPCWSTR szWindowsTargetPlatformMinVersion)
		{
			std::set<CStringA> ExclusionSymbols;

			CStringW InnoreListPath = VC_LTL_ROOT;
			InnoreListPath += L"Tools\\";
			InnoreListPath += szWindowsTargetPlatformMinVersion;
			InnoreListPath += L'\\';
			InnoreListPath += szPlatform;
			InnoreListPath += L'\\';
			InnoreListPath += DstLibName;
			InnoreListPath += L'.';
			InnoreListPath += szConfiguration;
			InnoreListPath += L".ignore.txt";

			if (PathFileExistsW(InnoreListPath))
			{
				if (FILE* file = _wfopen(InnoreListPath, L"r"))
				{
					char line[2048] = { 0 };

					while (fgets(line, _countof(line), file))
					{
						if (line[0] == '\0' || line[0] == ';')
							continue;
						CStringA Symbol = line;

						while (Symbol.GetLength() && Symbol[Symbol.GetLength() - 1] == '\n')
						{
							Symbol.ReleaseBufferSetLength(Symbol.GetLength() - 1);
						}

						if(Symbol.GetLength())
							ExclusionSymbols.insert(Symbol);
					}

					fclose(file);
				}
			}

			return ExclusionSymbols;
		}

	};
}
