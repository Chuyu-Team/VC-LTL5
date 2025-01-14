#
#  VC-LTL自动化加载配置，建议你将此文件单独复制到你的工程再使用，该文件能自动识别当前环境是否存在VC-LTL，并且自动应用。
#
#  使用方法：
#    方法: 1.启动Powershell，并执行如下命令：
#            $BuiltInVsWhereExe = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
#            $LatestVisualStudioRoot = & $BuiltInVsWhereExe -latest -prerelease -property installationPath
#            # arch支持 x86、amd64、arm、arm64
#            & "$LatestVisualStudioRoot\Common7\Tools\Launch-VsDevShell.ps1" -arch x86
#
#          2. 然后执行此脚本(VC-LTL helper for nmake.ps1)，运行完成后将自动修改include以及lib环境变量，方便nmake以及纯cl用户引用VC-LTL。
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

# (Fea #64 引入)，实验性功能！开启后，对于使用`/MD`构建项目将依赖ucrtbase.dll，使用后你可能需要下载VC-LTL.Redist.Dlls.zip（感谢msvcr14x项目提供兼容XP系统的ucrtbase.dll）。
# $env:VC_LTL_EnableNewStyleRuntimeDlls="true"
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
