#pragma once

#ifndef __LTL_Config_Include
#define __LTL_Config_Include

#ifndef _LTL_Core_Version
#define _LTL_Core_Version 5
#endif

#ifndef _Build_By_LTL
#define _Build_By_LTL 1
#endif


//////////////////////////////////////////////////////////////
//               vcruntime.lib
// 
//from vcruntime140.dll (ms default)
#define _LTL_vcruntime_module_type_from_vcruntime140 0
//from msvcrt.dll
#define _LTL_vcruntime_module_type_from_msvcrt    1
//from ucrtbase.dll
#define _LTL_vcruntime_module_type_from_ucrtbase  2
//
//
//////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////
//               urct.lib
// 
//form ucrtbase.dll or crt api set (ms default)
#define _LTL_ucrt_module_type_from_ucrtbase 0
//form msvcrt.dll
#define _LTL_ucrt_module_type_from_msvcrt   1
//
//
//////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////
//               vccorlib.lib
// 
//from vccorlib140.dll (ms default)
#define _LTL_vccorlib_module_type_form_vccorlib140 0
//form wincorlib.dll (support Windows 8.1 and leater)
#define _LTL_vccorlib_module_type_form_wincorlib   1
//
//
//////////////////////////////////////////////////////////////


#endif
