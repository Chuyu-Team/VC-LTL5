param($arch, $WindowsTargetPlatformMinVersion="", $CleanImport="")

$ProgramFiles = ${env:ProgramFiles(x86)}

if (-not $ProgramFiles)
{
    $ProgramFiles = $env:ProgramFiles
}
          
$BuiltInVsWhereExe = "$ProgramFiles\Microsoft Visual Studio\Installer\vswhere.exe"

if (-not (Test-Path $BuiltInVsWhereExe))
{
    Write-Error "找不到vswhere.exe！"
    exit 1
}
          
Write-Output $BuiltInVsWhereExe
          
$LatestVisualStudioRoot = & $BuiltInVsWhereExe -latest -prerelease -property installationPath
          
if (-not (Test-Path $LatestVisualStudioRoot))
{
    Write-Error "找不到 VisualStudioRoot！"
    exit 1
}

& "$LatestVisualStudioRoot\Common7\Tools\Launch-VsDevShell.ps1" -arch $arch

$env:VC_LTL_Root = "${PSScriptRoot}\..\..\.."

if ($WindowsTargetPlatformMinVersion -ne "")
{
    $env:WindowsTargetPlatformMinVersion = "$WindowsTargetPlatformMinVersion"
}

if ($CleanImport -ne "")
{
    $env:CleanImport = "$CleanImport"
}

& "$env:VC_LTL_Root\VC-LTL helper for nmake.ps1"

exit 0