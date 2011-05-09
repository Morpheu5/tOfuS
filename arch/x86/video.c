#include <video.h>


static u16 xpos = 0;
static u16 ypos = 0;
static volatile u8 *video = (u8*) VIDEO;
//static volatile u64 *qvideo = (u64*) VIDEO;

/**
 * Clears the screen filling it with \0 and puts back the cursor to (0,0)
 */
void clrscr(void) {
	u32 i = 0;
	u32* v = (u32*) VIDEO;
	while(i < COLUMNS*LINES/2) {
		*(v+i) &= 0x00000000;
		i++;
	}
	
	xpos = 0; ypos = 0;
}

/* Convert the integer D to a string and save the string in BUF. If
   BASE is equal to 'd', interpret that D is decimal, and if BASE is
   equal to 'x', interpret that D is hexadecimal. */
void itoa (char *buf, int base, unsigned long int d) {
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;
	
	/* If %d is specified and D is minus, put `-' in the head. */
	if (base == 'd' && d < 0) {
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x' || base == 'X') {
		divisor = 16;
	}
	
	/* Divide UD by DIVISOR until UD == 0. */
	do {
		int remainder = ud % divisor;
		
		*p++ = (remainder < 10) ? remainder + '0' : remainder + (base=='x'?'a':'A') - 10;
	}
	while (ud /= divisor);

	/* Terminate BUF. */
	*p = 0;
	
	/* Reverse BUF. */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

void putchar (u8 c) {
	if (c == '\n' || c == '\r') {
	newline:
		xpos = 0;
		ypos++;
		if (ypos >= LINES) {
			ypos--;
			scroll();
		}
		return;
	}
	if (c == '\t') {
		if (xpos % 8 == 0) {
			xpos += 8;
		} else {
			xpos += 8 - (xpos % 8);
		}
		return;
	}
	
	*(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
	*(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;
	
	xpos++;
	if (xpos >= COLUMNS)
		goto newline;
}

/* Scrolls one line up. */
void scroll(void) {
	u8* dst = (u8*)VIDEO;
	u8* src = (u8*)VIDEO;
	u32 i = 0;
	while(i < LINES) {
		src += COLUMNS*2;
		memcpy(dst, src, COLUMNS*2);
		dst += COLUMNS*2;
		i++;
	}
}

void kprintf (const char *format, ...) {
	char **arg = (char **) &format;
	int c;
	char buf[40];
	
	arg++;
	
	while ((c = *format++) != 0) {
		if (c != '%') {
			putchar (c);
		} else {
			char *p;
			
			c = *format++;
			switch (c) {
			case 'd':
			case 'u':
			case 'X':
			case 'x':
				itoa (buf, c, *((unsigned long int *) arg++));
				p = buf;
				goto string;
				break;
				
			case 's':
				p = *arg++;
				if (! p)
					p = "(null)";
				
			string:
				while (*p)
					putchar (*p++);
				break;
				
			default:
				putchar (*((int *) arg++));
				break;
			}
		}
	}
	outb(0x3D4, 15);
	outb(0x3D5, (u8)((xpos+ypos*COLUMNS)*2));
	outb(0x3D4, 14);
	outb(0x3D5, (u8)(((xpos+ypos*COLUMNS)*2)>>8));
}

void printFlags(const char* name, u32 flags) {
	u8 buff[33];
	u8 c;
	for (c = 0; c < 32; c++) {
		buff[c] = 0;
	}
	buff[32] = 0;
	u32 test = 0;
	s32 i = 31;
	while (i >= 0) {
		if ( (test | (1 << i)) <= flags ) {
			test |= 1 << i;
			buff[31-i] = '1';
		} else {
			buff[31-i] = '0';
		}
		i--;
	}
	kprintf("[ %s ]\n", name);
	kprintf("  28  24  20  16  12   8   4   0\n");
	kprintf("%s\n", buff);
	return;
}

void printDebug(const char *format, ...) {
#ifndef __DEBUG_OUT__
	return;
#endif
	char **arg = (char **) &format;
	int c;
	char buf[40];

	arg++;

	while ((c = *format++) != 0) {
		if (c != '%')
			putchar (c);
		else {
			char *p;

			c = *format++;
			switch (c) {
			case 'd':
			case 'u':
			case 'X':
			case 'x':
				itoa (buf, c, *((unsigned long int *) arg++));
				p = buf;
				goto string;
				break;

			case 's':
				p = *arg++;
				if (!p)
					p = "(null)";

			string:
				while (*p)
					putchar (*p++);
				break;

			default:
				putchar (*((int *) arg++));
				break;
			}
		}
	}
	outb(0x3D4, 15);
	outb(0x3D5, (u8)((xpos+ypos*COLUMNS)*2));
	outb(0x3D4, 14);
	outb(0x3D5, (u8)(((xpos+ypos*COLUMNS)*2)>>8));
}
