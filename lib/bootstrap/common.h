#pragma once

#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifndef __USE_MISC
#  define __USE_MISC
#endif

typedef unsigned char u_char;

#define STR_MAX 0x7fffffff

#ifdef __NO_USE_TIME
#  define __COMMON_NO_TIME
#endif
#ifndef __NO_USE_TIME
#  pragma region "Time"

enum {
  seconds_e      = 1,
  milliseconds_e = 1000,
  microseconds_e = 1000000,
  nanoseconds_e  = 1000000000,
};

typedef struct _timepoint_t {
#  if defined(_WIN32)
  double freq;
  long   c;
#  elif defined(__GNUC__)
  long s;
  long ns;

#  endif
} timepoint_t;

typedef struct timespec timespec_t;

void microsleep(long usec);

timepoint_t timenow();

double timeduration(timepoint_t end, timepoint_t start, double ratio);

void wait(double seconds);
void waitms(double ms);

#  pragma endregion "Time"
#endif

#pragma region "AVL"

typedef struct avl_node_s {
  char *key;
  void *mem;

  struct avl_node_s *parent;

  struct avl_node_s *left;
  struct avl_node_s *right;
} avl_node_t;

typedef struct avl_tree_s {
  avl_node_t *root;
  size_t      size;
} avl_tree_t;

int avl_height(avl_node_t *base);

int avl_findbalance(avl_node_t *base);

avl_node_t **avl_fside(avl_node_t *parent, avl_node_t *target);

void avl_leftrot(avl_node_t *X, avl_node_t *Z);

void avl_rightrot(avl_node_t *X, avl_node_t *Z);

void avl_balance(avl_tree_t *bintree, avl_node_t *base);

void avl_create(avl_tree_t *bintree, avl_node_t **target, avl_node_t *parent,
                char const *key, void *mem);

avl_node_t *avl_find(avl_tree_t *bintree, char const *key);

void avl_append(avl_tree_t *bintree, char const *key, void *mem);

void avl_destroysubtree(avl_tree_t *tree, avl_node_t *root);

void avl_free(avl_tree_t **bintree);

void avl_freeX(avl_tree_t *bintree);

void avl_destroynode(avl_tree_t *tree, avl_node_t *node);

void avl_clear(avl_tree_t *bintree);

avl_tree_t *avl_newtree();

#pragma endregion "AVL"

#pragma region "String"

#define npos ((size_t)-1)

char *str_add(char *lhs, char const *rhs);

char *str_substr(char const *src, size_t off, size_t len);

size_t str_ffo(char const *str, char c);

size_t str_flo(char const *str, char c);

size_t str_flox(char const *str, char const *cs);

size_t str_ffi(char const *str, char const *cmp);

size_t str_fli(char const *str, char const *cmp);

unsigned long str_hash(char const *str);

char *str_fmtv(char const *fmt, va_list args);

char *str_fmt(char const *fmt, ...);

char const *str_cpy(char const *src, size_t bytes);

char *str_append(char *src, char const *nstr, size_t bytes);

/*
 * Replaces a section of `src` with `str`
 * `src` source string for the replacing operation
 * `off` dictates the start of the replacing operation
 * `len` dictates how many bytes to replace (when 0, no bytes are removed from
 * `src`) `str` content replacing content in `src`
 */
char *str_replace(char const *src, long off, long len, char const *str);

char *str_colorfmt(char const *src, ...);

int utf8_charsize(u_char c);

long utf8_strlen(char const *str);

/*
 * Returns the actual offset of the `off` character position in `str`
 */
long utf8_actual(char const *str, long off);

int utf8_encode(int utf);

int utf8_decode(int utf8);

int utf8_swap(int utf8);

int utf8_literal(int utf);

char *utf8_tostring(int utf8);

#pragma endregion "String"

typedef struct buffer_s {
  char *data;
  long  size;
} buffer_t;

#if __unix__ & !defined(NO_DIRENT)
#  include <dirent.h>
#elif _WIN32 & !defined(NO_DIRENT)
#  include "windows/dirent.h"
#endif

#ifndef NO_DIRENT
#  define USE_DIRENT
typedef struct dirent dirent_t;
int io_scandir(char const *dir, dirent_t ***pList, int *pCount);
#endif

char *io_fixhome(char const *path);

char io_direxists(char const *path);

char io_exists(char const *path);

void io_mkdir(char const *path);

buffer_t io_read(char const *path);

void *mem_grow(void *src, int stride, int len, void *newData, int newDataLen);

void *mem_copy(void *src, int size);

double minf(double x, double y);

double maxf(double x, double y);

float clampf(float x, float y, float z);

int mini(int x, int y);

int maxi(int x, int y);

int clampi(int x, int y, int z);
