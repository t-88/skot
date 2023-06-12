#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#include "lexer.h"



size_t tokenIdx = 0;
LexerTokens Ltokens;

typedef struct AST AST;
typedef struct AST_Elem AST_Elem;




typedef  struct AST {
    enum type{
        Program,
        BinaryExp,
        BooleanExp,
        NumberLiteral,
        StringLiteral,
    } type;

    union data{
        struct binaryExpr{
            AST* lhs;
            AST* rhs;
            Str op;
        } binaryExpr;
        struct booleanExpr{
            AST* lhs;
            AST* rhs;
            Str op;
        } booleanExpr;        
        struct program {
            AST_Elem* body;
        } program;        
        struct numberLiteral{
            Str data;
        } numberLiteral;
        struct stringLiteral{
            Str data;
        } stringLiteral;    
    } data;
    
    
} AST;

typedef struct AST_Elem {
    AST* data;
    AST_Elem* next;
} AST_Elem;

void astElem_append(AST_Elem** head, AST* data) {
    AST_Elem** curr = head;
    while ((*curr) != NULL)
        curr = &(*curr)->next;
    (*curr) = malloc(sizeof(AST_Elem));
    (*curr)->next = NULL;
    (*curr)->data = data;
}


void ast_print(AST* ast,int depth) {
    switch (ast->type)
    {
        case Program:
            AST_Elem* curr = ast->data.program.body;
            printf("%*sProgram:\n",depth,"");
            while (curr) {
                ast_print(curr->data,depth + 1);
                curr = curr->next;
            }
        break;
        case BinaryExp:
            printf("%*sBinaryExp:\n",depth,"");
            printf("%*sOp:"STR_FMT"\n",depth + 1,"",STR_ARG(ast->data.binaryExpr.op));
            ast_print(ast->data.binaryExpr.lhs,depth + 1);
            ast_print(ast->data.binaryExpr.rhs,depth + 1);
        break;
        case NumberLiteral:
            printf("%*sNumberLiteral:\n",depth,"");
            printf("%*svalue:"STR_FMT"\n",depth+1,"",STR_ARG(ast->data.numberLiteral.data));
        break;
        case StringLiteral:
            printf("%*sStringLiteral:\n",depth,"");
            printf("%*svalue:"STR_FMT"\n",depth+1,"",STR_ARG(ast->data.numberLiteral.data));
        break;
    
    default:
        assert(0 && "[Error] Unreachable `ast_print`\n");
    }
}

AST* parser_parse_primary();
AST* parser_parse_additive();
AST* parser_parse_multiplicative();
AST* parser_parse() {
    AST* program = malloc(sizeof(AST));
    program->type = Program;
    while (tokenIdx < Ltokens.count) {
        astElem_append(&program->data.program.body,parser_parse_additive());
        tokenIdx++;
    }
    return program;
}
AST* parser_parse_additive() { 
    AST* lhs = parser_parse_multiplicative();
    if(tokenIdx < Ltokens.count && (*Ltokens.tokens[tokenIdx].val.str == '+' || *Ltokens.tokens[tokenIdx].val.str == '-')) {
        Str op = Ltokens.tokens[tokenIdx].val;
        tokenIdx++;
        AST* rhs = parser_parse_additive();


        AST* ast = malloc(sizeof(AST));
        ast->type = BinaryExp;
        ast->data.binaryExpr.lhs =  lhs; 
        ast->data.binaryExpr.rhs =  rhs; 
        ast->data.binaryExpr.op =  op; 
        return ast; 
    }

    return lhs;
}
AST* parser_parse_multiplicative() { 
    AST* lhs = parser_parse_primary();
    if(tokenIdx < Ltokens.count && (*Ltokens.tokens[tokenIdx].val.str == '*' || *Ltokens.tokens[tokenIdx].val.str == '/')) {
        Str op = Ltokens.tokens[tokenIdx].val;
        tokenIdx++;
        AST* rhs = parser_parse_multiplicative();

        AST* ast = malloc(sizeof(AST));
        ast->type = BinaryExp;
        ast->data.binaryExpr.lhs =  lhs; 
        ast->data.binaryExpr.rhs =  rhs; 
        ast->data.binaryExpr.op =  op; 
        return ast; 
    }

    return lhs;
}

AST* parser_parse_primary() {
    Token tkn = Ltokens.tokens[tokenIdx]; 
    tokenIdx++;
    if(tkn.type == NumberVal) {
        AST* ast = malloc(sizeof(AST));
        ast->type = NumberLiteral;
        ast->data.numberLiteral.data =  tkn.val; 
        return ast;
    } else if(tkn.type == StringVal) 
    {
        AST* ast = malloc(sizeof(AST));
        ast->type = StringLiteral;
        ast->data.stringLiteral.data =  tkn.val; 
        return ast;
    } else {
        token_print(tkn);
        assert(0 && "Unreachable `parser_parse_primary`");
    }
}


int main(void) {
    char * file = lexer_read_file("main.skt");
    Ltokens = lexer_lex();


    // for(size_t i = 0; i < Ltokens.count; i++) {
    //     token_print(Ltokens.tokens[i]);
    // }
    AST* program =  parser_parse();
    ast_print(program,0);
    

    return 0;
}