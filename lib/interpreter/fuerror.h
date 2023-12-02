#pragma once
#include <stdarg.h>

#include "fudef.h"

void stdError (char const* fmt, ...);

void detailedError (fu_lexer const* l, fu_token const* t, char const* fmt, ...);

void stdWarning (char const* fmt, ...);

void detailedWarning (fu_lexer const* l, fu_token const* t, char const* fmt,
                      ...);

void stdNote (char const* fmt, ...);