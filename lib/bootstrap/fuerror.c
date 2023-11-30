#define NO_DIRENT
#include "fuerror.h"

#include <stdio.h>

#include "common.h"

int const peekc = 15;

void stdError (char const* fmt, ...) {
  va_list args;
  va_start (args, fmt);
  char* fmtstr  = str_fmtv (fmt, args);
  char* cfmtstr = str_colorfmt ("%c(bright_red)Error: %s%c(reset)", fmtstr);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
}

void detailedError (fu_lexer const* l, fu_token const* t, char const* fmt,
                    ...) {
  if (!l || !t) {
    stdError ("Lexer or token pointer is null. Cannot make detailed error\n");
    return;
  }
  if (t->ln >= l->linec) {
    stdError ("Token line number is too large. %i / %i\n", t->ln + 1, l->linec);
    return;
  }
  char const* tline = l->linev[t->ln - 1];
  int         l0    = strlen (tline);
  int         l1    = strlen (t->ident);
  int         off   = clampi (t->col - 1 - peekc - l1, 0, l0 - 1);
  int         len   = clampi (l1 + peekc * 2, 0, l0 - 1 - off);
  // printf("col = %i, ln = %i, l0 = %i, l1 = %i, off = %i, len = %i\n",
  //   t->col-l1, t->ln, l0, l1, off, len);
  char const* content = str_substr (tline, off, len);
  va_list     args;
  va_start (args, fmt);
  char* fmtstr  = str_fmtv (fmt, args);
  char* cfmtstr = str_colorfmt ("%c(bright_red)%s: error: %s%c(reset)\t%s\n",
                                l->fpath, fmtstr, content);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
  printf ("\t");
  for (int i = 0; i < mini (t->col - l1 - 1, peekc); ++i) {
    printf (" ");
  }
  for (int i = 0; i < l1; ++i) {
    printf ("^");
  }
  printf ("\n");
}

void stdWarning (char const* fmt, ...) {
  va_list args;
  va_start (args, fmt);
  char* fmtstr = str_fmtv (fmt, args);
  char* cfmtstr =
      str_colorfmt ("%c(bright_yellow)Warning: %s%c(reset)", fmtstr);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
}

void detailedWarning (fu_lexer const* l, fu_token const* t, char const* fmt,
                      ...) {
  if (!l || !t) {
    stdError ("Lexer or token pointer is null. Cannot make detailed warning\n");
    return;
  }
  if (t->ln >= l->linec) {
    stdError ("Token line number is too large. %i / %i\n", t->ln + 1, l->linec);
    return;
  }
  char const* tline = l->linev[t->ln - 1];
  int         l0    = strlen (tline);
  int         l1    = strlen (t->ident);
  int         off   = clampi (t->col - 1 - peekc - l1, 0, l0 - 1);
  int         len   = clampi (l1 + peekc * 2, 0, l0 - 1 - off);
  // printf("col = %i, ln = %i, l0 = %i, l1 = %i, off = %i, len = %i\n",
  //   t->col-l1, t->ln, l0, l1, off, len);
  char const* content = str_substr (tline, off, len);
  va_list     args;
  va_start (args, fmt);
  char* fmtstr = str_fmtv (fmt, args);
  char* cfmtstr =
      str_colorfmt ("%c(bright_yellow)%s: warning: %s%c(reset)\t%s\n", l->fpath,
                    fmtstr, content);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
  printf ("\t");
  for (int i = 0; i < mini (t->col - l1 - 1, peekc); ++i) {
    printf (" ");
  }
  for (int i = 0; i < l1; ++i) {
    printf ("^");
  }
  printf ("\n");
}

void stdNote (char const* fmt, ...) {
  va_list args;
  va_start (args, fmt);
  char* fmtstr  = str_fmtv (fmt, args);
  char* cfmtstr = str_colorfmt ("%c(bright_magenta)Note: %s%c(reset)", fmtstr);
  free (fmtstr);
  printf ("%s", cfmtstr);
  free (cfmtstr);
  va_end (args);
}