#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#ifndef __USE_MISC
#	define __USE_MISC
#endif

typedef unsigned char u_char;

#define STR_MAX 0x7fffffff

#pragma region "Time"

enum {
  seconds_e = 1,
  milliseconds_e = 1000,
  microseconds_e = 1000000,
  nanoseconds_e = 1000000000,
};

typedef struct _timepoint_t
{
	#if defined(_WIN32)
	double freq;
	long c;
	#elif defined(__GNUC__)
	long s;
	long ns;

	#endif
} timepoint_t;

typedef struct timespec timespec_t;

void microsleep(long usec);

timepoint_t timenow();

double timeduration(timepoint_t end,
	timepoint_t start, double ratio);

void wait(double seconds);
void waitms(double ms);

#pragma endregion "Time"

#pragma region "AVL"

typedef struct avl_node_s
{
	char 											*key;
	void 											*mem;

	struct avl_node_s 	*parent;

	struct avl_node_s 	*left;
	struct avl_node_s 	*right;
} avl_node_t;

typedef struct avl_tree_s
{
	avl_node_t 								*root;
	size_t 										size;
} avl_tree_t;

int avl_height(avl_node_t *base);

int avl_findbalance(avl_node_t *base);

avl_node_t **avl_fside(avl_node_t *parent,
	avl_node_t*  target);

void avl_leftrot(avl_node_t *X,
	avl_node_t *Z);

void avl_rightrot(avl_node_t *X,
	avl_node_t *Z);

void avl_balance(avl_tree_t *bintree,
	avl_node_t *base);

void avl_create(avl_tree_t *bintree,
	avl_node_t **target, avl_node_t *parent,
	const char *key, void *mem);

avl_node_t *avl_find(avl_tree_t *bintree,
	const char *key);

void avl_append(avl_tree_t *bintree,
	const char *key,
	void *mem);

void avl_destroysubtree(avl_tree_t *tree,
	avl_node_t*  root);

void avl_free(avl_tree_t **bintree);

void avl_freeX(avl_tree_t *bintree);

void avl_destroynode(avl_tree_t *tree, avl_node_t *node);

void avl_clear(avl_tree_t *bintree);

avl_tree_t *avl_newtree();

#pragma endregion "AVL"

#pragma region "String"

#define npos ((size_t)-1)

char *str_add(char * lhs, const char * rhs);

const char *substr(const char * src,
	size_t off, size_t len);

size_t str_ffo(const char * str, char c);

size_t str_flo(const char * str, char c);

size_t str_flox(const char * str, const char *cs);

size_t str_ffi(const char * str,
	const char * cmp);

size_t str_fli(const char * str,
	const char * cmp);

unsigned long str_hash(const char *str);

char *str_fmtv(const char *fmt, va_list args);

char *str_fmt(const char *fmt, ...);

const char *str_cpy(const char *src, size_t bytes);

char *str_append(char *src, const char *nstr, size_t bytes);

/*
	* Replaces a section of `src` with `str`
	* `src` source string for the replacing operation
	* `off` dictates the start of the replacing operation
	* `len` dictates how many bytes to replace (when 0, no bytes are removed from `src`)
	* `str` content replacing content in `src`
*/
char *str_replace(const char *src, long off, long len, const char *str);

int utf8_charsize(u_char c);

long utf8_strlen(const char *str);

/*
	* Returns the actual offset of the `off` character position in `str`
*/
long utf8_actual(const char *str, long off);

int utf8_encode(int utf);

int utf8_decode(int utf8);

int utf8_swap(int utf8);

int utf8_literal(int utf);

char *utf8_tostring(int utf8);

#pragma endregion "String"

char *io_fixhome(const char *path);

char io_direxists(const char *path);

char io_exists(const char *path);

void io_mkdir(const char *path);

double minf(double x, double y);

double maxf(double x, double y);

float clampf(float x, float y, float z);

int mini(int x, int y);

int maxi(int x, int y);

int clampi(int x, int y, int z);
