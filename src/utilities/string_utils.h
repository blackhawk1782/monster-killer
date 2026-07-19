#ifndef MK_STRING_UTILS_H
#define MK_STRING_UTILS_H

#include "types.h"

s32 String_Copy(char* dst, const char* src, u32 size);
s32 String_Format(char* dst, u32 size, const char* fmt, ...);
bool String_Equal(const char* a, const char* b);
u32 String_Length(const char* s);

#endif
