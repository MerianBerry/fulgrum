#define NO_DIRENT
#include "fuseminfo.h"

#include <stdio.h>

#include "common.h"
#include "fuerror.h"
#include "futype.h"

fu_type* semCheckType (fu_semInfo* sinfo, char const* tname) {
  for (short i = 0; i < sinfo->typec; ++i) {
    if (!strcmp (sinfo->typev[i]->name, tname)) {
      return sinfo->typev[i];
    }
  }
  return findDefaultType (tname);
}

fu_varInfo semsearch (char const* name, fu_lexer const* lex, int start) {
  fu_varInfo vinfo = {0};
  for (int i = start; i < lex->tokenc; ++i) {
    if (lex->tokenv[i].tk == tk_ident && !strcmp (lex->tokenv[i].ident, name)) {
      ++vinfo.uses;
    }
  }
  return vinfo;
}

fu_semInfo* ful_semAnalyze (fu_lexer const* lex) {
  fu_semInfo root = {0};
  int        errs = 0;
  for (int i = 0; i < lex->tokenc; ++i) {
    fu_token* t = &lex->tokenv[i];
    if (t->tk == tk_var) {
      ++i;
      if (lex->tokenv[i + 1].tk != ':') {
        detailedError (lex, &lex->tokenv[i + 1],
                       "Missing ':' after variable declaration\n");
        ++errs;
        continue;
      }
      ++i;
      fu_type* vtype = semCheckType (&root, lex->tokenv[i + 1].ident);
      if (lex->tokenv[i + 1].tk != tk_ident || !vtype) {
        detailedError (lex, &lex->tokenv[i + 1],
                       "Invalid variable typename \"%s\"\n",
                       lex->tokenv[i + 1].ident);
        ++errs;
        continue;
      }
      fu_varInfo vinfo = semsearch ((t + 1)->ident, lex, i + 1);
      if (!vinfo.uses) {
        detailedWarning (lex, (t + 1),
                         "Variable %s is never used after declaration\n",
                         (t + 1)->ident);
      }
      vinfo.type = vtype;
      vinfo.name = (t + 1)->ident;
      root.memsize += vtype->size;
      vinfo.pos = root.memsize;
      root.varv =
          mem_grow (root.varv, sizeof (fu_varInfo), root.varc, &vinfo, 1);
      ++root.varc;
      ++i;
    }
  }
  if (errs) {
    return NULL;
  }
  return mem_copy (&root, sizeof (fu_semInfo));
}

void ful_semFree (fu_semInfo* sinfo) {
  if (!sinfo)
    return;
  if (sinfo->typev)
    free ((void*)sinfo->typev);
  if (sinfo->varv)
    free ((void*)sinfo->varv);
  for (int i = 0; i < sinfo->semc; ++i) {
    ful_semFree (sinfo->semv + i);
  }
  if (sinfo->semv)
    free ((void*)sinfo->semv);
  free ((void*)sinfo);
}
