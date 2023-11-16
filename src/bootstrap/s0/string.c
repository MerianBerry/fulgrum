#include "include/common.h"

#if !defined(MAX)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#if !defined(MIN)
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif
#if !defined(CLAMP)
#define CLAMP(x, y, z) MAX(y, MIN(z, x))
#endif


#pragma region "Strings"

char *str_add(char * lhs, const char * rhs)
{
	if (!rhs)
		return NULL;
	size_t lhss = 0;
	if (!!lhs)
		lhss = strlen(lhs);
  

	size_t size = lhss + strlen(rhs) + 1;
	char *buf = (char*)malloc(size);
	memset(buf, 0, size);
	if (!!lhs)
		memcpy(buf, lhs, lhss);
	memcpy(buf+lhss, rhs, strlen(rhs));
	
	return buf;
}

const char *str_substr(const char * src,
	size_t off, size_t len)
{
	assert(!(off > strlen(src)-1) && "String out of bounds exception");

	size_t size = MIN(strlen(src)-off, len)+1;
	char *buf = (char*)malloc(size);

	memcpy(buf, src + off, size-1);
	return buf;
}

size_t str_ffo(const char * str, char c)
{
	for (size_t i = 0; i < strlen(str); ++i)
	{
		if (str[i] == c)
			return i;
	}
	return npos;
}

size_t str_flo(const char * str, char c)
{
	for (int i = strlen(str) - 1; i >= 0; --i)
	{
		if (str[i] == c)
			return i;
	}
	return npos;
}

size_t str_flox(const char * str, const char *cs)
{
	
	for (int i = strlen(str) - 1; i >= 0; --i) {
		for (size_t i2 = 0; i2 < strlen(cs); ++i2) {
			char c = cs[i2];
			if (str[i] == c)
				return i;
		}
	}
	return npos;
}

size_t str_ffi(const char * str,
	const char * cmp)
{
	if (strlen(cmp) == 0)
		return npos;
	for (size_t i = 0; i < strlen(str) - (strlen(cmp)-1); ++i)
	{
		char *sub = (char*)str_substr(str, i, strlen(cmp));
		if (strcmp(sub, cmp) == 0)
		{
			free(sub);
			return i;
		}
		free(sub);
	}
	return npos;
}

size_t str_fli(const char * str,
	const char * cmp)
{
	if (!strlen(cmp))
		return npos;
	size_t s1 = strlen(str);
	size_t s2 = strlen(cmp);
	if (s2>s1)
		return npos;
	for (int i = s1-(s2); i >= 0; --i) {
		//printf("%lu : %lu\n", s1, i);
		char *sub = (char*)str_substr(str, i, s2);
		if (!strcmp(sub, cmp)) {
			free(sub);
			return i;
		}
		free(sub);
	}
	return npos;
}

unsigned long str_hash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

