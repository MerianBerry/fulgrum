#define HYDROGEN_STRING
#define HYDROGEN_MEM
#include "flex.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ferror.h"
#include "hydrogen/hydrogen.h"

#define validopc(c)    (str_ffo ("~+-*/!@$%%^&*[]<>|=", (char)c) != npos)

#define isnewline(c)   (c == '\n' || c == '\r')
#define noteof(c)      (c != EOF)
#define validc(c)      (!c && c != EOF)
#define ident(lx)      (str_substr (lx->in_progress.source, 0, lx->in_progress.len))
#define inProgress(lx) (lx->in_progress)
#define lastTK(lx)     ((!(lx)->tokenc) ? 0 : (lx)->tokenv[(lx)->tokenc - 1].tk)

int lx_next (Lexer* lexer) {
  if (lexer->indx + 1 >= lexer->size || lexer->cc == EOF) {
    lexer->indx = lexer->size;
    lexer->cc   = EOF;
    lexer->ccol = 0;
    return EOF;
  }
  ++lexer->ccol;
  if (isnewline (lexer->cc)) {
    lexer->ccol = 1;
    ++lexer->cln;
  }
  lexer->cc = lexer->content[++lexer->indx];
  return lexer->cc;
}

int lx_skip (Lexer* lexer, int off) {
  int i = lexer->indx + off;
  if (i >= lexer->size || i < 0)
    return (i < 0) ? 0 : EOF;
  lexer->indx = i;
  lexer->cc   = lexer->content[i];
  return lexer->cc;
}

int lx_peek (Lexer* lexer, int off) {
  int i = lexer->indx + off;
  if (i >= lexer->size || i < 0)
    return (i < 0) ? 0 : EOF;
  return lexer->content[i];
}

int lx_back (Lexer* lexer) {
  if (lexer->indx - 1 < 0) {
    lexer->indx = 0;
    lexer->cc   = 0;
    return 0;
  }
  int lc = lx_peek (lexer, -1);
  if (isnewline (lc)) {
    --lexer->cln;
    lexer->ccol = strlen (lexer->linev[lexer->cln - 1]) + 1;
  }
  lexer->cc = lexer->content[--lexer->indx];
  return lexer->cc;
}

char const* lx_save (Lexer* lexer) {
  if (!inProgress (lexer).source) {
    inProgress (lexer).source = (char const*)lexer->content + lexer->indx;
    inProgress (lexer).col    = lexer->ccol;
    inProgress (lexer).line   = lexer->cln;
  }
  ++lexer->in_progress.len;
  return lexer->in_progress.source;
}

void lx_delete (Lexer* lexer) { lexer->in_progress = (Token){0}; }

void lx_reset (Lexer* lexer) {
  lexer->cc   = 0;
  lexer->ccol = 1;
  lexer->cln  = 1;
  lx_delete (lexer);
  lexer->indx = 0;
  ful_freeTokens (lexer);
}

void lx_addtk (Lexer* lexer, short tk) {
  inProgress (lexer).tk = tk;
  lexer->tokenv = mem_grow (lexer->tokenv, sizeof (Token), lexer->tokenc,
                            &inProgress (lexer), 1);
  ++lexer->tokenc;
  lx_delete (lexer);
}

char lx_alpha (Lexer* lexer) {
  uInt          i;
  char*         id         = NULL;
  const Keyword keywords[] = {
      {"var",      TK_VAR   },
      {"function", TK_FUNC  },
      {"import",   TK_IMPORT},
      {"return",   TK_RETURN},
      {"if",       TK_IF    },
      {"else",     TK_ELSE  },
  };
  do {
    lx_save (lexer);
    lx_next (lexer);
  } while (isalnum (lexer->cc) ||
           (utf8_charsize (lexer->cc) > 1 && validc (lexer->cc)));
  id = ident (lexer);
  for (i = 0; i < sizeof (keywords) / sizeof (Keyword); ++i) {
    if (!strcmp (keywords[i].word, id)) {
      lx_addtk (lexer, keywords[i].tk);
      free (id);
      return 2;
    }
  }
  lx_addtk (lexer, TK_IDENT);
  free (id);
  return 1;
}

char lx_oper (Lexer* lexer) {
  do {
    lx_save (lexer);
    lx_next (lexer);
  } while (validopc (lexer->cc));
  lx_addtk (lexer, TK_OPER);
  return 1;
}

char lx_string (Lexer* lexer) {
  int q = lexer->cc;
  lx_next (lexer);
  while (lexer->cc != q && noteof (lexer->cc)) {
    /*if (lexer->cc == '\\') {
      int c = lx_peek(lexer, 1);

    }*/
    lx_save (lexer);
    lx_next (lexer);
  }
  if (!noteof (lexer->cc)) {
    stdError ("String literal extends to EOF\n");
    return 0;
  }
  lx_addtk (lexer, TK_STR);
  lx_next (lexer);
  return 1;
}

