#ifndef PARSER_H_
#define PARSER_H_

#include "state.h"


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
    } as;
    
    
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
            AST_Elem* curr = ast->as.program.body;
            printf("%*sProgram:\n",depth,"");
            while (curr) {
                ast_print(curr->data,depth + 1);
                curr = curr->next;
            }
        break;
        case BinaryExp:
            printf("%*sBinaryExp:\n",depth,"");
            printf("%*sOp:"STR_FMT"\n",depth + 1,"",STR_ARG(ast->as.binaryExpr.op));
            ast_print(ast->as.binaryExpr.lhs,depth + 1);
            ast_print(ast->as.binaryExpr.rhs,depth + 1);
        break;
        case NumberLiteral:
            printf("%*sNumberLiteral:\n",depth,"");
            printf("%*svalue:"STR_FMT"\n",depth+1,"",STR_ARG(ast->as.numberLiteral.data));
        break;
        case StringLiteral:
            printf("%*sStringLiteral:\n",depth,"");
            printf("%*svalue:"STR_FMT"\n",depth+1,"",STR_ARG(ast->as.numberLiteral.data));
        break;
    default:
        assert(0 && "[Error] Unreachable `ast_print`\n");
    }
}

AST* parser_parse_primary();
AST* parser_parse_additive();
AST* parser_parse_multiplicative();
AST* parser_parse_boolean();
AST* parser_parse_para();

void expect(TokenType type) {
    if(Ltokens.tokens[tokenIdx].type == type) {
        tokenIdx++;
        return;
    }
    printf("[Error] Expected %d token got",type);
    token_print(Ltokens.tokens[tokenIdx]);

}


AST* parser_parse() {
    AST* ast = malloc(sizeof(AST));
    ast->type = Program;
    while (tokenIdx < Ltokens.count) {
        astElem_append(&ast->as.program.body,parser_parse_additive());
        tokenIdx++;
    }
    return ast;
}


AST* parser_parse_para() {
  
    if(tokenIdx < Ltokens.count && (*Ltokens.tokens[tokenIdx].val.str == '(')) { 
        tokenIdx++;
        AST* lhs =  parser_parse_additive();

        expect(Cpara);
        return lhs;
    } else {
        return parser_parse_primary();
    }
}

AST* parser_parse_boolean() { 
    AST* lhs = parser_parse_multiplicative();
    if(tokenIdx < Ltokens.count && (*Ltokens.tokens[tokenIdx].val.str == '&' || *Ltokens.tokens[tokenIdx].val.str == '|' || *Ltokens.tokens[tokenIdx].val.str == '^' || *Ltokens.tokens[tokenIdx].val.str == '%' )) {
        Str op = Ltokens.tokens[tokenIdx].val;
        tokenIdx++;
        AST* rhs = parser_parse_boolean();

        AST* ast = malloc(sizeof(AST));
        ast->type = BinaryExp;
        ast->as.binaryExpr.lhs =  lhs; 
        ast->as.binaryExpr.rhs =  rhs; 
        ast->as.binaryExpr.op =  op; 
        return ast; 
    }

    return lhs;
}
AST* parser_parse_additive() { 
    AST* lhs = parser_parse_boolean();
    if(tokenIdx < Ltokens.count && (*Ltokens.tokens[tokenIdx].val.str == '+' || *Ltokens.tokens[tokenIdx].val.str == '-')) {
        Str op = Ltokens.tokens[tokenIdx].val;
        tokenIdx++;
        AST* rhs = parser_parse_boolean();


        AST* ast = malloc(sizeof(AST));
        ast->type = BinaryExp;
        ast->as.binaryExpr.lhs =  lhs; 
        ast->as.binaryExpr.rhs =  rhs; 
        ast->as.binaryExpr.op =  op; 
        return ast; 
    }

    return lhs;
}
AST* parser_parse_multiplicative() { 
    AST* lhs = parser_parse_para();
    if(tokenIdx < Ltokens.count && (*Ltokens.tokens[tokenIdx].val.str == '*' || *Ltokens.tokens[tokenIdx].val.str == '/')) {
        Str op = Ltokens.tokens[tokenIdx].val;
        tokenIdx++;
        AST* rhs = parser_parse_multiplicative();

        AST* ast = malloc(sizeof(AST));
        ast->type = BinaryExp;
        ast->as.binaryExpr.lhs =  lhs; 
        ast->as.binaryExpr.rhs =  rhs; 
        ast->as.binaryExpr.op =  op; 
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
        ast->as.numberLiteral.data =  tkn.val; 
        return ast;
    } else if(tkn.type == StringVal) 
    {
        AST* ast = malloc(sizeof(AST));
        ast->type = StringLiteral;
        ast->as.stringLiteral.data =  tkn.val; 
        return ast;
    } else {
        token_print(tkn);
        assert(0 && "Unreachable `parser_parse_primary`");
    }
}


#endif // PARSER_H_
