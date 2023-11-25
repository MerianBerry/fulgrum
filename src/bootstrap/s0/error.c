#include "include/tokens.h"
#include "include/lex.h"
#include "include/error.h"
#include "include/common.h"
#include <stdio.h>

const int peekc = 15;

void stdError(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char* fmtstr = str_fmtv(fmt, args);
  char* cfmtstr = str_colorfmt("%c(bright_red)Error: %s%c(reset)", fmtstr);
  free(fmtstr);
  printf("%s", cfmtstr);
  free(cfmtstr);
  va_end(args);
}

void detailedError(lexer_t* l, token_t* t, const char* fmt, ...) {
  if (!l || !t) {
    stdError("Lexer or token pointer is null. Cannot make detailed error\n");
    return;
  }
  if (t->ln >= l->linec) {
    stdError("Token line number is too large. %i / %i\n", t->ln+1, l->linec);
    return;
  }
  const char* tline = l->linev[t->ln-1];
  int l0 = strlen(tline);
  int l1 = strlen(t->ident);
  int off = clampi(t->col-1-peekc-l1, 0, l0-1);
  int len = clampi(l1+peekc*2, 0, l0-1-off);
  //printf("col = %i, ln = %i, l0 = %i, l1 = %i, off = %i, len = %i\n",
  //  t->col-l1, t->ln, l0, l1, off, len);
  const char* content = str_substr(tline, off, len);
  va_list args;
  va_start(args, fmt);
  char* fmtstr = str_fmtv(fmt, args);
  char* cfmtstr = str_colorfmt("%c(bright_red)Error [%s]: %s%c(reset)\t%s\n",
    l->fpath, fmtstr, content);
  free(fmtstr);
  printf("%s", cfmtstr);
  free(cfmtstr);
  va_end(args);
  printf("\t");
  for (int i = 0; i < mini(t->col-l1-1, peekc); ++i) {
    printf(" ");
  }
  for (int i = 0; i < l1; ++i) {
    printf("^");
  }
  printf("\n");
}

void stdWarning(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char* fmtstr = str_fmtv(fmt, args);
  char* cfmtstr = str_colorfmt("%c(bright_yellow)Warning: %s%c(reset)", fmtstr);
  free(fmtstr);
  printf("%s", cfmtstr);
  free(cfmtstr);
  va_end(args);
}

void detailedWarning(lexer_t* l, token_t* t, const char* fmt, ...) {
  if (!l || !t) {
    stdError("Lexer or token pointer is null. Cannot make detailed warning\n");
    return;
  }
  if (t->ln >= l->linec) {
    stdError("Token line number is too large. %i / %i\n", t->ln+1, l->linec);
    return;
  }
  const char* tline = l->linev[t->ln-1];
  int l0 = strlen(tline);
  int l1 = strlen(t->ident);
  int off = clampi(t->col-1-peekc-l1, 0, l0-1);
  int len = clampi(l1+peekc*2, 0, l0-1-off);
  //printf("col = %i, ln = %i, l0 = %i, l1 = %i, off = %i, len = %i\n",
  //  t->col-l1, t->ln, l0, l1, off, len);
  const char* content = str_substr(tline, off, len);
  va_list args;
  va_start(args, fmt);
  char* fmtstr = str_fmtv(fmt, args);
  char* cfmtstr = str_colorfmt("%c(bright_yellow)Warning [%s]: %s%c(reset)\t%s\n",
    l->fpath, fmtstr, content);
  free(fmtstr);
  printf("%s", cfmtstr);
  free(cfmtstr);
  va_end(args);
  printf("\t");
  for (int i = 0; i < mini(t->col-l1-1, peekc); ++i) {
    printf(" ");
  }
  for (int i = 0; i < l1; ++i) {
    printf("^");
  }
  printf("\n");
}

void stdNote(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char* fmtstr = str_fmtv(fmt, args);
  char* cfmtstr = str_colorfmt("%c(bright_magenta)Note: %s%c(reset)", fmtstr);
  free(fmtstr);
  printf("%s", cfmtstr);
  free(cfmtstr);
  va_end(args);
}