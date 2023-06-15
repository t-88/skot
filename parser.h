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
        IfStatement,
        NumberLiteral,
        StringLiteral,
        VarDeclarationExpr,
        IdentifierLiteral,
        VarAssignmentExpr,
        BlockExpr,
        OutputStatment,
        ForStatement,
        WhileStatement,
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
        struct block {
            AST_Elem* body;
        } block;         
        struct numberLiteral{
            Str data;
        } numberLiteral;
        struct stringLiteral{
            Str data;
        } stringLiteral;    

        struct varDeclaration{
            Str name;
            AST* value;
        } varDeclaration;        
        struct identifierLiteral{
            Str name;
            AST* value;
        } identifierLiteral;                    
        struct varAssignment{
            Str name;
            AST* value;
        } varAssignment;     
        struct ifStatement{
            AST* cond;
            AST* if_block;
            AST_Elem* else_if_blocks;
            AST* else_block;
        } ifStatement;    
        struct whileStatement{
            AST* cond;
            AST* block;
        } whileStatement;            
        struct forStatement{
            AST* start;
            AST* end;
            AST* block;
        } forStatement;            
        struct outputStatment{
            AST* expr;
        } outputStatment;                 
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
    if(ast == NULL) {
        printf("%*sNull Value: NULL\n",depth,"");
        return;
    }
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
            printf("%*sOp: "STR_FMT"\n",depth + 1,"",STR_ARG(ast->as.binaryExpr.op));
            ast_print(ast->as.binaryExpr.lhs,depth + 1);
            ast_print(ast->as.binaryExpr.rhs,depth + 1);
        break;
        case BooleanExp:
            printf("%*sBooleanExp:\n",depth,"");
            printf("%*sOp: "STR_FMT"\n",depth + 1,"",STR_ARG(ast->as.booleanExpr.op));
            ast_print(ast->as.booleanExpr.lhs,depth + 1);
            ast_print(ast->as.booleanExpr.rhs,depth + 1);
        break;        
        case NumberLiteral:
            printf("%*sNumberLiteral:\n",depth,"");
            printf("%*svalue: "STR_FMT"\n",depth+1,"",STR_ARG(ast->as.numberLiteral.data));
        break;
        case StringLiteral:
            printf("%*sStringLiteral:\n",depth,"");
            printf("%*svalue: "STR_FMT"\n",depth+1,"",STR_ARG(ast->as.numberLiteral.data));
        break;
        case IdentifierLiteral:
            printf("%*sIdentifierLiteral:\n",depth,"");
            printf("%*svalue: "STR_FMT"\n",depth+1,"",STR_ARG(ast->as.identifierLiteral.name));
            ast_print(ast->as.identifierLiteral.value,depth + 1);
        break;
        case VarDeclarationExpr:
            printf("%*sVarDeclarationExpr:\n",depth,"");
            printf("%*svar: "STR_FMT"\n",depth+1,"",STR_ARG(ast->as.varDeclaration.name));
            ast_print(ast->as.varDeclaration.value,depth + 1);
        break;    
        case VarAssignmentExpr:
            printf("%*sVarAssignmentExpr:\n",depth,"");
            printf("%*svar: "STR_FMT"\n",depth+1,"",STR_ARG(ast->as.varAssignment.name));
            ast_print(ast->as.varAssignment.value,depth + 1);
        break;   
        case IfStatement:
            printf("%*sIf_Statement:\n",depth,"");
            ast_print(ast->as.ifStatement.cond,depth + 1);
            AST_Elem* currInIfElse = ast->as.ifStatement.else_if_blocks;
            while (currInIfElse) {
                ast_print(currInIfElse->data,depth + 1);
                currInIfElse = currInIfElse->next;
            }
            ast_print(ast->as.ifStatement.if_block,depth + 1);
        break;  
        case BlockExpr:
            printf("%*sBlockExpr:\n",depth,"");
            AST_Elem* currInBlock = ast->as.program.body;
            while (currInBlock) {
                ast_print(currInBlock->data,depth + 1);
                currInBlock = currInBlock->next;
            }
        break;
        case ForStatement:
            printf("%*sForStatement:\n",depth,"");
            ast_print(ast->as.forStatement.start,depth + 1);
            ast_print(ast->as.forStatement.end,depth + 1);
            ast_print(ast->as.forStatement.block,depth + 1);
        break;   
        case WhileStatement:
            printf("%*sWhileStatement:\n",depth,"");
            ast_print(ast->as.whileStatement.cond,depth + 1);
            ast_print(ast->as.whileStatement.block,depth + 1);
        break;         
        case OutputStatment:
            printf("%*sOutputStatment:\n",depth,"");
            ast_print(ast->as.outputStatment.expr,depth + 1);
        break;                                
    default:
        assert(0 && "[Error] Unreachable `ast_print`\n");
    }
}

