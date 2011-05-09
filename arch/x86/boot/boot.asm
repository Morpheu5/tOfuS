[bits 32]
[section .text]
%include "arch/i386/boot/multiboot.asm"
%include "tofus/arch/i386/defines.asm"

extern EXT_C(setup)
extern gdtr

ALIGN 4
multiboot_header:
	dd	MULTIBOOT_HEADER_MAGIC
	dd	MULTIBOOT_HEADER_FLAGS
	dd	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

global EXT_C(start)
EXT_C(start):
multiboot_entry:
	; Disable interrupts
	cli

	; Load the TR's trick GDT. This is for linking the kernel at an arbitray
	; high virtual address (e.g. 0xf0000000) so to later setup virtual memory.
	lgdt	[_trick_gdt_ptr]
	mov		dx, DATA_SEL
	mov		ds, dx
	mov		es, dx
	mov		fs, dx
	mov		gs, dx
	mov		ss, dx

	; CS register cannot be addressed so we can't do mov cs, CODE_SEL, let's do
	; it with some jumpy-dumpy head-trippin' code. (Hey, that's what Intel's
	; saying about...)
	jmp		CODE_SEL:ahead

ahead:
	; TODO: create four 4kB stacks for the various PL.
	; May this be done later in the setup.c code?
	; set the stack pointer so we have a stack of size STACK_SIZE
	mov		esp, dword _stack

	; prepare the stack frame for calling the main setup procedure
	; zero flags
	push		dword 0x0
	popf
	; eax and ebx contains parameters passed by the bootloader
	push		dword ebx		; multiboot structure
	push		dword eax		; magic number
	call		EXT_C(setup)

halt_loop:
	jmp		$

global EXT_C(gdt_flush)
EXT_C(gdt_flush):
	lgdt	[gdtr]
	mov		ax, 0x10
	mov		ds, ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax
	mov		ss, ax
	jmp		CODE_SEL:flush2
flush2:
	ret

global EXT_C(warm_reboot)
EXT_C(warm_reboot):
	cli
	mov		eax, cr0
	lidt	[reset_IDTR]
	and		eax, 0xfffffffe
	mov		cr0, eax
	sti
real_flush:
	db		0xea
	dw		0x0000
	dw		0xffff

[section .data]
reset_IDTR:
	dw	0x3ff
	dd	0

[section .setup]
_trick_gdt_ptr:
_boot_gdt_ptr:
        dw _boot_gdt_end - _boot_gdt - 1
        dd _boot_gdt

_boot_gdt:
	;; NULL descriptor, needed for flat, no pagination
        dw 0            ; limit 15:0
        dw 0            ; base 15:0
        db 0            ; base 23:16
        db 0            ; access
        db 0            ; limit 19:16, flags
        db 0            ; base 31:24

global CODE_SEL
CODE_SEL equ     $-_boot_gdt
	;; CODE selector (0x08)
        dw 0FFFFh
        dw 0
        db 0
        db 9Ah          ; present,ring 0,code,non-conforming,readable
        db 0CFh         ; page-granular (4 gig limit), 32-bit
        db 04h

global DATA_SEL
DATA_SEL equ     $-_boot_gdt
	;; DATA selector (0x10)
        dw 0FFFFh
        dw 0
        db 0
        db 92h          ; present, ring 0, data, expand-up, writable
        db 0CFh         ; page-granular (4 gig limit), 32-bit
        db 04h

_boot_gdt_end:

_edata:

_end:

[section .bss]
ALIGN 4096
stack:
	resb		STACK_SIZE
_stack:
