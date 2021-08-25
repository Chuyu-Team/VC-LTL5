#include <internal_shared.h>

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _splitpath_s(
	_In_z_                             char const* _FullPath,
	_Out_writes_opt_z_(_DriveCount)    char*       _Drive,
	_In_                               size_t      _DriveCount,
	_Out_writes_opt_z_(_DirCount)      char*       _Dir,
	_In_                               size_t      _DirCount,
	_Out_writes_opt_z_(_FilenameCount) char*       _Filename,
	_In_                               size_t      _FilenameCount,
	_Out_writes_opt_z_(_ExtCount)      char*       _Ext,
	_In_                               size_t      _ExtCount
	)
{
	if (_FullPath == nullptr
		|| _Drive==nullptr && _DriveCount!=0
		|| _Dir==nullptr && _DirCount!=0
		|| _Filename==nullptr && _FilenameCount!=0
		|| _Ext==nullptr && _ExtCount!=0)
	{
		if (_Drive&&_DriveCount)
			*_Drive = '\0';

		if (_Dir&&_DirCount)
			*_Dir = '\0';

		if (_Dir&&_DirCount)
			*_Dir = '\0';

		if (_Filename&&_FilenameCount)
			*_Filename = '\0';

		if (_Ext&&_ExtCount)
			*_Ext = '\0';

		_VALIDATE_RETURN_ERRCODE(false, EINVAL);
	}

	char _DriveTmp[_MAX_DRIVE] = {};
	char _DirTmp[_MAX_DIR] = {};
	char _FilenameTmp[_MAX_FNAME] = {};
	char _ExtTmp[_MAX_EXT] = {};

	errno = 0;

	_splitpath(_FullPath, _Drive ? _DriveTmp : nullptr, _Dir ? _DirTmp : nullptr, _Filename ? _FilenameTmp : nullptr, _Ext ? _ExtTmp : nullptr);


	auto __errno = errno;

	if (__errno == 0)
	{
		//复制数据
		if (_Drive)
		{
			auto DriveCountTmp = strlen(_DriveTmp);

			if (DriveCountTmp >= _DriveCount)
			{
				__errno = errno = ERANGE;

				goto _Error;
			}

			memcpy(_Drive, _DriveTmp, (DriveCountTmp + 1) * sizeof(_Drive[0]));
		}

		if (_Dir)
		{
			auto _DirCountTmp = strlen(_DirTmp);

			if (_DirCountTmp >= _DirCount)
			{
				__errno = errno = ERANGE;

				goto _Error;
			}

			memcpy(_Dir, _DirTmp, (_DirCountTmp + 1) * sizeof(_Dir[0]));
		}

		if (_Filename)
		{
			auto _FilenameCountTmp = strlen(_FilenameTmp);

			if (_FilenameCountTmp >= _FilenameCount)
			{
				__errno = errno = ERANGE;

				goto _Error;
			}

			memcpy(_Filename, _FilenameTmp, (_FilenameCountTmp + 1) * sizeof(_Filename[0]));
		}

		if (_Ext)
		{
			auto _ExtCountTmp = strlen(_ExtTmp);

			if (_ExtCountTmp >= _ExtCount)
			{
				__errno = errno = ERANGE;

				goto _Error;
			}

			memcpy(_Ext, _ExtTmp, (_ExtCountTmp + 1) * sizeof(_Ext[0]));

		}

		return 0;
	}

_Error:


	if (_Drive&&_DriveCount)
		*_Drive = '\0';

	if (_Dir&&_DirCount)
		*_Dir = '\0';

	if (_Dir&&_DirCount)
		*_Dir = '\0';

	if (_Filename&&_FilenameCount)
		*_Filename = '\0';

	if (_Ext&&_ExtCount)
		*_Ext = '\0';

	return __errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_splitpath_s);
#endif

