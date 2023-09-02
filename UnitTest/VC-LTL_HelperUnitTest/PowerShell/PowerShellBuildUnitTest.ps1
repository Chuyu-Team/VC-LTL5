param($OutDir, $arch, $WindowsTargetPlatformMinVersion="", $CleanImport="")

&"${PSScriptRoot}\PowerShellInitUnitTest.ps1" -arch $arch -WindowsTargetPlatformMinVersion $WindowsTargetPlatformMinVersion -CleanImport $CleanImport
if($lastexitcode -ne 0)
{
    Write-Error "nmake失败"
    exit $lastexitcode
}

&pushd "${PSScriptRoot}\..\Example\NMake"
$env:OutDir = $OutDir


if (-not (Test-Path $env:OutDir))
{
    mkdir -p "$env:OutDir"
}
& nmake -f Example.mak
&popd

if($lastexitcode -ne 0)
{
    Write-Error "nmake失败"
    exit $lastexitcode
}

exit 0