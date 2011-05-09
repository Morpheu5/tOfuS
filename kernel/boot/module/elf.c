#include <tofus/boot/module.h>
#include <pm.h>
#include <elf.h>

void module_elf_printInfo(void* hdr) {
	printDebug("ELF ");
	Elf32_Header* elf_h = (Elf32_Header*)hdr;
	u8* class = hdr+4;
	switch(class[0]) {
	case 0:
		printDebug("Invalid class\n");
		return;
	case 1:
		printDebug("32-bit ");
		break;
	case 2:
		printDebug("64-bit ");
		break;
	}
	switch(class[1]) {
	case 0:
		printDebug("Invalid encoding\n");
		return;
	case 1:
		printDebug("LSB ");
		break;
	case 2:
		printDebug("MSB ");
		break;
	}
	switch(elf_h->e_type) {
	case ET_NONE:
		printDebug("No file type\n");
		return;
	case ET_REL:
		printDebug("relocatable, ");
		break;
	case ET_EXEC:
		printDebug("executable, ");
		break;
	case ET_DYN:
		printDebug("shared object, ");
		break;
	case ET_CORE:
		printDebug("core, ");
		break;
	default:
		printDebug("%x ", elf_h->e_type);
		break;
	}
	switch(elf_h->e_machine) {
	case EM_NONE:
		printDebug("No machine, ");
		break;
	case EM_386:
		printDebug("Intel 80386, ");
		break;
	default:
		printDebug("Unsupported machine: %d ", elf_h->e_machine);
		break;
	}
	printDebug("version %d ", elf_h->e_version);
}

void module_elf_findText(void* hdr) {
	Elf32_Header* elf_h = (Elf32_Header*)hdr;
	printDebug("\n\t_start is at 0x%x\n", elf_h->e_entry);
	int (*moduleStart)(void) = (void *)elf_h->e_entry;
	printDebug("\n%d\n", moduleStart());
}
