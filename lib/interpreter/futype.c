#define NO_DIRENT
#include "futype.h"

#include "common.h"

const fu_type _d_types[] = {
    {.name = "int", .size = sizeof (int), .id = 0}
};

int defaultTypesAmmount() { return sizeof (_d_types) / sizeof (fu_type); }

fu_type* findDefaultType (char const* tname) {
  fu_dword i;
  for (i = 0; i < sizeof (_d_types) / sizeof (fu_type); ++i)
    if (!strcmp (_d_types[i].name, tname))
      return (fu_type*)_d_types + i;
  return NULL;
}

fu_dword ful_typeUnionSize (fu_type** typev, fu_dword typec) {
  fu_dword max = 0;
  fu_dword i;
  for (i = 0; i < typec; ++i) {
    if (typev[i]->size > max)
      max = typev[i]->size;
  }
  return max;
}
