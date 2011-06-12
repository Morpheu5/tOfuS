#ifndef ELF_H_
#define ELF_H_

#include <types.h>

typedef u32 Elf32_Addr;
typedef u16 Elf32_Half;
typedef u32 Elf32_Off;
typedef u32 Elf32_Sword;
typedef u32 Elf32_Word;

#define SHT_NULL		0
#define SHT_PROGBITS	1
#define SHT_SYMTAB		2
#define SHT_STRTAB		3
#define SHT_RELA		4
#define SHT_HASH		5
#define SHT_DYNAMIC		6
#define SHT_NOTE		7
#define SHT_NOBITS		8
#define SHT_REL			9
#define SHT_SHLIB		10
#define SHT_DYNSYM		11
#define SHT_LOPROC		0x70000000
#define SHT_HIPROC		0x7fffffff
#define SHT_LOUSER		0x80000000
#define SHT_HIUSER		0xffffffff

#define SHF_WRITE		0x1
#define SHF_ALLOC		0x2
#define SHF_EXECINSTR	0x4
#define SHF_MASKPROC	0xf0000000

enum e_machine {
	EM_NONE,
	EM_M32,
	EM_SPARC,
	EM_386,
	EM_68K,
	EM_88K,
	EM_860,
	EM_MIPS
};

enum e_type {
	ET_NONE,
	ET_REL,
	ET_EXEC,
	ET_DYN,
	ET_CORE,
	ET_LOPROC = 0xff00,
	ET_HIPROC = 0xffff
};

typedef struct Elf32_SectionHeader_s {
	Elf32_Word	name;		// name of the section
	Elf32_Word	type;		// type of the section
	Elf32_Word	flags;		// section-specific attributes
	Elf32_Addr	addr;		// memory location of the section
	Elf32_Off	offset;		// file offset to section
	Elf32_Word	size;		// size of the section
	Elf32_Word	link;		// section type dependent
	Elf32_Word	info;		// extra informations
	Elf32_Word	addralign;	// address alignment constrain
	Elf32_Word	entrysize;	// size of an entry in the section
} Elf32_SectionHeader;

#define	EI_NIDENT       16
typedef struct Elf32_Header_s {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word	e_version;
	Elf32_Addr	e_entry;
	Elf32_Off	e_phoff;
	Elf32_Off	e_shoff;
	Elf32_Word	e_flags;
	Elf32_Half	e_ehsize;
	Elf32_Half	e_phentsize;
	Elf32_Half	e_phnum;
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrndx;
} Elf32_Header;

#endif /*ELF_H_*/
