/*
  This file is part of tOfuS.
  Copyright (C) 2004 by tOfuS team

  tOfuS is free software; you can redistribute it and/or modify
  it under the terms of the BSD license. Please look at the COPYING file that
  comes with the distribution. If there's no COPYING nor LICENSE file in there,
  please contact the team.
*/

#include <isr.h>

/** I think the whole thing is quite simple so no need to comment this.
    Maybe a little change in the routine names will occour in the future.
 */

void nisr(void) {
	kprintf("!!! Unhandled exception !!!\n");
}

void isr0(void) {
	kprintf("00 Division by zero.\n");
	asm("hlt");
}

void isr1(void) {
	kprintf("01 DEBUG: Single step.\n");
}

void isr2(void) {
	kprintf("02 Intel reserved\n");
}

void isr3(void) {
	kprintf("03 DEBUG: Breakpoint.\n");
}

void isr4(void) {
	kprintf("04 Aritmethic overflow.\n");
}

void isr5(void) {
	kprintf("05 BIOS Print-screen / Bounds check.\n");
}

void isr6(void) {
	kprintf("06 Invalid OpCode (reserved).\n");
}

void isr7(void) {
	kprintf("07 Coprocessor not available (reserved).\n");
}

void isr8(void) {
	kprintf("08 Double fault.\n");
}

void isr9(void) {
	kprintf("09 Coprocessor Segment Overrun.\n");
}

void isr10(void) {
	kprintf("10 Invalid TSS.\n");
}

void isr11(void) {
	kprintf("11 Segment not present.\n");
}

void isr12(void) {
	kprintf("12 Stack exception.\n");
}

void isr13(u32 error) {
	kprintf("13 General Protection Fault :: %x :: voodoo error\n", error);
}

void isr15(void) {
	kprintf("15 Intel reserved.\n");
}

void isr16(void) {
	kprintf("16 Coprocessor error.\n");
}

volatile u16 count = 0;

void tmr(void) {
/*
 * prints out timestamp in format hh:mm:ss
 */
	/*u8 h, m, s;*/
 	//if((++count % 1024) == 0) {
		/*
		outb(0x70,0);
		s = inb(0x71);
		outb(0x70,2);
		m = inb(0x71);
		outb(0x70,4);
		h = inb(0x71);
		kprintf("%x:%x:%x\n", h, m, s);
		*/
	//	count = 0;
 	//}
}
