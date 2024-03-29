﻿Function InitMuiStrings()
{
    $private:lang=@{}
    $lang.Add("ERROR_VC_LTL_DONOT_USE_THIS_FILE","Please don't call this script file directly, you should use 'VC-LTL helper for nmake'")
    $lang.Add("ERROR_VC_LTL_CANNOT_FOUND_INCLUDE_ENV","The environment variable INCLUDE cannot be found, please make sure Developer PowerShell is executed.")
    $lang.Add("ERROR_VC_LTL_CANNOT_FOUND_LIB_ENV","The environment variable LIB cannot be found, please make sure Developer PowerShell is executed.")
    $lang.Add("ERROR_VC_LTL_NOT_SUPPORT_PLATFORM_TOOLSET","VC-LTL only supports Visual Studio 2015, 2017, 2019 and 2022.")
    $lang.Add("ERROR_VC_LTL_NOT_SUPPORT_PLATFORM","VC-LTL does not support $env:VSCMD_ARG_TGT_ARCH platform.")
    $lang.Add("ERROR_VC_LTL_FILE_MISSING","VC-LTL can't find lib files, please download the binary files from https://github.com/Chuyu-Team/VC-LTL5/releases/latest then continue.")

    return $lang;
}