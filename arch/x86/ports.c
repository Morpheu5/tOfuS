/* ports.c
 * 
 * please note: intel syntax
 * 
 * Routines for I/O memory mapped devices
 * 
 * naming convention: <direction><size>
 * 		direction: <in|out> (input, output)
 * 		size: <b|w> (byte, word)
 */

#include <ports.h>

void outb(u16 port, u8 value) {
	asm("outb %1, %0"
	    :
	    :"a"(value), // %0
	    "Nd"(port)   // %1
		);
}

void outw(u16 port, u16 value) {
	asm("outw %1, %0"
	    :
	    :"a"(value), // %0
	    "Nd"(port)   // %1
		);
}

u8 inb(u16 port) {
	u8 value;
	asm("inb %0, %1"
	    :"=a"(value) // %0
	    :"Nd"(port)  // %1
		);
	return value;
}

u16 inw(u16 port) {
	u16 value;
	asm("inw %0, %1"
	    :"=a"(value) // %0
	    :"Nd"(port)  // %1
		);
	return value;
}
