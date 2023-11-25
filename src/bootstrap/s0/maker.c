#include "include/common.h"
#include "include/lex.h"
#include "include/ast.h"
#include "include/maker.h"
#include "include/seminfo.h"
#include "include/error.h"
#include <stdio.h>

int mk_var(maker_t* parser) {
   
}

int make(lexer_t* lexer) {
  maker_t maker = {0};
  maker.lexer = lexer;
  seminfo_t* sinfo = semAnalyze(lexer);
  maker.seminfo = sinfo;
  if (!sinfo) {
    stdError("Failed to do semantic analyzation\n");
    return -1;
  }
  semFree(maker.seminfo);
  return 0;
}
