#ifndef STR_H_
#define STR_H_

#include <stdlib.h>

#define STR_FMT "%.*s" 
#define STR_ARG(token) (int)token.count,  token.str



typedef struct {
    const char* str;
    size_t count; 
} Str;


void str_print(Str str) {
    printf(STR_FMT"\n",STR_ARG(str));
}

char* str_get_charp(Str str) {
    char* charp = malloc(sizeof(char) * str.count);
    memcpy(charp,str.str,str.count);
    return charp; 
}



#endif // STR_H_
