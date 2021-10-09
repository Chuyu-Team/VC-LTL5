// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
LSTATUS RunCmd(LPCWSTR FilePath, CString CmdString, CString* pOutString)
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



LSTATUS CreateFileByData(LPCWSTR FilePath, const void* Data, DWORD ccbData)
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


byte* ReadFileData(LPCWSTR szFile)
{
	auto hFile = CreateFileW(szFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return nullptr;

	DWORD cbData = GetFileSize(hFile, nullptr);
	auto pData = (byte*)malloc(cbData);

	ReadFile(hFile, pData, cbData, &cbData, nullptr);

	return pData;
}

CString RunMSBuildTest(
	const void* FileData,
	DWORD cbFileData,
	LPCWSTR DetectName,
	LPCWSTR szPlatform,
	LPCWSTR szConfiguration,
	LPCWSTR szWindowsTargetPlatformMinVersion,
	LPCWSTR BuildProperty/* = nullptr*/,
	CStringW* pTempRoot/* = nullptr*/
	)
{
	CString SymbolsTestCppRootPath = SymbolBuildTestPath;
	SymbolsTestCppRootPath += L".test\\";

	CreateDirectoryW(SymbolsTestCppRootPath, nullptr);

	SymbolsTestCppRootPath += DetectName;
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


	if (pTempRoot)
		*pTempRoot = SymbolsTestCppRootPath;


	CreateDirectoryW(SymbolsTestCppRootPath, nullptr);

	auto lStatus = CreateFileByData(SymbolsTestCppRootPath + L"SymbolsTest.h", FileData, cbFileData);

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

	return OutString;

}

CString RunMSBuildTest(
	CStringA FileData,
	LPCWSTR DetectName,
	LPCWSTR szPlatform,
	LPCWSTR szConfiguration,
	LPCWSTR szWindowsTargetPlatformMinVersion,
	LPCWSTR BuildProperty/* = nullptr*/,
	CStringW* pTempRoot/* = nullptr*/
	)
{
	return RunMSBuildTest(FileData.GetString(), FileData.GetLength(), DetectName, szPlatform, szConfiguration, szWindowsTargetPlatformMinVersion, BuildProperty, pTempRoot);
}