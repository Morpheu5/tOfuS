/** \file */

#include <string.h>

/**	Computes the length of the given null terminated string.
 * 
 * \param	a		A string
 * \return			The length of the string
 */
size_t strlen(const char* a) {
	u32 len = 0;
	while(*a++ != '\0') {
		len++;
	}
	
	return len;
}
