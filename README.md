# cprintf
Portable C Library for printing colored text on the terminal with the C formatting style. Usage:
```c
#include <cprintf.h>

int main() {
    cprintf("Hello, %fg world!\n", "green text!!!");
    
    return 0;
}
```

# Build
Building is easy and straight-forward.
```sh
gcc -c -Wall -O3 cprintf.c -o cprintf.o
ar rcs -o libcprintf.a cprintf.o
```

# Table of formats
| Format name | Description                              | Extra notes                                                                        |
|-------------|------------------------------------------|------------------------------------------------------------------------------------|
| `%fk`       | Changes the text color to black.         | Windows builds does not support this format.                                       |
| `%fr`       | Changes the text color to red.           | -                                                                                  |
| `%fg`       | Changes the text color to green.         | -                                                                                  |
| `%fy`       | Changes the text color to yellow.        | -                                                                                  |
| `%fb`       | Changes the text color to blue.          | -                                                                                  |
| `%fm`       | Changes the text color to magenta.       | -                                                                                  |
| `%fc`       | Changes the text color to cyan.          | -                                                                                  |
| `%fw`       | Changes the text color to white.         | -                                                                                  |
| `%bk`       | Changes the background color to black.   | Windows builds does not support this format.                                       |
| `%br`       | Changes the background color to red.     | -                                                                                  |
| `%bg`       | Changes the background color to green.   | -                                                                                  |
| `%by`       | Changes the background color to yellow.  | -                                                                                  |
| `%bb`       | Changes the background color to blue.    | -                                                                                  |
| `%bm`       | Changes the background color to magenta. | -                                                                                  |
| `%bc`       | Changes the background color to cyan.    | -                                                                                  |
| `%bw`       | Changes the background color to white.   | -                                                                                  |
| `%i`        | Inverts the background and text color.   | In windows, this is an alias for `%bw`.                                            |
| `%u`        | Adds an underline.                       | In windows, if no color is provided (e.g: only `%u`), no visible text will appear. |

# Bold
You can make a text bold or background color brighter by capitalizing the format specifier. Example:
- `%FR` will output a bold, red text.
- `%BB` will output a bright, blue background.

# Combining formats
You can combine multiple formats too, with `%{...}`. Example:
- `%{fr,u}` combines `%fr` (red text) and `%u` (underline/underscore).

Invalid combinations will cause undefined behaviour.
Repeated combinations, or more than 3 combinations will cause undefined behaviour on POSIX builds.
