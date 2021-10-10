gcc -c -Wall -O3 cprintf.c -o cprintf.o
ar rcs -o libcprintf.a cprintf.o
gcc -Wall -O3 test.c -L. -lcprintf -I. -o test
./test
rm ./test