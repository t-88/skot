#ifndef RUNTIME_H_
#define RUNTIME_H_


typedef enum { 
    Noth,
    TombStone,
    StringValue,
    NumberValue,
} RunTimeType;

typedef struct {
    RunTimeType type;
    union  {
        Str str;
        double number;
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
    default:
        assert(0 && "[Error] Unreachable `runtime_print`");
        break;
    }
}


#endif // RUNTIME_H_
