#include "include/common.h"

/*
  DIR *d;
  struct dirent **list;
  int count = scandir(".", &list, NULL, alphasort)-1;
  if (count<0)
    abort();
  else {
    for (;count>=0;--count) {
      if (list[count]->d_type == DT_DIR)
        puts(list[count]->d_name);
    }
  }
  free(list);
  count = scandir("..", &list, NULL, alphasort)-1;
  if (count<0)
    abort();
  else {
    for (;count>=0;--count) {
      if (list[count]->d_type == DT_DIR)
        puts(list[count]->d_name);
    }
  }
  free(list);*/

#ifdef __unix__
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <dirent.h>

typedef struct dirent dirent_t;
typedef struct stat stat_t;

char *io_fixhome(const char *path) {
  char *var = getenv("HOME");
  if (!var)
    return NULL;
  long pos = str_ffo(path, '~');
  if (pos != npos) {
    char *nstr = str_replace(path, pos, 1, var);
    return nstr;
  }
  return (char*)str_cpy(path, npos);
}

char io_direxists(const char *path) {
  char *npath = io_fixhome(path);
  DIR *d = opendir(npath);
  free(npath);
  if (!d)
    return 0;
  closedir(d);
  return 1;
}

char io_exists(const char *path) {
  char *npath = io_fixhome(path);
  FILE *f = fopen(npath, "r");
  free(npath);
  if (!f)
    return 0;
  fclose(f);
  return 1;
}

void io_mkdir(const char *path) {
  stat_t s = {0};
  char *npath = io_fixhome(path);
  if (stat(npath, &s) == -1) {
    mkdir(npath, 0755);
  }
  free(npath);
}

#endif