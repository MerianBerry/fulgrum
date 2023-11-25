#pragma once

#include "type.h"
typedef struct varinfo_s {
  const char*       name;
  type_t*           type;
  int               uses;
} varinfo_t;

typedef struct seminfo_s {
  varinfo_t*        varv;
  struct seminfo_s* semv;
  type_t**          typev;
  short             varc;
  short             semc;
  short             typec;
} seminfo_t;

typedef struct lexer_s lexer_t;

seminfo_t* semAnalyze(lexer_t* lex);

void semFree(seminfo_t* sinfo);