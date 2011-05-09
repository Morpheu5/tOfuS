#include <tofus/malloc.h>
#include <setup.h>
#include <video.h>

extern pageDir4kEntry kernelPageDir[1024];
extern pageTab4kEntry lowerPageTable[1024];
extern pageTab4kEntry lowHeapPT[1024];

extern systemInfo sInfo;
extern kernelInfo kInfo;

void* heap_init() {
	// Reserve 4M as initial heap
	void* heap_phys_base = (void*)kInfo.freeMem;
	kInfo.freeMem += 0x400000;

	heap_base = (void*)K_HEAP_BASE;
	heap_ptr = heap_base;
	heap_size = 0;

	u32 pdi = pde(K_HEAP_BASE);
	u32 i;

	for(i = 0; i < 1024; i++) {
		*((u32*) &lowHeapPT[i]) = 0;
		lowHeapPT[i].present = 1;
		lowHeapPT[i].writable = 1;
		lowHeapPT[i].address = pte((u32)heap_phys_base) + i;
	}

	kernelPageDir[pdi].present = 1;
	kernelPageDir[pdi].writable = 1;
	kernelPageDir[pdi].address = ((u32)lowHeapPT - KERNEL_BASE) >> 12;

	return heap_base;
}

void* heap_alloc(u32 bytes) {
	if(heap_size + bytes > K_HEAP_SIZE) {
		kprintf("Kernel out of heap space.\n");
		return 0;
	}
	void* p = heap_ptr;
	heap_ptr += bytes;
	heap_size += bytes;
	return p;
}

void* heap_alloc4kAligned() {
	if(heap_size + 4096 > K_HEAP_SIZE) {
		kprintf("Kernel out of heap space.\n");
		return 0;
	}

	u32 pad = 0x1000 - ((u32)heap_ptr + 0x1000) % 0x1000;
	void* p = heap_ptr + pad;
	heap_ptr += pad + 0x1000;
	heap_size += pad + 0x1000;
	return p;
}

u32 heap_getSize() {
	return heap_size;
}
