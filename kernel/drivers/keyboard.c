#include <video.h>
#include <ports.h>

#include <keyboard.h>

/* What should this do?
 * 
 * Acquire a lock over a FIFO queue, enqueue the just arrived scancode and
 * reset the keyboard controller. If the queue is full, just wait.
 * 
 * Depends on: numerical semaphores, FIFO queue.
 * 
 * 
 * What's next?
 * 
 * A separate process will try to extract the next scancode from the queue,
 * decode it and do all the useful things.
 * 
 * Depends on: process scheduler, basical keyboard handler.
 */

static u8 ctrl = 0;
static u8 alt = 0;
static u8 shift = 0;
static u8 gray = 0;

void b_keyboardHandler(void) {
	u8 code, value;
	
	code = inb(KEYB_PORT);		// get scancode
	value = inb(KEYB_ACK);		// get acknowledgement
	outb(KEYB_ACK, value|0x80);	// disable 7th bit
	outb(KEYB_ACK, value);		// send the data back and reenable the controller
	// kprintf("%X ", code);

	if (code == 0xe0) {
		gray = 0xff;
		return;
	}
	
	switch(code) {
		case 0x36:
			shift |= 0x0f;
			break;
		case 0x2a:
			shift |= 0xf0;
			break;
		case 0xb6:
			shift &= ~0x0f;
			break;
		case 0xaa:
			shift &= ~0xf0;
			break;
		case 0x1d:
			ctrl |= 0xf0;
			break;
		case 0x9d:
			ctrl &= ~0xf0;
			break;
		case 0x38:
			alt |= 0xf0;
			break;
		case 0xb8:
			alt &= ~0xf0;
			break;
		case 0x53:
			if(ctrl != 0 && alt != 0) {
				kprintf("SUDDEN DEATH!\n");
				//u32* p = (u32*)0x472;
				//*p = 0x1234;			// WE LIKE IT WARM
				warm_reboot();
			}
			break;
	}
//	kprintf("%d %d %d\n", ctrl, alt, shift);
	
	if (ctrl != 0 && alt != 0 && shift != 0) {
		// We have a problem on contemporary equivalent key pressed
		kprintf("GOGOGO!!!\n");
	}

	gray = 0x0;
}
