# VC-LTL——编译精巧的程序原来如此简单
![license](https://img.shields.io/github/license/Chuyu-Team/VC-LTL5)
![downloads](https://img.shields.io/github/downloads/Chuyu-Team/VC-LTL5/total)
![contributors](https://img.shields.io/github/contributors-anon/Chuyu-Team/VC-LTL5)
![release](https://img.shields.io/github/v/release/Chuyu-Team/VC-LTL5?include_prereleases)
![nuget](https://img.shields.io/nuget/vpre/VC-LTL)
[![crates.io](https://img.shields.io/crates/v/vc-ltl.svg)](https://crates.io/crates/vc-ltl)
[![Build&Test](https://github.com/Chuyu-Team/VC-LTL5/actions/workflows/Build&Test.yml/badge.svg)](https://github.com/Chuyu-Team/VC-LTL5/actions/workflows/Build&Test.yml)

- [English](Readme.md)

> 这是VC-LTL 5.0版本，代码完全重构，仍然在试水中！如果是生产力环境中请使用[VC-LTL 4](https://github.com/Chuyu-Team/VC-LTL/releases/latest)。

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

### 2.2. 支持的IDE
* Visual Studio 2015（包含Clang with Microsoft CodeGen、Clang 3.7 with Microsoft CodeGen、Clang-LLVM）
* Visual Studio 2017（包含Clang with Microsoft CodeGen、Clang-LLVM）
* Visual Studio 2019（包含Clang-LLVM）
* Visual Studio 2022

### 2.3. 支持的编译工具
|    编译工具    | 支持文件
| -------------- | --
| Visual Studio  | [VC-LTL helper for Visual Studio.props](#31-在visual-studio中使用vc-ltl)
| CMake          | [VC-LTL helper for cmake.cmake](#32-在cmake中使用vc-ltl)
| NMake、CL      | [VC-LTL helper for nmake.cmd](#33-在nmake纯cl中使用vc-ltl)
| QMake          | VC-LTL helper for qmake.pri

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


### 3.1. 在Visual Studio中使用VC-LTL

#### 3.1.1. 引用VC-LTL

##### 3.1.1.1. 通过NuGet引用（推荐）

在 项目右键，选择“管理 NuGet 程序包”，然后搜索 `VC-LTL` 并选择适合您的版本，最后点击安装即可。

![InstallByNuGet](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/zh-Hans/image/InstallByNuGet.png)

##### 3.1.1.2. 通过注册表引用
假如，你将[VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest)下载并解压至`D:\Src\VC-LTL`（具体位置无任何要求），双击`D:\Src\VC-LTL\Install.cmd`即可。

> 脚本会在`HKCU\Code\VC-LTL`创建注册表。

将属性表`VC-LTL helper for Visual Studio.props`复制到你的工程目录，你可以打开属性管理器（视图 - 属性管理器），然后Release配置上右键`添加现有属性表`，然后选择`VC-LTL helper for Visual Studio.props`即可。

![AddShared](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/zh-Hans/image/AddShared.png)

#### 3.1.2. 配置工程属性
* C/C++ - 代码生成 -【运行库】调整为【多线程 (/MT)】

![ConfigurationProject](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/zh-Hans/image/ConfigurationProject.png)

> 建议使用`/MT`编译代码。如需支持XP，请右键项目 - 属性 - NuGet程序包设置 - VC-LTL - 目标CRT版本 －`msvcrt 5.1.2600.0`，除此之外建议您安装YY-Thunks。

### 3.2. 在CMake中使用VC-LTL

假如，你将[VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest)下载并解压至`D:\Src\VC-LTL`（具体位置无任何要求），双击`D:\Src\VC-LTL\Install.cmd`即可。

> 脚本会在`HKCU\Code\VC-LTL`创建注册表。

#### 3.2.1. 添加VC-LTL配置文件

将模块文件`VC-LTL helper for cmake.cmake`复制到你的工程目录（顶层CMakeLists.txt同级目录）。然后在`CMakeLists.txt`中添加一行 `include("VC-LTL helper for cmake.cmake")` 即可。

**示例：**
```
cmake_minimum_required(VERSION 3.5.2)
project(ltltest)

include("VC-LTL helper for cmake.cmake")

add_subdirectory(src)
```

#### 3.2.2. 调整配置工程

> 建议使用`/MT`编译代码。如需支持XP，请修改`VC-LTL helper for cmake.cmake`启用 `set(WindowsTargetPlatformMinVersion "5.1.2600.0")`，除此之外建议您安装YY-Thunks。

### 3.3. 在NMake/纯CL中使用VC-LTL

假如，你将[VC-LTL Binary](https://github.com/Chuyu-Team/VC-LTL5/releases/latest)下载并解压至`D:\Src\VC-LTL`（具体位置无任何要求），双击`D:\Src\VC-LTL\Install.cmd`即可。

> 脚本会在`HKCU\Code\VC-LTL`创建注册表。

#### 3.3.1. 运行VC-LTL辅助脚本

将辅助脚本`VC-LTL helper for nmake.cmd`复制到你的工程目录。启动`vcvars32.bat/vcvars64.bat`执行此脚本即可，脚本将自动修改`include`以及`lib`环境变量。

**示例：**
```
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
call "D:\VC-LTL\VC-LTL helper for nmake.cmd"

nmake /f Test.mak
```
#### 3.3.2. 配置工程属性

> 建议使用`/MT`编译代码。如需支持XP，请修改`VC-LTL helper for nmake.cmd`启用 `set WindowsTargetPlatformMinVersion=5.1.2600.0`，除此之外建议您安装YY-Thunks。


### 3.4. 重新编译
现在会引用msvcrt.dll或者ucrtbase.dll，体积小了很多。如果你编译不通过，可以先参考 [4. 常见问题](#4-常见问题)。如果还是不通过可以反馈，共同改进VC-LTL。

> 使用VC-LTL 5.0编译时推荐采用`/MT`编译，依赖的静态库不需要重新编译。

![AppBuildByVC-LTL](https://raw.githubusercontent.com/wiki/Chuyu-Team/VC-LTL/image/AppWithLTL.png)

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
* VC-LTL的Debug模式不支持堆调试功能，如果您需要使用这些功能，可以针对Debug配置关闭VC-LTL。


## 附：第三方依赖项许可
* [Visual Studio](https://github.com/Chuyu-Team/VC-LTL5/tree/master/ThirdPartyLicenses/Visual%20Studio)
* [Windows 10 SDK](https://github.com/Chuyu-Team/VC-LTL5/tree/master/ThirdPartyLicenses/Windows%2010%20SDK)
