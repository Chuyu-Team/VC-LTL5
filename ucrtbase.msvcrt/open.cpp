



extern "C" __declspec(dllimport) int __cdecl _open(
    _In_z_ char const* _FileName,
    _In_   int         _OpenFlag,
    ...);

extern "C" __declspec(dllimport) int __cdecl _sopen(
    _In_z_ char const* _FileName,
    _In_   int         _OpenFlag,
    _In_   int         _ShareFlag,
    ...);

extern "C" __declspec(dllimport) errno_t __cdecl _sopen_s(
        _Out_  int*        _FileHandle,
        _In_z_ char const* _FileName,
        _In_   int         _OpenFlag,
        _In_   int         _ShareFlag,
        _In_   int         _PermissionMode
        );

extern "C" __declspec(dllimport) int __cdecl _wopen(
    _In_z_ wchar_t const* _FileName,
    _In_   int            _OpenFlag,
    ...);

extern "C" __declspec(dllimport) int __cdecl _wsopen(
    _In_z_ wchar_t const* _FileName,
    _In_   int            _OpenFlag,
    _In_   int            _ShareFlag,
    ...);

extern "C" __declspec(dllimport) errno_t __cdecl _wsopen_s(
    _Out_  int*           _FileHandle,
    _In_z_ wchar_t const* _FileName,
    _In_   int            _OpenFlag,
    _In_   int            _ShareFlag,
    _In_   int            _PermissionFlag
    );


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
