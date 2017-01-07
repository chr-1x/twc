#ifndef ASPRINTF_H
#include <stdarg.h>

char* asprintf(char** OutString, const char* Format, ...) {
    char* Result = NULL;
    if (Format == NULL) return Result;

    va_list Args;
    va_start(Args, Format);
    int Size = _vscprintf(Format, Args);

    if (Size > 0) {
        Size++;
        Result = (char*)malloc(Size + 2);
        if (Result) _vsnprintf(Result, Size, Format, Args);
    }

    va_end(Args);
    *OutString = Result;
    return Result;
}

#define ASPRINTF_H
#endif
