#define _XOPEN_SOURCE 700
#include "common.h"

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
#include <stdio.h>
#include <sys/stat.h>

#ifdef USE_DIRENT
int io_scandir (char const *dir, dirent_t ***pList, int *pCount) {
  DIR *d;
  int  count = scandir (dir, pList, NULL, NULL);
  if (count < 0)
    return 0;
  (*pCount) = count;
  return 1;
}

#endif
#ifdef _WIN32
#  include <WinBase.h>
#endif

typedef struct stat stat_t;

char *io_fixhome (char const *path) {
#if defined(_WIN32)
  char  var[PATH_MAX + 1] = {0};
  DWORD l                 = GetEnvironmentVariable ("HOMEDRIVE", var, PATH_MAX);
  GetEnvironmentVariable ("HOMEPATH", var + l, PATH_MAX - l);
#else
  char *var   = getenv ("HOME");
#endif
  long pos = str_ffo (path, '~');
  if (pos != npos && var) {
    char *nstr = str_replace (path, pos, 1, var);
    return nstr;
  }
  return (char *)str_cpy (path, strlen (path));
}

char io_direxists (char const *path) {
  char *npath = io_fixhome (path);
  DIR  *d     = opendir (npath);
  free (npath);
  if (!d)
    return 0;
  closedir (d);
  return 1;
}

char io_exists (char const *path) {
  char *npath = io_fixhome (path);
  FILE *f     = fopen (npath, "r");
  free (npath);
  if (!f)
    return 0;
  fclose (f);
  return 1;
}

void io_mkdir (char const *path) {
#ifdef __unix__
  stat_t s     = {0};
  char  *npath = io_fixhome (path);
  if (stat (npath, &s) == -1) {
    mkdir (npath, 0755);
  }
  free (npath);
#elif _WIN32
  char *npath = io_fixhome (path);
  CreateDirectoryA (npath, NULL);
  free (npath);
#endif
}

buffer_t io_read (char const *path) {
  if (!path)
    return (buffer_t){NULL, 0};
  char *fp     = io_fixhome (path);
  FILE *stream = fopen (fp, "r");
  if (!stream) {
    free (fp);
    return (buffer_t){NULL, 0};
  }
  fseek (stream, -1, SEEK_END);
  long     strlen = ftell (stream) + 1;
  buffer_t buf    = {0};
  buf.data        = malloc (strlen);
  buf.size        = strlen;
  fseek (stream, 0, SEEK_SET);
  for (size_t i = 0; i < (size_t)strlen; ++i) {
    ((char *)buf.data)[i] = getc (stream);
  }
  free (fp);
  fclose (stream);
  return buf;
}