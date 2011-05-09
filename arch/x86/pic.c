#include <pic.h>

void remapPIC(u8 pic1, u8 pic2) {
	// backup master and slave data
	u8 md, sd;
	md = inb(MASTERD);
	sd = inb(SLAVED);

	// reset-chip sending him the mystical End Of Interrupt command
	outb(MASTER, EOI);

	/** gotta check in the 8259 datasheet for this sequence
	    although is quite simple.
	 */
	outb(MASTER, ICW1_INIT+ICW1_ICW4);
	outb(SLAVE, ICW1_INIT+ICW1_ICW4);

	outb(MASTERD, pic1);
	outb(SLAVED, pic2);

	outb(MASTERD, 0x4);
	outb(SLAVED, 0x2);

	outb(MASTERD, ICW4_8086);
	outb(SLAVED, ICW4_8086);

	/** restore previously saved DATA
	    I'm not so sure this is necessary but I don't want to mess up things, gotta check.
	 */
	outb(MASTERD, md);
	outb(SLAVED, sd);
}

/** Mystical procedures... gotta check their flow, they don't seem to work
	properly.
    Use outb functions instead, for now, check setup.c for details.
 */
void maskIRQ(u16 irq) {
        if(irq == ALL) {
                outb(MASTERD,0xFF);
                outb(SLAVED,0xFF);
        }
        else {
                irq = irq | (1 << irq);
                if(irq < 8)
                        outb(MASTERD, irq & 0xFF);
                else
                        outb(SLAVED, irq >> 8);
        }
}

/** Opposite of above, same advice
 */
void unmaskIRQ(u32 irq) {
        if(irq == ALL) {
                outb(MASTERD,0x00);
                outb(SLAVED,0x00);
        }
        else {
                irq = irq & (1 << irq);
                if(irq < 8)
                        outb(MASTERD, irq & 0xFF);
                else
                        outb(SLAVED, (irq >> 8) & 0xFF);
        }
}
