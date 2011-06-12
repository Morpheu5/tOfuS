#ifndef MALLOC_H_
#define MALLOC_H_

#include <tofus/common.h>
#include <setup.h>

void* heap_base;
void* heap_ptr;
u32 heap_size;

/** Initializes the kernel heap. It sets up the required space and returns a
 *  void pointer to the base. This pointer contains the virtual address of the
 *  heap.
 */
void* heap_init();

/** Allocates a specified amount of bytes in the kernel heap. It returns a void
 *  pointer to the first free byte requested.
 */
void* heap_alloc(u32);

/** Allocates 0x1000 bytes aligned to 0x1000 boundaries. It returns a void
 *  pointer to the first free bytw requested.
 */
void* heap_alloc4kAligned();

#endif /*MALLOC_H_*/
