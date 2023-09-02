#
#  VC-LTL自动化加载配置，建议你将此文件单独复制到你的工程再使用，该文件能自动识别当前环境是否存在VC-LTL，并且自动应用。
#
#  使用方法：
#    方法一：启动Developer PowerShell for VS 20XX然后执行此脚本(或使用Import-Module导入这个脚本)，运行完成后将自动修改include以及lib环境变量，方便nmake以及纯cl用户引用VC-LTL。
#    方法二：1.启动Powershell
#           2.安装VSSetup模块:Install-Module VSSetup -Scope CurrentUser (如果已安装。则跳过)
#           3.获取InstanceId和安装路径：Get-VSSetupInstance
#             输出（因人而异）：
#                InstanceId          : a2a36881
#                DisplayName         : Visual Studio Enterprise 2022
#                InstallationVersion : 17.7.34024.191
#                InstallationPath    : C:\Program Files\Microsoft Visual Studio\2022\Enterprise
#                InstallDate         : 2022/12/29 12:33:15
################################################################################################################################
#             $InstanceId=(Get-VSSetupInstance)[0].InstanceId
#             $InstallationPath=(Get-VSSetupInstance)[0].InstallationPath
#           4.加载模块：Import-Module "$InstallationPath\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
#           5.进入开发环境：X64位：Enter-VsDevShell $InstanceId -DevCmdArguments '-arch=x64 -host_arch=x64 -winsdk=10.0.22621.0 -no_logo'
#                        ARM64位：Enter-VsDevShell $InstanceId -DevCmdArguments '-arch=arm64 -host_arch=x64 -winsdk=10.0.22621.0 -no_logo'
#                       X86_32位：Enter-VsDevShell $InstanceId -DevCmdArguments '-arch=x86 -host_arch=x64 -winsdk=10.0.22621.0 -no_logo'
#           6.执行此脚本(或使用Import-Module导入这个脚本)，运行完成后将自动修改include以及lib环境变量，方便nmake以及纯cl用户引用VC-LTL。
#
#
#  VC-LTL默认搜索顺序
#	  1. “VC-LTL helper for nmake.ps1”所在根目录，即 $PSScriptRoot
#	  2. “VC-LTL helper for nmake.ps1”所在根目录下的VC-LTL目录，即 ${PSScriptRoot}\VC-LTL
#	  3. “VC-LTL helper for nmake.ps1”所在父目录，即 ${PSScriptRoot}\..
#	  3. “VC-LTL helper for nmake.ps1”所在父目录下的VC-LTL目录，即 ${PSScriptRoot}\..\VC-LTL
#     4. 当前目录，即 (Get-Location).Path
#     5. 当前目录下的VC-LTL目录，即 ${(Get-Location).Path}\VC-LTL
#     6. 当前目录的父目录下的VC-LTL目录，即 ${(Get-Location).Path}\..\VC-LTL
#     7. 注册表HKEY_CURRENT_USER\Code\VC-LTL@Root
#
#   把VC-LTL放在其中一个位置即可，VC-LTL就能被自动引用。
#
#   如果你对默认搜索顺序不满，你可以修改此文件。你也可以直接指定 $env:VC_LTL_Root 变量更加任性的去加载VC-LTL。
#
###########################################################VC-LTL设置###########################################################

# 控制TargetPlatform版本，目前可用版本为 5.1.2600.0    6.0.6000.0（默认）    6.2.9200.0     10.0.10240.0    10.0.19041.0
# $env:WindowsTargetPlatformMinVersion="10.0.10240.0"

# 启用干净的导入表，消除 ucrt apiset(如：api-ms-win-crt-time-l1-1-0.dll)，满足强迫症患者。
# $env:CleanImport="true"

################################################################################################################################

do
{
    if ($env:VC_LTL_Root.Length -ne 0)
    {
        break
    }
        
    #脚本文件根目录存在VC-LTL？
    if(Test-Path "${PSScriptRoot}\_msvcrt.h" -PathType Leaf)
    {
        $env:VC_LTL_Root=$PSScriptRoot
        break
    }

    #脚本文件根目录下存在VC-LTL？
    if(Test-Path "${PSScriptRoot}\VC-LTL\_msvcrt.h" -PathType Leaf)
    {
        $env:VC_LTL_Root="${PSScriptRoot}\VC-LTL"
        break
    }

    #脚本文件父目录存在VC-LTL？
    if(Test-Path "${PSScriptRoot}\..\_msvcrt.h" -PathType Leaf)
    {
        $env:VC_LTL_Root="${PSScriptRoot}\.."
        break
    }

    #脚本文件父目录存在VC-LTL？
    if(Test-Path "${PSScriptRoot}\..\VC-LTL\_msvcrt.h" -PathType Leaf)
    {
        $env:VC_LTL_Root="${PSScriptRoot}\..\VC-LTL"
        break
    }
    #当前根目录就是VC-LTL？
    if(Test-Path ".\_msvcrt.h" -PathType Leaf){
        $env:VC_LTL_Root=(Get-Location).Path
        break
    }
    #当前根目录存在VC-LTL？
    if(Test-Path "${(Get-Location).Path}\VC-LTL\_msvcrt.h" -PathType Leaf)
    {
        $env:VC_LTL_Root="${(Get-Location).Path}\VC-LTL"
        break
    }
    #当前父目录存在VC-LTL？
    if(Test-Path "{(Get-Location).Path}\..\VC-LTL\_msvcrt.h" -PathType Leaf)
    {
        $env:VC_LTL_Root="${(Get-Location).Path}\..\VC-LTL"
        break
    }

    #读取注册表 HKCU\Code\VC-LTL@Root
    try
    {
        $env:VC_LTL_Root=(Get-ItemProperty -Name "Root" -Path "HKCU:\Code\VC-LTL").Root
        break
    }catch
    {
        return
    }

}while($false)

&"${env:VC_LTL_Root}\config\config.ps1"
