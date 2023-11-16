#pragma once

#include "tokens.h"
#include "keyword.h"

#define MAKE_KEYWORD(word) {#word, tk_ ##word}

typedef struct {
  const char*   content;
  char*         ident;
  token_t*      tokenv;
  int           tokenc;
  int           size;
  int           indx;
  int           cc;
} lexer_t;


int lex(lexer_t* lexer, const char* content);