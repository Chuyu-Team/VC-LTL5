#include "pch.h"
namespace Platform
{
    namespace Details
    {
        //wincorlib版本
        __declspec(dllimport) ::Platform::Object^ __stdcall CreateValueBule(class Platform::Type^, void const*);


#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows10_10240
        ::Platform::Object^ __stdcall CreateValue(::Platform::TypeCode Code, const void* constValue)
        {
            static FARPROC pCreateValueBule;
            static FARPROC pCreateValueWin10;


            if (pCreateValueBule == nullptr || pCreateValueWin10 == nullptr)
            {
                do
                {
                    auto hWincorlib = GetModuleHandleW(L"wincorlib.dll");

                    auto pCreateValue = GetProcAddress(hWincorlib, __FUNCDNAME__);

                    if (pCreateValue)
                    {
                        pCreateValueBule = (FARPROC)INVALID_HANDLE_VALUE;
                        pCreateValueWin10 = pCreateValue;
                        break;
                    }                   

#if defined(_X86_)
                    #define WindowsBlueCreateValue "?CreateValue@Details@Platform@@YGP$AAVObject@2@P$AAVType@2@PBX@Z"
#elif defined(_AMD64_)
                    #define WindowsBlueCreateValue "?CreateValue@Details@Platform@@YAPE$AAVObject@2@PE$AAVType@2@PEBX@Z"
#elif defined(_ARM_)
                    #define WindowsBlueCreateValue "?CreateValue@Details@Platform@@YAP$AAVObject@2@P$AAVType@2@PBX@Z"
#else
#error 你搞错了吗？
#endif
                    pCreateValue = GetProcAddress(hWincorlib, WindowsBlueCreateValue);

                    if (pCreateValue)
                    {
                        pCreateValueBule = pCreateValue;
                        pCreateValueWin10 = (FARPROC)INVALID_HANDLE_VALUE;
                        break;
                    }

                    
                    pCreateValueBule = (FARPROC)INVALID_HANDLE_VALUE;
                    pCreateValueWin10 = (FARPROC)INVALID_HANDLE_VALUE;
                } while (false);

            }

            ::Platform::Object^ o = nullptr;

            if (pCreateValueWin10 && pCreateValueWin10 != (FARPROC)INVALID_HANDLE_VALUE)
            {
                o = ((decltype(CreateValue)*)pCreateValueWin10)(Code, constValue);
            }
            else if (pCreateValueBule && pCreateValueBule != (FARPROC)INVALID_HANDLE_VALUE)
            {
                __abi_type_descriptor type_descriptor = {};
                type_descriptor.__typeId = (int)Code;
                auto type = __abi_make_type_id(type_descriptor);
                if (type)
                    o = ((decltype(CreateValueBule)*)pCreateValueBule)(type, constValue);
            }

            //if (o)
            {
                //auto pUnknown = reinterpret_cast<__abi_IUnknown*>(o);
                //pUnknown->__abi_AddRef();
            }

            return o;
        }

        _LCRT_DEFINE_IAT_RAW_SYMBOL(
            ?CreateValue@Details@Platform@@YGP$AAVObject@2@W4TypeCode@2@PBX@Z,
            ?CreateValue@Details@Platform@@YAPE$AAVObject@2@W4TypeCode@2@PEBX@Z,
            ?CreateValue@Details@Platform@@YAP$AAVObject@2@W4TypeCode@2@PBX@Z,
            ?CreateValue@Details@Platform@@YAPE$AAVObject@2@W4TypeCode@2@PEBX@Z
            );
#endif

    }
}