// Str fmting based off of lua fmt function
char *str_fmtv(const char *fmt, va_list args)
{
	char *buf = NULL;
	const char *e = NULL;
	const char *ogfmt = fmt;
	const size_t oglen = strlen(fmt);
	while ((e = strchr(fmt, '%')) != NULL)
	{
		size_t len = buf ? strlen(buf) : 0;
		buf = str_append(buf, fmt, e-fmt);
		len = buf ? strlen(buf) : 0;
		int extr = 0;
		switch (*(e+1)) {
			case 's': { // String
				const char *s = va_arg(args, char *);
				buf = str_append(buf, (s) ? s : "(null)", -1);
				break;
			}
			case 'c': { // Character
				char c = (char)va_arg(args, int);
				buf = str_append(buf, &c, 1);
				break;
			}
			case 'x': {
				unsigned long n = va_arg(args, unsigned long);
				int l = snprintf(NULL, 0, "%lx", n)+1;
        char nbuf[l];
        snprintf(nbuf, l, "%lx", n);
        buf = str_append(buf, nbuf, -1);
				break;
			}
			case 'i': { // 32 bit integer
				int n = va_arg(args, int);
				int l = snprintf(NULL, 0, "%i", n)+1;
        char numbuf[l];
        snprintf(numbuf, l, "%i", n);
        buf = str_append(buf, numbuf, -1);
				break;
			}
			case 'l': { // 64 bit integer
				if (*(e+2) == 'u') { // long unsigned
					unsigned long n = va_arg(args, unsigned long);
					int l = snprintf(NULL, 0, "%lu", n)+1;
          char numbuf[l];
          snprintf(numbuf, l, "%lu", n);
          buf = str_append(buf, numbuf, -1);
					extr+=1;
					break;
				}
				long n = va_arg(args, long);
				int l = snprintf(NULL, 0, "%li", n)+1;
        buf = str_append(buf, "(null)", npos);
        char numbuf[l];
        snprintf(numbuf, l, "%li", n);
        buf = str_append(buf, numbuf, -1);
				break;
			}
			case 'f': { // 64 bit float
				double n = va_arg(args, double);
				int l = snprintf(NULL, 0, "%lf", n)+1;
        char numbuf[l];
        snprintf(numbuf, l, "%lf", n);
        buf = str_append(buf, numbuf, -1);
				break;
			}
			case 'd': { // 64 bit float
				double n = va_arg(args, double);
				int l = snprintf(NULL, 0, "%lf", n)+1;
        char numbuf[l];
        snprintf(numbuf, l, "%lf", n);
        buf = str_append(buf, numbuf, -1);
				break;
			}
			case 'u': { // Possible long unsigned
				unsigned int n = va_arg(args, unsigned int);
				int l = snprintf(NULL, 0, "%u", n)+1;
        char numbuf[l];
        snprintf(numbuf, l, "%u", n);
        buf = str_append(buf, numbuf, -1);
				break;
			}
			case '%': {
				buf = str_append(buf, "%", 1);
				break;
			}
		}
		fmt = e+2+extr;
	}
	buf = str_append(buf, fmt, -1);
	return buf;
}

char *str_fmt(const char *fmt, ...)
{
	char *msg;
	va_list args;
	va_start(args, fmt);
	msg = (char*)str_fmtv(fmt, args);
	va_end(args);
	return msg;
}

const char *str_cpy(const char *src, size_t bytes)
{
	size_t len = MIN(strlen(src), bytes);
	char *cpy = malloc(len+1);
	memcpy(cpy, src, len);
	return cpy;
}

char *str_append(char *src, const char *nstr, size_t bytes)
{
	if (!nstr || !strlen(nstr) || !bytes)
		return src;
	const size_t l = src ? strlen(src) : 0;
	const size_t l2 = MIN(strlen(nstr), bytes);
	char *buf = malloc(l+l2+1);
	//memset(buf, 0, l+l2+1);
	if (src) {
		memcpy(buf, src, l);
		free(src);
	}
	memcpy(((char*)buf)+l, nstr, l2+1);
	return buf;
}

char *str_replace(const char *src, long off, long len, const char *str) {
  if (!src || !str)
    return NULL;
  off = MAX(off, 0);
  len = MAX(len, 0);
  const long l1 = strlen(src);
  const long l2 = strlen(str);
  if (off >= l1)
    return NULL;
  len = MIN(len, l1-off);
  const long l3 = l1 - len + l2;
  char *nstr = malloc(l3+1);
  memset(nstr, 0, l3+1);
  memcpy(nstr, src, MAX(off, 0));
  memcpy(nstr+off, str, MAX(l2, 0));
  memcpy(nstr+off+l2, src+off+len, MAX(l1-off-len, 0));
  return nstr;
}

#pragma endregion "Strings"

double minf(double x, double y) {
	return (x < y) ? x : y;
}

double maxf(double x, double y) {
	return (x > y) ? x : y;
}

float clampf(float x, float y, float z) {
	return (x < y) ? y : ((x > z) ? z : x);
}

int mini(int x, int y) {
	return (x < y) ? x : y;
}

int maxi(int x, int y) {
	return (x > y) ? x : y;
}

int clampi(int x, int y, int z) {
	return (x < y) ? y : ((x > z) ? z : x);
}