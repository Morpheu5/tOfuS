#ifndef TIMER_H_
#define TIMER_H_

#include <types.h>
#include <ports.h>
#include <video.h>

#define HZ 1000
#define CLOCK 1193182
#define COUNTER_DIVIDER (CLOCK/HZ)

void setupPit(u16);

#endif /*TIMER_H_*/