char lx_comment (Lexer* lexer) {
  int c = lx_peek (lexer, 1);
  if (c == '-') {
    while (!isnewline (lexer->cc) && noteof (lexer->cc)) {
      lx_next (lexer);
    }
    lx_next (lexer);
    return 1;
  } else if (c == '*') {
    while (noteof (lexer->cc)) {
      lx_next (lexer);
      if (lexer->cc == '*' && lx_peek (lexer, 1) == '-')
        break;
    }
    lx_skip (lexer, 2);
    return 2;
  } else {
    lx_oper (lexer);
    return 3;
  }
}

char lx_number (Lexer* lexer) {
  char  dotex = '.';
  char* id    = NULL;
  do {
    if (lexer->cc == '.')
      dotex = 0;
    lx_save (lexer);
  } while (isdigit (lx_next (lexer)) || lexer->cc == dotex);
  id = ident (lexer);
  if (!strcmp (id, ".")) {
    int lxl = lx_peek (lexer, -2);
    if (isalnum (lxl) || lxl == '_' || utf8_charsize (lxl) > 1) {
      lx_delete (lexer);
      lx_save (lexer);
      lx_next (lexer);
      lx_addtk (lexer, '.');
      free (id);
      return 2;
    } else {
      stdError ("Expected a number literal, but got \".\"\n");
      free (id);
      return 0;
    }
  }
  lx_addtk (lexer, TK_NUMBER);
  free (id);
  return 1;
}

char lx_access (Lexer* lexer) {
  lx_next (lexer);
  lx_addtk (lexer, TK_MACCESS);
  return 1;
}

int ful_lex (Lexer* lexer, char const* content) {
  lexer->content = content;
  lexer->size    = strlen (content);
  lx_reset (lexer);
  lexer->cc = lexer->content[0];
  while (lexer->cc != EOF) {
    if (noteof (lexer->cc))
      lx_save (lexer);
    else if (!lexer->ccol) {
      break;
    }
    if (isnewline (lexer->cc) || (!noteof (lexer->cc) && lexer->ccol)) {
      char* line = ident (lexer);
      lexer->linev =
          mem_grow (lexer->linev, sizeof (char const*), lexer->linec, &line, 1);
      lx_delete (lexer);
      ++lexer->linec;
    }
    lx_next (lexer);
  }
  lx_reset (lexer);
  lexer->cc = lexer->content[0];


  while (1) {
    while (isspace (lexer->cc)) {
      lx_next (lexer);
    }
    if (lexer->cc == EOF || lexer->indx >= lexer->size) {
      break;
    }
    if (isalpha (lexer->cc) || utf8_charsize (lexer->cc) > 1) {
      /* Alpha (ident) */
      lx_alpha (lexer);
      lx_delete (lexer);
      continue;
    }
    if (lexer->cc == '\"' || lexer->cc == '`') {
      /* String literal */
      lx_string (lexer);
      lx_delete (lexer);
      continue;
    }
    if (lexer->cc == '.') {
      if (lastTK (lexer) == TK_IDENT) { /* YAY! member access! */
        lx_access (lexer);
        lx_delete (lexer);
        continue;
      } else {
        lx_number (lexer);
        lx_delete (lexer);
        continue;
      }
    }
    if (isdigit (lexer->cc) || lexer->cc == '.') {
      lx_number (lexer);
      lx_delete (lexer);
      continue;
    }
    if (lexer->cc == '-') {
      /* Comment? */
      lx_comment (lexer);
      lx_delete (lexer);
      continue;
    }
    /*if (lastTK (lexer) == TK_MACCESS) {
      detailedError (
          lexer, lexer->tokenv + (lexer->tokenc - 1),
          "Lexer didnt recognize an identity after member access.\n");
      ful_freeLexer (lexer);
      return 1;
    }*/
    if (validopc (lexer->cc)) {
      lx_oper (lexer);
      lx_delete (lexer);
      continue;
    }
    if (!lexer->cc || isnewline (lexer->cc)) {
      lx_next (lexer);
      continue;
    }
    lx_save (lexer);
    lx_addtk (lexer, lexer->cc);
    lx_next (lexer);
    lx_delete (lexer);
  }
  return 0;
}

void ful_freeTokens (Lexer* lexer) {
  uInt i;
  if (lexer->tokenv) {
    free (lexer->tokenv);
    lexer->tokenv = NULL;
    lexer->tokenc = 0;
  }
}

void ful_freeLexer (Lexer* lexer) {
  uInt i;
  ful_freeTokens (lexer);
  if (lexer->linev) {
    for (i = 0; i < lexer->linec; ++i) {
      free ((void*)lexer->linev[i]);
    }
    free ((void*)lexer->linev);
    lexer->linev = NULL;
    lexer->linec = 0;
  }
}