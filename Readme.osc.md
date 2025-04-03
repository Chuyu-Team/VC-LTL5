# VC-LTL——编译精巧的程序原来如此简单
[![license](https://img.shields.io/github/license/Chuyu-Team/VC-LTL5)](https://github.com/Chuyu-Team/VC-LTL5/blob/master/LICENSE)
![downloads](https://img.shields.io/github/downloads/Chuyu-Team/VC-LTL5/total)
[![contributors](https://img.shields.io/github/contributors-anon/Chuyu-Team/VC-LTL5)](https://github.com/Chuyu-Team/VC-LTL5/graphs/contributors)
[![release](https://img.shields.io/github/v/release/Chuyu-Team/VC-LTL5?include_prereleases)](https://github.com/Chuyu-Team/VC-LTL5/releases)
[![nuget](https://img.shields.io/nuget/vpre/VC-LTL)](https://www.nuget.org/packages/VC-LTL)
[![crates.io](https://img.shields.io/crates/v/vc-ltl.svg)](https://crates.io/crates/vc-ltl)
[![Build&Test](https://github.com/Chuyu-Team/VC-LTL5/actions/workflows/Build&Test.yml/badge.svg)](https://github.com/Chuyu-Team/VC-LTL5/actions/workflows/Build&Test.yml)

- [English](Readme.md)

![Logo](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/Logo.png)<br/>
我愿化成一座做石桥，经受五百年的风吹，五百年的日晒，五百年的雨打，只求她从桥上走过！


## 1. 关于VC-LTL

VC-LTL是一个基于微软VC修改的开源运行时，有效减少应用程序体积并摆脱微软运行时DLL，比如msvcr120.dll、api-ms-win-crt-time-l1-1-0.dll等依赖。

VC-LTL最初是Dism++专用运行时。2017年3月6号从Dism++源代码中分离，并正式对外开源，为社区贡献自己的微薄之力。

在大型项目中往往有众多模块，如果都采用静态编译那么造成的空间浪费先不说，最后也会因为`Fls上限`导致程序无法正常运行。

而VC-LTL能让你的项目如同系统文件一样共享系统内置`msvcrt.dll`或者`ucrtbase.dll`，有效的解决`Fls上限`以及`运行时部署问题`，同时`大大缩减程序体积`，可以说一箭三雕！

所有人都可以无条件、免费使用，包括用于商业环境。当然如果大家在自己的程序说明文件中声明使用了VC-LTL那就更好了。

* QQ群: [633710173](https://shang.qq.com/wpa/qunwpa?idkey=21d51d8ad1d77b99ea9544b399e080ec347ca6a1bc04267fb59cebf22644a42a)

### 1.1. 原理
使用VC-LTL后可以将程序动态链接到系统自带的msvcrt.dll或者ucrtbase.dll中，来减少程序体积。目前使用CRT、STL以及MFC的工程一般都可以使用。

> 使用VC-LTL，C++程序体积大约缩减30%，而纯C程序则大约缩减50%。

### 1.2. 亮点
* 晚起的鸟儿也有虫虫吃，优雅的引用方式，仅添加一个属性表就能享受极致的体积体验。
* 无缝使用最新C/C++库以及最新编译器，尽情的使用最新规范。神马异常流防护（guard:cf）、静态对象线程安全初始化（threadSafeInit）……统统放马过来吧！！
* 拥有比微软原版更好的兼容性，即使想兼容Windows XP RTM也可以安心的对新编译器说“Yes”。
* 完全的开放代码，广泛的接受用户意见，希望大家能踊跃的 pull requests，为VC-LTL添砖加瓦。

> 让我们一起跟VS 2008说拜拜！


## 2. VC-LTL兼容性
此表展示了VC-LTL，C/C++库函数覆盖率，通过覆盖情况，可以大致了解VC-LTL的完善程度。

|  模块  | 5.1.2600.0/5.2.3790.0  | 6.0.6000.0 | 6.2.9200.0 | 10.0.10240.0 | 相关文件 
| :----: | :--------------------: | :--------: | :--------: | :----------: | --------
|  依赖  | msvcrt.dll             | msvcrt.dll | msvcrt.dll | ucrtbase.dll |
|  VCRT  | 100%                   | 100%       |   100%     |   100%       | vcruntime[d].lib、libvcruntime[d].lib
|  UCRT  | 97.458%                | 97.676%    |  97.676%   |   100%       | libucrt[d].lib、ucrt[d].lib
| WinRT  |   X                    |  X         |  99.609%   |   100%       | vccorlib.lib（仅支持Windows 8.1以及更高版本）
|   STL  | 100%                   | 100%       |   100%     |   100%       | Visual Studio自身提供。
| ConcRT | 100%                   | 100%       |   100%     |   100%       | Visual Studio自身提供。
|   ATL  | 100%                   | 100%       |   100%     |   100%       | Visual Studio自身提供。
|   MFC  | 100%                   | 100%       |   100%     |   100%       | Visual Studio自身提供。
|   AMP  | X                      | X          |   X        |   100%       | Visual Studio自身提供，需要带上`vcamp140.dll`。
| OpenMP | 100%                   | 100%       |   100%     |   100%       | Visual Studio自身提供，需要带上`vcomp140.dll`。



### 2.1. ABI兼容性
VC-LTL 5.0开始，ABI与微软原版`兼容`，您可以直接使用现有的静态库与VC-LTL进行链接，无需重新编译！

除此之外，为了提供行为的一致性还提供了Debug版库，它依然会使用msvcrt.dll或者ucrtbase.dll，但是它不支持堆调试功能，如果您需要在Debug使用堆调试功能请临时禁用VC-LTL。

### 2.2. 支持的Visual Studio版本
* Visual Studio 2015（包含Clang with Microsoft CodeGen、Clang 3.7 with Microsoft CodeGen、Clang-LLVM）
* Visual Studio 2017（包含Clang with Microsoft CodeGen、Clang-LLVM）
* Visual Studio 2019（包含Clang-LLVM）
* Visual Studio 2022

### 2.3. 支持的编译工具
|    编译工具    | 支持文件
| -------------- | --
| Visual Studio  | NuGet 或者 [VC-LTL helper for Visual Studio.props](#31-在visual-studio中使用vc-ltl)
| CMake          | [VC-LTL helper for cmake.cmake](#32-在cmake中使用vc-ltl)
| NMake、CL      | [VC-LTL helper for nmake.cmd/VC-LTL helper for nmake.ps1](#33-在nmake纯cl中使用vc-ltl)
| QMake          | VC-LTL helper for qmake.pri
| Rust           | crate

### 2.4. 支持的操作系统
|   操作系统                                          | x86 | x64 | arm | arm64 
|    ----                                             | --- | --- | --- | ----
| Windows XP、Windows Server 2003                     | √  | √  | -   | -
| Windows Vista、Windows Server 2008                  | √  | √  | -   | -
| Windows 7、Windows Server 2008 R2                   | √  | √  | -   | -
| Windows 8、Windows Server 2012、Windows RT          | √  | √  | √  | -
| Windows 8.1、Windows Server 2012 R2、Windows RT 8.1 | √  | √  | √  | -
| Windows 10、Windows Server 2016、Windows Server 2019| √  | √  | √  | √
| Windows 11                                          | √  | √  | √  | √

> 温馨提示：如果您需要编译Windows XP兼容的程序（5.1.2600.0/5.2.3790.0），那么您还需要使用[YY-Thunks](https://github.com/Chuyu-Team/YY-Thunks)库。

> 采用VC-LTL编译后的程序能兼容Windows XP RTM以上所有操作系统，无需安装任何SP补丁包。

## 3. 使用方法
下面我们将进入主题，我们给大家准备了丰富的[VC-LTL示例](https://github.com/Chuyu-Team/vc-ltl-samples)供大家参考，也欢迎加入我们的QQ群（633710173）。

### 3.1. Vistual Studio C++项目如何使用？
1. 项目右键 “管理 NuGet 程序包”。NuGet搜索框中输入：`VC-LTL`，搜索后点击安装。
2. C/C++ - 代码生成 -【运行库】调整为【多线程 (/MT)】
  - 如果需要支持XP，请项目右键 - 属性 - YY-Thunks - 最小兼容系统版本，设置为5.1.2600.0。
3. 重新编译代码

> 温馨提示：NuGet版VC-LTL会自动依赖YY-Thunks，您无需手动安装YY-Thunks。

![InstallByNuGet](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/zh-Hans/image/InstallByNuGet.png)

![ConfigurationProject](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/zh-Hans/image/ConfigurationProject.png)

![AppBuildByVC-LTL](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/AppWithLTL.png)

### 3.2. Vistual Studio .NET Native AOT项目如何使用？
1. 给`TargetFramework`添加`Windows`系统平台，比如修改为`net8.0-windows`或者`net9.0-windows`。
2. 项目右键 `管理 NuGet 程序包`。NuGet搜索框中输入：`VC-LTL`，搜索后点击安装。
3. 如果需要支持XP，请将项目属性`WindowsSupportedOSPlatformVersion`调整为`5.1`。大致如下：
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
4. 重新编译代码

> 温馨提示：NuGet版VC-LTL会自动依赖YY-Thunks，您无需手动安装YY-Thunks。

![InstallByNuGet](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/zh-Hans/image/InstallByNuGet.png)

![AppBuildByVC-LTL](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/AppWithLTL.png)

### 3.3. CMake项目如何使用？
1. 在CMake根目录创建`Directory.Build.props`，内容如下：
    ```xml
    <?xml version="1.0" encoding="utf-8"?>
    <Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
      <ItemGroup Condition="'$(MSBuildProjectExtension)' == '.vcxproj'">
        <ProjectCapability Include="PackageReferences" />
      </ItemGroup>
      <PropertyGroup Condition="'$(MSBuildProjectExtension)' == '.vcxproj'">
        <NuGetTargetMoniker Condition="'$(NuGetTargetMoniker)' == ''">native,Version=v0.0</NuGetTargetMoniker>
        <RuntimeIdentifiers Condition="'$(RuntimeIdentifiers)' == ''">win;win-x86;win-x64;win-arm;win-arm64</RuntimeIdentifiers>

        <!--将项目最小支持到Windows XP，可根据自己的情况设置-->
        <WindowsTargetPlatformMinVersion>5.1.2600</WindowsTargetPlatformMinVersion>
      </PropertyGroup>
      <ItemGroup Condition="'$(MSBuildProjectExtension)' == '.vcxproj'">
        <PackageReference Include="VC-LTL">
          <!--根据自己的情况选择VC-LTL版本-->
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
2. 将VS作为Gen启动编译过程，代码如下：
    ```
    # Gen必须选择Visual Studio系列，因为Visual Studio才支持nuget。
    # 假设输出目录为build\x86-Release，自己可根据情况修改。
    cmake -G "Visual Studio 17 2022" -A Win32 -DCMAKE_CONFIGURATION_TYPES:STRING=Release -DCMAKE_INSTALL_PREFIX:PATH=.\build\x86-Release .

    # 注意尾部的 `-- -r`，该命令是还原nuget包，此时会自动下载VC-LTL，并且配置到工程。
    cmake --build .\build\x86-Release --config Release -- -r

    cmake --install .\build\x86-Release --config Release
    ```
3. 重新编译代码

> 温馨提示：NuGet版VC-LTL会自动依赖YY-Thunks，您无需手动安装YY-Thunks。

![AppBuildByVC-LTL](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/AppWithLTL.png)

### 3.4. 我不喜欢NuGet，如何纯手工配置链接器参数？
#### 3.4.1. 纯手工配置Vistual Studio C++项目
1. 下载VC-LTL。假如，你将[VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest)下载并解压至`D:\Src\VC-LTL`（具体位置无任何要求），双击`D:\Src\VC-LTL\Install.cmd`即可。
  - 脚本会在`HKCU\Code\VC-LTL`创建注册表。
2. 添加VC-LTL配置文件。将属性表`VC-LTL helper for Visual Studio.props`复制到你的工程目录，你可以打开属性管理器（视图 - 属性管理器），然后Release配置上右键`添加现有属性表`，然后选择`VC-LTL helper for Visual Studio.props`即可。
3. 根据安装[YY-Thunks文档](https://github.com/Chuyu-Team/YY-Thunks/blob/master/Readme.osc.md)配置YY-Thunks。
4. C/C++ - 代码生成 -【运行库】调整为【多线程 (/MT)】
  - 如果需要支持XP，项目右键 - 属性 - YY-Thunks 中，自行调整YY-Thunks等级，允许 Windows 2000、Windows XP 以及 Windows Vista（默认）。
5. 重新编译代码

![AddShared](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/zh-Hans/image/AddShared.png)

![ConfigurationProject](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/zh-Hans/image/ConfigurationProject.png)

![AppBuildByVC-LTL](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/AppWithLTL.png)

#### 3.4.2. 纯手工配置CMake项目
1. 下载VC-LTL。假如，你将[VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest)下载并解压至`D:\Src\VC-LTL`（具体位置无任何要求），双击`D:\Src\VC-LTL\Install.cmd`即可。
  - 脚本会在`HKCU\Code\VC-LTL`创建注册表。
2. 添加VC-LTL配置文件。将模块文件`VC-LTL helper for cmake.cmake`复制到你的工程目录（顶层CMakeLists.txt同级目录）。然后在`CMakeLists.txt`中添加一行 `include("VC-LTL helper for cmake.cmake")` 即可。

    **示例：**
    ```
    cmake_minimum_required(VERSION 3.5.2)
    project(ltltest)

    include("VC-LTL helper for cmake.cmake")

    add_subdirectory(src)
    ```
3. 根据安装[YY-Thunks文档](https://github.com/Chuyu-Team/YY-Thunks/blob/master/Readme.osc.md)配置YY-Thunks。
4. 调整配置工程。建议使用`/MT`编译代码。
  - 如需支持XP，请修改`VC-LTL helper for cmake.cmake`启用 `set(WindowsTargetPlatformMinVersion "5.1.2600.0")`。
5. 重新编译代码

#### 3.4.3. 在NMake/纯CL中使用VC-LTL
1. 下载VC-LTL。假如，你将[VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest)下载并解压至`D:\Src\VC-LTL`（具体位置无任何要求），双击`D:\Src\VC-LTL\Install.cmd`即可。
  - 脚本会在`HKCU\Code\VC-LTL`创建注册表。
2. 根据安装[YY-Thunks文档](https://github.com/Chuyu-Team/YY-Thunks/blob/master/Readme.osc.md)配置YY-Thunks。
3. 运行VC-LTL辅助脚本。将辅助脚本`VC-LTL helper for nmake.cmd`复制到你的工程目录。启动`vcvars32.bat/vcvars64.bat`执行此脚本即可，脚本将自动修改`include`以及`lib`环境变量。

    **CMD 示例：**
    ```
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
    call "D:\VC-LTL\VC-LTL helper for nmake.cmd"

    nmake /f Test.mak
    ```

    **powershell 示例：**
    ```
    $BuiltInVsWhereExe = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $LatestVisualStudioRoot = & $BuiltInVsWhereExe -latest -prerelease -property installationPath

    # x86、amd64、arm、arm64
    & "$LatestVisualStudioRoot\Common7\Tools\Launch-VsDevShell.ps1" -arch x86
    & D:\VC-LTL\VC-LTL helper for nmake.ps1"

    & nmake /f Test.mak
    ```
4. 配置工程属性。建议使用`/MT`编译代码。
  - 如需支持XP，请修改`VC-LTL helper for nmake.cmd`启用 `set WindowsTargetPlatformMinVersion=5.1.2600.0`。
5. 重新编译代码

## 4. 常见问题
### 4.1. 未共享到msvcrt.dll/ucrtbase.dll
#### 问题原因
未正确引用VC-LTL。建议看看链接日志，是否链接了VC-LTL的lib。

#### 解决方案
1：请务必确保 `VC-LTL helper for Visual Studio.props` 已经添加到工程。

2：确保以下设置正确：
* VC++ 目录 - 包含目录 - 【√ 从父项或项目默认设置继承(I)】
* VC++ 目录 - 库目录 - 【√ 从父项或项目默认设置继承(I)】


## 5. 已知问题
* [Bug 22](https://github.com/Chuyu-Team/VC-LTL5/issues/22), VC-LTL的Debug模式不支持堆调试功能也无法使用ASAN
  - 如果您需要使用这些功能，可以针对Debug配置关闭VC-LTL。
* [Bug 60](https://github.com/Chuyu-Team/VC-LTL5/issues/60), msvcrt模式下locale相关函数（如setlocale、std::locale）无法指定为区域为 `.utf8`
  - 如果需求如此必须使用，建议使用VC-LTL的UCRT模式（使用后将依赖ucrtbase.dll），如果不想依赖DLL就直接静态链接微软原版CRT吧。


## 附：第三方依赖项许可
* [Visual Studio](https://visualstudio.microsoft.com/license-terms/)
* [Microsoft.Windows.SDK.CRTSource](https://www.nuget.org/packages/Microsoft.Windows.SDK.CRTSource/10.0.22621.3/License)
