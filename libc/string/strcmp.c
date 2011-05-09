/** \file */

#include <string.h>

/**	Compares two zero terminated strings
 * 
 * \param	a		First string
 * \param	b		Second string
 * \return			The difference between the values in the first different
 * 					character
 * \see		 		\ref memcmp
 */
u32 strcmp(const char* a, const char* b) {
	while (*a != '\0' && *a == *b) {
		a++;
		b++;
	}
	return (u32) (*(char*) a) - (*(char*) b);
}
