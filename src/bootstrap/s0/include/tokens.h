#pragma once

enum {
  tk_var = 1,
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

typedef struct {
  char*       ident;
  short       tk;
} token_t;