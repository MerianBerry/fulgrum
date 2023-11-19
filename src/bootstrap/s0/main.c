#include <stdio.h>
#define __NO_USE_TIME
#include "include/common.h"
#include "include/lex.h"

int main(int argc, char **argv) {
  /*printf("%lu\n", str_hash("black"));
  printf("%lu\n", str_hash("bright_black"));
  printf("%lu\n", str_hash("red"));
  printf("%lu\n", str_hash("bright_red"));
  printf("%lu\n", str_hash("green"));
  printf("%lu\n", str_hash("bright_green"));
  printf("%lu\n", str_hash("yellow"));
  printf("%lu\n", str_hash("bright_yellow"));
  printf("%lu\n", str_hash("blue"));
  printf("%lu\n", str_hash("bright_blue"));
  printf("%lu\n", str_hash("magenta"));
  printf("%lu\n", str_hash("bright_magenta"));
  printf("%lu\n", str_hash("cyan"));
  printf("%lu\n", str_hash("bright_cyan"));*/

  /*dirent_t **dlist = NULL;
  int dlistc;
  if (io_scandir("./src", &dlist, &dlistc) > 0) { 
    if (dlistc > 0) {
      for (int i = 0; i < dlistc; ++i) {
        printf("%s\n", dlist[i]->d_name);
      }
    }
  }*/

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
  char* colorp = str_colorfmt("%c(bright_magenta)Running %s\n%c(reset)", argv[1]);
  printf("%s", colorp);
  free(colorp);
  lex(&l, content.data);
  /*printf("\033[1A");
  printf("\033[K");*/
  free(content.data);
  free(l.tokenv);
  return 0;
}
