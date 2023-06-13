#ifndef STR_H_
#define STR_H_

#include <stdlib.h>

typedef struct {
    const char* str;
    size_t count; 
} Str;

char* str_get_charp(Str str) {
    char* charp = malloc(sizeof(char) * str.count);
    memcpy(charp,str.str,str.count);
    return charp; 
}



#endif // STR_H_
