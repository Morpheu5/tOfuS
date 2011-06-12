#ifndef MM_H_
#define MM_H_

#include <tofus/common.h>

typedef struct pageDir4kEntry_s {		// 4KiB page table
	u32 present			: 1;	// whether it is in or off the physical memory
	u32 writable		: 1;	// 0 = read only, 1 = read+write
	u32 user			: 1;	// 0 = user, 1 = supervisor
	u32 writeback		: 1;	// 0 = write-through, 1 = write-back -> &~CD
	u32 cachedisable	: 1;	// 0 = caching enabled, 1 = caching disabled
	u32	accessed		: 1;	// sticky, 0 = not accessed, 1 = accessed
	u32 dirty			: 1;	// sticky, 0 = clean, 1 = dirty
	u32 pagesize		: 1; 	// 0 = 4K
	u32 global			: 1;	// ignored for directory entries
	u32 flags			: 3;	// available for the memory manager
	u32 address			: 20;	// base address of the page-table
} pageDir4kEntry;				// SIZE: 32bit (4B)

typedef struct pageTab4kEntry_s {		// 4KiB page table
	u32 present			: 1;	// whether it is in or off the physical memory
	u32 writable		: 1;	// 0 = read only, 1 = read+write
	u32 user			: 1;	// 0 = supervisor, 1 = user
	u32 writeback		: 1;	// 0 = write-through, 1 = write-back -> &~CD
	u32 cachedisable	: 1;	// 0 = caching enabled, 1 = caching disabled
	u32	accessed		: 1;	// sticky, 0 = not accessed, 1 = accessed
	u32 dirty			: 1;	// sticky, 0 = clean, 1 = dirty
	u32 pageattr		: 1; 	// check for support
	u32 global			: 1;	// ignored for directory entries
	u32 flags			: 3;	// available for the memory manager
	u32 address			: 20;	// base address of the page-table
} pageTab4kEntry;				// SIZE: 32bit (4B)

enum { PF_ERROR_P, PF_ERROR_RW, PF_ERROR_SU, PF_ERROR_RSVD, PF_ERROR_ID };

void isr_pageFault(u32, u32);

#endif
