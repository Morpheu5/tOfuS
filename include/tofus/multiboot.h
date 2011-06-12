#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

/** The Multiboot header.
    Magic thing that makes boot easier :)
 */
typedef struct multiboot_header_s {
	u32 magic;
	u32 flags;
	u32 checksum;
	u32 header_addr;
	u32 load_addr;
	u32 load_end_addr;
	u32 bss_end_addr;
	u32 entry_addr;
} multiboot_header;

/** The section header table for ELF.
 */
typedef struct elf_section_header_table_s {
	u32 num;
	u32 size;
	u32 addr;
	u32 shndx;
} elf_section_header_table;

/** The Multiboot information.
 */
typedef struct multiboot_info_s {
	u32 flags;
	u32 mem_lower;
	u32 mem_upper;
	u32 boot_device;
	u32 cmdline;
	u32 mods_count;
	u32 mods_addr;
	elf_section_header_table elf_sec; // elf_section_header_table*
	u32 mmap_length;
	u32 mmap_addr;
} multiboot_info;

/** The module structure.
    In case you want to load some modules with GRUB...
*/
typedef struct module_s {
	u32 mod_start;
	u32 mod_end;
	u32 string;
	u32 reserved;
} multiboot_module;

/** The memory map. Be careful that the offset 0 is base_addr_low
    but no size.
*/
typedef struct memory_map_s {
	u32 size;
	u32 base_addr_low;
	u32 base_addr_high;
	u32 length_low;
	u32 length_high;
	u32 type;
} multiboot_memoryMap;

#endif /*MULTIBOOT_H_*/
