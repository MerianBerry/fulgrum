#include <stdio.h>

#include "lib/bootstrap/include/fulgrum.h"

int main (int argc, char** argv) {
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
  if (argc <= 1) {
    printf ("you stupid give an argument\n");
    return 1;
  }

  ful_import (argv[1]);
  return 0;
}
