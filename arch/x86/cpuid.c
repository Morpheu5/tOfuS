#include <cpuid.h>
#include <video.h>
#include <setup.h>

void cpuid() {
	gpr regs;
	u32 maxParm;
	asm volatile ( \
			"movl 0, %%eax\n\t" \
			"cpuid\n\t" \
			"movl %%eax, %0\n\t" \
			"movl %%ebx, %1\n\t" \
			"movl %%ecx, %2\n\t" \
			"movl %%edx, %3\n\t" \
			: "=m"(maxParm), "=m"(regs.ebx), "=m"(regs.ecx), "=m"(regs.edx));

	u32* vp = (u32*)cpuInfo.vendor;
	*vp++ = regs.ebx;
	*vp++ = regs.edx;
	*vp++ = regs.ecx;
	*vp++ = 0;

	if(1 <= maxParm) {
		asm volatile ( \
				"movl 1, %%eax\n\t" \
				"cpuid\n\t" \
				"movl %%eax, %0\n\t" \
				"movl %%ebx, %1\n\t" \
				"movl %%ecx, %2\n\t" \
				"movl %%edx, %3\n\t" \
				: "=m"(regs.eax), "=m"(regs.ebx), "=m"(regs.ecx), "=m"(regs.edx));
	}

	regs.eax &= 0x0fff3fff;		// Mask reserved bits
	u32* sig = (u32*)&(cpuInfo.signature);
	*sig = regs.eax;
	cpuInfo.featFlags.ebx = regs.ebx;
	cpuInfo.featFlags.ecx = regs.ecx;
	cpuInfo.featFlags.edx = regs.edx;

	if(2 <= maxParm) {
		asm volatile ( \
				"movl 2, %%eax\n\t" \
				"cpuid\n\t" \
				"movl %%eax, %0\n\t" \
				"movl %%ebx, %1\n\t" \
				"movl %%ecx, %2\n\t" \
				"movl %%edx, %3\n\t" \
				: "=m"(regs.eax), "=m"(regs.ebx), "=m"(regs.ecx), "=m"(regs.edx));
	}

	// TODO: Go further, betterize this function
	return;
}
