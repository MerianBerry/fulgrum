#pragma once
#include <stdarg.h>

typedef struct lexer_s lexer_t;
typedef struct token_s token_t;

void stdError(const char* fmt, ...);

void detailedError(lexer_t* l, token_t* t, const char* fmt, ...);

void stdWarning(const char* fmt, ...);

void detailedWarning(lexer_t* l, token_t* t, const char* fmt, ...);

void stdNote(const char* fmt, ...);