#ifndef LEXER_H_
#define LEXER_H_
#include <stdio.h>


#define STR_FMT "%.*s" 
#define STR_ARG(token) (int)token.count,  token.str

#define TOKEN_FMT STR_FMT
#define TOKEN_ARG(token) (int)token.val.count,  token.val.str
#define MK_TOKEN(tknType,s,c) (Token) {.val = (Str) {.str=(s),.count=(c)} ,.type=(tknType)}
#define NUM_OF_TOKENS 1000

static char* lexed_file;

typedef struct {
    const char* str;
    size_t count; 
} Str;





typedef enum TokenType {
    NumberVal,
    Identifier,
    Equal,
    BinaryOp,
    StringVal
} TokenType;
typedef struct {
    Str val;
    TokenType type;
} Token;

typedef struct {
    Token* tokens;
    size_t count;
} LexerTokens;


void lexer_set_file(char* file) {
    lexed_file = file;
}



void token_print(Token tkn) {
    printf("Token: ");
    switch (tkn.type) {
    case NumberVal:
        printf("NumberVal");
    break;
    case Identifier:
        printf("Identifier");
    break;    
    case Equal:
        printf("Equal");
    break;   
    case BinaryOp:
        printf("BinaryOp");
    break;            
    case StringVal:
        printf("StringVal");
    break;            
    default:
        assert(0 && "Unreachable");
    }
    printf(" " TOKEN_FMT "\n",TOKEN_ARG(tkn));
}
void str_print(Str str) {
    printf(STR_FMT"\n",STR_ARG(str));
}


char* lexer_read_file(const char* file_path) {
    lexed_file = NULL;
    FILE *f = fopen(file_path,"rb");
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
    size_t fs = ftell(f);
    if(fs == -1) {
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
    lexed_file =  malloc(sizeof(char) *fs);// ARENA_ALLOC(*lexer_arena,char,fs);
    res = fread(lexed_file,fs,1,f);
    if(res == -1) {
        free(f);
        free(lexed_file); 
        printf("[Error] could not read file using 'fread'\n");
        exit(1);
    }
    return lexed_file;
} 



LexerTokens lexer_lex() {
    LexerTokens lex_tokens = {0};
    lex_tokens.tokens = malloc(sizeof(Token) * NUM_OF_TOKENS); 

    int curr = -1;

    while(lexed_file[++curr] != '\0') {

        if(isspace(lexed_file[curr]) || lexed_file[curr] == '\n' || lexed_file[curr] == '\t') {

        } 
        else if(lexed_file[curr] == '=') 
        {
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(Equal,lexed_file,1);
        } 
        else if(lexed_file[curr] == '"') 
        {
            curr++;
            int start = curr;
            while(lexed_file[curr] != 0 && lexed_file[curr] != '"')
                curr++;
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(StringVal,lexed_file + start,curr - start);
        } 
        else if(lexed_file[curr] == '+' || lexed_file[curr] == '-' || lexed_file[curr] == '*' || lexed_file[curr] == '/') 
        {
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(BinaryOp,lexed_file + curr,1);
        } 
        else if(isalpha(lexed_file[curr])) 
        {
            int start = curr;
            while(lexed_file[curr] != 0 && isalpha(lexed_file[curr]))
                curr++;
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(Identifier,lexed_file + start,curr - start + 1);
        } 
        else if (isalnum(lexed_file[curr])) 
        {
            int start = curr;
            while(lexed_file[curr] != '\0' && isalnum(lexed_file[curr])) 
                curr++;
            lex_tokens.tokens[lex_tokens.count++] = MK_TOKEN(NumberVal,lexed_file + start ,curr - start + 1);
        }
        
    }

    return lex_tokens;
}



#endif //LEXER_H_
