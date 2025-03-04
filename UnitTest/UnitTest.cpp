#include "pch.h"
#include "CppUnitTest.h"
#include <winnt.h>

#pragma comment(lib, "ntdll.lib")

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


template<class IMAGE_THUNK_DATA_T>
static void BuildProcNames(PVOID pBase, PIMAGE_NT_HEADERS  pNtHeader, IMAGE_THUNK_DATA_T* pThunkData, std::vector<CStringA>& ProcNames)
{
	for (; pThunkData->u1.AddressOfData; ++pThunkData)
	{
		if (pThunkData->u1.AddressOfData >> ((sizeof(pThunkData->u1.AddressOfData) * 8) - 1))
		{
			//不支持序号，直接忽略即可
			//Ordinal.insert(pThunkData->u1.Ordinal & 0xffff);
		}
		else if (auto pImportByName = (PIMAGE_IMPORT_BY_NAME)YY::RtlImageRvaToVa(pNtHeader, pBase, pThunkData->u1.AddressOfData, NULL))
		{
			ProcNames.push_back(pImportByName->Name);
		}
		else
		{
			CStringW Error;
			Error.Format(L"Error：无法加载偏移 %I64X /?\n", (long long)pThunkData->u1.AddressOfData);

			Assert::Fail(Error);
		}
	}
}

#define IMAGE_FIRST_DIRECTORY(ntheader) (IMAGE_DATA_DIRECTORY*)((byte*)IMAGE_FIRST_SECTION(ntheader)-sizeof(IMAGE_DATA_DIRECTORY)*IMAGE_NUMBEROF_DIRECTORY_ENTRIES)

template<class IMAGE_THUNK_DATA_T>
static void BuildImport(PVOID pBase, PIMAGE_NT_HEADERS  pNtHeader, std::vector<DllImportInfo>& Infos)
{
	auto pDirectorys = IMAGE_FIRST_DIRECTORY(pNtHeader);

	auto& Imort = pDirectorys[IMAGE_DIRECTORY_ENTRY_IMPORT];

	auto pImport = (IMAGE_IMPORT_DESCRIPTOR*)YY::RtlImageRvaToVa(pNtHeader, pBase, Imort.VirtualAddress, NULL);

	if (!pImport)
	{
		Assert::AreNotEqual((void*)pImport, (void*)nullptr);
		return;
	}

	for (; pImport->Name; ++pImport)
	{
		auto DllName = (const char*)YY::RtlImageRvaToVa(pNtHeader, pBase, pImport->Name, NULL);

		Assert::AreNotEqual(DllName, nullptr);

		if (!DllName)
		{
			continue;
		}

		DllImportInfo Info;
		Info.DllName = DllName;

		auto pThunkData = (IMAGE_THUNK_DATA_T*)YY::RtlImageRvaToVa(pNtHeader, pBase, pImport->OriginalFirstThunk, NULL);

		Assert::AreNotEqual((void*)pThunkData, (void*)nullptr);

		if (!pThunkData)
		{
			//wprintf(L"Error：程序无法读取 dllname = %S OriginalFirstThunk Rva= 0x%.8X。\n", DllName, pImport->OriginalFirstThunk);

			continue;
		}


		BuildProcNames(pBase, pNtHeader, pThunkData, Info.ImportNames);

		Infos.push_back(std::move(Info));

	}
}

template<class IMAGE_THUNK_DATA_T>
static void BuildDelayImport(PVOID pBase, PIMAGE_NT_HEADERS  pNtHeader, std::vector<DllImportInfo>& Infos)
{
	auto pDirectorys = IMAGE_FIRST_DIRECTORY(pNtHeader);

	auto& Imort = pDirectorys[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];

	if (Imort.VirtualAddress == 0)
		return;

	auto pImport = (IMAGE_DELAYLOAD_DESCRIPTOR*)YY::RtlImageRvaToVa(pNtHeader, pBase, Imort.VirtualAddress, NULL);

	Assert::AreNotEqual((void*)pImport, (void*)nullptr);

	if (!pImport)
	{
		//wprintf(L"Warning：无法加载导入表。\n");
		return;
	}

	for (; pImport->DllNameRVA; ++pImport)
	{
		auto DllName = (const char*)YY::RtlImageRvaToVa(pNtHeader, pBase, pImport->DllNameRVA, NULL);

		Assert::AreNotEqual(DllName, nullptr);

		if (!DllName)
		{
			//wprintf(L"Error：程序无法读取 Rva= 0x%.8X。\n", pImport->DllNameRVA);

			continue;
		}

		DllImportInfo Info;
		Info.DllName = DllName;


		auto pThunkData = (IMAGE_THUNK_DATA_T*)YY::RtlImageRvaToVa(pNtHeader, pBase, pImport->ImportNameTableRVA, NULL);

		Assert::AreNotEqual((void*)pThunkData, (void*)nullptr);

		if (!pThunkData)
		{
			//wprintf(L"Error：程序无法读取 dllname = %S OriginalFirstThunk Rva= 0x%.8X。\n", DllName, pImport->ImportNameTableRVA);

			continue;
		}

		BuildProcNames(pBase, pNtHeader, pThunkData, Info.ImportNames);

		Infos.push_back(std::move(Info));

	}
}

