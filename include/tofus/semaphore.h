#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <types.h>

typedef struct semaphore_s {
	u32 status;
	u32 value;
	u32 *queue;
} semaphore;

void signal(semaphore*);

#define	LOCK	0
#define UNLOCK	0xfffffffe

void wait(semaphore*);
#endif /*SEMAPHORE_H_*/
