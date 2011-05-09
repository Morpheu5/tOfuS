 /** Setup of the operating environment.
 * The main procedure in this file is setup(u32, multiboot_info_t*) and its
 * purpose is to set up the basical working environment: interrupts, RTC, MM...
 */

#include <setup.h>

u8* v;
u32* p;

#define	TRICK_OFF	(0xffffffff - KERNEL_BASE) + 1

void init_paging(void) {
	pageDir4kEntry* pd = (pageDir4kEntry*) ((u32)kernelPageDir + TRICK_OFF);
	pageTab4kEntry* pt = (pageTab4kEntry*) ((u32)lowerPageTable + TRICK_OFF);

	/* Initialize the page tables for 0-4MB.
	 * Each entry accounts for a 4KB page, so a full page table addresses 4MB.
	 */
	u32 i;
	for(i = 0; i < 1024; i++) {
		*((u32*) &lowerPageTable[i]) = 0;
		lowerPageTable[i].present = 1;
		lowerPageTable[i].writable = 1;
		lowerPageTable[i].address = i;
		*((u32*) &kernelPageDir[i]) = 0;
	}
	/* Make the video memory uncached. */
	for(i = 0; i < 0x18; i++) {
		lowerPageTable[0xa0 + i].cachedisable = 1;
	}

	/* Insert 0-4MB interval in the main directory of page tables. */
	kernelPageDir[0].present = 1;
	kernelPageDir[0].writable = 1;
	kernelPageDir[0].address = ((u32) pt) >> 12;

	/* Insert 0xfc000000-0xfc3fffff interval in the main directory of page
	 * tables and map it to 0-4MB physical. */
	kernelPageDir[pde(KERNEL_BASE)].present = 1;
	kernelPageDir[pde(KERNEL_BASE)].writable = 1;
	kernelPageDir[pde(KERNEL_BASE)].address = ((u32) pt) >> 12;

	writeCR3((u32)pd);
	writeCR0(readCR0() | 0x80000000);
}

void init_gdt(void) {
	gdt[0] = nullSegDesc();
	gdt[1] = initSegDesc(0x0, 0xfffff, SEGT_EXECUTE | CS_READ, DESCT_CODE, DPL0, 1, 0, 1, 1);
    gdt[2] = initSegDesc(0x0, 0xfffff, DS_WRITE, DESCT_DATA, DPL0, 1, 0, 1, 1);
	gdt[3] = nullSegDesc();
	gdt[4] = nullSegDesc();
	gdt[5] = nullSegDesc();
	gdt[6] = nullSegDesc();

	gdtr.limit = (sizeof(segmentdesc) * 7) - 1;
	gdtr.base = (u32) gdt;

	gdt_flush();
}

void init_idt(void) {
	u32 i;
	/* Initialize the IDT pointer, let's make it point to linear 0x500.
	 * Please note we can only use the range 0x500-0x9ffff for doing stuff
	 * here. */
	idt = (gatedesc*) 0x500;

	// mask ALL irq lines
	maskIRQ(ALL);

	// remap PIC
	remapPIC(0x20, 0x28);

	// fill the IDT
	// . put 256 default NULL ISRs all over the IDT
	for(i = 0x10; i < 256; i++) {
		installGate(i, idt, (u32)b_nisr, 0x8, SS_32BIT_INTGATE, 0);
	}
	installGate(0x0, idt, (u32)b_isr0, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x1, idt, (u32)b_isr1, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x2, idt, (u32)b_isr2, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x3, idt, (u32)b_isr3, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x4, idt, (u32)b_isr4, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x5, idt, (u32)b_isr5, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x6, idt, (u32)b_isr6, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x7, idt, (u32)b_isr7, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x8, idt, (u32)b_isr8, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0x9, idt, (u32)b_isr9, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0xa, idt, (u32)b_isr10, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0xb, idt, (u32)b_isr11, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0xc, idt, (u32)b_isr12, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0xd, idt, (u32)b_isr13, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0xe, idt, (u32)b_isr14, 0x8, SS_32BIT_INTGATE, 0);
	installGate(0xf, idt, (u32)b_isr15, 0x8, SS_32BIT_INTGATE, 0);

	// int 0x00 :: system clock
	installGate(0x20, idt, (u32)b_tmr, 0x8, SS_32BIT_INTGATE, 0);

	// int 0x21 :: keyboard
	installGate(0x21, idt, (u32)b_kbd, 0x8, SS_32BIT_INTGATE, 0);

	// . load the IDT into idtr register
		// this is odd, defined in intutils.asm
		// maybe there's a way to do this better
	loadIDT(256*sizeof(gatedesc), idt);
	unmaskIRQ(ALL);
	asm __volatile__ ("sti");
}

