#include "include/common.h"
#include "include/lex.h"
#include "include/ast.h"
#include "include/maker.h"

int mk_var(maker_t* parser) {
   
}

int make(maker_t* maker, lexer_t* lexer) {
  maker->lexer = lexer;
  
  return 1;
}