std::vector<DllImportInfo> GetDllImportInfo(LPCWSTR szWin32PEFilePath)
{
	std::vector<DllImportInfo> Infos;

	PVOID pBase = nullptr;

	do
	{
		auto hFile = CreateFileW(szWin32PEFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, 0);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			break;
		}

		auto hMap = CreateFileMappingW(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hMap != NULL)
		{
			pBase = (PVOID)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);

			CloseHandle(hMap);
		}

		CloseHandle(hFile);

	} while (false);

	if (pBase == nullptr)
		return Infos;


	auto pNtHeader = YY::RtlImageNtHeader((PVOID)pBase);

	switch (pNtHeader->FileHeader.Machine)
	{
	case IMAGE_FILE_MACHINE_I386:
	case IMAGE_FILE_MACHINE_ARMNT:
		BuildImport<IMAGE_THUNK_DATA32>(pBase, pNtHeader, Infos);
		BuildDelayImport<IMAGE_THUNK_DATA32>(pBase, pNtHeader, Infos);
		break;
	case IMAGE_FILE_MACHINE_AMD64:
	case IMAGE_FILE_MACHINE_ARM64:
		BuildImport<IMAGE_THUNK_DATA64>(pBase, pNtHeader, Infos);
		BuildDelayImport<IMAGE_THUNK_DATA64>(pBase, pNtHeader, Infos);
		break;
	default:
		Assert::Fail();
		break;
	}
	if(pBase)
		UnmapViewOfFile(pBase);

	return Infos;
}


