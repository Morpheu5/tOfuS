## This Makefile is SACRED. Don't touch it.

include config/config.mk

.SUFFIXES: .asm .a

normal=\033[0m
red=\033[0;31m
green=\033[0;32m
yellow=\033[0;33m
blue=\033[0;34m
magenta=\033[0;35m
cyan=\033[0;36m
lgrey=\033[0;37m
dgrey=\033[1;30m
lred=\033[1;31m
lgreen=\033[1;32m
lyellow=\033[1;33m
lblue=\033[1;34m
lmagenta=\033[1;35m
lcyan=\033[1;36m
white=\033[1;37m

CCS=${lgreen}CC${normal}
ASS=${lcyan}AS${normal}
LDS=${white}LD${normal}

CC=gcc
ASM=nasm

INCLUDE_PATH=-Iinclude/ -Iinclude/tofus/arch/${BUILDARCH}/ -Iinclude/libc/
ASMFLAGS=
CFLAGS=-ffreestanding -nostdlib -fno-builtin -nostdinc -static -Wall ${CUSTOMCFLAGS}

# without -s for debugging
LDFLAGS=-nostdlib -Wl,-N -Wl,-e,start -Wall -Wl,-X -Wl,-T,config/ldscript

BUILDTIMESTAMP = `date +%Y-%m-%d\ %R\ %z`

DEFINES=${DEBUGD} -D__ELF__ -DBUILDTIMESTAMP="\"${BUILDTIMESTAMP}\"" ${DEFINEARCH} ${MEMORYMAP}

OBJS=\
	arch/${BUILDARCH}/boot/boot.a\
	arch/${BUILDARCH}/boot/setup.o\
	arch/${BUILDARCH}/cpuid.o\
	arch/${BUILDARCH}/intutils.a\
	arch/${BUILDARCH}/pm.a\
	arch/${BUILDARCH}/pm.o\
	arch/${BUILDARCH}/video.o\
	arch/${BUILDARCH}/ports.o\
	arch/${BUILDARCH}/pic.o\
	arch/${BUILDARCH}/semaphore.o\
	arch/${BUILDARCH}/isr.a\
	arch/${BUILDARCH}/isr.o\
	arch/${BUILDARCH}/idt.o\
	arch/${BUILDARCH}/mm.o\
	libc/string/strcmp.o\
	libc/string/strlen.o\
	libc/string/memcmp.o\
	libc/string/memcpy.o\
	kernel/boot/module/module.o\
	kernel/boot/module/elf.o\
	kernel/memory/heap_alloc.o\
	kernel/drivers/timer.o\
	kernel/drivers/keyboard.o

HDRS=\
	include/tofus/arch/i386/defines.asm \
	include/libc/string.h \
	include/tofus/arch \/i386/keyboard.h \
	include/tofus/arch \/i386/video.h \
	include/tofus/arch \/i386/setup.h \
	include/tofus/arch \/i386/pm.h \
	include/tofus/arch \/i386/elf.h \
	include/tofus/arch \/i386/idt.h \
	include/tofus/arch \/i386/types.h \
	include/tofus/arch \/i386/isr.h \
	include/tofus/arch \/i386/ports.h \
	include/tofus/arch \/i386/pic.h \
	include/tofus/arch \/i386/mm.h \
	include/tofus/arch \/i386/cpuid.h \
	include/tofus/boot/module.h \
	include/tofus/semaph \ore.h \
	include/tofus/multiboot.h \
	include/tofus/malloc.h \
	include/tofus/common.h \
	include/tofus/timer.h \
	include/tofus/mmap.h

all: tofus

tofus: ${OBJS}
	@ echo -e "[${LDS}] building OS image"
	@ ${CC} ${OBJS} -o tofus ${LDFLAGS} ${INCLUDE_PATH} ${DEFINES}
	@ echo "*** OS image is ready, copy it into the proper location and reboot."

.asm.a:
	@ echo -e "[${ASS}] $@"
	@ ${ASM} $< -o $@ -f elf ${ASMFLAGS} ${INCLUDE_PATH} ${DEFINES}

.c.o:
	@ echo -e "[${CCS}] $@"
	@ ${CC} -c $< -o $@ ${CFLAGS} ${INCLUDE_PATH} ${DEFINES}

.m.o:
	@ echo "[${CCS}] $@"
	@ ${CC} -c $< -o $@ ${CFLAGS} ${INCLUDE_PATH} ${DEFINES} -lobjc

clean:
	@ rm -rf ${OBJS}
	@ rm -rf *~

distclean: clean
	@ rm -rf tofus
