#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "state.h"
#include "lexer.h"
#include "parser.h"

typedef enum { 
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



RunTimeValue interpreter_eval(AST* ast);
RunTimeValue interpreter_eval_program(AST* ast); 



RunTimeValue interpreter_eval_program(AST* ast) { 
    RunTimeValue lastEval;
    for(AST_Elem* exp = ast->as.program.body; exp; exp = exp->next) {
        lastEval = interpreter_eval(exp->data);
    }
    return lastEval;
}

RunTimeValue interpreter_eval_binary_expr(AST* ast) {
    RunTimeValue lhs = interpreter_eval(ast->as.binaryExpr.lhs);
    RunTimeValue rhs = interpreter_eval(ast->as.binaryExpr.rhs);

    RunTimeValue out = {0};

        // double + double -> double 
        // str + str -> str
        // str + double -> error
    if(rhs.type == StringValue && lhs.type == StringValue) {
        switch (*ast->as.binaryExpr.op.str)
        {
            case '+':
                Str string;
                string.count =  lhs.as.str.count + rhs.as.str.count;
                char* str = malloc(sizeof(char) * string.count);
                memcpy(str,lhs.as.str.str,lhs.as.str.count);
                memcpy(str + lhs.as.str.count,rhs.as.str.str,rhs.as.str.count);
                string.str = str;
                return (RunTimeValue) {.type = StringValue,.as.str=string };
            break;
            default:
                assert(0 && "[Error] cant preforme more thant addition on strings\n");
            break;
        }
    } else if (rhs.type == NumberValue && lhs.type == NumberValue) {
        switch (*ast->as.binaryExpr.op.str)
        {
	     case '+':
            return (RunTimeValue) {.type = NumberValue,.as.number = (lhs.as.number + rhs.as.number)};
            break;
            case '-' :
                return (RunTimeValue) {.type = NumberValue,.as.number = (lhs.as.number + rhs.as.number)};
            break;
            case '*' :
               return (RunTimeValue) {.type = NumberValue,.as.number = (lhs.as.number * rhs.as.number)};
            break;
            case '/' :
                return (RunTimeValue) {.type = NumberValue,.as.number = (lhs.as.number / rhs.as.number)};
            break;
            case '&' :
                return (RunTimeValue) {.type = NumberValue,.as.number = ((int)lhs.as.number & (int) rhs.as.number)};
            break;
            case '|' :
                return (RunTimeValue) {.type = NumberValue,.as.number = ((int)lhs.as.number | (int)rhs.as.number)};
            break;
            case '^' :
                return (RunTimeValue) {.type = NumberValue,.as.number = ((int)lhs.as.number ^ (int)rhs.as.number)};
            break;
            case '%' :
                return (RunTimeValue) {.type = NumberValue,.as.number = ((int)lhs.as.number % (int)rhs.as.number)};
            break;
            default:
                assert(0 && "[Error] Unreachable `interpreter_eval_binary_expr`");
            break;
        }
    } else {
        assert(0 && "[Error] cant preforme binary ops on differnt types\n");
    }
}

RunTimeValue interpreter_eval(AST* ast) {
    switch (ast->type)
    {
        case Program:
            return interpreter_eval_program(ast);
        break;
        case BinaryExp:
            return interpreter_eval_binary_expr(ast);
        break;
        case NumberLiteral:
            RunTimeValue out = {0};
            out.type = NumberValue;
            out.as.number = strtod(ast->as.numberLiteral.data.str,NULL);
            return out;
        break;
        case StringLiteral:
            return (RunTimeValue) {.type = StringValue,.as.str = ast->as.stringLiteral.data};
        break;
    default:
        assert(0 && "[Error] Unreachable `interpreter_eval`");
    }
    return (RunTimeValue) {0};
}
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


int main(void) {
    char * file = lexer_read_file("main.skt");
    Ltokens = lexer_lex();
    
    for(int i = 0 ; i  < Ltokens.count; i++) {
        token_print(Ltokens.tokens[i]);
    }
    printf("->\n");
    
    
    AST* program =  parser_parse();
    ast_print(program,0);

    

    printf("--------------------\n");
        runtime_print(interpreter_eval(program));
    printf("--------------------\n");

    return 0;
}