namespace UnitTest
{
	TEST_CLASS(符号保证)
	{
	public:
		
		TEST_METHOD(vcruntime_msvcrt模式所有符号可以被链接)
		{
			DllImportInfo Info;

			Info.DllName = L"msvcrt.dll";
			Info.ImportNames.push_back(L"__unDName");


			TestLib(
				VCToolsInstallDir LR"(lib\x86\vcruntime.lib)",
				L"vcruntime",
				L"Win32",
				{ L"Dynamic", L"Static" },
				{ L"5.1.2600.0", L"6.0.6000.0", L"6.2.9200.0" },
				Info
				);


			TestLib(
				VCToolsInstallDir LR"(lib\x64\vcruntime.lib)",
				L"vcruntime",
				L"x64",
				{ L"Dynamic", L"Static" },
				{ L"5.2.3790.0", L"6.0.6000.0", L"6.2.9200.0" },
				Info
				);

			
			//TestLib(
			//	VCToolsInstallDir LR"(lib\arm\vcruntime.lib)",
			//	L"vcruntime",
			//	L"arm",
			//	{ L"Dynamic", L"Static" },
			//	{ L"6.2.9200.0" },
			//	Info
			//	);
		}
		
		TEST_METHOD(vcruntime_msvcrt_Debug模式所有符号可以被链接)
		{
			DllImportInfo Info;

			Info.DllName = L"msvcrt.dll";
			Info.ImportNames.push_back(L"__unDName");


			TestLib(
				VCToolsInstallDir LR"(lib\x86\vcruntimed.lib)",
				L"vcruntime",
				L"Win32",
				{ L"DDynamic", L"DStatic" },
				{ L"5.1.2600.0", L"6.0.6000.0", L"6.2.9200.0" },
				Info
				);


			TestLib(
				VCToolsInstallDir LR"(lib\x64\vcruntimed.lib)",
				L"vcruntime",
				L"x64",
				{ L"DDynamic", L"DStatic" },
				{ L"5.2.3790.0", L"6.0.6000.0", L"6.2.9200.0" },
				Info
				);

			
			//TestLib(
			//	VCToolsInstallDir LR"(lib\arm\vcruntimed.lib)",
			//	L"vcruntime",
			//	L"arm",
			//	{ L"DDynamic", L"DStatic" },
			//	{ L"6.2.9200.0" },
			//	Info
			//	);
		}

		TEST_METHOD(vcruntime_ucrt模式所有符号可以被链接)
		{
			DllImportInfo Info;

			Info.DllName = L"ucrtbase.dll";
			Info.ImportNames.push_back(L"__unDName");

			TestLib(
				VCToolsInstallDir LR"(lib\x86\vcruntime.lib)",
				L"vcruntime",
				L"Win32",
				{ L"Dynamic", L"Static" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);

			TestLib(
				VCToolsInstallDir LR"(lib\x64\vcruntime.lib)",
				L"vcruntime",
				L"x64",
				{ L"Dynamic", L"Static" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);

			//TestLib(
			//	VCToolsInstallDir LR"(lib\arm\vcruntime.lib)",
			//	L"vcruntime",
			//	L"arm",
			//	{ L"Dynamic", L"Static" },
			//	{ L"10.0.10240.0", L"10.0.19041.0" },
			//	Info
			//	);

			TestLib(
				VCToolsInstallDir LR"(lib\arm64\vcruntime.lib)",
				L"vcruntime",
				L"arm64",
				{ L"Dynamic", L"Static" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);
		}

		TEST_METHOD(vcruntime_ucrt_debug所有符号可以被链接)
		{
			DllImportInfo Info;

			Info.DllName = L"ucrtbase.dll";
			Info.ImportNames.push_back(L"__unDName");


			TestLib(
				VCToolsInstallDir LR"(lib\x86\vcruntimed.lib)",
				L"vcruntime",
				L"Win32",
				{ L"DDynamic", L"DStatic" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);

			TestLib(
				VCToolsInstallDir LR"(lib\x64\vcruntimed.lib)",
				L"vcruntime",
				L"x64",
				{ L"DDynamic", L"DStatic" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);
			
			//TestLib(
			//	VCToolsInstallDir LR"(lib\arm\vcruntimed.lib)",
			//	L"vcruntime",
			//	L"arm",
			//	{ L"DDynamic", L"DStatic" },
			//	{ L"10.0.10240.0", L"10.0.19041.0" },
			//	Info
			//	);

			TestLib(
				VCToolsInstallDir LR"(lib\arm64\vcruntimed.lib)",
				L"vcruntime",
				L"arm64",
				{ L"DDynamic", L"DStatic" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);
		}

		TEST_METHOD(ucrt_msvcrt模式所有符号可以被链接)
		{
			DllImportInfo Info;

			Info.DllName = L"msvcrt.dll";
			Info.ImportNames.push_back(L"_beginthread");


			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x86\ucrt.lib)",
				L"ucrt",
				L"Win32",
				{ L"Dynamic", L"Static" },
				{ L"5.1.2600.0", L"6.0.6000.0", L"6.2.9200.0" },
				Info
				);


			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x64\ucrt.lib)",
				L"ucrt",
				L"x64",
				{ L"Dynamic", L"Static" },
				{ L"5.2.3790.0", L"6.0.6000.0", L"6.2.9200.0" },
				Info
				);


			//TestLib(
			//	CurrentUniversalCRTSdkDir LR"(ucrt\arm\ucrt.lib)",
			//	L"ucrt",
			//	L"arm",
			//	{ L"Dynamic", L"Static" },
			//	{ L"6.2.9200.0" },
			//	Info
			//	);
		}
		
		TEST_METHOD(ucrt_msvcrt_Debug模式所有符号可以被链接)
		{
			DllImportInfo Info;

			Info.DllName = L"msvcrt.dll";
			Info.ImportNames.push_back(L"_beginthread");

			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x86\ucrtd.lib)",
				L"ucrt",
				L"Win32",
				{ L"DDynamic", L"DStatic" },
				{ L"5.1.2600.0", L"6.0.6000.0", L"6.2.9200.0" },
				Info
				);


			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x64\ucrtd.lib)",
				L"ucrt",
				L"x64",
				{ L"DDynamic", L"DStatic" },
				{ L"5.2.3790.0", L"6.0.6000.0", L"6.2.9200.0" },
				Info
				);


