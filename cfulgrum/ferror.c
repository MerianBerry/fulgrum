#define HYDROGEN_STRING
#define HYDROGEN_MATH
#include "ferror.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hydrogen/hydrogen.h"

int const peekc = 15;

void stdError (char const* fmt, ...) {
  va_list args;
  va_start (args, fmt);
  char* fmtstr  = str_fmtv (fmt, args);
  char* cfmtstr = str_colorfmt ("&c(bright_red)Error: %s&c(reset)", fmtstr);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
}

void detailedError (Lexer const* l, Token const* t, char const* fmt, ...) {
  if (!l || !t) {
    stdError ("Lexer or token pointer is null. Cannot make detailed error\n");
    return;
  }
  if (t->line >= l->linec) {
    stdError ("Token line number is too large. %i / %i\n", t->line + 1,
              l->linec);
    return;
  }
  va_list args;
  va_start (args, fmt);
  uInt        i;
  char const* tline   = l->linev[t->line - 1];
  int         l0      = strlen (tline);
  int         l1      = t->len;
  int         off     = clampi (t->col - 1 - peekc - l1, 0, l0 - 1);
  int         len     = clampi (l1 + peekc * 2, 0, l0 - 1 - off);
  char const* content = str_substr (tline, off, len);
  char*       fmtstr  = str_fmtv (fmt, args);
  char* cfmtstr = str_colorfmt ("&c(bright_red)%s: error: %s&c(reset)\t%s\n",
                                l->fpath, fmtstr, content);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
  printf ("\t");
  for (i = 0; i < mini (t->col - l1 - 1, peekc); ++i) {
    printf (" ");
  }
  for (i = 0; i < l1; ++i) {
    printf ("^");
  }
  printf ("\n");
}

void stdWarning (char const* fmt, ...) {
  va_list args;
  va_start (args, fmt);
  char* fmtstr = str_fmtv (fmt, args);
  char* cfmtstr =
      str_colorfmt ("&c(bright_yellow)Warning: %s&c(reset)", fmtstr);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
}

void detailedWarning (Lexer const* l, Token const* t, char const* fmt, ...) {
  if (!l || !t) {
    stdError ("Lexer or token pointer is null. Cannot make detailed warning\n");
    return;
  }
  if (t->line >= l->linec) {
    stdError ("Token line number is too large. %i / %i\n", t->line + 1,
              l->linec);
    return;
  }
  va_list args;
  va_start (args, fmt);
  uInt        i;
  char const* tline   = l->linev[t->line - 1];
  int         l0      = strlen (tline);
  int         l1      = t->len;
  int         off     = clampi (t->col - 1 - peekc - l1, 0, l0 - 1);
  int         len     = clampi (l1 + peekc * 2, 0, l0 - 1 - off);
  char const* content = str_substr (tline, off, len);
  char*       fmtstr  = str_fmtv (fmt, args);
  char*       cfmtstr =
      str_colorfmt ("&c(bright_yellow)%s: warning: %s&c(reset)\t%s\n", l->fpath,
                    fmtstr, content);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
  printf ("\t");
  for (i = 0; i < mini (t->col - l1 - 1, peekc); ++i) {
    printf (" ");
  }
  for (i = 0; i < l1; ++i) {
    printf ("^");
  }
  printf ("\n");
}

void stdNote (char const* fmt, ...) {
  va_list args;
  va_start (args, fmt);
  char* fmtstr  = str_fmtv (fmt, args);
  char* cfmtstr = str_colorfmt ("&c(bright_magenta)Note: %s&c(reset)", fmtstr);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
}