AST* parser_parse_primary();
AST* parser_parse_para();
AST* parser_parse_multiplicative();
AST* parser_parse_additive();
AST* parser_parse_boolean();
AST* parser_parse_boolean_expr();
AST* parser_parse_var_declaration();
AST* parser_parse_statement();
AST* parser_parse_var_assignment();
AST* parser_parse_if_statement();
AST* parser_parse_for_statement();
AST* parser_parse_while_statement();

AST* parser_parse_output_statment();
AST* parser_parse_block();


bool peak(TokenType tkn_type) {
    return Ltokens.tokens[tokenIdx].type == tkn_type;
}
bool peakNext(TokenType tkn_type) {
    return Ltokens.tokens[tokenIdx+1].type == tkn_type;
}

Token expect(TokenType type) {
    if(Ltokens.tokens[tokenIdx].type == type) {
        return Ltokens.tokens[tokenIdx++];
    }
    printf("[Error] Expected:");
    token_print((Token){.type=type});
    printf("        Got: ");
    token_print(Ltokens.tokens[tokenIdx]);

    exit(1);
}


AST* parser_parse() {

    AST* ast = malloc(sizeof(AST));
    ast->type = Program;
    while (Ltokens.tokens[tokenIdx].type != Eof) {
        if(peak(VarDeclaration) ||  peak(Token_If_Keyword) || peak(Token_Output) || peak(Token_For_keyword) || peak(Token_While_keyword)) {
            astElem_append(&ast->as.program.body,parser_parse_statement());
        } else {
            astElem_append(&ast->as.program.body,parser_parse_var_assignment());
            expect(SemiColon);
        }
    }
    return ast;
}




AST* parser_parse_var_assignment() {
    AST* lhs = parser_parse_boolean_expr();
    if(tokenIdx < Ltokens.count && Ltokens.tokens[tokenIdx].type == Equal)  {
        tokenIdx++;
        AST* rhs = parser_parse_var_assignment();

        AST* varAssig = malloc(sizeof(AST));
        varAssig->type = VarAssignmentExpr;
        varAssig->as.varDeclaration.name = lhs->as.identifierLiteral.name;
        varAssig->as.varDeclaration.value = rhs;
        return varAssig;
    }
    return lhs;
}
AST* parser_parse_statement() {


    if(peak(VarDeclaration)) {
        return parser_parse_var_declaration();
    } else if (peak(Token_If_Keyword)) {
        return parser_parse_if_statement();
    } else if (peak(Token_Output)) {
        return parser_parse_output_statment();
    } else if (peak(Token_For_keyword)) {
        return parser_parse_for_statement();
    } else if (peak(Token_While_keyword)) {
        return parser_parse_while_statement();
        
    } 
    
    
    assert(0 && "Unreachable in `parser_parse_statement`");
    return NULL; 
}

AST* parser_parse_block() { 
    AST* block = malloc(sizeof(AST));

    block->type = BlockExpr;
    while(!peak(Token_CsqrPra)) {
        if(peak(VarDeclaration) ||  peak(Token_If_Keyword) || peak(Token_Output) || peak(Token_For_keyword) || peak(Token_While_keyword)) {
            astElem_append(&block->as.block.body,parser_parse_statement());
        } else {
            astElem_append(&block->as.block.body,parser_parse_var_assignment());
            expect(SemiColon);
        }
    }
    return block;
}


AST* parser_parse_for_statement() { 
    tokenIdx++;
    AST* output = malloc(sizeof(AST));
    output->type = ForStatement;
    expect(Opara);
        output->as.forStatement.start = parser_parse_var_assignment();
        expect(Token_Range);
        output->as.forStatement.end = parser_parse_var_assignment();
    expect(Cpara);
    expect(Token_OsqrPra);
        output->as.forStatement.block = parser_parse_block();
    expect(Token_CsqrPra);

    return output;
    
}


