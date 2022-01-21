libcprintf.a: cprintf.o
	ar rcs -o libcprintf.a cprintf.o

cprintf.o: cprintf.c cprintf.h
	gcc -O3 -Wall -ansi -c cprintf.c -o cprintf.o