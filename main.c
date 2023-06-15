#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "state.h"
#include "lexer.h"
#include "parser.h"
#include "pphat.h"
#include "interpreter.h"
#include "str.h"







int main(void) {
    lexer_init("main.skt");
    char * file = lexer_read_file();
    
    globaEnv.parent = NULL;
    globaEnv.vars = pphat_create_runtime();

    Ltokens = lexer_lex();

    printf("Lexer Output: \n");
    printf("---------------------------- \n");
    for(int i = 0 ; i  < Ltokens.count; i++) {
        token_print(Ltokens.tokens[i]);
    }
    printf("---------------------------- \n");
    
    
    printf("Parser Output: \n");
    printf("---------------------------- \n");
    AST* program =  parser_parse();
    ast_print(program,0);
    printf("---------------------------- \n");

    printf("Program Output: \n");
    printf("============================ \n");
        interpreter_eval(program,&globaEnv);
    printf("============================ \n");

    // printf("Global variables: \n");
    // printf("---------------------------- \n");
    // pphat_print_runtime(globaEnv.vars);
    // printf("---------------------------- \n");

    return 0;
}
