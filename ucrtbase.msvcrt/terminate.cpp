
#include <corecrt_terminate.h>


extern "C" terminate_handler __cdecl _get_terminate()
{
    //通过二次二次调用 set_terminate 模拟 _get_terminate
    auto handler = set_terminate(nullptr);
    set_terminate(handler);
    return handler;
}

_LCRT_DEFINE_IAT_SYMBOL(_get_terminate);
