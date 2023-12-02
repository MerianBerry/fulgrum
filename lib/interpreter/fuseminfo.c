#define NO_DIRENT
#include "fuseminfo.h"

#include <stdio.h>

#include "common.h"
#include "fuerror.h"
#include "futype.h"

#define semaNext(t, i)      (t + i + 1)
#define semaPeek(t, i, off) (t + i + off)
#define itrI(sinfo, i)      ((*(i)) = mini (++(*(i)), (sinfo)->lex->tokenc))

fu_type* semCheckType (fu_semInfo* sinfo, char const* tname) {
  short i;
  for (i = 0; i < sinfo->typec; ++i) {
    if (!strcmp (sinfo->typev[i]->name, tname)) {
      return sinfo->typev[i];
    }
  }
  return findDefaultType (tname);
}

fu_varInfo semSearch (char const* name, fu_lexer const* lex, int start) {
  int        i;
  fu_varInfo vinfo = {0};
  for (i = start; i < lex->tokenc; ++i) {
    if (lex->tokenv[i].tk == tk_ident && !strcmp (lex->tokenv[i].ident, name)) {
      ++vinfo.uses;
    }
  }
  return vinfo;
}

int semaVar (fu_semInfo* sinfo, int* i, fu_dword* errs) {
  itrI (sinfo, i);
  fu_token const* t0       = sinfo->lex->tokenv;
  fu_varInfo      vinfo    = {0};
  fu_byte         has_type = 1;
  fu_byte         has_def  = 1;
  if ((t0 + *i)->tk != tk_ident) {
    detailedError (sinfo->lex, (t0 + *i),
                   "Variable declaration name \"%s\" is not an identity\n",
                   (t0 + *i));
    ++(*errs);
    return 1;
  }
  vinfo.name = (t0 + *i)->ident;
  if (semaNext (t0, *i)->tk != ':') {
    fu_type const* type = semCheckType (sinfo, semaNext (t0, *i)->ident);
    if (type) {
      detailedWarning (
          sinfo->lex, semaNext (t0, *i),
          "Did you mean to use a ':' after variable declaration?\n");
      has_def = 0;
    }
    has_type = 0;
  }
  if (has_type) {
    do {
      itrI (sinfo, i);
      fu_type* type = semCheckType (sinfo, semaNext (t0, *i)->ident);
      if (!type) {
        detailedError (sinfo->lex, semaNext (t0, *i),
                       "Invalid typename \"%s\"\n", semaNext (t0, *i)->ident);
        ++(*errs);
        goto semaVarTypeLoopTail;
      }
      vinfo.typev =
          mem_grow (vinfo.typev, sizeof (fu_type*), vinfo.typec, &type, 1);
      ++vinfo.typec;
semaVarTypeLoopTail:
      if (semaPeek (t0, *i, 2)->tk != ',')
        break;
      itrI (sinfo, i);
    } while (1);
    itrI (sinfo, i);
  }
  if (!strcmp (semaNext (t0, *i)->ident, "=")) {
    has_def = 1;
    itrI (sinfo, i);
    printf ("%s\n", semaNext (t0, *i)->ident);
  }
  fu_varInfo var_sema_search = semSearch (vinfo.name, sinfo->lex, *i + 1);
  if (!var_sema_search.uses) {
    detailedWarning (sinfo->lex, semaNext (t0, *i),
                     "Variable %s is never used after declaration\n",
                     vinfo.name);
  }
  vinfo.uses = var_sema_search.uses;
  if (has_type)
    sinfo->memsize += ful_typeUnionSize (vinfo.typev, vinfo.typec);
  vinfo.pos = sinfo->memsize;
  sinfo->varv =
      mem_grow (sinfo->varv, sizeof (fu_varInfo), sinfo->varc, &vinfo, 1);
  ++sinfo->varc;
}

fu_semInfo* ful_semAnalyze (fu_lexer const* lex) {
  fu_dword   i;
  fu_semInfo root = {0};
  int        errs = 0;
  fu_token*  t0   = lex->tokenv;
  root.lex        = lex;
  for (i = 0; i < lex->tokenc; ++i) {
    fu_token* t = &lex->tokenv[i];
    if (t->tk == tk_var) {
      semaVar (&root, &i, &errs);
      ++i;
    }
  }
  if (errs) {
    return NULL;
  }
  return mem_copy (&root, sizeof (fu_semInfo));
}

void ful_semFree (fu_semInfo* sinfo) {
  fu_dword i;
  if (!sinfo)
    return;
  if (sinfo->typev)
    free ((void*)sinfo->typev);
  if (sinfo->varv)
    free ((void*)sinfo->varv);
  for (i = 0; i < sinfo->semc; ++i) {
    ful_semFree (sinfo->semv + i);
  }
  if (sinfo->semv)
    free ((void*)sinfo->semv);
  free ((void*)sinfo);
}
