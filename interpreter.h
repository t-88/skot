#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <stdlib.h>

#include "runtime.h"
#include "parser.h"
#include "state.h"
#include "env.h"

RunTimeValue interpreter_eval(AST* ast,Env* env);
RunTimeValue interpreter_eval_program(AST* ast,Env* env); 
RunTimeValue interpreter_eval_var_declaration(AST* ast,Env* env); 
RunTimeValue interpreter_eval_boolean_expr(AST* ast,Env* env); 


RunTimeValue interpreter_eval_program(AST* ast,Env* env) { 
    RunTimeValue lastEval;
    for(AST_Elem* exp = ast->as.program.body; exp; exp = exp->next) {
        lastEval = interpreter_eval(exp->data,env);
    }
    return lastEval;
}
RunTimeValue interpreter_eval_binary_expr(AST* ast,Env* env) {
    RunTimeValue lhs = interpreter_eval(ast->as.binaryExpr.lhs,env);
    RunTimeValue rhs = interpreter_eval(ast->as.binaryExpr.rhs,env);

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
RunTimeValue interpreter_eval_var_declaration(AST* ast,Env* env) {
    RunTimeValue value = interpreter_eval(ast->as.varDeclaration.value,env);

    env_declare_variable(env,str_get_charp(ast->as.varDeclaration.name),value);
    return value;
}
RunTimeValue interpreter_eval_var_assignment(AST* ast,Env* env) { 
    RunTimeValue value = interpreter_eval(ast->as.varAssignment.value,env);
    env_assing_variable(env,str_get_charp(ast->as.varDeclaration.name),value);
    return value;
}

// TODO: Type Check 
// TODO: && , || 
RunTimeValue interpreter_eval_boolean_expr(AST* ast,Env* env) {
    RunTimeValue lhs = interpreter_eval(ast->as.binaryExpr.lhs,env);
    RunTimeValue rhs = interpreter_eval(ast->as.binaryExpr.rhs,env);

    RunTimeValue out = {0};

        switch (*ast->as.binaryExpr.op.str)
        {
	     case '>':
            return (RunTimeValue) {.type = BooleanValue,.as.boolean = (lhs.as.number > rhs.as.number)};
            break;
            case '<' :
                return (RunTimeValue) {.type = BooleanValue,.as.boolean = (lhs.as.number < rhs.as.number)};
            break;
            case '>=' :
               return (RunTimeValue) {.type = BooleanValue,.as.boolean = (lhs.as.number >= rhs.as.number)};
            break;
            case '<=' :
                return (RunTimeValue) {.type = BooleanValue,.as.boolean = (lhs.as.number <= rhs.as.number)};
            default:
                assert(0 && "[Error] Unreachable `interpreter_eval_boolean_expr`");
            break;  
        }  
}

RunTimeValue interpreter_eval(AST* ast,Env* env) {
    switch (ast->type)
    {
        case Program:
            return interpreter_eval_program(ast,env);
        break;
        case BinaryExp:
            return interpreter_eval_binary_expr(ast,env);
        break;
        case VarDeclarationExpr:
            return interpreter_eval_var_declaration(ast,env);
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
        case IdentifierLiteral:
            return env_get_variable(env,str_get_charp(ast->as.identifierLiteral.name));
        break;        
        case VarAssignmentExpr:
            return interpreter_eval_var_assignment(ast,env);
        break;      
        case BooleanExp:
            return interpreter_eval_boolean_expr(ast,env);
        break;             
        default:
            assert(0 && "Unreachable `interpreter_eval`");
        break;
    }
    return (RunTimeValue) {0};
}



#endif //INTERPRETER_H_
