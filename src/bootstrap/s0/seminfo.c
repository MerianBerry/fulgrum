#include "include/tokens.h"
#include "include/lex.h"
#include "include/seminfo.h"
#include "include/common.h"
#include "include/type.h"
#include "include/error.h"
#include <stdio.h>

type_t* semCheckType(seminfo_t* sinfo, const char* tname) {
  for (short i = 0; i < sinfo->typec; ++i) {
    if (!strcmp(sinfo->typev[i]->name, tname)) {
      return sinfo->typev[i];
    }
  }
  return findDefaultType(tname);
}

varinfo_t semsearch(const char* name, lexer_t* lex, int start) {
  varinfo_t vinfo = {0};
  for (int i = start; i < lex->tokenc; ++i) {
    if (lex->tokenv[i].tk == tk_ident && !strcmp(lex->tokenv[i].ident, name)) {
      ++vinfo.uses;
      /*printf("%s left: %s\n", name, lex->tokenv[i-1].ident);
      if (i+1 < lex->tokenc && lex->tokenv[i+1].tk == tk_oper) {
        printf("%s right: %s\n", name, lex->tokenv[i+1].ident);
      } else {
        printf("%s right: none\n", name);
      }*/
    }
  }
  return vinfo;
}

seminfo_t* semAnalyze(lexer_t* lex) {
  seminfo_t root = {0};
  int errs = 0;
  for (int i = 0; i < lex->tokenc; ++i) {
    token_t* t = &lex->tokenv[i];
    if (t->tk == tk_var) {
      /*char* cfmt = str_colorfmt("%c(bright_cyan)Var token %s%c(reset)\n", (t+1)->ident);
      printf("%s", cfmt);
      free(cfmt);*/
      ++i;
      if (lex->tokenv[i+1].tk != ':') {
        detailedError(lex, &lex->tokenv[i+1], "Missing ':' after variable declaration\n");
        ++errs;
        //stdError("Missing ':' after variable declaration\n");
        continue;
      }
      ++i;
      type_t* vtype = semCheckType(&root, lex->tokenv[i+1].ident);
      if (lex->tokenv[i+1].tk != tk_ident || !vtype) {
        detailedError(lex, &lex->tokenv[i+1], "Invalid variable typename \"%s\"\n",
          lex->tokenv[i+1].ident);
        ++errs;
        //stdError("Invalid variable typename \"%s\"\n", lex->tokenv[i+1].ident);
        continue;
      }
      varinfo_t vinfo = semsearch((t+1)->ident, lex, i+1);
      if (!vinfo.uses) {
        detailedWarning(lex, (t+1), 
          "Variable %s is never used after declaration\n", (t+1)->ident);
        //stdWarning("Variable %s is never used after declaration\n", (t+1)->ident);
      }
      vinfo.type = vtype;
      vinfo.name = (t+1)->ident;
      root.varv = mem_grow(root.varv, sizeof(varinfo_t), root.varc, &vinfo, 1);
      ++i;
    }
  }
  if (errs) {
    semFree(&root);
    return NULL;
  }
  return mem_copy(&root, sizeof(seminfo_t));
}

void semFree(seminfo_t* sinfo) {
  if (sinfo->typev)
    free((void*)sinfo->typev);
  if (sinfo->varv)
    free((void*)sinfo->varv);
  for (int i = 0; i < sinfo->semc; ++i) {
    semFree(sinfo->semv+i);
  }
  if (sinfo->semv)
    free((void*)sinfo->semv);
  sinfo->typev = NULL;
  sinfo->varv = NULL;
  sinfo->semv = NULL;
  sinfo->typec = 0;
  sinfo->varc = 0;
  sinfo->semc = 0;
}
