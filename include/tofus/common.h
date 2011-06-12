#ifndef COMMON_H_
#define COMMON_H_

#ifndef __BITS__
	#ifdef i386
		#define __BITS__ 32
	#endif
	#ifdef x86_64
		#define __BITS__ 64
	#endif
#endif

#include <types.h>
#include <video.h>

#define CHECK_FLAG(flags,bit)	((flags) & (1 << (bit)))

#endif /*COMMON_H_*/
