#define NO_DIRENT
#include <stdio.h>

#include "common.h"

#if !defined(MAX)
#  define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#if !defined(MIN)
#  define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif
#if !defined(CLAMP)
#  define CLAMP(x, y, z) MAX (y, MIN (z, x))
#endif

#pragma region "Strings"

char *str_add (char *lhs, char const *rhs) {
  if (!rhs)
    return NULL;
  size_t lhss = 0;
  if (!!lhs)
    lhss = strlen (lhs);

  size_t size = lhss + strlen (rhs) + 1;
  char  *buf  = (char *)malloc (size);
  memset (buf, 0, size);
  if (!!lhs)
    memcpy (buf, lhs, lhss);
  memcpy (buf + lhss, rhs, strlen (rhs));

  return buf;
}

char *str_substr (char const *src, size_t off, size_t len) {
  assert (!(off > strlen (src) - 1) && "String out of bounds exception");

  size_t size = MIN (strlen (src) - off, len) + 1;
  char  *buf  = (char *)malloc (size);
  memset (buf, 0, size);
  memcpy (buf, src + off, size - 1);
  return buf;
}

size_t str_ffo (char const *str, char c) {
  size_t i;
  for (i = 0; i < strlen (str); ++i) {
    if (str[i] == c)
      return i;
  }
  return npos;
}

size_t str_flo (char const *str, char c) {
  long i;
  for (i = strlen (str) - 1; i >= 0; --i) {
    if (str[i] == c)
      return i;
  }
  return npos;
}

size_t str_flox (char const *str, char const *cs) {
  long   i1;
  size_t i2;
  for (i1 = strlen (str) - 1; i1 >= 0; --i1) {
    for (i2 = 0; i2 < strlen (cs); ++i2) {
      char c = cs[i2];
      if (str[i1] == c)
        return i1;
    }
  }
  return npos;
}

size_t str_ffi (char const *str, char const *cmp) {
  if (!str || !cmp) {
    return npos;
  }
  size_t     i;
  long const l1 = strlen (str);
  long const l2 = strlen (cmp);
  if (!l1 || !l2)
    return npos;
  for (i = 0; i < l1 - (l2 - 1); ++i) {
    char *sub = (char *)str_substr (str, i, l2);
    if (!strcmp (sub, cmp)) {
      free (sub);
      return i;
    }
    free (sub);
  }
  return npos;
}

size_t str_fli (char const *str, char const *cmp) {
  if (!strlen (cmp))
    return npos;
  long         i;
  const size_t s1 = strlen (str);
  const size_t s2 = strlen (cmp);
  if (s2 > s1)
    return npos;
  for (i = s1 - (s2); i >= 0; --i) {
    char *sub = (char *)str_substr (str, i, s2);
    if (!strcmp (sub, cmp)) {
      free (sub);
      return i;
    }
    free (sub);
  }
  return npos;
}

