$private:Platform=""
$private:lang=@{}
switch($env:VSCMD_ARG_TGT_ARCH)
{
    "x86" {$Platform="Win32"}
    "x64" {$Platform="x64"}
    "arm" {$Platform="arm"}
    "arm64" {$Platform="arm64"}
    default {$Platform="unknown"}
}
$private:LCID=[globalization.cultureinfo]::CurrentUICulture.LCID
if($LCID -notin 1033,2052){
    $LCID=1033
}
."$PSScriptRoot\$LCID\config.strings.ps1"
if($Platform -eq "unknown"){
    Write-Error $lang.ERROR_VC_LTL_NOT_SUPPORT_PLATFORM
    return
}
if(-not (Test-Path env:INCLUDE -PathType Leaf)){
    Write-Error $lang.ERROR_VC_LTL_CANNOT_FOUND_INCLUDE_ENV
    return
}
if(-not (Test-Path env:LIB -PathType Leaf)){
    Write-Error $lang.ERROR_VC_LTL_CANNOT_FOUND_LIB_ENV
    return
}
if(Test-Path env:VisualStudioVersion -PathType Leaf){
    if($env:VisualStudioVersion -notin "14.0","15.0","16.0","17.0"){
        Write-Error $lang.ERROR_VC_LTL_NOT_SUPPORT_PLATFORM_TOOLSET
        return
    } 
}else{
    Write-Error $lang.ERROR_VC_LTL_NOT_SUPPORT_PLATFORM_TOOLSET
    return
}

if(-not (Test-Path "$env:VC_LTL_Root\TargetPlatform\$WindowsTargetPlatformMinVersion\lib\$Platform")){
    Write-Error $lang.ERROR_VC_LTL_FILE_MISSING
    return
}
if($env:VSCMD_ARG_TGT_ARCH -eq "arm"){
    if($WindowsTargetPlatformMinVersion -notin "6.2.9200.0","10.0.10240.0","10.0.19041.0"){
        $WindowsTargetPlatformMinVersion= "6.2.9200.0"
    }
}
if($env:VSCMD_ARG_TGT_ARCH -eq "arm64"){
    if($WindowsTargetPlatformMinVersion -notin "10.0.10240.0","10.0.19041.0"){
        $WindowsTargetPlatformMinVersion= "10.0.10240.0"
    }
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
Write-Host "VC Tools Version : $env:VSCMD_VER"
Write-Host "WindowsTargetPlatformMinVersion : $WindowsTargetPlatformMinVersion"
Write-Host "Platform : $Platform"
$env:INCLUDE="$env:VC_LTL_Root\TargetPlatform\header;$env:VC_LTL_Root\$WindowsTargetPlatformMinVersion\header;$env:INCLUDE"
$env:LIB="$env:VC_LTL_Root\TargetPlatform\$WindowsTargetPlatformMinVersion\lib\$env:VSCMD_ARG_TGT_ARCH;$env:LIB"

if($CleanImport){
    if(Test-Path "$env:VC_LTL_Root\TargetPlatform\$WindowsTargetPlatformMinVersion\lib\$env:VSCMD_ARG_TGT_ARCH\CleanImport"){
        $env:LIB="$env:VC_LTL_Root\TargetPlatform\$WindowsTargetPlatformMinVersion\lib\$env:VSCMD_ARG_TGT_ARCH\CleanImport;$env:LIB"
    }
}
$env:HAVE_VC_LTL=$true