AST* parser_parse_while_statement() { 
    tokenIdx++;
    AST* output = malloc(sizeof(AST));
    output->type = WhileStatement;
    
    expect(Opara);
        output->as.whileStatement.cond = parser_parse_boolean_expr();
    expect(Cpara);
    
    expect(Token_OsqrPra);
        output->as.whileStatement.block = parser_parse_block();
    expect(Token_CsqrPra);

    return output;
}

AST* parser_parse_output_statment() { 
    tokenIdx++;
    AST* output = malloc(sizeof(AST));
    output->type = OutputStatment;
    expect(Opara);
        output->as.outputStatment.expr = parser_parse_boolean_expr();
    expect(Cpara);
    expect(SemiColon);
    return output;
} 
AST* parser_parse_if_statement() { 
    tokenIdx++;
    AST* ifSta = malloc(sizeof(AST));
    ifSta->type = IfStatement;
    ifSta->as.ifStatement.else_block = NULL;
    ifSta->as.ifStatement.else_if_blocks = NULL;
    expect(Opara);
        ifSta->as.ifStatement.cond = parser_parse_boolean_expr();
    expect(Cpara);
    expect(Token_OsqrPra);
        ifSta->as.ifStatement.if_block = parser_parse_block();
    expect(Token_CsqrPra);
    
    while(peak(Token_Else_Keyword) && peakNext(Token_If_Keyword)) {
        tokenIdx+=2;
        AST* elseIfSta = malloc(sizeof(AST));
        elseIfSta->type = IfStatement;
        elseIfSta->as.ifStatement.else_block = NULL;
        elseIfSta->as.ifStatement.else_if_blocks = NULL;
        expect(Opara);
            elseIfSta->as.ifStatement.cond = parser_parse_boolean_expr();
        expect(Cpara);
        expect(Token_OsqrPra);
            elseIfSta->as.ifStatement.if_block = parser_parse_block();
        expect(Token_CsqrPra);
        astElem_append(&ifSta->as.ifStatement.else_if_blocks,elseIfSta);
    }
    if(peak(Token_Else_Keyword)) {
        tokenIdx++;
        printf("asdasd\n");
        expect(Token_OsqrPra);
            ifSta->as.ifStatement.else_block = parser_parse_block();
        expect(Token_CsqrPra);
    }
    return ifSta;
} 
AST* parser_parse_var_declaration() { 
    tokenIdx++;
    AST* varDec = malloc(sizeof(AST));


    varDec->type = VarDeclarationExpr;
    varDec->as.varDeclaration.name = expect(Identifier).val;

    expect(Equal);

    AST* rhs = parser_parse_additive();

    varDec->as.varDeclaration.value = rhs;
    expect(SemiColon);
    return varDec;
}
AST* parser_parse_boolean_expr() { 
    AST* lhs = parser_parse_additive();
    if(Ltokens.tokens[tokenIdx].type == Token_Comparation) {
        Str op = Ltokens.tokens[tokenIdx].val;
        tokenIdx++;
        AST* rhs = parser_parse_boolean_expr();
        
        AST* comparationOp = malloc(sizeof(AST));
        comparationOp->type = BooleanExp;
        comparationOp->as.booleanExpr.op = op;
        comparationOp->as.booleanExpr.rhs = rhs;
        comparationOp->as.booleanExpr.lhs = lhs;

        return comparationOp;
    }
    return lhs;
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
        AST* rhs = parser_parse_additive();


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
    } else if(tkn.type == StringVal)  {
        AST* ast = malloc(sizeof(AST));
        ast->type = StringLiteral;
        ast->as.stringLiteral.data =  tkn.val; 
        return ast;
    } 
    else if(tkn.type == Identifier)  { 
        AST* ast = malloc(sizeof(AST));
        ast->type = IdentifierLiteral;
        ast->as.identifierLiteral.name =  tkn.val; 
        ast->as.identifierLiteral.value =  NULL; 
        return ast;
    }
    else {
        token_print(tkn);
        assert(0 && "Unreachable `parser_parse_primary`");
    }
}


#endif // PARSER_H_