void printSystemInfo(void) {
	printDebug("\nSYSTEM INFORMATIONS\t%x\n", &sInfo);
	printDebug("\t* physical mem:\t%d MB\n", sInfo.physicalMem >> 10);
	printDebug("\t* boot device:\t0x%x\n", sInfo.bootDevice);
}

void printKernelInfo(void) {
	printDebug("\nKERNEL INFORMATIONS\t%x\n", &kInfo);
	printDebug("\t* address:\t0x%x\n", sInfo.kInfo->address);
	printDebug("\t* free memory:\t0x%x\n", sInfo.kInfo->freeMem);
	printDebug("\t* cmdline:\t%s\n", sInfo.kInfo->cmdline);
	printDebug("\t* page dir:\t0x%x\n", sInfo.kInfo->pageDir);
	printDebug("\t* memory map:\t0x%x\n", sInfo.kInfo->memoryMap);
}

void printCPUID(void) {
	printDebug("\nCPU IDENTIFICATION\t%x\n", &cpuInfo);
	printDebug("\t* vendor:\t%s\n", cpuInfo.vendor);
	printDebug("\t* stepping:\t%x\n", cpuInfo.signature.stepping);
	printDebug("\t* model:\t%d\n", cpuInfo.signature.model + (cpuInfo.signature.e_model << 4));
	printDebug("\t* family:\t%d\n", cpuInfo.signature.family + cpuInfo.signature.e_family);
	printDebug("\t* type:\t\t%x\n", cpuInfo.signature.type);
	printFlags("Feature EDX", cpuInfo.featFlags.edx);
	printFlags("Feature ECX", cpuInfo.featFlags.ecx);
}

