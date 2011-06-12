#ifndef IDT_H_
#define IDT_H_

#include <types.h>
#include <pm.h>

typedef struct descreg_s {
	u16	limit	: 16;	// limit
	u32	base	: 32;	// base
} descreg;

void installGate(u16, gatedesc*, u32, u32, u8, u8);

void loadIDT(u16, gatedesc*);

#endif /*IDT_H_*/
