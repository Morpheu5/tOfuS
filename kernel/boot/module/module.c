#include <tofus/boot/module.h>

enum module_type module_type(void* module) {
	// ELF
	char elf_magic[4] = { 0x7f, 'E', 'L', 'F' };
	if(!memcmp(module, elf_magic, 4)) {
		return MODULE_ELF;
	}
	
	// Unrecognized file type
	return MODULE_RAW;
}
