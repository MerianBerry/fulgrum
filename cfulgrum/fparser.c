#define HYDROGEN_STRING
#define HYDROGEN_MEM
#include "fparser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ferror.h"
#include "hydrogen/hydrogen.h"

#define at(v, i)     ((v)[i])
#define offset(p, o) ((p) + (o))
#define ident(tk)    ((char*)str_cpy ((tk).source, (tk).len))

#define grow(pointer, vecName, type, dataPointer, n)         \
  ((pointer)->##vecName##v =                                 \
       mem_grow ((pointer)->##vecName##v, sizeof (##type##), \
                 (pointer)->##vecName##c, dataPointer, n))


#define insertPhrases(p, sp, n) (grow (p, phrase, Phrase, sp, n))

int p_var (ParseInfo* pinfo, Token* tk) {
  if (offset (tk, 1)->tk != TK_IDENT) {
    detailedError (pinfo->lex, offset (tk, 1),
                   "Expected an identity after var keyword\n");
    return -1;
  }
  char* ident_str = ident (*offset (tk, 1));
  if (!ident_str) {
    printf ("Failed to copy identity string??\ndbg ptr: %p\n", tk);
    return -1;
  }
  printf ("var: %s\n", ident_str);
  free (ident_str);
  return 1;
}

int parse (ParseInfo* pinfo) {
  for (uInt i = 0; i < pinfo->lex->tokenc; ++i) {
    if (at (pinfo->lex->tokenv, i).tk == TK_VAR) {
      int o = p_var (pinfo, &at (pinfo->lex->tokenv, i));
      if (o < 0)
        return 1;
    }
  }
  return 0;
}