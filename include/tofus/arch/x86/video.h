#ifndef VIDEO_H_
#define VIDEO_H_

#include <types.h>
#include <ports.h>

#include <string.h>

#define VIDEO 0xb8000
#define COLUMNS 80
#define LINES 25
#define ATTRIBUTE 0x7

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define GRAY 7
#define DGRAY 8
#define LBLUE 9
#define LGREEN 10
#define LCYAN 11
#define LRED 12
#define LMAGENTA 13
#define YELLOW 14
#define WHITE 15

void clrscr(void);
void putchar(u8);
void scroll();
void kprintf(const char*, ...);

void printFlags(const char*, u32);
void printDebug(const char*, ...);

#endif /*VIDEO_H_*/
