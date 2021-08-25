#include <internal_shared.h>


//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Flags
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#define _S_IFMT   0xF000 // File type mask
#define _S_IFDIR  0x4000 // Directory
#define _S_IFCHR  0x2000 // Character special
#define _S_IFIFO  0x1000 // Pipe
#define _S_IFREG  0x8000 // Regular
#define _S_IREAD  0x0100 // Read permission, owner
#define _S_IWRITE 0x0080 // Write permission, owner
#define _S_IEXEC  0x0040 // Execute/search permission, owner


extern "C" __declspec(dllimport) int __cdecl _open(
    _In_z_ char const* _FileName,
    _In_   int         _OpenFlag,
    ...);

extern "C" __declspec(dllimport) int __cdecl _sopen(
    _In_z_ char const* _FileName,
    _In_   int         _OpenFlag,
    _In_   int         _ShareFlag,
    ...);

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _sopen_s(
        _Out_  int*        _FileHandle,
        _In_z_ char const* _FileName,
        _In_   int         _OpenFlag,
        _In_   int         _ShareFlag,
        _In_   int         _PermissionMode
        )
{
    _VALIDATE_RETURN_ERRCODE(_FileHandle != nullptr, EINVAL);
    *_FileHandle = -1;

    _VALIDATE_RETURN_ERRCODE(_FileName != nullptr, EINVAL);


    _VALIDATE_RETURN_ERRCODE((_PermissionMode & (~(_S_IREAD | _S_IWRITE))) == 0, EINVAL);


    return (*_FileHandle = _sopen(_FileName, _OpenFlag, _ShareFlag, _PermissionMode)) == -1 ? errno : 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_sopen_s);
#else
extern "C" __declspec(dllimport) errno_t __cdecl _sopen_s(
        _Out_  int*        _FileHandle,
        _In_z_ char const* _FileName,
        _In_   int         _OpenFlag,
        _In_   int         _ShareFlag,
        _In_   int         _PermissionMode
        );
#endif

extern "C" __declspec(dllimport) int __cdecl _wopen(
    _In_z_ wchar_t const* _FileName,
    _In_   int            _OpenFlag,
    ...);

extern "C" __declspec(dllimport) int __cdecl _wsopen(
    _In_z_ wchar_t const* _FileName,
    _In_   int            _OpenFlag,
    _In_   int            _ShareFlag,
    ...);

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wsopen_s(
    _Out_  int*           _FileHandle,
    _In_z_ wchar_t const* _FileName,
    _In_   int            _OpenFlag,
    _In_   int            _ShareFlag,
    _In_   int            _PermissionFlag
    )
{
    _VALIDATE_RETURN_ERRCODE(_FileHandle != nullptr, EINVAL);
    *_FileHandle = -1;

    _VALIDATE_RETURN_ERRCODE(_FileName != nullptr, EINVAL);


    _VALIDATE_RETURN_ERRCODE((_PermissionFlag & (~(_S_IREAD | _S_IWRITE))) == 0, EINVAL);


    return (*_FileHandle = _wsopen(_FileName, _OpenFlag, _ShareFlag, _PermissionFlag)) == -1 ? errno : 0;
}

_LCRT_DEFINE_IAT_SYMBOL(_wsopen_s);
#else
extern "C" __declspec(dllimport) errno_t __cdecl _wsopen_s(
    _Out_  int*           _FileHandle,
    _In_z_ wchar_t const* _FileName,
    _In_   int            _OpenFlag,
    _In_   int            _ShareFlag,
    _In_   int            _PermissionFlag
    );
#endif

extern "C" errno_t __cdecl _sopen_dispatch(
    char const* const path,
    int         const oflag,
    int         const shflag,
    int         const pmode,
    int*        const pfh,
    int         const secure
    )
{
    if (secure)
    {
        return _sopen_s(pfh, path, oflag, shflag, pmode);
    }
    else
    {
        auto fh = _sopen(path, oflag, shflag, pmode);
        *pfh = fh;

        return fh != -1 ? 0 : errno;
    }
}

_LCRT_DEFINE_IAT_SYMBOL(_sopen_dispatch);


extern "C" errno_t __cdecl _wsopen_dispatch(
    wchar_t const* const path,
    int            const oflag,
    int            const shflag,
    int            const pmode,
    int*           const pfh,
    int            const secure
    )
{
    if (secure)
    {
        return _wsopen_s(pfh, path, oflag, shflag, pmode);
    }
    else
    {
        auto fh = _wsopen(path, oflag, shflag, pmode);
        *pfh = fh;

        return fh != -1 ? 0 : errno;
    }
}

_LCRT_DEFINE_IAT_SYMBOL(_wsopen_dispatch);
