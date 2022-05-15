CC=gcc
WIN=no

ifeq ($(WIN),yes)
DEL=del
else
DEL=rm
endif

LFLAGS=rcs

ifeq ($(CC),gcc)
AR=ar
CFLAGS=-O3 -Wall -ansi
else ifeq ($(CC),clang)
AR=llvm-ar
CFLAGS=-Ofast -Wall
endif

LIB=libcprintf.a

.PHONY: clean

clean: $(LIB) cprintf.o
	$(DEL) cprintf.o

$(LIB): cprintf.o
	$(AR) $(LFLAGS) $(LIB) cprintf.o

cprintf.o: cprintf.c cprintf.h
	$(CC) $(CFLAGS) -c cprintf.c -o cprintf.o