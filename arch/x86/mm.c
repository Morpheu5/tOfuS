#include <tofus/malloc.h>
#include <setup.h>
#include <mm.h>

extern systemInfo sInfo;

void isr_pageFault(u32 cr2, u32 error) {
	kprintf("A ");
	if(CHECK_FLAG(error, PF_ERROR_P)) {
		kprintf("page level protection violation ");
	} else {
		kprintf("non-present page error ");
	}
	kprintf("has been triggered by a ");
	if(CHECK_FLAG(error, PF_ERROR_RW)) {
		kprintf("write ");
	} else {
		kprintf("read ");
	}
	kprintf("operation ");
	if(CHECK_FLAG(error, PF_ERROR_RSVD)) {
		kprintf("on a reserved page ");
	}
	kprintf("while executing in ");
	if(CHECK_FLAG(error, PF_ERROR_SU)) {
		kprintf("user ");
	} else {
		kprintf("supervisor ");
	}
	kprintf("mode at location %x.\n", cr2);
//
//	// TODO: Refine the mechanism, we are allocating whole 4MB intervals...
//	u32 directory = (cr2 & 0xffc00000) >> 22;
//	u32 table = (cr2 & 0x3ff000) >> 12;
//
//	pageTab4kEntry* newPageTable = (pageTab4kEntry*)heap_alloc4kAligned();
//	kprintf("new page table allocated at %x\n", newPageTable);
//
//	u32 i;
//	for(i = 0; i < 1024; i++) {
//		*((u32*) &newPageTable[i]) = 0;
//		newPageTable[i].present = 1;
//		newPageTable[i].writable = 1;
//		newPageTable[i].address = table + i;
//	}
//
//	kernelPageDir[directory].present = 1;
//	kernelPageDir[directory].writable = 1;
//	kernelPageDir[directory].address = (u32)newPageTable >> 12;
}

