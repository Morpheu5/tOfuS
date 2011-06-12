#ifndef PIC_H_
#define PIC_H_

#include <ports.h>

#define MASTER 0x20
#define MASTERD 0x21
#define SLAVE 0xA0
#define SLAVED 0xA1
#define EOI 0x20
#define ALL 0xFF

#define ICW1_INIT 0x10
#define ICW1_EDGE 0x8
#define ICW1_SINGLE 0x02
#define ICW1_ICW4 0x01

#define ICW4_SFNM 0x10
#define ICW4_BUFFER 0x08
#define ICW4_MASTER 0x04
#define ICW4_AEOI 0x02
#define ICW4_8086 0x01

void remapPIC(u8, u8);
void maskIRQ(u16);
void unmaskIRQ(u32);

#endif /*PIC_H_*/
