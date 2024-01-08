#define HYDROGEN_IO
#include "include/fulgrum.h"

#include <stdio.h>
#include <stdlib.h>

#include "flex.h"
#include "fparser.h"
#include "hydrogen/hydrogen.h"

ful_Module* ful_import (char const* path) {
  Lexer     l      = {0};
  ParseInfo p      = {0};
  l.fpath          = io_fixhome (path);
  p.lex            = &l;
  h_buffer content = io_read (path);
  if (!content.data) {
    printf ("Could not open file %s\n", path);
    return NULL;
  }
  if (ful_lex (&l, content.data)) {
    printf ("Failed to tokenize file %s\n", path);
    return NULL;
  }

  if (parse (&p)) {
    printf ("Failed to parse tokens\n");
    goto lex_end;
  }

lex_end:
  ful_freeLexer (&l);
  free (content.data);
  return NULL;
}