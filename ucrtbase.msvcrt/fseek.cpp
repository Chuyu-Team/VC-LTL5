#include <stdio.h>
#include <io.h>
#include <errno.h>
/*
	
Windows XP的msvcrt没有这个接口，不过我们可以使用其他接口模拟实现该功能。

当 _Origin = SEEK_SET时，直接调用 fsetpos

当 _Origin = SEEK_CUR时，先调用 fgetpos 获取当前文件位置，修正_Offset后再调用 fsetpos

当 _Origin = SEEK_END时，先触发内部缓冲区写入，然后调用 _lseeki64

_Origin 其他情况，返回无效参数

*/
#if WindowsTargetPlatformMinVersion < WindowsTargetPlatformWindows6
extern "C" int __cdecl _fseeki64(
	_Inout_ FILE*   _Stream,
	_In_    __int64 _Offset,
	_In_    int     _Origin
	)
{
	if (_Origin == SEEK_SET)
	{
		//如果是以文件起始为基准，则直接调用fsetpos，减少函数调用次数
		return fsetpos(_Stream, &_Offset);
	}
	else
	{
		int result = -1;

		//对文件进行加锁，防止其他线程篡改
		_lock_file(_Stream);

		fpos_t _Position;

		//获取文件当前位置
		if (fgetpos(_Stream, &_Position) == 0)
		{
			if (_Origin == SEEK_CUR)
			{
				//以当前位置为准，修正偏移到当前位置
				_Offset += _Position;

				result = fsetpos(_Stream, &_Offset);
			}
			else if (_Origin == SEEK_END)
			{
				//调用一次set，触发，内部缓冲区写入硬盘
				if (fsetpos(_Stream, &_Position) == 0)
				{
					result = _lseeki64(_Stream->_file, _Offset, _Origin) == -1 ? -1 : 0;
				}
			}
			else
			{
				//无效参数
				errno = EINVAL;
			}
		}

		//解锁文件，允许其他线程操作此句柄
		_unlock_file(_Stream);

		return result;
	}
}

_LCRT_DEFINE_IAT_SYMBOL(_fseeki64);
#endif