#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" errno_t __cdecl _wsplitpath_s(
	_In_z_                             wchar_t const* _FullPath,
	_Out_writes_opt_z_(_DriveCount)    wchar_t*       _Drive,
	_In_                               size_t         _DriveCount,
	_Out_writes_opt_z_(_DirCount)      wchar_t*       _Dir,
	_In_                               size_t         _DirCount,
	_Out_writes_opt_z_(_FilenameCount) wchar_t*       _Filename,
	_In_                               size_t         _FilenameCount,
	_Out_writes_opt_z_(_ExtCount)      wchar_t*       _Ext,
	_In_                               size_t         _ExtCount
	)
{
	if (_FullPath == nullptr
		|| _Drive == nullptr && _DriveCount != 0
		|| _Dir == nullptr && _DirCount != 0
		|| _Filename == nullptr && _FilenameCount != 0
		|| _Ext == nullptr && _ExtCount != 0)
	{
		if (_Drive&&_DriveCount)
			*_Drive = L'\0';

		if (_Dir&&_DirCount)
			*_Dir = L'\0';

		if (_Dir&&_DirCount)
			*_Dir = L'\0';

		if (_Filename&&_FilenameCount)
			*_Filename = L'\0';

		if (_Ext&&_ExtCount)
			*_Ext = L'\0';

		_VALIDATE_RETURN_ERRCODE(false, EINVAL);
	}

	wchar_t _DriveTmp[_MAX_DRIVE] = {};
	wchar_t _DirTmp[_MAX_DIR] = {};
	wchar_t _FilenameTmp[_MAX_FNAME] = {};
	wchar_t _ExtTmp[_MAX_EXT] = {};

	errno = 0;

	_wsplitpath(_FullPath, _Drive ? _DriveTmp : nullptr, _Dir ? _DirTmp : nullptr, _Filename ? _FilenameTmp : nullptr, _Ext ? _ExtTmp : nullptr);


	auto __errno = errno;

	if (__errno == 0)
	{
		//复制数据
		if (_Drive)
		{
			auto DriveCountTmp = wcslen(_DriveTmp);

			if (DriveCountTmp >= _DriveCount)
			{
				__errno = errno = ERANGE;

				goto _Error;
			}

			memcpy(_Drive, _DriveTmp, (DriveCountTmp + 1) * sizeof(_Drive[0]));
		}

		if (_Dir)
		{
			auto _DirCountTmp = wcslen(_DirTmp);

			if (_DirCountTmp >= _DirCount)
			{
				__errno = errno = ERANGE;

				goto _Error;
			}

			memcpy(_Dir, _DirTmp, (_DirCountTmp + 1) * sizeof(_Dir[0]));
		}

		if (_Filename)
		{
			auto _FilenameCountTmp = wcslen(_FilenameTmp);

			if (_FilenameCountTmp >= _FilenameCount)
			{
				__errno = errno = ERANGE;

				goto _Error;
			}

			memcpy(_Filename, _FilenameTmp, (_FilenameCountTmp + 1) * sizeof(_Filename[0]));
		}

		if (_Ext)
		{
			auto _ExtCountTmp = wcslen(_ExtTmp);

			if (_ExtCountTmp >= _ExtCount)
			{
				__errno = errno = ERANGE;

				goto _Error;
			}

			memcpy(_Ext, _ExtTmp, (_ExtCountTmp + 1) * sizeof(_Ext[0]));

		}

		return 0;
	}

_Error:


	if (_Drive&&_DriveCount)
		*_Drive = L'\0';

	if (_Dir&&_DirCount)
		*_Dir = L'\0';

	if (_Dir&&_DirCount)
		*_Dir = L'\0';

	if (_Filename&&_FilenameCount)
		*_Filename = L'\0';

	if (_Ext&&_ExtCount)
		*_Ext = L'\0';

	return __errno;
}

_LCRT_DEFINE_IAT_SYMBOL(_wsplitpath_s);
#endif