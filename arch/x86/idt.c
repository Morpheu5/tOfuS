#include <idt.h>

void installGate(u16 number, gatedesc* table, u32 offset, u32 selector, u8 type, u8 dpl) {
	(*(table+number)).lowOffset = offset & 0xffff;
	(*(table+number)).segSelector = selector;
	(*(table+number)).zero1 = 0;
	(*(table+number)).type = type;
	(*(table+number)).zero2 = 0;
	(*(table+number)).dpl = dpl;
	(*(table+number)).present = 1;
	(*(table+number)).highOffset = offset >> 16;
}
