#pragma once
#include "fudef.h"

int ful_lex (fu_lexer* lexer, char const* content);

void ful_freeTokens (fu_lexer* lexer);

void ful_freeLexer (fu_lexer* lexer);