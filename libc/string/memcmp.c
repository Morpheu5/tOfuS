/** \file */

#include <string.h>

/**	Compares two memory locations for a given amount of bytes.
 *
 * \param	m1		First memory location
 * \param	m2		Second memory location
 * \param	length	Length of the area in bytes
 * \return			The difference between the values in the first different
 * 					position
 * \see		 		\ref strcmp
 */
u32 memcmp(const void* m1, const void* m2, size_t length) {
	u32* a = (u32*) m1;
	u32* b = (u32*) m2;
	u8* ab = (u8*) a;
	u8* bb = (u8*) b;
	
	if(!UNALIGNED(m1, m2)) {
		while(length >= sizeof(u32)) {
			if(*a != *b) {
				break;
			}
			a++; b++;
			length -= sizeof(u32);
		}
		ab = (u8*) a;
		bb = (u8*) b;
	}

	while(length--) {
		if(*ab != *bb) {
			return *ab - *bb;
			ab++; bb++;
		}
	}

	return 0;
}
