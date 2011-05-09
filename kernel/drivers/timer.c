#include <tofus/timer.h>

void setupPit(u16 rate) {
	// Load mode control register
	outb(0x43, 0x36);

	// Load the divider's LSB
	outb(0x40, (u8)rate);

	// Load the divider's MSB
	outb(0x40, (u8)(rate >> 8));
}
