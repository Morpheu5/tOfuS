/*
 * This file is part of tOfuS
 * Copyright (C) 2005 by tOfuS team
 * http://tofus.morpheu5.net/
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Original author: Andrea Franceschini <andrea.franceschini@gmail.com>
 */

#ifndef PM_H_
#define PM_H_

#include <types.h>

// System segments and gate descriptor types
// Just put these values into the type field like this:
//   segdesc.desctype = 0;
//   segdesc.segtype = SS_THEONEYOULIKE

#define SS_RESERVED			0
#define SS_16BIT_TSS_AVL	1
#define SS_LDT				2
#define SS_16BIT_TSS_BUSY	3
#define SS_16BIT_CALLGATE	4
#define SS_TASKGATE			5
#define SS_16BIT_INTGATE	6
#define SS_16BIT_TRAPGATE	7
#define SS_32BIT_TSS_AVL	9
#define SS_32BIT_TSS_BUSY	11
#define SS_32BIT_CALLGATE	12
#define SS_32BIT_INTGATE	14
#define SS_32BIT_TRAPGATE	15

// Code and Data Segments types
// Use these values as bits in the type field
#define SEGT_EXECUTE		8	// 0 DATA, 1 CODE

// descriptor type
#define DESCT_SYSTEM		0
#define DESCT_DATA			1
#define DESCT_CODE			DESCT_DATA

#define CS_CONFORMING		4
#define CS_READ				2
#define CS_ACCESSED			1

#define DS_EXPANSION		4	// 0 UP, 1 DOWN
#define DS_WRITE			2
#define DS_ACCESSED			1

#define DPL0	0
#define	DPL1	1
#define	DPL2	2
#define	DPL3	3

// Align all the structures to 1byte bound
#pragma pack (push, 1)

typedef struct segmentdesc_s {
	u16	lowLimit	: 16;	// LLIMIT	15:00 part of segment limit
	u16	lowBase		: 16;	// LBASE	15:00 part of segment base address
	u8	midBase		: 8;	// MBASE	23:16 part of segment base address
	u8	segType		: 4;	// TYPE		Segment type
	u8	descType	: 1;	// S		Descriptor type (0: system, 1: code/data)
	u8	dpl			: 2;	// DPL		Descriptor privilege level (0 is max)
	u8	present		: 1;	// P		Segment present
	u8	highLimit	: 4;	// HLIMIT	16:19 part of segment limit
	u8	available	: 1;	// AVL		Available for system software
	u8	zero		: 1;	// 0		MUST BE ZERO
	u8	size		: 1;	// D/B		Default operation size (0: 16, 1: 32)
	u8	granularity	: 1;	// G		Granularity
	u8	highBase	: 8;	// HBASE		24:31 part of segment base address
} segmentdesc;			// SIZE: 64 (8 B)

typedef struct GDTr_s {
	u16	limit		: 16;
	u32 base		: 32;
} GDTr;

typedef struct gatedesc_s {
	u16	lowOffset	: 16;	// Offset 15:00
	u16	segSelector	: 16;	// code segment selector
	u8	reserved	: 5;	// 5 bits reserved (call gate param count)
	u8	zero1		: 3;	// reserved, must be 0
	u8	type		: 4;	// 
	u8	zero2		: 1;	// must be 0
	u8	dpl			: 2;	// Descriptor privilege level
	u8	present		: 1;	// Present
	u16	highOffset	: 16;	// Offset 31:16
} gatedesc;

typedef struct tss_s {
	u32	previous;	// Previous task pointer, low 16bit used, high reserved
	u32	esp0;		// Stack pointer for pl0
	u32	ss0;		// Stack segment for pl0 16bit
	u32	esp1;		// Stack pointer for pl1
	u32	ss1;		// Stack segment for pl1 16bit
	u32	esp2;		// Stack pointer for pl2
	u32	ss2;		// Stack segment for pl2 16bit
	u32	cr3;
	u32	eip;
	u32	eflags;
	u32	eax;
	u32	ecx;
	u32	edx;
	u32	ebx;
	u32	esp;
	u32	ebp;
	u32	esi;
	u32	edi;
	u32	es;			// 16bit
	u32	cs;			// 16bit
	u32	ss;			// 16bit
	u32	ds;			// 16bit
	u32	fs;			// 16bit
	u32	gs;			// 16bit
	u32	ldtseg;		// LDT Segment selector
	u16	trap;		// Trap (boolean 0/1)
	u16	iomap_base;	// IO Map Base Address
} tss;

#pragma pack (pop)

u32 readMSW();
u32 readCR0();
u32 writeCR0(u32); // RETURNS THE NEW CR0
u32 readCR3();
u32 writeCR3(u32); // RETURNS THE NEW CR3

void loadGDT(GDTr*, u32, u16);
segmentdesc initSegDesc(u32 limit,
						u32 base,
						u8 segType,
						u8 descType,
						u8 dpl,
						u8 present,
						u8 available,
						u8 size,
						u8 granularity
						);
segmentdesc nullSegDesc();

#endif /* PM_H_*/
