#ifndef ISR_H_
#define ISR_H_

#include <types.h>
#include <pm.h>
#include <video.h>
#include <ports.h>

u32* b_nisr();
u32* b_isr0();
u32* b_isr1();
u32* b_isr2();
u32* b_isr3();
u32* b_isr4();
u32* b_isr5();
u32* b_isr6();
u32* b_isr7();
u32* b_isr8();
u32* b_isr9();
u32* b_isr10();
u32* b_isr11();
u32* b_isr12();
u32* b_isr13();
u32* b_isr14();
u32* b_isr15();
u32* b_isr16();
u32* b_tmr();
u32* b_kbd();

#endif /*ISR_H_*/
