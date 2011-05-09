%include "defines.asm"

extern gdtr

global EXT_C(loadGDT)
EXT_C(loadGDT):
	push	ebp
	mov		ebp,esp
	mov		ecx,[ebp+0x8]
	mov		eax,[ebp+0xc]
	mov		edx,[ebp+0x10]
	and		edx, 0xffff
	mov		[ecx], edx
	mov		[ecx+0x2], eax
	lgdt	[ecx]
	leave
	ret

global EXT_C(readMSW)
EXT_C(readMSW):
	push	ebp
	mov		ebp, esp
	smsw	ax
	leave
	ret

global EXT_C(readCR0)
EXT_C(readCR0):
	push	ebp
	mov		ebp, esp
	mov		eax, cr0
	leave
	ret

global EXT_C(writeCR0)
EXT_C(writeCR0):
	push	ebp
	mov		ebp,esp
	mov		eax,[ebp+0x8]
	mov		cr0, eax
	mov		eax, cr0
	leave
	ret

global EXT_C(readCR3)
EXT_C(readCR3):
	push	ebp
	mov		ebp,esp
	mov		eax, cr3
	leave
	ret

global EXT_C(writeCR3)
EXT_C(writeCR3):
	push	ebp
	mov		ebp,esp
	mov		eax, [ebp+0x8]
	mov		cr3, eax
	mov		eax, cr3
	leave
	ret
