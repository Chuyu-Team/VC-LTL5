	.686P
	.XMM
	include listing.inc
	.model	flat


PUBLIC __NLG_Return2
PUBLIC __imp___NLG_Return2

PUBLIC __NLG_Return
PUBLIC __imp___NLG_Return

PUBLIC	__NLG_Dispatch2
PUBLIC __imp___NLG_Dispatch2

PUBLIC	__NLG_Notify1
PUBLIC	__NLG_Notify
PUBLIC	__NLG_Call


;¶¨Òå IAT ·ûºÅ
CONST	SEGMENT
__imp___NLG_Return DD FLAT:__NLG_Return
CONST	ENDS

CONST	SEGMENT
__imp___NLG_Return2 DD FLAT:__NLG_Return2
CONST	ENDS

CONST	SEGMENT
__imp___NLG_Dispatch2 DD FLAT:__NLG_Dispatch2
CONST	ENDS

PUBLIC	__NLG_Destination
_DATA	SEGMENT
__NLG_Destination DD 019930520H
	DD	00H
	DD	00H
	DD	00H
_DATA	ENDS

_TEXT	SEGMENT
__NLG_Notify1 PROC
	push	ebx
	push	ecx
	mov	ebx, OFFSET __NLG_Destination
	jmp	SHORT _NLG_Go
__NLG_Notify::
	push	ebx
	push	ecx
	mov	ebx, OFFSET __NLG_Destination
	mov	ecx, DWORD PTR [esp+12]
_NLG_Go:
	mov	DWORD PTR [ebx+8], ecx
	mov	DWORD PTR [ebx+4], eax
	mov	DWORD PTR [ebx+12], ebp
	push	ebp
	push	ecx
	push	eax
__NLG_Dispatch2::
	pop	eax
	pop	ecx
	pop	ebp
	pop	ecx
	pop	ebx
	ret	4
__NLG_Notify1 ENDP
_TEXT	ENDS

_TEXT	SEGMENT
__NLG_Call PROC
	call    eax
__NLG_Return2::
	retn
__NLG_Call ENDP
_TEXT	ENDS


_TEXT	SEGMENT
__NLG_Return PROC
	pop     edi
	pop     esi
	mov     ebx, ebp
	pop     ebp
	mov     ecx, [ebp + 10h]
	push    ebp
	mov     ebp, ebx
	cmp     ecx, 100h
	jnz     short _NLG_Continue
	mov     ecx, 2
_NLG_Continue:
	push    ecx
	call    __NLG_Notify1
	pop     ebp
	pop     ecx
	pop     ebx
	leave
	retn    0Ch
__NLG_Return ENDP
_TEXT	ENDS


END
