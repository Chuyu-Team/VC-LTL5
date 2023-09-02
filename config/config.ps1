#
#  请不要直接使用此脚本，应该使用VC-LTL helper for nmake.ps1
#

Function FoundBestTargetPlatformDefault
{
    if($LTLPlatform -ieq "arm64")
    {
        return "10.0.10240.0"
    }
    elseif($LTLPlatform -ieq "arm")
    {
        return "6.2.9200.0"
    }
    else
    {
        return "6.0.6000.0"
    }
}

Function FoundBestTargetPlatform
{
    if($env:WindowsTargetPlatformMinVersion.Length -eq 0)
    {
        return FoundBestTargetPlatformDefault
    }

    $CurrentWindowsTargetPlatformMinVersion = [Version]::new($env:WindowsTargetPlatformMinVersion)

    if($CurrentWindowsTargetPlatformMinVersion -gt [Version]::new("10.0.10240.0"))
    {
        return "10.0.19041.0"
    }
    elseif($LTLPlatform -ieq "arm64")
    {
        return "10.0.10240.0"
    }
    elseif($CurrentWindowsTargetPlatformMinVersion -gt [Version]::new("6.2.9200.0"))
    {
        return "10.0.10240.0"
    }
    elseif($LTLPlatform -ieq "arm")
    {
        return "6.2.9200.0"
    }
    elseif($CurrentWindowsTargetPlatformMinVersion -gt [Version]::new("6.0.6000.0"))
    {
        return "6.2.9200.0"
    }
    elseif($CurrentWindowsTargetPlatformMinVersion -gt [Version]::new("5.2.3790.0"))
    {
        return "6.0.6000.0"
    }
    elseif($LTLPlatform -ieq "x64")
    {
        return "5.2.3790.0"
    }
    else
    {
        return "5.1.2600.0"
    }
}

if($env:VC_LTL_Helper_Load -ieq "true")
{
    return
}

$private:LCID=[globalization.cultureinfo]::CurrentUICulture.LCID
if(-not (Test-Path "${PSScriptRoot}\$LCID\config.strings.ps1"))
{
    # 找不到匹配就使用英语
    $LCID=1033
}

."${PSScriptRoot}\$LCID\config.strings.ps1"
$private:lang = InitMuiStrings

# 检查环境是否可以加载VC-LTL
if($env:INCLUDE.Length -eq 0)
{
    Write-Error $lang.ERROR_VC_LTL_CANNOT_FOUND_INCLUDE_ENV
    return
}

if($env:LIB.Length -eq 0)
{
    Write-Error $lang.ERROR_VC_LTL_CANNOT_FOUND_INCLUDE_ENV
    return
}

# 当前VisualStudio版本符合条件？
if($env:VisualStudioVersion -notin "14.0","15.0","16.0","17.0")
{
    Write-Error $lang.ERROR_VC_LTL_NOT_SUPPORT_PLATFORM_TOOLSET
    return
}

$private:LTLPlatform=""
switch($env:VSCMD_ARG_TGT_ARCH)
{
    "x86" {$LTLPlatform="Win32"}
    "x64" {$LTLPlatform="x64"}
    "arm" {$LTLPlatform="arm"}
    "arm64" {$LTLPlatform="arm64"}
    default {$LTLPlatform=""}
}

if($LTLPlatform -eq "")
{
    Write-Error $lang.ERROR_VC_LTL_NOT_SUPPORT_PLATFORM_TOOLSET
    return
}

# VC-LTL核心版本号，由于4.X并不兼容3.X。此值可以用于兼容性判断。
$env:LTL_CoreVersion=5

$env:VC_LTL_Helper_Load="true"

# 搜索最佳TargetPlatform
$private:LTLWindowsTargetPlatformMinVersion = FoundBestTargetPlatform 

if(-not (Test-Path "$env:VC_LTL_Root\TargetPlatform\$LTLWindowsTargetPlatformMinVersion\lib\$LTLPlatform"))
{
    Write-Error $lang.ERROR_VC_LTL_FILE_MISSING
    return
}

Write-Host "#######################################################################"
Write-Host "#                                                                     #"
Write-Host "# 8b           d8  ,ad8888ba,         88     888888888888 88          #"
Write-Host "# `8b         d8' d8`"'      `"8b        88          88      88          #"
Write-Host "#  `8b       d8' d8'                   88          88      88          #"
Write-Host "#   `8b     d8'  88                    88          88      88          #"
Write-Host "#    `8b   d8'   88           aaaaaaaa 88          88      88          #"
Write-Host "#     `8b d8'    Y8,          `"`"`"`"`"`"`"`" 88          88      88          #"
Write-Host "#      `888'      Y8a.     .a8P        88          88      88          #"
Write-Host "#       `8'         `"Y8888Y`"'          88888888888 88      88888888888 #"
Write-Host "#                                                                     #"
Write-Host "#######################################################################"
Write-Host "VC-LTL Path : $env:VC_LTL_Root"
Write-Host "VC Tools Version : $env:VCToolsVersion"
Write-Host "WindowsTargetPlatformMinVersion : $LTLWindowsTargetPlatformMinVersion"
Write-Host "Platform : $LTLPlatform"

$env:INCLUDE="$env:VC_LTL_Root\TargetPlatform\header;$env:VC_LTL_Root\$LTLWindowsTargetPlatformMinVersion\header;$env:INCLUDE"
$env:LIB="$env:VC_LTL_Root\TargetPlatform\$LTLWindowsTargetPlatformMinVersion\lib\$LTLPlatform;$env:LIB"

if($CleanImport -ieq "true")
{
    if(Test-Path "$env:VC_LTL_Root\TargetPlatform\$LTLWindowsTargetPlatformMinVersion\lib\$LTLPlatform\CleanImport")
    {
        $env:LIB="$env:VC_LTL_Root\TargetPlatform\$LTLWindowsTargetPlatformMinVersion\lib\$LTLPlatform\CleanImport;$env:LIB"
    }
}
