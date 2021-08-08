
#include "framework.h"

typedef void* HINSTANCE;

extern "C" void __cdecl __telemetry_main_invoke_trigger(HINSTANCE)
{
}

_LCRT_DEFINE_IAT_SYMBOL(__telemetry_main_invoke_trigger);

extern "C" void __cdecl __telemetry_main_return_trigger(HINSTANCE)
{
}

_LCRT_DEFINE_IAT_SYMBOL(__telemetry_main_return_trigger);