			//TestLib(
			//	CurrentUniversalCRTSdkDir LR"(ucrt\arm\ucrtd.lib)",
			//	L"ucrt",
			//	L"arm",
			//	{ L"DDynamic", L"DStatic" },
			//	{ L"6.2.9200.0" },
			//	Info
			//	);
		}

		TEST_METHOD(ucrt_ucrt模式所有符号可以被链接)
		{
			DllImportInfo Info;

			Info.DllName = L"ucrtbase.dll";
			Info.ImportNames.push_back(L"_beginthread");

			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x86\ucrt.lib)",
				L"ucrt",
				L"Win32",
				{ L"Dynamic", L"Static" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);

			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x64\ucrt.lib)",
				L"ucrt",
				L"x64",
				{ L"Dynamic", L"Static" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);

			//TestLib(
			//	CurrentUniversalCRTSdkDir LR"(ucrt\arm\ucrt.lib)",
			//	L"ucrt",
			//	L"arm",
			//	{ L"Dynamic", L"Static" },
			//	{ L"10.0.10240.0", L"10.0.19041.0" },
			//	Info
			//	);

			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\arm64\ucrt.lib)",
				L"ucrt",
				L"arm64",
				{ L"Dynamic", L"Static" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);
		}

		TEST_METHOD(ucrt_ucrt_Debug模式所有符号可以被链接)
		{
			DllImportInfo Info;

			Info.DllName = L"ucrtbase.dll";
			Info.ImportNames.push_back(L"_beginthread");



			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x86\ucrtd.lib)",
				L"ucrt",
				L"Win32",
				{ L"DDynamic", L"DStatic" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);

			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\x64\ucrtd.lib)",
				L"ucrt",
				L"x64",
				{ L"DDynamic", L"DStatic" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);

			//TestLib(
			//	CurrentUniversalCRTSdkDir LR"(ucrt\arm\ucrtd.lib)",
			//	L"ucrt",
			//	L"arm",
			//	{ L"DDynamic", L"DStatic" },
			//	{ L"10.0.10240.0", L"10.0.19041.0" },
			//	Info
			//	);
			
			TestLib(
				CurrentUniversalCRTSdkDir LR"(ucrt\arm64\ucrtd.lib)",
				L"ucrt",
				L"arm64",
				{ L"DDynamic", L"DStatic" },
				{ L"10.0.10240.0", L"10.0.19041.0" },
				Info
				);
		}
	private:
		static LSTATUS TestLib(
			LPCWSTR SrcLibPath,
			LPCWSTR DstLibName,
			LPCWSTR szPlatform,
			std::initializer_list<LPCWSTR> szConfigurations,
			std::initializer_list<LPCWSTR> szWindowsTargetPlatformMinVersions,
			const DllImportInfo& Info,
			LPCWSTR BuildProperty = nullptr
			)
		{
			auto&& Symbols = GetALlSymbols(SrcLibPath);

			for (auto szConfiguration : szConfigurations)
			{
				for (auto szWindowsTargetPlatformMinVersion : szWindowsTargetPlatformMinVersions)
				{
					CStringW ErrorExt;
					ErrorExt.AppendFormat(L"DstLibName=%s\r\n", DstLibName);
					ErrorExt.AppendFormat(L"szPlatform=%s\r\n", szPlatform);
					ErrorExt.AppendFormat(L"szConfiguration=%s\r\n", szConfiguration);
					ErrorExt.AppendFormat(L"szWindowsTargetPlatformMinVersion=%s\r\n", szWindowsTargetPlatformMinVersion);
					ErrorExt.AppendFormat(L"BuildProperty=%s\r\n", BuildProperty);

					auto&& ExclusionSymbols = GetIgnoreSymbolsList(DstLibName, szPlatform, szConfiguration, szWindowsTargetPlatformMinVersion);
					std::vector<DllImportInfo> DllImportInfos;

					TestSymbols(Symbols, ExclusionSymbols, DstLibName, szPlatform, szConfiguration, szWindowsTargetPlatformMinVersion, DllImportInfos, BuildProperty);

					Assert::AreNotEqual(DllImportInfos.size(), size_t(0), ErrorExt.GetString());


					//关键的来自DLL的函数是否被导入
					const DllImportInfo* pInfo = FindDllImport(DllImportInfos, Info.DllName);

					Assert::IsNotNull(pInfo, ErrorExt);

					if (pInfo)
					{
						for (auto& FunName : Info.ImportNames)
						{
							bool bFound = false;

							for (auto& ImportName : pInfo->ImportNames)
							{
								if (ImportName == FunName)
								{
									bFound = true;
									break;
								}
							}


							if (!bFound)
							{
								Assert::Fail(CStringW(Info.DllName) + L"->" + CStringW(FunName) + L"   没有发现被导入！\r\n" + ErrorExt);
							}
						}
					}

				}
			}

			return ERROR_SUCCESS;
		}

		static LSTATUS TestSymbols(
			const std::vector<CStringA>& TestSymbols,
			const std::set<CStringA>& ExclusionSymbols,
			LPCWSTR DstLibName,
			LPCWSTR szPlatform,
			LPCWSTR szConfiguration,
			LPCWSTR szWindowsTargetPlatformMinVersion,
			std::vector<DllImportInfo>& Infos,
			LPCWSTR BuildProperty = nullptr
			)
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


			auto SymbolsTest = BuildRefCPPCode(TestSymbols, Machine, ExclusionSymbols, StrStrIW(szConfiguration, L"Static") != nullptr);


