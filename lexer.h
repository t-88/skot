#ifndef LEXER_H_
#define LEXER_H_
#include <stdio.h>
#include "pphat.h"
#include "str.h"

#define TOKEN_FMT STR_FMT
#define TOKEN_ARG(token) (int)token.val.count,  token.val.str
#define MK_TOKEN(tknType,s,c) (Token) {.val = (Str) {.str=(s),.count=(c)} ,.type=(tknType)}
#define NUM_OF_TOKENS 1000





static char* lexed_file;
static size_t file_size;
static PpHat* reserved_keywords;



typedef enum TokenType {
    NumberVal,
    Identifier,
    Equal,
    BinaryOp,
    StringVal,
    Opara, 
    Cpara,
    VarDeclaration,
    SemiColon,
    Token_Comparation,
    Eof,
} TokenType;



typedef struct {
    Str val;
    TokenType type;
} Token;

typedef struct {
    Token* tokens;
    size_t count;
} LexerTokens;



void token_print(Token tkn) {
    printf("Token: ");
    switch (tkn.type) {
    case NumberVal:printf("NumberVal");break;
        case Identifier:printf("Identifier");break;    
        case Equal:printf("Equal");break;   
        case BinaryOp:printf("BinaryOp");break;            
        case StringVal:printf("StringVal");break;            
        case Opara:printf("Oprara");break;            
        case Cpara: printf("Cprara"); break;            
        case VarDeclaration: printf("VarDeclaration"); break;            
        case SemiColon: printf("SemiColon"); break;            
        case Token_Comparation: printf("Token_Comparation"); break;            
        case Eof: printf("Eof"); break;            
        default: assert(0 && "Unreachable in `token_print`");
    }
    printf(" ");
    str_print(tkn.val);
}
    
char* lexer_free() { 
    pphat_free(reserved_keywords);
}




char* lexer_read_file() {
    FILE *f = fopen(lexed_file,"rb");
    if(f == NULL) { 
        printf("[Error] could not open file\n");
        exit(1);
    }
    int res =  fseek (f, 0,SEEK_END);
    if(res == -1) {
        printf("[Error] could not read file using 'fseek'\n");
        free(f);
        exit(1);
    }
    file_size = ftell(f);
    if(file_size == -1) {
        printf("[Error] could not read file using 'ftell'\n");
        free(f);
        exit(1);
    }
    res = fseek(f,0,SEEK_SET);
    if(res == -1) {
        printf("[Error] could not read file using 'rewind'\n");
        free(f);
        exit(1);
    }
    lexed_file =  malloc(sizeof(char) *file_size);// ARENA_ALLOC(*lexer_arena,char,fs);
    res = fread(lexed_file,file_size,1,f);
    if(res == -1) {
        free(f);
        free(lexed_file); 
        printf("[Error] could not read file using 'fread'\n");
        exit(1);
    }
    return lexed_file;
} 


void lexer_init(char* file_path) {
    lexed_file = file_path;
    reserved_keywords = pphat_create();
    pphat_insert(&reserved_keywords,"let",VarDeclaration);
}

LexerTokens lexer_lex() {
    LexerTokens lex_tokens = {0};
    lex_tokens.tokens = malloc(sizeof(Token) * NUM_OF_TOKENS); 
    int curr = -1;
    while(++curr != file_size) {
        // printf("%.*s %d %lu\n" ,1,lexed_file + curr,curr,file_size);
        if(isspace(lexed_file[curr]) || lexed_file[curr] == '\n' || lexed_file[curr] == '\t') {}
        else if(lexed_file[curr] == '=')  { 
                lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(Equal,lexed_file + curr,1);
        }
        else if(lexed_file[curr] == '<' || lexed_file[curr] == '>')  {
            if(lexed_file[curr + 1] == '=') {
                lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(Token_Comparation,lexed_file + curr,2);
                curr++;
            } else {

                lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(Token_Comparation,lexed_file + curr,1);
            }
        } 
        else if(lexed_file[curr] == ';')  {
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(SemiColon,lexed_file + curr,1);
        }
        else if(lexed_file[curr] == '"') 
        {
            curr++;
            int start = curr;
            while(lexed_file[curr] != 0 && lexed_file[curr] != '"')
                curr++;
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(StringVal,lexed_file + start,curr - start);
            curr--;
        } 
        else if(lexed_file[curr] == '(') {
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(Opara,lexed_file + curr,1);
        }
        else if(lexed_file[curr] == ')') {
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(Cpara,lexed_file + curr,1);
        }
        else if(lexed_file[curr] == '+' || lexed_file[curr] == '-' || lexed_file[curr] == '*' || lexed_file[curr] == '/' || lexed_file[curr] == '|' || lexed_file[curr] == '^' || lexed_file[curr] == '&' || lexed_file[curr] == '%' ) 
        {
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(BinaryOp,lexed_file + curr,1);
        } 
        else if(isalpha(lexed_file[curr])) 
        {
            int start = curr;
            while(lexed_file[curr] != '\0' && isalpha(lexed_file[curr]))
                curr++;
            

            Str str  = (Str) {.str =  lexed_file + start,.count =  curr - start};
            int fromTable = pphat_get(reserved_keywords,str_get_charp(str));
            if(fromTable == 0) {
                lex_tokens.tokens[lex_tokens.count++] = (Token){.type = Identifier,.val = str};
            } else  {
            printf("%d\n",fromTable);
                lex_tokens.tokens[lex_tokens.count++] = (Token){.type = fromTable,.val = str};
            }
            curr--;
        } 
        else if (isdigit(lexed_file[curr])) 
        {
            int start = curr;
            while(lexed_file[curr] != '\0' && isdigit(lexed_file[curr])) 
                curr++;
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(NumberVal,lexed_file + start ,curr - start);
            curr--;
        }
        else {
            printf("`%.*s` in %d\n",1,lexed_file + curr ,curr);
            assert(0 && "[Error] char not recognized");
        }

    }
    lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(Eof,"eof",3);

    return lex_tokens;
}



#endif //LEXER_H_
