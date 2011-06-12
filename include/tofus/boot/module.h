#ifndef MODULES_H_
#define MODULES_H_

#include <types.h>
#include <video.h>

enum module_type { MODULE_RAW, MODULE_ELF };

enum module_type module_type(void*);
void module_elf_printInfo(void*);
void module_elf_findText(void*);

#endif /*MODULES_H_*/
