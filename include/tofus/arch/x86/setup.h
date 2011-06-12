#ifndef SETUP_C_
#define SETUP_C_

#include <types.h>
#include <pm.h>
#include <isr.h>
#include <idt.h>
#include <mm.h>
#include <video.h>
#include <pic.h>
#include <elf.h>

#include <cpuid.h>

#include <string.h>

#include <tofus/common.h>

#include <tofus/multiboot.h>
#include <tofus/boot/module.h>
#include <tofus/semaphore.h>
#include <tofus/timer.h>

#include <tofus/mmap.h>
#include <tofus/malloc.h>

typedef struct kernelInfo_s {
	void*			address;	// The address where the kernel has been loaded
	void*			freeMem;	// Pointer to the first 4k aligned free memory frame
	char*			cmdline;	// The command line, if any
	pageDir4kEntry*	pageDir;	// Pointer to the Page Directory
	mmap_block*		memoryMap;	// Pointer to the memory map
} kernelInfo;

typedef struct systemInfo_s {
	kernelInfo*	kInfo;
	u32			physicalMem;	// Total physical memory in kB
	u32			bootDevice;		// From where we've been booted
} systemInfo;

gatedesc* idt;
descreg idtr_p;

segmentdesc gdt[8];
GDTr gdtr;

systemInfo sInfo;
kernelInfo kInfo;

CPUid cpuInfo;

void gdt_flush(void);

pageDir4kEntry kernelPageDir[1024] __attribute__ ((aligned (4096)));
pageTab4kEntry lowerPageTable[1024] __attribute__ ((aligned (4096)));
pageTab4kEntry lowHeapPT[1024] __attribute__ ((aligned (4096)));

void setup(u32, multiboot_info*);
void init_paging(void);

#define pde(a) \
	(a >> 22)

#define pte(a) \
	((a & 0x3ff000) >> 12)

#endif /*SETUP_C_*/
