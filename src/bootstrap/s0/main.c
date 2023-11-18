#include <stdio.h>
#define __NO_USE_TIME
#include "include/common.h"
#include "include/lex.h"

#define BNGRN "\x1B[92m"
#define BBGRN "\x1B[92;1m"
#define NBGRN "\x1B[32;1m"
#define NNGRN "\x1B[32m"
#define KBLU  "\x1B[34m"
#define KCYN  "\x1B[36m"
#define RESET "\x1B[0m"
#define BRTBMGN "\x1B[95;1m"

int main(int argc, char **argv) {
  lexer_t l = {0};
  if (argc <= 1) {
    printf("you stupid give an argument\n");
    return 1;
  }
  buffer_t content = io_read(argv[1]);
  if (!content.data) {
    printf("Could not open %s\n", argv[1]);
    return 2;
  }
  printf(BRTBMGN "Running %s\n" RESET, argv[1]);
  lex(&l, content.data);
  /*printf("\033[1A");
  printf("\033[K");*/
  free(content.data);
  free(l.tokenv);
  return 0;
}