/** \file */

#include <string.h>

/**	Copies a given amount of bytes from one location to another.
 * 
 * \param	to		Destination
 * \param	from	Source
 * \param	length	Length of the area in bytes
 */

void* memcpy(void* to, const void* from, size_t length) {
	u32* d = (u32*) to;
	u32* s = (u32*) from;
	u8* db = (u8*) d;
	u8* sb = (u8*) s;

	if(!UNALIGNED(to, from)) {
		while(length >= sizeof(u32)) {
			*d = *s;
			d++; s++;
			length -= sizeof(u32);
		}
		db = (u8*) d;
		sb = (u8*) s;
	}

	while(length--) {
		*db = *sb;
		db++; sb++;
	}

	return to;
}
