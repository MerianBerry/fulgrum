#pragma once
#include "fdef.h"

int ful_lex (Lexer* lexer, char const* content);

void ful_freeTokens (Lexer* lexer);

void ful_freeLexer (Lexer* lexer);