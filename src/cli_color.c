#include <stdio.h>
#define NO_DIRENT
#include "lib/interpreter/common.h"

char escapec (char c) {
  switch (c) {
  case 'a': return '\a';
  case 'b': return '\b';
  case 'f': return '\f';
  case 'n': return '\n';
  case 'r': return '\r';
  case 't': return '\t';
  case 'v': return '\v';
  case '\\': return '\\';
  case '\'': return '\'';
  case '\"': return '\"';
  case '?': return '\?';
  }
  return -1;
}

int main (int argc, char** argv) {
  if (argc < 2) {
    printf ("Not enough arguments\n");
    return 1;
  }
  uint32_t i1;
  uint32_t i2;
  for (i1 = 1; i1 < argc; ++i1) {
    char* cpy  = str_colorfmt (argv[i1]);
    char* fcpy = NULL;
    for (i2 = 0; i2 < strlen (cpy); ++i2) {
      if (cpy[i2] == '\\') {
        char ec = escapec (cpy[i2 + 1]);
        if (ec > 0) {
          fcpy = str_append (fcpy, &ec, 1);
          ++i2;
          continue;
        }
      }
      fcpy = str_append (fcpy, cpy + i2, 1);
    }
    printf ("%s\n", fcpy);
    free (cpy);
    free (fcpy);
  }
  return 0;
}
