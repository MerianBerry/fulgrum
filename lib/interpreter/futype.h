#pragma once
#include "fudef.h"

int defaultTypesAmmount();

fu_type* findDefaultType (char const* tname);

fu_dword ful_typeUnionSize (fu_type** typev, fu_dword typec);