void setup(u32 magic, multiboot_info* mbi) {
	v = (u8*) 0xb8000 + KERNEL_BASE;
	p = (u32*) 0xd00 + KERNEL_BASE;

	u32 i;

	*v++ = 'D'; *v++ = 0x0a;

	init_paging();

	*v++ = 'E'; *v++ = 0x0f;

	init_gdt();

	*v++ = 'I'; *v++ = 0x0c;

	/* From this moment, paging is enabled and the kernel is virtually loaded
	 * at 0xfc000000 (but physically loaded at 0x100000).
	 * Initally the kernel sees just 4M of this space since just one page table
	 * has been initialized.
	 *
	 * TODO: Make this lower bound configurable by the user. */

	/* Clear the screen and greet the user. */
	clrscr();
	kprintf("tOfuS 0.0.0 (built on %s)\n\n", BUILDTIMESTAMP);

	// ASSERT: mbi->flags bits 4 and 5 are mutually exclusive
	if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)) {
		printDebug("Flags 4 and 5 in Multiboot Structure are both set\n");
		return;
	}

	printDebug("\nRTC: ");
	/* mode 3, write LSB followed by write of MSB, counter 0
	 * Program counter 0 to generate HZ ticks per second.
	 * HZ is defined in timer.h. */
	setupPit(COUNTER_DIVIDER);
	printDebug("working at %d Hz.", HZ);

	/* Install a basical set of ISRs: exceptions and so on... */
	printDebug("\nIDT: ");
	init_idt();
	printDebug("loaded at %x linear\n\t* gate descriptor is %d byte long\n\t* table ends at %x linear (first free byte)", idt, sizeof(gatedesc), ((u32)idt)+2048);

	sInfo.kInfo = &kInfo;
	kInfo.pageDir = kernelPageDir;

	if (CHECK_FLAG(mbi->flags, 0)) {
		sInfo.physicalMem = mbi->mem_upper + 1024;
	}

	if (CHECK_FLAG(mbi->flags, 1)) {
		sInfo.bootDevice = mbi->boot_device;
	}

	if (CHECK_FLAG(mbi->flags, 2)) {
		kInfo.cmdline = (char*)mbi->cmdline;
	}

	if (CHECK_FLAG(mbi->flags, 5)) {
		elf_section_header_table sht = mbi->elf_sec;
		printDebug("\nELF: %d sections of %d B\n", sht.num, sht.size);
		printDebug("\t* address:\t%x\n", sht.addr);

		// We have found the section header
		// First entry should be null (type = SHT_NULL)
		Elf32_SectionHeader* sh = (Elf32_SectionHeader*) (sht.addr);
		u32* strtab = (u32*) sh[sht.shndx].addr;
		printDebug("\t* strings at %x\n", (u32)strtab);

		for(i = 0; i < sht.num; i++) {
			if(i == 1) {
				kInfo.address = (u32*)sh[i].addr;
			}
//			printDebug("%s\t::\tt(%x)\ta(%x)\ts(%x)\n", (char*) strtab + sh[i].name, sh[i].type, sh[i].addr, sh[i].size);
		}

		/* Can we assume that a multiboot loader loads the sections one after
		 * the other when nothing else has been specified? If so, the entire
		 * kernel image ends at the end of the last section, therefore we can
		 * compute its size.
		 */
		void* freespace = (void*)(sh[sht.num - 1].addr + sh[sht.num - 1].size);
		u32 f = (u32)freespace;
		f += 0x1000;
		f &= 0xfffff000;
		freespace = (u32*)f;
		kInfo.freeMem = freespace;
	}

	/* Map the heap's initial 4M (out of ~200M).
	 * In this moment the memory map is the following:
	 *
	 * KERNEL .text + .data + .bss
	 * 0xfc000000 -> 0xffffffff
	 *
	 * HEAP
	 * 0xf0000000 -> 0xfbffffff
	 */

	heap_init();

	/* Modules loading is a great feature by multiboot compliant bootloaders
	 * like the almighty GRUB. Please note that a "module" is a file grabbed
	 * from the rootfs and simply contiguously loaded into memory. It is up to
	 * us how to use it. The typical case is when the file is an ELF and we want
	 * to be able to link the code and execute it. */
	if (CHECK_FLAG(mbi->flags, 3)) {
		// We've loaded modules, let's see...
		u32 mods_count = mbi->mods_count;
		printDebug("%d module(s) loaded.\n", mods_count);
		// Say we want to know what modules we have...
		multiboot_module* mod = (multiboot_module*) mbi->mods_addr;
		for (i = 0; i < mods_count; i++) {
			// Increment busy space
			kInfo.freeMem += mod[i].mod_end - mod[i].mod_start;
			u32 f = (u32)kInfo.freeMem;
			f += 0x1000;
			f &= 0xfffff000;
			kInfo.freeMem = (void*)f;
			// Detect module's type and take the appropriate action
			switch(module_type((void*)mod[i].mod_start)) {
			case MODULE_ELF:
				module_elf_printInfo((void*)mod[i].mod_start);
//				module_elf_findText((void*)mod[i].mod_start);
				break;
			case MODULE_RAW:
			default:
				break;
			}
		}
	}

	// Detect CPU features
	cpuid();

	// Kernel's TSS
	tss* kTss = (tss*)heap_alloc(sizeof(tss));

	gdt[3] = initSegDesc(sizeof(tss), (u32)kTss, SS_32BIT_TSS_BUSY, DESCT_SYSTEM, DPL0, 1, 1, 0, 0);

	/* The memory map is something we'd worship: we can somehow rely on it to
	 * build an initial mapping for the memory allocator and manager.
	 */
	mmap_block* memoryMap;

	i = 0;
	u32 base, size;
	void* physFree = 0;
	if (CHECK_FLAG(mbi->flags, 6)) {
		multiboot_memoryMap* mmap;
		u32 mmap_base = mbi->mmap_addr;
		u32 mmap_size = mbi->mmap_length / sizeof(multiboot_memoryMap);
		physFree = (void*)((mmap_base + mmap_size + 0x1000) & 0xfffff000);

		memoryMap = (mmap_block*)heap_alloc(mmap_size*sizeof(mmap_block));

		printDebug("\nMemory Map :: base: 0x%x, size: 0x%x\n", mmap_base, mmap_size);

		mmap = (multiboot_memoryMap*) (mbi->mmap_addr);

		while((mmap < (multiboot_memoryMap*) mmap_base + mmap_size)) {
			base = mmap->base_addr_low;

			if(base != 0x100000) {
				memoryMap[i].base_32 = mmap->base_addr_low;
				memoryMap[i].size_32 = mmap->length_low;
				memoryMap[i].available = mmap->type == 1 ? MMAP_FREE : MMAP_RESERVED;
				memoryMap[i].pid = 0;
				memoryMap[i].next = &memoryMap[i+1];
				memoryMap[i].prev = &memoryMap[i-1];
			} else {
				size = mmap->length_low;

				memoryMap[i].base_32 = 0x100000;
				memoryMap[i].size_32 = (u32)sInfo.kInfo->freeMem - 0x100000;
				memoryMap[i].available = MMAP_ALLOC;
				memoryMap[i].pid = 0;
				memoryMap[i].next = &memoryMap[i+1];
				memoryMap[i].prev = &memoryMap[i-1];
				i++;
				memoryMap[i].base_32 = (u32)sInfo.kInfo->freeMem;
				memoryMap[i].size_32 = (u32) (u32)size - (u32)((u32)sInfo.kInfo->freeMem - 0x100000);
				memoryMap[i].available = MMAP_FREE;
				memoryMap[i].pid = 0;
				memoryMap[i].next = &memoryMap[i+1];
				memoryMap[i].prev = &memoryMap[i-1];
			}
			i++;
			mmap = (multiboot_memoryMap*) ((u32) mmap + mmap->size + sizeof(mmap->size));
		}
		memoryMap[i-1].next = 0;
		memoryMap[0].prev = 0;
		memoryMap[0].available = MMAP_RESERVED;

		sInfo.kInfo->memoryMap = &memoryMap[0];

//		mmap_block* mp = memoryMap;
//		while(mp) {
//			printDebug("%d %x %x %d\n",	mp->pid,
//										mp->base_32,
//										mp->size_32,
//										mp->available);
//			mp = mp->next;
//		}
//		printDebug("%d entries (%dB) in the initial memory map\n", i, sizeof(mmap_block));
	} else {
		kprintf("No memory map, how can I proceed?\n");
		return;
	}
	printDebug("\n");

	/* Now, let's make a page table that spans beyond 4MB, precisely mapping the
	 * page that contains kInfo.freeMem.
	 */
	pageTab4kEntry* jumpstartPT = (pageTab4kEntry*)physFree;
	physFree += 0x1000;

	u32 dirEntry = (u32)kInfo.freeMem >> 22;
	u32 tableEntry = (u32)jumpstartPT >> 12;
	u32 offset = ((u32)kInfo.freeMem >> 12) & 0x3ff;

	*((u32*)jumpstartPT) = 0;
	jumpstartPT[offset].present = 1;
	jumpstartPT[offset].writable = 1;
	jumpstartPT[offset].address = offset; // identimap
	kInfo.pageDir[dirEntry].present = 1;
	kInfo.pageDir[dirEntry].writable = 1;
	kInfo.pageDir[dirEntry].address = tableEntry;

	kprintf("physFree: %x (first free 4k page frame)\n", physFree);

	/*
	printSystemInfo();
	printKernelInfo();

	printDebug("%x\n", (u32*)heap_alloc(1));
	printCPUID();
	/**/
}
