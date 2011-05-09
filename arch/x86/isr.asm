;; interrupt gate -> clear IF
;; trap gate -> unmodified IF

%include "defines.asm"

extern DATA_SEL
extern CODE_SEL

	; ISR_enter <hnd_name>
	;   <hnd_name>	the handler function name
%macro ISR_enter 1
	extern	EXT_C(%1)
	call		EXT_C(%1)
%endmacro

%macro ISR_leave_m 0
;	push		eax
	mov		al, 0x20
	out		0x20, al
;	pop		eax
	iretd
%endmacro

%macro ISR_leave_s 0
;	push		eax
	mov	al, 0x20
	out	0x20, al
	out	0xa0, al
;	pop		eax
	iretd
%endmacro

	;; default handler
global EXT_C(b_nisr)
global EXT_C(b_nisr)
EXT_C(b_nisr):
	ISR_enter nisr
	ISR_leave_m

	;; division by zero (fault)
global EXT_C(b_isr0)
global EXT_C(b_isr0)
EXT_C(b_isr0):
	ISR_enter isr0
	ISR_leave_m

	;;  reserved (fault/trap)
global EXT_C(b_isr1)
EXT_C(b_isr1):
	ISR_enter isr1
	ISR_leave_m

	;; NMI interrupt (interrupt)
global EXT_C(b_isr2)
EXT_C(b_isr2):
	ISR_enter isr2
	ISR_leave_m

	;; Breakpoint (trap)
global EXT_C(b_isr3)
EXT_C(b_isr3):
	ISR_enter isr3
	ISR_leave_m

	;; Overflow (trap)
global EXT_C(b_isr4)
EXT_C(b_isr4):
	ISR_enter isr4
	ISR_leave_m

	;; BOUND range exceeded (fault)
global EXT_C(b_isr5)
EXT_C(b_isr5):
	ISR_enter isr5
	ISR_leave_m

	;; Invalid Opcode (fault)
global EXT_C(b_isr6)
EXT_C(b_isr6):
	ISR_enter isr6
	ISR_leave_m

	;; Device not available, no math coprocessor (fault)
global EXT_C(b_isr7)
EXT_C(b_isr7):
	ISR_enter isr7
	ISR_leave_m

	;; Double fault (abort) (E)
global EXT_C(b_isr8)
EXT_C(b_isr8):
	ISR_enter isr8
	ISR_leave_s

	;; Coprocessor segment overrun (fault)
global EXT_C(b_isr9)
EXT_C(b_isr9):
	ISR_enter isr9
	ISR_leave_s

	;; Invalid TSS (fault) (E)
global EXT_C(b_isr10)
EXT_C(b_isr10):
	ISR_enter isr10
	ISR_leave_s

	;; Segment not present (fault) (E)
global EXT_C(b_isr11)
EXT_C(b_isr11):
	ISR_enter isr11
	ISR_leave_s

	;; Stack segment fault (fault) (E)
global EXT_C(b_isr12)
EXT_C(b_isr12):
	ISR_enter isr12
	ISR_leave_s

	;; General Protection fault (fault) (E)
global EXT_C(b_isr13)
EXT_C(b_isr13):
	pop		eax
	push	eax
	ISR_enter isr13

	hlt
 	jmp	$

	ISR_leave_s

	; Page fault (fault) (E)
global EXT_C(b_isr14)
EXT_C(b_isr14):
	mov		eax, cr2
	push		eax

	ISR_enter isr_pageFault
	ISR_leave_s

	; Reserved (do not use)
global EXT_C(b_isr15)
EXT_C(b_isr15):
	ISR_enter isr15
	ISR_leave_s

	; x87 FPU Floating-Point Error (fault)
global EXT_C(b_isr16)
EXT_C(b_isr16):
	ISR_enter isr16
	ISR_leave_s

	;; Alignment check (fault) (E)
	;; Machine check (abort)
	;; SIMD floating point exception

	;; System timer 8253/54
global EXT_C(b_tmr)
EXT_C(b_tmr):
	ISR_enter tmr
	ISR_leave_m

	;; Keyboard
global EXT_C(b_kbd)
EXT_C(b_kbd):
	ISR_enter b_keyboardHandler
	ISR_leave_m
