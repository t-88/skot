#ifndef RUNTIME_H_
#define RUNTIME_H_

#include <stdbool.h>

typedef enum { 
    //TODO: hash table stuff no good
    Noth,
    TombStone,
    
    StringValue,
    NumberValue,
    BooleanValue,
} RunTimeType;

typedef struct {
    RunTimeType type;
    union  {
        Str str;
        double number;
        bool boolean;
    } as;
} RunTimeValue;

void  runtime_print(RunTimeValue runtime) {
    switch (runtime.type)
    {
        case StringValue:
            str_print(runtime.as.str);
        break;
        case NumberValue:
           	printf("%f\n",runtime.as.number);
        break;
        case BooleanValue:
           	printf("%s\n",runtime.as.boolean == 1 ? "True" : "False" );
        break;

    default:
        assert(0 && "[Error] Unreachable `runtime_print`");
        break;
    }
}


#endif // RUNTIME_H_
