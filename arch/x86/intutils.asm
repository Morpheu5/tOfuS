%include "defines.asm"

	; loads a given IDT
	; loadIDT(size, address)
extern idtr_p

global EXT_C(loadIDT)
EXT_C(loadIDT):
	push	ebp
	mov		ebp, esp
	sub		esp, 4
	mov		eax, [ebp+8]
	mov		[ebp-2], ax
	mov		eax, [ebp+12]
	mov		[idtr_p+2], eax
	mov		eax, dword [ebp-2]
	mov		[idtr_p], ax
	lidt	[idtr_p]
	leave
	ret

;;	asm("lidt %0"::"m"(&idt_p));
