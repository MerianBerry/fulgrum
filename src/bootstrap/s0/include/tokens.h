#pragma once

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

typedef struct token_s {
  char*       ident;
  short       tk;
  int         col;
  int         ln;
} token_t;
