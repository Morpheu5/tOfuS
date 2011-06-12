#ifndef MMAP_H_
#define MMAP_H_

#include <types.h>
#include <mm.h>

/**
 * Enumerates the possible mmap_block.available states.
 * MMAP_INVALID: This doesn't describe a valid block, thus it is available for
 * describing a new block;
 * MMAP_FREE: This describes a free memory chunk;
 * MMAP_ALLOC: This describes an allocated chunk of memory;
 * MMAP_RESERVED: This describes a special chunk of memory that shouldn't be
 * touched.
 */
enum mmap_avl { MMAP_INVALID, MMAP_FREE, MMAP_ALLOC, MMAP_RESERVED };

typedef struct mmap_block_s {
#if __BITS__ == 32
	u32 base_32;
	u32 size_32;
#else	// __BITS__ 64
	union {
		struct {
			u32 low;
			u32 high;
		} base_32;
		u64	base_64;
	};
	union {
		struct {
			u32 low;
			u32 high;
		} size_32;
		u64 size_64;
	};
#endif
	enum mmap_avl		available	: 2;	// See enum mmap_avl above
	u32					flags		: 30;	// Reserved for future expansion
	u32					pid;				// The pid of the owner process
	struct mmap_block_s	*next;
	struct mmap_block_s	*prev;
} mmap_block;		// 24/32B

#endif /*MMAP_H_*/
