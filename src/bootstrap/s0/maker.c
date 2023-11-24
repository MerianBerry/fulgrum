#include "include/common.h"
#include "include/lex.h"
#include "include/ast.h"
#include "include/maker.h"
#include <stdio.h>

int mk_var(maker_t* parser) {
   
}

int make(maker_t* maker, lexer_t* lexer) {
  maker->lexer = lexer;
  for (int i = 0; i < lexer->tokenc; ++i) {
    char* str = str_colorfmt("%c(yellow)%i :: %s%c(reset)\n", lexer->tokenv[i].tk,
                             lexer->tokenv[i].ident);
    printf("%s", str);
    free(str);
  } 
  return 1;
}
