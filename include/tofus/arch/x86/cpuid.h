#ifndef CPUID_H_
#define CPUID_H_

#include <types.h>

typedef struct gpr_s {
	u32	eax;
	u32 ebx;
	u32 ecx;
	u32	edx;
} gpr;

#pragma pack (push, 1)

typedef struct CPUid_s {
	char	vendor[13];
	struct signature_s {
		u32	stepping	: 4;
		u32	model		: 4;
		u32	family		: 4;
		u32	type		: 2;
		u32	res_1		: 2;
		u32	e_model		: 4;		// e_model << 4 + model
		u32 e_family	: 8;		// e_family + family
		u32	res_2		: 4;
	} signature;
	struct featFlags_s {
		u32	edx;
		u32	ecx;
		u32 ebx;
	} featFlags;
	gpr		cacheInfo;
	gpr		e_procInfo;
	char	brand[48];
} CPUid;

#pragma pack (pop)

void cpuid(void);

extern CPUid cpuInfo;

#endif /*CPUID_H_*/
