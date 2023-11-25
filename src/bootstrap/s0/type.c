#include "include/type.h"
#include "include/common.h"

const type_t _d_types[] = {
  {
    .name = "int", 
    .size = sizeof(int), 
    .id = 0
  }
};

int defaultTypesAmmount() {
  return sizeof(_d_types)/sizeof(type_t);
}

type_t* findDefaultType(const char* tname) {
  for (int i = 0; i < sizeof(_d_types)/sizeof(type_t); ++i)
    if (!strcmp(_d_types[i].name, tname))
      return (type_t*)_d_types + i;
  return NULL;
}
