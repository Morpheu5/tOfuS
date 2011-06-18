#include <tofus/semaphore.h>

/*
 	semaphore sem;
	semaphore* s;
	s = &sem;
	s->status = 0;
	s->value = 0x100;
	kprintf("status: %x\nvalue: %x\n", s->status, s->value);
	wait(s);
	kprintf("status: %x\nvalue: %x\n", s->status, s->value);
	wait(s);
	kprintf("status: %x\nvalue: %x\n", s->status, s->value);
	signal(s);
	kprintf("status: %x\nvalue: %x\n", s->status, s->value);
*/

/* Must be called with lock(&s) */
#define lock(sem)	asm volatile (	"0:\n\t"	\
					"lock bts %1,%0\n\t"		\
					"jc 0b\n\t"				\
					: "=m" (*(u32*)sem->status)	\
					: "n" (LOCK)				\
					: "cc"						\
				);

/* Must be called as unlock(&s) */
#define unlock(sem)	asm volatile (	"lock and %1,%0\n\t"	\
					: "=m" (*(u32*)sem->status)				\
					: "n" (UNLOCK)							\
				);

void wait(semaphore* sem) {
	lock(&sem);
	while(sem->value == 0) {
		unlock(&sem);
		asm volatile("nop");
		lock(&sem);
	}
	sem->value--;
	unlock(&sem);
}

void signal(semaphore* sem) {
	lock(&sem);
	sem->value++;
	unlock(&sem);
}
