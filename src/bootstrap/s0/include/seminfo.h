#pragma once

#include "type.h"
typedef struct varinfo_s {
  const char*       name;
  type_t*           typev;
  short             typec;
  short             maxsize;
} varinfo_t;

typedef struct seminfo_s {
  varinfo_t*        varv;
  struct seminfo_s* semv;
  short             varc;
  short             semc;
} seminfo_t;

#include "lex.h"

seminfo_t* semanalyze(lexer_t* lex);
