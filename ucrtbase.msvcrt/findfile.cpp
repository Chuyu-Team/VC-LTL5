
#include <io.h>
#include <internal_shared.h>

//通过ASCII文件名搜索
__forceinline intptr_t __cdecl _tfindfirst(
	_In_z_ char const* _FileName,
	_Out_  struct __finddata64_t* _FindData
)
{
	return _findfirst64(_FileName, _FindData);
}

//通过Unicode路径搜索
__forceinline intptr_t __cdecl _tfindfirst(
	_In_z_ wchar_t const* _FileName,
	_Out_  struct _wfinddata64_t* _FindData
	)
{
	return _wfindfirst64(_FileName, _FindData);
}

template<class __finddata, class File, class Type>
__forceinline intptr_t __cdecl common_findfirst(
	_In_z_ File              _FileName,
	_Out_  Type * _FindData
	)
{
	_VALIDATE_CLEAR_OSSERR_RETURN(_FileName != nullptr && _FindData != nullptr, EINVAL, -1);

	__finddata finddataT;
	auto _FindHandle = _tfindfirst(_FileName, &finddataT);
	if (_FindHandle != -1)
	{
		_FindData->attrib = finddataT.attrib;
		_FindData->time_create = finddataT.time_create;
		_FindData->time_access = finddataT.time_access;
		_FindData->time_write = finddataT.time_write;
		_FindData->size = finddataT.size;

		static_assert(sizeof(_FindData->name) == sizeof(finddataT.name), "名称缓冲区必须完全一致！");

		memcpy(_FindData->name, finddataT.name, sizeof(finddataT.name));
	}

	return _FindHandle;
}

//msvcrt不支持_findfirst64i32，不过我们可以用_findfirst64转换
extern "C" intptr_t __cdecl _findfirst64i32(
	_In_z_ char const* _FileName,
	_Out_  struct _finddata64i32_t* _FindData
	)
{
	return common_findfirst<__finddata64_t>(_FileName, _FindData);
}

_LCRT_DEFINE_IAT_SYMBOL(_findfirst64i32);

//msvcrt不支持_wfindfirst64i32，不过我们可以用_wfindfirst64转换
extern "C" intptr_t __cdecl _wfindfirst64i32(
	_In_z_ wchar_t const* _FileName,
	_Out_  struct _wfinddata64i32_t* _FindData
	)
{
	return common_findfirst<_wfinddata64_t>(_FileName, _FindData);
}

_LCRT_DEFINE_IAT_SYMBOL(_wfindfirst64i32);


__forceinline int __cdecl _tfindnext(
	_In_  intptr_t                 _FindHandle,
	_Out_ struct __finddata64_t* _FindData
)
{
	return _findnext64(_FindHandle, _FindData);
}

__forceinline int __cdecl _tfindnext(
	_In_  intptr_t                  _FindHandle,
	_Out_ struct _wfinddata64_t* _FindData
)
{
	return _wfindnext64(_FindHandle, _FindData);
}

template<class __finddata, class Type>
__forceinline intptr_t __cdecl common_findnext(
	_In_  intptr_t                 _FindHandle,
	_Out_  Type * _FindData
)
{
	_VALIDATE_CLEAR_OSSERR_RETURN(_FindHandle != -1 && _FindData != nullptr, EINVAL, -1);

	__finddata finddataT;
	auto result = _tfindnext(_FindHandle, &finddataT);
	if (result == 0)
	{
		_FindData->attrib = finddataT.attrib;
		_FindData->time_create = finddataT.time_create;
		_FindData->time_access = finddataT.time_access;
		_FindData->time_write = finddataT.time_write;
		_FindData->size = finddataT.size;

		static_assert(sizeof(_FindData->name) == sizeof(finddataT.name), "名称缓冲区必须完全一致！");

		memcpy(_FindData->name, finddataT.name, sizeof(finddataT.name));
	}

	return result;
}

extern "C" int __cdecl _findnext64i32(
	_In_  intptr_t                 _FindHandle,
	_Out_ struct _finddata64i32_t* _FindData
)
{
	return common_findnext<__finddata64_t>(_FindHandle, _FindData);
}

_LCRT_DEFINE_IAT_SYMBOL(_findnext64i32);

extern "C" int __cdecl _wfindnext64i32(
	_In_  intptr_t                  _FindHandle,
	_Out_ struct _wfinddata64i32_t* _FindData
)
{
	return common_findnext<_wfinddata64_t>(_FindHandle, _FindData);
}

_LCRT_DEFINE_IAT_SYMBOL(_wfindnext64i32);
