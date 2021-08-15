#include <stdio.h>
#include <limits.h>
#include <internal_shared.h>

#ifdef _DEBUG
#define _BUFFER_FILL_PATTERN _SECURECRT_FILL_BUFFER_PATTERN
#else
#define _BUFFER_FILL_PATTERN 0
#endif

/*
msvcrt并没有导出fread_s，只能靠fread模拟行为

fread_s，主要提供了二大功能
1：操作失败时，将_Buffer所有内容设置为 _BUFFER_FILL_PATTERN
2：读取文件时提供_BufferSize检查，当_Buffer不足以容纳目标时，则返回 ERANGE

根据文档https://msdn.microsoft.com/zh-cn/library/hh977173.aspx 以及UCRT源代码确定，fread_s失败时文件位置是未知的。
因此我们没有必要事先保存文件指针位置，来提供恢复机制。
*/
extern "C" size_t __cdecl fread_s(
	_Out_writes_bytes_to_(_BufferSize, _ElementSize* _ElementCount)   void* _Buffer,
	_In_range_(>= , _ElementSize* _ElementCount)                       size_t _BufferSize,
	_In_                                                               size_t _ElementSize,
	_In_                                                               size_t _ElementCount,
	_Inout_                                                            FILE* _Stream
)
{
	if (_BufferSize == SIZE_MAX)
	{
		//_BufferSize为 SIZE_MAX时，走老逻辑
		return fread(_Buffer, _ElementSize, _ElementCount, _Stream);
	}

	if (_ElementSize == 0 || _ElementCount == 0)
		return 0;

	//缓冲区初始化
	memset(_Buffer, _BUFFER_FILL_PATTERN, _BufferSize);


	const auto Total_bytes = _ElementSize * _ElementCount;

	const auto Need_Read_bytes = min(Total_bytes, _BufferSize);

	errno = 0;

	size_t num_read = fread(_Buffer, 1, Need_Read_bytes, _Stream);

	if (num_read == 0)
	{
		if (errno)
		{
			memset(_Buffer, _BUFFER_FILL_PATTERN, _BufferSize);
		}

		return 0;
	}
	else if (num_read == Need_Read_bytes && Total_bytes != Need_Read_bytes)
	{
		//可能有更多数据可用测试下是否还有更多数据可以读取
		errno = 0;

		unsigned char Tmp;

		if (fread(&Tmp, 1, 1, _Stream))
		{
			//还有更多数据
			memset(_Buffer, _BUFFER_FILL_PATTERN, _BufferSize);

			_VALIDATE_RETURN(("buffer too small", 0), ERANGE, 0)
		}
		else if (errno)
		{
			//遇到了错误
			memset(_Buffer, _BUFFER_FILL_PATTERN, _BufferSize);
			return 0;
		}
	}

	return num_read / _ElementSize;
}

_LCRT_DEFINE_IAT_SYMBOL(fread_s);
