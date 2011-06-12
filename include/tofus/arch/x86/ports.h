#ifndef PORTS_H_
#define PORTS_H_

#include <types.h>

void outb(u16, u8);
void outw(u16, u16);
u8 inb(u16);
u16 inw(u16);

#endif /*PORTS_H_*/