unsigned long str_hash (char const *str) {
  unsigned long hash = 5381;
  int           c;

  while (c = *(str++)) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

/* Str fmting based off of lua fmt function */
char *str_fmtv (char const *fmt, va_list args) {
  char        *buf   = NULL;
  char const  *e     = NULL;
  char const  *ogfmt = fmt;
  const size_t oglen = strlen (fmt);
  while ((e = strchr (fmt, '%')) != NULL) {
    size_t len = buf ? strlen (buf) : 0;
    buf        = str_append (buf, fmt, e - fmt);
    len        = buf ? strlen (buf) : 0;
    int extr   = 0;
    switch (*(e + 1)) {
    case 's': { /* String */
      char const *s = va_arg (args, char *);
      buf           = str_append (buf, (s) ? s : "(null)", -1);
      break;
    }
    case 'c': { /* Character */
      char c = (char)va_arg (args, int);
      buf    = str_append (buf, &c, 1);
      break;
    }
    case 'x': {
      unsigned long n = va_arg (args, unsigned long);
      int           l = snprintf (NULL, 0, "%lx", n) + 1;
      char          nbuf[l];
      snprintf (nbuf, l, "%lx", n);
      buf = str_append (buf, nbuf, -1);
      break;
    }
    case 'i': { /* 32 bit integer */
      int  n = va_arg (args, int);
      int  l = snprintf (NULL, 0, "%i", n) + 1;
      char numbuf[l];
      snprintf (numbuf, l, "%i", n);
      buf = str_append (buf, numbuf, -1);
      break;
    }
    case 'l': { /* 64 bit integer */
      if (*(e + 2) == 'u') { /* long unsigned */
        unsigned long n = va_arg (args, unsigned long);
        int           l = snprintf (NULL, 0, "%lu", n) + 1;
        char          numbuf[l];
        snprintf (numbuf, l, "%lu", n);
        buf = str_append (buf, numbuf, -1);
        extr += 1;
        break;
      }
      long n = va_arg (args, long);
      int  l = snprintf (NULL, 0, "%li", n) + 1;
      buf    = str_append (buf, "(null)", npos);
      char numbuf[l];
      snprintf (numbuf, l, "%li", n);
      buf = str_append (buf, numbuf, -1);
      break;
    }
    case 'f': { /* 64 bit float */
      double n = va_arg (args, double);
      int    l = snprintf (NULL, 0, "%lf", n) + 1;
      char   numbuf[l];
      snprintf (numbuf, l, "%lf", n);
      buf = str_append (buf, numbuf, -1);
      break;
    }
    case 'd': { /* 64 bit float */
      double n = va_arg (args, double);
      int    l = snprintf (NULL, 0, "%lf", n) + 1;
      char   numbuf[l];
      snprintf (numbuf, l, "%lf", n);
      buf = str_append (buf, numbuf, -1);
      break;
    }
    case 'u': { /* Possible long unsigned */
      unsigned int n = va_arg (args, unsigned int);
      int          l = snprintf (NULL, 0, "%u", n) + 1;
      char         numbuf[l];
      snprintf (numbuf, l, "%u", n);
      buf = str_append (buf, numbuf, -1);
      break;
    }
    case '%': {
      buf = str_append (buf, "%", 1);
      break;
    }
    }
    fmt = e + 2 + extr;
  }
  buf = str_append (buf, fmt, -1);
  return buf;
}

char *str_fmt (char const *fmt, ...) {
  char   *msg;
  va_list args;
  va_start (args, fmt);
  msg = (char *)str_fmtv (fmt, args);
  va_end (args);
  return msg;
}

char const *str_cpy (char const *src, size_t bytes) {
  size_t len = MIN (strlen (src), bytes);
  char  *cpy = malloc (len + 1);
  memset (cpy, 0, len + 1);
  memcpy (cpy, src, len);
  return cpy;
}

char *str_append (char *src, char const *nstr, size_t bytes) {
  if (!nstr || !strlen (nstr) || !bytes)
    return src;
  const size_t l   = src ? strlen (src) : 0;
  const size_t l2  = MIN (strlen (nstr), bytes);
  char        *buf = malloc (l + l2 + 1);
  memset (buf, 0, l + l2 + 1);
  if (src) {
    memcpy (buf, src, l);
    free (src);
  }
  memcpy (((char *)buf) + l, nstr, l2);
  return buf;
}

char *str_replace (char const *src, long off, long len, char const *str) {
  if (!src || !str)
    return NULL;
  off           = MAX (off, 0);
  len           = MAX (len, 0);
  long const l1 = strlen (src);
  long const l2 = strlen (str);
  if (off >= l1)
    return NULL;
  len             = MIN (len, l1 - off);
  long const l3   = l1 - len + l2;
  char      *nstr = malloc (l3 + 1);
  memset (nstr, 0, l3 + 1);
  memcpy (nstr, src, MAX (off, 0));
  memcpy (nstr + off, str, MAX (l2, 0));
  memcpy (nstr + off + l2, src + off + len, MAX (l1 - off - len, 0));
  return nstr;
}

char *str_colorfmt (char const *src, ...) {
  va_list args;
  va_start (args, src);

  char *cpy = (char *)str_cpy (src, npos);
  int   itr = 0;
  while (1) {
    if (itr > 100) {
      printf (
          "\x1B[91;1mEmergency while loop abort at itr=%i\n\tstring: "
          "\"%s\"\n\x1B[0m",
          itr, cpy);
      break;
    }
    size_t p = str_ffi (cpy, "&c");
    if (p == npos)
      break;
    if (((p > 0 && cpy[p - 1] != '\\') || !p) && cpy[p + 2] == '(') {
      size_t eb    = str_ffo (cpy, ')');
      char  *color = (char *)str_substr (cpy, p + 3, eb - (p + 3));
      if (color) {
        char *ocolor = NULL;
        switch (str_hash (color)) {
        case 210707760194U: ocolor = "\x1B[30m"; break;
        case 14928609233751675713U: ocolor = "\x1B[90m"; break;
        case 193504576U: ocolor = "\x1B[31m"; break;
        case 8246139787930773631U: ocolor = "\x1B[91m"; break;
        case 210713909846U: ocolor = "\x1B[32m"; break;
        case 14928609233757825365U: ocolor = "\x1B[92m"; break;
        case 6954248304353U: ocolor = "\x1B[33m"; break;
        case 13028758798249174464U: ocolor = "\x1B[93m"; break;
        case 6385084301U: ocolor = "\x1B[34m"; break;
        case 13868195969781240492U: ocolor = "\x1B[94m"; break;
        case 229474533704194U: ocolor = "\x1B[35m"; break;
        case 5673926631242730689U: ocolor = "\x1B[95m"; break;
        case 6385133744U: ocolor = "\x1B[36m"; break;
        case 13868195969781289935U: ocolor = "\x1B[96m"; break;
        case 210726503048: ocolor = "\x1B[0m"; break;
        }
        if (!ocolor) {
          printf ("\x1B[91mInvalid color name used. %s\n\x1B[0m", color);
          ocolor = "";
        }
        free (color);
        cpy = str_replace (cpy, p, eb - p + 1, ocolor);
      }
    }
    ++itr;
  }
  if (itr < 100) {
    char *fmtstr = str_fmtv (cpy, args);
    free (cpy);
    va_end (args);
    return fmtstr;
  }
  va_end (args);
  return cpy;
}

#pragma endregion "Strings"

double minf (double x, double y) { return (x < y) ? x : y; }

double maxf (double x, double y) { return (x > y) ? x : y; }

float clampf (float x, float y, float z) {
  return (x < y) ? y : ((x > z) ? z : x);
}

int mini (int x, int y) { return (x < y) ? x : y; }

int maxi (int x, int y) { return (x > y) ? x : y; }

int clampi (int x, int y, int z) { return (x < y) ? y : ((x > z) ? z : x); }

void *mem_grow (void *src, int stride, int len, void *newData, int newDataLen) {
  void *buf = malloc (stride * (len + newDataLen));
  memset (buf, 0, stride * (len + newDataLen));
  memcpy (buf, src, stride * len);
  memcpy ((char *)buf + stride * len, newData, stride * newDataLen);
  if (src)
    free (src);
  return buf;
}

void *mem_copy (void *src, int size) {
  void *pb = malloc (size);
  memcpy (pb, src, size);
  return pb;
}