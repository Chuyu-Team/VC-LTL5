
; Debug 下代码优化可能会破坏rax，所以我们使用汇编实现

PUBLIC	_NLG_Notify
EXTRN	__imp___NLG_Dispatch2:PROC


_TEXT	SEGMENT

;extern "C" void _NLG_Notify(void* p1, void* p2, int p3)
;{
;	return __NLG_Dispatch2(p1, p2, p3, 0x19930520);
;}

_NLG_Notify PROC
	mov	r9d, 19930520H
	jmp	QWORD PTR [__imp___NLG_Dispatch2]
_NLG_Notify ENDP
_TEXT	ENDS
END
