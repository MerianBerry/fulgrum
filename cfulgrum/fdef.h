#pragma once

#define MAKE_KEYWORD(word) \
  { #word, tk_##word }

enum {
  TK_VAR = 0x100,
  TK_FUNC,
  TK_IMPORT,
  TK_RETURN,
  TK_IF,
  TK_ELSE,
  /*TK_SWITCH,
  TK_case,
  TK_while,
  TK_for,
  TK_foreach,
  TK_break,
  TK_continue,*/
  TK_IDENT,
  TK_NUMBER,
  TK_STR,
  TK_NIL,
  TK_OPER,
  TK_MACCESS,
};

typedef unsigned char      uByte;
typedef unsigned short     uWord;
typedef unsigned int       uInt;
typedef unsigned long long uLong;
typedef signed char        sByte;
typedef signed short       sWord;
typedef signed int         sInt;
typedef signed long long   sLong;

typedef struct ful_Module {
} ful_Module;

typedef struct Token {
  char const* source;
  uInt        col;
  uInt        line;
  uWord       tk;
  uWord       len;
} Token;

typedef struct Keyword {
  char const* word;
  uWord       tk;
} Keyword;

typedef struct Lexer {
  Token        in_progress;
  char const*  fpath;
  char const*  content;
  Token*       tokenv;
  char const** linev;
  uInt         linec;
  uInt         tokenc;
  uInt         size;
  uInt         indx;
  int          cc;
  uInt         ccol;
  uInt         cln;
} Lexer;

typedef struct Phrase {
  struct Phrase* phrasev;
  void*          data;
  uInt           phrasec;
  uInt           size;
  uWord          type;
} Phrase;

typedef struct ParseInfo {
  Lexer const* lex;
  Phrase       root;
} ParseInfo;
