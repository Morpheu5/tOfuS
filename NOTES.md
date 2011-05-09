                          -=[ THE BIG WARNING ]=-
Make sure you follow the procedures described inside the utils/ directory in
order to have everything you need to build tOfuS
                      -=[ MAY GOD SHAVE THE QUEEN ]=-

Arch dependent headers should be referred with just their name (i.e., types.h
rather than tofus/arch/i386/types.h). This is because the path is correctly set
by the Makefile and it allows to quickly change the target arch.
The other files have to be referred as usual (i.e., tofus/timer.h).

CPUID: develop a field structure to refer to single fileds within bytes.

libc: in the compare functions it would be great to check for the maximum size
of the registers to take advantage of it, i.e., in 32bit machines check by 32bit
blocks and in 64bit machines check by 64bit blocks.

32+64bit access:
	union {
		struct {
			u32	low;
			u32 high;
		} base_32;
		u64	base_64;
	};

PAGING: In the page directories it is obviously needed the physical address of
the page tables. It can be obtained by simulating the translation mechanism.
TODO: write a routine that, given a pointer to a virtual address, retrieves the
physical address. WARNING: Different tasks have different PDs (and PTs).

