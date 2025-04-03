# VC-LTL - An elegant way to compile lighter binaries.
[![license](https://img.shields.io/github/license/Chuyu-Team/VC-LTL5)](https://github.com/Chuyu-Team/VC-LTL5/blob/master/LICENSE)
![downloads](https://img.shields.io/github/downloads/Chuyu-Team/VC-LTL5/total)
[![contributors](https://img.shields.io/github/contributors-anon/Chuyu-Team/VC-LTL5)](https://github.com/Chuyu-Team/VC-LTL5/graphs/contributors)
[![release](https://img.shields.io/github/v/release/Chuyu-Team/VC-LTL5?include_prereleases)](https://github.com/Chuyu-Team/VC-LTL5/releases)
[![nuget](https://img.shields.io/nuget/vpre/VC-LTL)](https://www.nuget.org/packages/VC-LTL)
[![crates.io](https://img.shields.io/crates/v/vc-ltl.svg)](https://crates.io/crates/vc-ltl)
[![Build&Test](https://github.com/Chuyu-Team/VC-LTL5/actions/workflows/Build&Test.yml/badge.svg)](https://github.com/Chuyu-Team/VC-LTL5/actions/workflows/Build&Test.yml)

- [简体中文](Readme.osc.md)

![Logo](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/Logo.png)<br/>
I would like to turn into a stone bridge, go through 500 years of wind, 500 years of Sun, rain for 500 years, but she walked off a bridge!

## 1. About VC-LTL
VC-LTL is an open source CRT library based on the MS VCRT that reduce program binary size and say goodbye to Microsoft runtime DLLs, such as msvcr120.dll, api-ms-win-crt-time-l1-1-0.dll and other dependencies.

VC-LTL was originally a dedicated Dism++ runtime and was isolated and open sourced from Dism++ source code on Mar 6, 2017.

There are plenty of modules in large projects. If all binaries uses static 
compilation, it will use plenty of disk space and finally may crashed because 
of the fiber local storage (FLS) limitation.

But the VC-LTL can make your project using the built-in `msvcrt.dll` in the 
Windows. It solves the C runtime deployment problem and the fiber local storage
(FLS) limitation problem effectively, and greatly reduce the binaries size. 
What a handy library!

Everyone can use it for free, even for the commerical use. Of course, I hope 
that if you mentioned the VC-LTL in your project, because I want to help more
people.

* QQ Group: [633710173](https://shang.qq.com/wpa/qunwpa?idkey=21d51d8ad1d77b99ea9544b399e080ec347ca6a1bc04267fb59cebf22644a42a)

### 1.1. The principle of VC-LTL
After using the VC-LTL, the binaries will be dynamically linked to the built-in
`msvcrt.dll` or `ucrtbase.dll` in the Windows to reduce the binaries size. It is generally 
available if you use the VC-LTL in the project using the C Runtime, STL or MFC.

> After using the VC-LTL, the size of the binaries will reduce about 30%
if compiled from the C++ source code, and about 50% if compiled 
from the pure C source code.

### 1.2. Highlight
* 晚起的鸟儿也有虫虫吃，优雅的引用方式，仅添加一个属性表就能享受极致的体积体验。
* 无缝使用最新C/C++库以及最新编译器，尽情的使用最新规范。神马异常流防护（guard:cf）、静态对象线程安全初始化（threadSafeInit）……统统放马过来吧！！
* 拥有比微软原版更好的兼容性，即使想兼容Windows XP RTM也可以安心的对新编译器说“Yes”。
* 完全的开放代码，广泛的接受用户意见，希望大家能踊跃的 pull requests，为VC-LTL添砖加瓦。

> Let's say goodbye to the Visual Studio 2008.

## 2. VC-LTL Compatibility

|  Module  | 5.1.2600.0/5.2.3790.0  | 6.0.6000.0 | 6.2.9200.0 | 10.0.10240.0 | Files 
| :----: | :--------------------: | :--------: | :--------: | :----------: | --------
|depends | msvcrt.dll             | msvcrt.dll | msvcrt.dll | ucrtbase.dll |
|  VCRT  | 100%                   | 100%       |   100%     |   100%       | vcruntime[d].lib、libvcruntime[d].lib
|  UCRT  | 97.458%                | 97.676%    |  97.676%   |   100%       | libucrt[d].lib、ucrt[d].lib
| WinRT  |   X                    |  X         |  99.609%   |   100%       | vccorlib.lib (Windows 8.1 or later)
|   STL  | 100%                   | 100%       |   100%     |   100%       | 
| ConcRT | 100%                   | 100%       |   100%     |   100%       | 
|   ATL  | 100%                   | 100%       |   100%     |   100%       | 
|   MFC  | 100%                   | 100%       |   100%     |   100%       | 
|   AMP  | X                      | X          |   X        |   100%       | Requires `vcamp140.dll` from the Visual C++ Runtime
| OpenMP | 100%                   | 100%       |   100%     |   100%       | Requires `vcomp140.dll` from the Visual C++ Runtime


### 2.1. ABI Compatibility
Starting from VC-LTL 5.0, ABI is `compatible` with Microsoft. You can link use the static library with VC-LTL without recompiling!

In addition, in order to provide consistency in behavior, a Debug version of the library is also provided.
It will still use msvcrt.dll or ucrtbase.dll, but it does not support the heap debugging functions.
If you need to use the heap debugging, please disable VC-LTL.

### 2.2. Supported Visual Studio Versions
* Visual Studio 2015 (Support Clang with Microsoft CodeGen, Clang 3.7 with Microsoft CodeGen, Clang-LLVM)
* Visual Studio 2017 (Support Clang with Microsoft CodeGen, Clang-LLVM)
* Visual Studio 2019 (Support Clang-LLVM)
* Visual Studio 2022

### 2.3. Supported Build Tools
|   Build Tool   | Helper      
| -------------- | --
| Visual Studio  | NuGet or [VC-LTL helper for Visual Studio.props](#31-using-vc-ltl-in-visual-studio)
| CMake          | [VC-LTL helper for cmake.cmake](#32-using-vc-ltl-in-cmake)
| NMake, CL      | [VC-LTL helper for nmake.cmd/VC-LTL helper for nmake.ps1](#33-using-vc-ltl-in-nmakecl)
| QMake          | VC-LTL helper for qmake.pri
| Rust           | crate

### 2.4. Supported Windows Versions
|         OS                                          | x86 | x64 | arm | arm64 
|        ----                                         | --- | --- | --- | ----
| Windows XP, Windows Server 2003                     | √  | √  | -   | -
| Windows Vista, Windows Server 2008                  | √  | √  | -   | -
| Windows 7, Windows Server 2008 R2                   | √  | √  | -   | -
| Windows 8, Windows Server 2012, Windows RT          | √  | √  | √  | -
| Windows 8.1, Windows Server 2012 R2, Windows RT 8.1 | √  | √  | √  | -
| Windows 10, Windows Server 2016, Windows Server 2019| √  | √  | √  | √
| Windows 11                                          | √  | √  | √  | √

> Tips: If you need to compile Windows XP compatible app (5.1.2600.0/5.2.3790.0), please don't forget to used [YY-Thunks](https://github.com/Chuyu-Team/YY-Thunks).

> The binaries compiled with the VC-LTL is compatible with Windows XP and 
later, even in the environment which installed no hotfixes.

## 3. How to used?
We will enter the theme, we have prepared a rich [VC-LTL Samples](https://github.com/Chuyu-Team/vc-ltl-samples) for your reference, and welcome to join our QQ group (633710173).

### 3.1. Using VC-LTL in Visual Studio C++ Project
1. Right-click on the project and select "Manage NuGet Packages". Search for `VC-LTL` and choose the version that suits you, and finally click Install.
2. C/C++ - Code Generation -`Runtime Library` adjust to `Multi-threaded (/MT)`
  - For XP support, Right click on the project, Properties - NuGet Packages Settings - YY-Thunks - 最小兼容系统版本 - 5.1.2600.0.
3. Rebuild the project

![InstallByNuGet](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/en/image/InstallByNuGet.png)

![ConfigurationProject](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/en/image/ConfigurationProject.png)

![AppBuildByVC-LTL](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/AppWithLTL.png)

### 3.2. Using VC-LTL in Visual Studio .NET Native AOT Project
1. `TargetFramework` property to add `Windows` system platform, e.g. `net8.0-windows` or `net9.0-windows`.
2. Right-click on the project and select "Manage NuGet Packages". Search for `VC-LTL` and choose the version that suits you, and finally click Install.
3. For XP support, please add project `WindowsSupportedOSPlatformVersion` = `5.1`, for example:
    ```xml
    <Project Sdk="Microsoft.NET.Sdk">
        <PropertyGroup>
            <!-- ... -->
            <TargetFramework>net8.0-windows</TargetFramework>
            <SupportedOSPlatformVersion>5.1</SupportedOSPlatformVersion>
            <!-- ... -->
        </PropertyGroup>
      <!--...-->
    </Project>
    ```
4. Rebuild the project

![InstallByNuGet](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/en/image/InstallByNuGet.png)

![AppBuildByVC-LTL](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/AppWithLTL.png)

### 3.3. Using VC-LTL in CMake
1. Create file `Directory.Build.props` in source directory, and add the following code:
    ```xml
    <?xml version="1.0" encoding="utf-8"?>
    <Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
      <ItemGroup Condition="'$(MSBuildProjectExtension)' == '.vcxproj'">
        <ProjectCapability Include="PackageReferences" />
      </ItemGroup>
      <PropertyGroup Condition="'$(MSBuildProjectExtension)' == '.vcxproj'">
        <NuGetTargetMoniker Condition="'$(NuGetTargetMoniker)' == ''">native,Version=v0.0</NuGetTargetMoniker>
        <RuntimeIdentifiers Condition="'$(RuntimeIdentifiers)' == ''">win;win-x86;win-x64;win-arm;win-arm64</RuntimeIdentifiers>

        <!--Turn on Windows XP support, and you can choose according to your situation.-->
        <WindowsTargetPlatformMinVersion>5.1.2600</WindowsTargetPlatformMinVersion>
      </PropertyGroup>
      <ItemGroup Condition="'$(MSBuildProjectExtension)' == '.vcxproj'">
        <PackageReference Include="VC-LTL">
          <!--VC-LTL Version-->
          <Version>5.1.1</Version>
        </PackageReference>      
      </ItemGroup>
      <!--从兼容性考虑，继续向上搜索 Directory.Build.props-->
      <PropertyGroup>
        <DirectoryBuildPropsPath>$([MSBuild]::GetPathOfFileAbove('Directory.Build.props', '$(MSBuildThisFileDirectory)../'))</DirectoryBuildPropsPath>
      </PropertyGroup>
      <Import Project="$(DirectoryBuildPropsPath)" Condition="'$(DirectoryBuildPropsPath)' != ''"/>
    </Project>
    ```
2. Start Build, for example:
    ```
    # The Gen parameter must be use `Visual Studio`, as Visual Studio only supports nuget.
    # Assuming that the output dir is `.build\x86-Release`, you can modify it as needed.
    cmake -G "Visual Studio 17 2022" -A Win32 -DCMAKE_CONFIGURATION_TYPES:STRING=Release -DCMAKE_INSTALL_PREFIX:PATH=.\build\x86-Release .

    # Note the `-- -r` at the end, which is the command to restore the nuget package.
    cmake --build .\build\x86-Release --config Release -- -r

    cmake --install .\build\x86-Release --config Release
    ```
3. Rebuild the project

### 3.4. I don't want to use NuGet, how do I configure the project manually?
#### 3.4.1. Using VC-LTL in Visual Studio C++ Project
1. Download VC-LTL. If you download and unzip [VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest) to `D:\Src\VC-LTL`, please double-click `D:\Src\VC-LTL\Install.cmd`.
  - The script will save the information in the registry `HKCU\Code\VC-LTL`.
2. Add VC-LTL Module File. Copy `VC-LTL helper for Visual Studio.props` to your project, then open the Property Manager ( View - Property Manager ) and right-click on the Release Configuration, click on `Add Existing Property Sheet...`, and finally select `VC-LTL helper for Visual Studio.props`.
3. Configure YY-Thunks according to the installation [YY-Thunks documentation](https://github.com/Chuyu-Team/YY-Thunks/blob/master/Readme.md).
4. C/C++ - Code Generation -`Runtime Library` adjust to `Multi-threaded (/MT)`
  - For XP support, please Right click on the project, Properties - NuGet Packages Settings - YY-Thunks - 最小兼容系统版本 - 5.1.2600.0.
5. Rebuild the project

![AddShared](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/en/image/AddShared.png)

#### 3.4.2. Using VC-LTL in CMake
1. Download VC-LTL. If you download and unzip [VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest) to `D:\Src\VC-LTL`, please double-click `D:\Src\VC-LTL\Install.cmd`.
  - The script will save the information in the registry `HKCU\Code\VC-LTL`.
2. Add VC-LTL Module File. Copy `VC-LTL helper for cmake.cmake` to your project. Then add `include("VC-LTL helper for cmake.cmake")` to `CMakeLists.txt`.

    **Example:**
    ```
    cmake_minimum_required(VERSION 3.5.2)
    project(ltltest)

    include("VC-LTL helper for cmake.cmake")

    add_subdirectory(src)
    ```
3. Configure YY-Thunks according to the installation [YY-Thunks documentation](https://github.com/Chuyu-Team/YY-Thunks/blob/master/Readme.md).
4. Recommended to use `/MT` to compile the project when using VC-LTL.
  - For XP support, please modify `VC-LTL helper for cmake.cmake` to enable `set(WindowsTargetPlatformMinVersion "5.1.2600.0")`. 
5. Rebuild the project

#### 3.4.3. Using VC-LTL in NMake/CL
1. Download VC-LTL. If you download and unzip [VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest) to `D:\Src\VC-LTL`, please double-click `D:\Src\VC-LTL\Install.cmd`.
  - The script will save the information in the registry `HKCU\Code\VC-LTL`.
2. Configure YY-Thunks according to the installation [YY-Thunks documentation](https://github.com/Chuyu-Team/YY-Thunks/blob/master/Readme.md).
3. Copy `VC-LTL helper for nmake.cmd` to your project. Run `vcvars32.bat` or `vcvars64.bat` and execute this script. The script will automatically modify the `include` and `lib` environment variables.

    **Example: cmd**
    ```
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
    call "D:\VC-LTL\VC-LTL helper for nmake.cmd"

    nmake /f Test.mak
    ```

    **Example: powershell**
    ```
    $BuiltInVsWhereExe = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $LatestVisualStudioRoot = & $BuiltInVsWhereExe -latest -prerelease -property installationPath

    # x86、amd64、arm、arm64
    & "$LatestVisualStudioRoot\Common7\Tools\Launch-VsDevShell.ps1" -arch x86
    & D:\VC-LTL\VC-LTL helper for nmake.ps1"

    & nmake /f Test.mak
    ```
4. Recommended to use `/MT` to compile the project when using VC-LTL.
  - For XP support, please modify `VC-LTL helper for nmake.cmd` to enable `set WindowsTargetPlatformMinVersion=5.1.2600.0`.
5. Rebuild the project

## 4. FAQ
### 4.1. 未共享到msvcrt.dll/ucrtbase.dll
#### Cause
未正确引用VC-LTL。建议看看链接日志，是否链接了VC-LTL的lib。

#### Workaround
1：请务必确保 `VC-LTL helper for Visual Studio.props` 已经添加到工程。

2：确保以下设置正确：
* VC++ 目录 - 包含目录 - 【√ 从父项或项目默认设置继承(I)】
* VC++ 目录 - 库目录 - 【√ 从父项或项目默认设置继承(I)】

## 5. Known Issues
* [Bug 22](https://github.com/Chuyu-Team/VC-LTL5/issues/22), VC-LTL的Debug模式不支持堆调试功能也无法使用ASAN
  - 如果您需要使用这些功能，可以针对Debug配置关闭VC-LTL。
* [Bug 60](https://github.com/Chuyu-Team/VC-LTL5/issues/60), msvcrt模式下locale相关函数（如setlocale、std::locale）无法指定为区域为 `.utf8`
  - 如果需求如此必须使用，建议使用VC-LTL的UCRT模式（使用后将依赖ucrtbase.dll），如果不想依赖DLL就直接静态链接微软原版CRT吧。

## Excursus - Third Party Licenses
* [Visual Studio](https://visualstudio.microsoft.com/license-terms/)
* [Microsoft.Windows.SDK.CRTSource](https://www.nuget.org/packages/Microsoft.Windows.SDK.CRTSource/10.0.22621.3/License)
