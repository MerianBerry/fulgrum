#define NO_DIRENT
#include "include/fulgrum.h"

#include <stdio.h>

#include "common.h"
#include "fulex.h"
#include "fuseminfo.h"

fu_module* ful_import (char const* path) {
  fu_lexer l       = {0};
  l.fpath          = io_fixhome (path);
  buffer_t content = io_read (path);
  if (!content.data) {
    printf ("Could not open file %s\n", path);
    return NULL;
  }
  if (ful_lex (&l, content.data)) {
    printf ("Failed to tokenize file %s\n", path);
    return NULL;
  }
  fu_semInfo* sinfo = ful_semAnalyze (&l);
  if (!sinfo)
    goto sem_cleanup;

  ful_semFree (sinfo);
sem_cleanup:
  ful_freeLexer (&l);
  return NULL;
}