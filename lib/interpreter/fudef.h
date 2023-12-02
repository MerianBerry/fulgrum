#pragma once

#define MAKE_KEYWORD(word) \
  { #word, tk_##word }

enum {
  tk_var = 0x100,
  tk_func,
  tk_class,
  tk_import,
  tk_reserve,
  tk_return,
  tk_if,
  tk_else,
  tk_switch,
  tk_case,
  tk_while,
  tk_for,
  tk_foreach,
  tk_break,
  tk_continue,
  tk_ident,
  tk_number,
  tk_str,
  tk_nil,
  tk_oper,
};

typedef unsigned char  fu_byte;
typedef unsigned short fu_word;
typedef unsigned int   fu_dword;

typedef struct fu_module {
} fu_module;

typedef struct fu_token {
  char*    ident;
  fu_word  tk;
  fu_dword col;
  fu_dword ln;
} fu_token;

typedef struct fu_keyword {
  char const* word;
  fu_word     tk;
} fu_keyword;

typedef struct fu_lexer {
  char const*  fpath;
  char const*  content;
  char*        ident;
  fu_token*    tokenv;
  char const** linev;
  fu_dword     linec;
  fu_dword     tokenc;
  fu_dword     size;
  fu_dword     indx;
  int          cc;
  fu_dword     ccol;
  fu_dword     cln;
} fu_lexer;

typedef struct fu_type {
  char const* name;
  short       id;
  short       size;
} fu_type;

typedef struct fu_varInfo {
  char const* name;
  fu_type**   typev;
  fu_dword    typec;
  fu_dword    uses;
  fu_dword    pos;
} fu_varInfo;

typedef struct fu_semInfo {
  fu_lexer const*    lex;
  fu_varInfo*        varv;
  struct fu_semInfo* semv;
  fu_type**          typev;
  short              varc;
  short              semc;
  short              typec;
  int                memsize;
} fu_semInfo;

typedef struct fu_irBuilder {
} fu_irBuilder;