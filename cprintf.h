#ifndef CPRINTF_H
#define CPRINTF_H

#include <stdarg.h>
#include <stddef.h>

size_t cprintf_ansi(char * str, const size_t size, const char * fmt, ...);
void cprintf(const char * fmt, ...);

#endif