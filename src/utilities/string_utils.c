#include "utilities/string_utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

s32 String_Copy(char* dst, const char* src, u32 size) {
    if (!dst || !src || size == 0) return 0;
    strncpy(dst, src, size - 1);
    dst[size - 1] = '\0';
    return (s32)strlen(dst);
}

s32 String_Format(char* dst, u32 size, const char* fmt, ...) {
    if (!dst || !fmt || size == 0) return 0;
    va_list args;
    va_start(args, fmt);
    s32 n = vsnprintf(dst, size, fmt, args);
    va_end(args);
    if (n < 0 || (u32)n >= size) dst[size - 1] = '\0';
    return n;
}

bool String_Equal(const char* a, const char* b) {
    if (!a || !b) return a == b;
    return strcmp(a, b) == 0;
}

u32 String_Length(const char* s) {
    if (!s) return 0;
    return (u32)strlen(s);
}
