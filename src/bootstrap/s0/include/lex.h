#pragma once

#include "keyword.h"

typedef struct token_s token_t;

#define MAKE_KEYWORD(word) {#word, tk_ ##word}

typedef struct lexer_s {
  const char*   fpath;
  const char*   content;
  char*         ident;
  token_t*      tokenv;
  const char**  linev;
  int           linec;
  int           tokenc;
  int           size;
  int           indx;
  int           cc;
  int           ccol;
  int           cln;
} lexer_t;


int lex(lexer_t* lexer, const char* content);

void lx_freeTokens(lexer_t* lexer);

void lx_freeLexer(lexer_t* lexer);