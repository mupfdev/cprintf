#include "cprintf.h"

#ifdef _WIN32
#ifdef _MSC_VER
#include <windows.h>
#else
#include <wincon.h>
#endif

static HANDLE con = NULL;
static WORD def_attr = 0;
static DWORD written;

#else
#include <string.h>
#endif

#include <ctypes.h>
#include <stdio.h>

#ifdef _WIN32
static unsigned char cprintf_init(void) {
    if (con != NULL)
        return 1;
    
    if ((con = GetStdHandle(-11)) == NULL)
        return 0;
    else if (def_attr != 0)
        return 1;
    
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(con, &info) == 0)
        return 0;

    def_attr = info.wAttributes;
}

static const WORD colors[7][3] = {
    { 114, FOREGROUND_RED,                     BACKGROUND_RED },
    { 103, FOREGROUND_GREEN,                   BACKGROUND_GREEN },
    { 121, FOREGROUND_RED | FOREGROUND_GREEN,  BACKGROUND_RED | BACKGROUND_GREEN },
    { 98,  FOREGROUND_BLUE,                    BACKGROUND_BLUE },
    { 109, FOREGROUND_RED | FOREGROUND_BLUE,   BACKGROUND_RED | BACKGROUND_BLUE },
    { 99,  FOREGROUND_GREEN | FOREGROUND_BLUE, BACKGROUND_GREEN | BACKGROUND_BLUE },
    
    { 119,
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE }
};
#else
static const char ansi_order[8] = {
    'k', 'r', 'g', 'y', 'b', 'm', 'c', 'w'
};
#endif

typedef struct {
#ifdef _WIN32
    WORD attr;
#else
    char ansi[20];
    unsigned char ansi_len;
#endif
    unsigned char input_size;
} context_t;

static void cprintf_parse(const char * str, context_t * out) {
#ifdef _WIN32
    out->attr = 0;
#endif
    const char l = tolower(str[0]);

#ifdef _WIN32
    if (l == 's' || l != str[0])
        out->attr |= l == 'b' ? BACKGROUND_INTENSITY : FOREGROUND_INTENSITY;
#else
    if (l == 's') {
        strcpy(out->ansi + out->ansi_len, "\x1b[1m");
        out->ansi_len += 4;
    }
#endif

    out->input_size = 1;

    if (l == 'f' || l == 'b') {
        out->input_size = 2;
        
        unsigned char i = 0;
#ifdef _WIN32
        while (i < 8)
#else
        while (i < 9)
#endif
        {
#ifdef _WIN32
            if (colors[i][0] == str[1]) {
                out->attr |= colors[i][l == 'f' ? 1 : 2];
                break;
            }
#else
            if (ansi_order[i] == tolower(str[1])) {
                strcpy(out->ansi + out->ansi_len, "\x1b[");
                strcpy(out->ansi + out->ansi_len + 4, "m");
                out->ansi[out->ansi_len + 2] = l == 'f' ? '3' : '4';
                out->ansi[out->ansi_len + 3] = '0' + i;
                out->ansi_len += 5;
                
                if (str[0] != l) {
                    strcpy(out->ansi + out->ansi_len - 1, ";1m");
                    out->ansi_len += 2;
                }
                
                break;
            }
#endif
        }
    } else if (l == 'i') {
#ifdef _WIN32
        out->attr |= COMMON_LVB_REVERSE_VIDEO;
#else
        strcpy(out->ansi + out->ansi_len, "\x1b7m");
        out->ansi_len += 4;
#endif
    }
    
    if (l == 'u') {
#ifdef _WIN32
        out->attr |= COMMON_LVB_UNDERSCORE;
#else
        strcpy(out->ansi + out->ansi_len, "\x1b[4m");
        out->ansi_len += 4;
#endif
    }
    
    if (str)
} 

/*
%d = ignored

%bb = background blue 
%bg = background green
%br = background red
%bk = background black
%bm = backgorund magenta
%bc = background cyan
%bw = background white
%by = backtround yello

%u = underline
%i = invert

*/

void cprintf(const size_t fmt_am, const char * fmt, ...) {
#ifdef _WIN32
    if (cprintf_init() == 0)
        return;
#endif

    context_t ctx;

    va_list vl;
    va_start(vl, fmt_am);

    unsigned char is_fmt = 0;
    size_t i = 0;
    char c;
    
    while ((c = fmt[i]) != 0) {
        if (c == '%') {
            is_fmt = 1;
            i++;
            continue;
        } else if (is_fmt == 0) {
            putchar(c);
            i++;
            continue;
        }

        memset(&ctx, 0, sizeof(context_t));
        
        if (c == '{') {
            while (1) {
                cprintf_parse(fmt + i, &ctx);
                
                i += ctx.input_size;
                if (fmt[i] == ',')
                    i++;
                else
                    break;
            }
            
#ifdef _WIN32
            SetConsoleTextAttribute(con, ctx.attr);
            printf(va_arg(vl, char *));
            SetConsoleTextAttribute(con, def_attr);
#else
            printf("%s%s\x1b[0m", ctx.ansi, va_arg(vl, char *));
#endif
        } else {
            cprintf_parse(fmt + i, &ctx);
            
#ifdef _WIN32
            SetConsoleTextAttribute(con, ctx.attr);
            printf(va_arg(vl, char *));
            SetConsoleTextAttribute(con, def_attr);
#else
            printf("%s%s\x1b[0m", ctx.ansi, va_arg(vl, char *));
#endif

            i += ctx.input_size;
        }

        is_fmt = 0;
    }
    
    va_end(vl);
}
