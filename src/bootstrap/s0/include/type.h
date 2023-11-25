#pragma once

typedef struct {
  const char*   name;
  short         id;
  short         size;
} type_t;

int defaultTypesAmmount();

type_t* findDefaultType(const char* tname);
