#pragma once

typedef struct lexer_s lexer_t;

typedef struct symbol_s {
  char*           name;
  int             pos;
} symbol_t;

typedef struct relocation_s {
  int             info;
  int             pos;
} relocation_t;

typedef struct maker_s {
  lexer_t*        lexer;
  char*           code;
  int             csize;
  char*           data;
  int             dsize;
  symbol_t*       symbv;
  int             symbc;
  relocation_t*   relocv;
  int             relocc;
} maker_t;

int make(maker_t* maker, lexer_t* lexer);
