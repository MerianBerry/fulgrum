#include "include/seminfo.h"
#include "include/common.h"
#include <stdio.h>

varinfo_t semsearch(const char* name, lexer_t* lex, int start) {
  for (int i = start; i < lex->tokenc; ++i) {
    if (lex->tokenv[i].tk == tk_ident && !strcmp(lex->tokenv[i].ident, name)) {
      char* cfmtl = str_colorfmt("%c(cyan)%s left: %s%c(reset)\n", 
       name, lex->tokenv[i-1].ident);
      printf("%s", cfmtl);
      free(cfmtl);
      if (i+1 < lex->tokenc && lex->tokenv[i+1].tk == tk_oper) {
        char* cfmtr = str_colorfmt("%c(cyan)%s right: %s%c(reset)\n", 
       name, lex->tokenv[i+1].ident);
        printf("%s", cfmtr);
        free(cfmtr);
      } else {
        char* cfmtr = str_colorfmt("%c(blue)%s right: none%c(reset)\n", 
       name);
        printf("%s", cfmtr);
        free(cfmtr);
      }
    }
  }
  return (varinfo_t){0};
}

seminfo_t* semanalyze(lexer_t* lex) {
  seminfo_t root = {0};
  for (int i = 0; i < lex->tokenc; ++i) {
    token_t* t = &lex->tokenv[i];
    if (t->tk == tk_var) {
      char* cfmt = str_colorfmt("%c(bright_cyan)Var token %s%c(reset)\n", (t+1)->ident);
      printf("%s", cfmt);
      free(cfmt);
      semsearch((t+1)->ident, lex, i+1);
      ++i;
    }
  }
  return NULL;
}
