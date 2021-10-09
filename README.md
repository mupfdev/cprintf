# cprintf
Portable C Library for printing colored text on the terminal with the C formatting style. Usage:
```c
#include <cprintf.h>

int main() {
    cprintf(1, "Hello, %fg world!\n", "green");
    
    return 0;
}
```
