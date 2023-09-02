Function InitMuiStrings()
{
    $private:lang=@{}
    $lang.Add("ERROR_VC_LTL_DONOT_USE_THIS_FILE","请不要直接使用此脚本，应该使用VC-LTL helper for nmake.ps1。")
    $lang.Add("ERROR_VC_LTL_CANNOT_FOUND_INCLUDE_ENV","找不到环境变量INCLUDE，请在Developer PowerShell执行后调用此脚本。")
    $lang.Add("ERROR_VC_LTL_CANNOT_FOUND_LIB_ENV","找不到环境变量LIB，请在Developer PowerShell执行后调用此脚本。")
    $lang.Add("ERROR_VC_LTL_NOT_SUPPORT_PLATFORM_TOOLSET","VC-LTL仅支持VS 2015、2017、2019以及2022。")
    $lang.Add("ERROR_VC_LTL_NOT_SUPPORT_PLATFORM","VC-LTL POWERSHELL脚本不支持体系 : $env:VSCMD_ARG_TGT_ARCH")
    $lang.Add("ERROR_VC_LTL_FILE_MISSING","VC-LTL不找不到lib文件，请从 https://github.com/Chuyu-Team/VC-LTL5/releases/latest 下载完整二进制文件然后继续。")

    return $lang;
}