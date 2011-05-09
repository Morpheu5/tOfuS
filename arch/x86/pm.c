#include <pm.h>

segmentdesc initSegDesc(u32 base,
						u32 limit,
						u8 segType,
						u8 descType,
						u8 dpl,
						u8 present,
						u8 available,
						u8 size,
						u8 granularity
						)
{
	segmentdesc s;
	s.highBase = base >> 24;
	s.granularity = granularity;
	s.size = size;
	s.zero = 0;
	s.available = available;
	s.highLimit = limit >> 16;
	s.present = present;
	s.dpl = dpl;
	s.descType = descType;
	s.segType = segType;
	s.midBase = base >> 16;
	s.lowBase = base;
	s.lowLimit = limit;
	
	return s;
}

segmentdesc nullSegDesc(void) {
	return initSegDesc(0,0,0,0,0,0,0,0,0);
}