#if 0
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

			if (BuildProperty)
			{
				SymbolsTestCppRootPath += L'.';
				CString szBuildProperty = BuildProperty;

				szBuildProperty.Replace(L'\\', L'_');
				szBuildProperty.Replace(L'/', L'_');
				SymbolsTestCppRootPath += szBuildProperty;
			}

			SymbolsTestCppRootPath += L'\\';

			CreateDirectoryW(SymbolsTestCppRootPath, nullptr);

			;

			auto lStatus = CreateFileByData(SymbolsTestCppRootPath + L"SymbolsTest.h", SymbolsTest.GetString(), SymbolsTest.GetLength());

			Assert::AreEqual(lStatus, ERROR_SUCCESS, SymbolsTestCppRootPath + L"无法创建");


			CString Cmd;

			Cmd.Format(L"\"%s\" \"%s\" -t:Rebuild \"-p:Configuration=%s;Platform=%s;WindowsTargetPlatformMinVersion=%s;SymbolsTestCppRootPath=%s\\\"",
				MSBuildBinPath LR"(MSBuild.exe)",
				SymbolBuildTestPath LR"(SymbolBuildTest.vcxproj)",
				szConfiguration,
				szPlatform,
				szWindowsTargetPlatformMinVersion,
				SymbolsTestCppRootPath.GetString());

			if (BuildProperty)
			{
				Cmd.ReleaseBufferSetLength(Cmd.GetLength() - 1);
				Cmd += L';';
				Cmd += BuildProperty;
			}

			CString OutString;

			lStatus = RunCmd(nullptr, Cmd, &OutString);

			

			OutString.Insert(0, (wchar_t)0xfeff);


			auto BuildLog = SymbolsTestCppRootPath + L"Build.log";
			CreateFileByData(BuildLog, OutString.GetString(), OutString.GetLength() * sizeof(OutString[0]));

			Assert::AreEqual(lStatus, ERROR_SUCCESS, BuildLog);
#else
			CString SymbolsTestCppRootPath;

			auto OutString = RunMSBuildTest(SymbolsTest, DstLibName, szPlatform, szConfiguration, szWindowsTargetPlatformMinVersion, BuildProperty, &SymbolsTestCppRootPath);

#endif


			//把这些链接信息拉黑处理
			constexpr static LPCWSTR LinkWarnings[] =
			{
				L" LNK4286:", //一般是缺少 IAT符号所致
			};

			for (auto Warning : LinkWarnings)
			{
				Assert::AreEqual(OutString.Find(Warning), -1, CString(L"出现了") + Warning + L"\r\n" + SymbolsTestCppRootPath + L"Build.log");
			}

			Infos = std::move(GetDllImportInfo(SymbolsTestCppRootPath + L"SymbolBuildTest.exe"));
			
			return ERROR_SUCCESS;
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

				// 内部私有符号全部忽略
				if (Symbol.GetLength() > 2
					&& Symbol[0]== '?' && Symbol[1] == '?')
				{
					continue;
				}

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

                // xmm浮点数立即数
                if (strnicmp(Symbol, "__xmm@", 6) == 0)
                {
                    continue;
                }
				Symbols.push_back(Symbol);
			}

			return Symbols;
		}


		static CStringA BuildRefCPPCode(const std::vector<CStringA>& Symbols, WORD Machine, const std::set<CStringA>& ExclusionSymbols, bool IgnoreIAT)
		{
			CStringA CCode;

			//CCode += "#pragma warning(disable : 4483)\r\n"
			//	     "namespace SymbolsTest {\r\n";


			//_CrtCheckMemory

			for (auto& Symbol : Symbols)
			{
				//忽略所有 IAT
				if (IgnoreIAT && strncmp(Symbol, "__imp_", 5) == 0)
					continue;

				if (ExclusionSymbols.find(Symbol) != ExclusionSymbols.end())
					continue;

				CCode += "#pragma comment(linker, \"/include:" + Symbol + "\")\r\n";
				
			}

			return CCode;
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

			if (wcsicmp(szConfiguration, L"ddynamic") == 0)
				szConfiguration = L"dynamic";
			else if (wcsicmp(szConfiguration, L"dstatic") == 0)
				szConfiguration = L"static";

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
