#pragma once
#include <stdarg.h>

#include "fdef.h"

void stdError (char const* fmt, ...);

void detailedError (Lexer const* l, Token const* t, char const* fmt, ...);

void stdWarning (char const* fmt, ...);

void detailedWarning (Lexer const* l, Token const* t, char const* fmt, ...);

void stdNote (char const* fmt, ...);