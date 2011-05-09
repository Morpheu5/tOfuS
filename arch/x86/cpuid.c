#include <cpuid.h>
#include <video.h>
#include <setup.h>

void cpuid() {
	gpr regs;
	u32 maxParm;
	asm volatile ( \
			"mov %%eax, 0\n\t" \
			"cpuid\n\t" \
			"mov %0, %%eax\n\t" \
			"mov %1, %%ebx\n\t" \
			"mov %2, %%ecx\n\t" \
			"mov %3, %%edx\n\t" \
			: "=m"(maxParm), "=m"(regs.ebx), "=m"(regs.ecx), "=m"(regs.edx));

	u32* vp = (u32*)cpuInfo.vendor;
	*vp++ = regs.ebx;
	*vp++ = regs.edx;
	*vp++ = regs.ecx;
	*vp++ = 0;

	if(1 <= maxParm) {
		asm volatile ( \
				"mov %%eax, 1\n\t" \
				"cpuid\n\t" \
				"mov %0, %%eax\n\t" \
				"mov %1, %%ebx\n\t" \
				"mov %2, %%ecx\n\t" \
				"mov %3, %%edx\n\t" \
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
				"mov %%eax, 2\n\t" \
				"cpuid\n\t" \
				"mov %0, %%eax\n\t" \
				"mov %1, %%ebx\n\t" \
				"mov %2, %%ecx\n\t" \
				"mov %3, %%edx\n\t" \
				: "=m"(regs.eax), "=m"(regs.ebx), "=m"(regs.ecx), "=m"(regs.edx));
	}

	// TODO: Go further, betterize this function
	return;
}
