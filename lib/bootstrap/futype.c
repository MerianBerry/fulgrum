#define NO_DIRENT
#include "futype.h"

#include "common.h"

const fu_type _d_types[] = {
    {.name = "int", .size = sizeof (int), .id = 0}
};

int defaultTypesAmmount() { return sizeof (_d_types) / sizeof (fu_type); }

fu_type* findDefaultType (char const* tname) {
  for (int i = 0; i < sizeof (_d_types) / sizeof (fu_type); ++i)
    if (!strcmp (_d_types[i].name, tname))
      return (fu_type*)_d_types + i;
  return NULL;
}
