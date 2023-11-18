#pragma once
#include "type.h"

enum {
  varstat = 1,
  valstat,
};

typedef struct varstat_s varstat_t;
typedef struct valstat_s valstat_t;

typedef struct {
  void*         stat;
  char          type;
} stathandle_t;

typedef struct varstat_s {
  const char*   name;
  valstat_t*    val;
} varstat_t;

typedef struct valstat_s {
  void*         v;
  type_t*       type;
} valstat_t;


