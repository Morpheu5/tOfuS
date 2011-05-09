/** \file
 * This file contains all the definitions about strings and memory.
 * 
 * Functions in this file may be able to take advantage of 4B boundary alignment
 * of processed data on 32 bit machines.
 */

#ifndef STRING_H_
#define STRING_H_

#include <types.h>

/** Checks whether two given values are aligned to 4B boundary.
 */
#define UNALIGNED(X, Y) \
	(((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))

u32 strcmp(const char*, const char*);
size_t strlen(const char*);

u32 memcmp(const void*, const void*, size_t);
void* memcpy(void*, const void*, size_t);

#endif /*STRING_H_*/
