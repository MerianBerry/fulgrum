#pragma once
#include "fudef.h"

fu_semInfo* ful_semAnalyze (fu_lexer const* lex);

void ful_semFree (fu_semInfo* sinfo);