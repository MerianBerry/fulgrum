#define NO_DIRENT
#include "fulex.h"

#include <ctype.h>
#include <stdio.h>

#include "common.h"
#include "fuerror.h"

#define validopc(c)  (str_ffo ("~+-*/!@$%%^&*[]<>|=", (char)c) != npos)

#define isnewline(c) (c == '\n' || c == '\r')
#define noteof(c)    (c != EOF)
#define validc(c)    (!c && c != EOF)

int lx_next (fu_lexer* lexer) {
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

int lx_skip (fu_lexer* lexer, int off) {
  int i = lexer->indx + off;
  if (i >= lexer->size || i < 0)
    return (i < 0) ? 0 : EOF;
  lexer->indx = i;
  lexer->cc   = lexer->content[i];
  return lexer->cc;
}

int lx_peek (fu_lexer* lexer, int off) {
  int i = lexer->indx + off;
  if (i >= lexer->size || i < 0)
    return (i < 0) ? 0 : EOF;
  return lexer->content[i];
}

int lx_back (fu_lexer* lexer) {
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

char const* lx_save (fu_lexer* lexer) {
  lexer->ident = str_append (lexer->ident, lexer->content + lexer->indx,
                             utf8_charsize (lexer->cc));
  return lexer->ident;
}

void lx_delete (fu_lexer* lexer) {
  if (lexer->ident)
    free (lexer->ident);
  lexer->ident = NULL;
}

void lx_reset (fu_lexer* lexer) {
  lexer->cc   = 0;
  lexer->ccol = 1;
  lexer->cln  = 1;
  lx_delete (lexer);
  lexer->indx = 0;
  ful_freeTokens (lexer);
}

void lx_addtk (fu_lexer* lexer, short tk) {
  fu_token t = {0};
  t.ident    = (char*)str_cpy (lexer->ident, npos);
  t.tk       = tk;
  t.col      = lexer->ccol;
  t.ln       = lexer->cln;
  lexer->tokenv =
      mem_grow (lexer->tokenv, sizeof (fu_token), lexer->tokenc, &t, 1);
  ++lexer->tokenc;
}

char lx_alpha (fu_lexer* lexer) {
  fu_dword         i;
  const fu_keyword keywords[] = {
    MAKE_KEYWORD(var),
    MAKE_KEYWORD(func),
    MAKE_KEYWORD(class),
    MAKE_KEYWORD(import),
    MAKE_KEYWORD(reserve),
    MAKE_KEYWORD(return),
    MAKE_KEYWORD(if),
    MAKE_KEYWORD(else),
    MAKE_KEYWORD(switch),
    MAKE_KEYWORD(case),
    MAKE_KEYWORD(while),
    MAKE_KEYWORD(for),
    MAKE_KEYWORD(foreach),
    MAKE_KEYWORD(break),
    MAKE_KEYWORD(continue),
  };
  do {
    lx_save (lexer);
    lx_next (lexer);
  } while (isalnum (lexer->cc) ||
           (utf8_charsize (lexer->cc) > 1 && validc (lexer->cc)));
  for (i = 0; i < sizeof (keywords) / sizeof (fu_keyword); ++i) {
    if (!strcmp (keywords[i].word, lexer->ident)) {
      lx_addtk (lexer, keywords[i].tk);
      return 2;
    }
  }
  lx_addtk (lexer, tk_ident);
  return 1;
}

char lx_oper (fu_lexer* lexer) {
  do {
    lx_save (lexer);
    lx_next (lexer);
  } while (validopc (lexer->cc));
  lx_addtk (lexer, tk_oper);
  return 1;
}

char lx_string (fu_lexer* lexer) {
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
  lx_addtk (lexer, tk_str);
  lx_next (lexer);
  return 1;
}

char lx_comment (fu_lexer* lexer) {
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

char lx_number (fu_lexer* lexer) {
  char dotex = '.';
  do {
    if (lexer->cc == '.')
      dotex = 0;
    lx_save (lexer);
  } while (isdigit (lx_next (lexer)) || lexer->cc == dotex);
  if (!strcmp (lexer->ident, ".")) {
    int lxl = lx_peek (lexer, -2);
    if (isalnum (lxl) || lxl == '_' || utf8_charsize (lxl) > 1) {
      lx_delete (lexer);
      lx_save (lexer);
      lx_next (lexer);
      lx_addtk (lexer, '.');
      return 2;
    } else {
      stdError ("Expected a number literal, but got \".\"\n");
      return 0;
    }
  }
  lx_addtk (lexer, tk_number);
  return 1;
}

int ful_lex (fu_lexer* lexer, char const* content) {
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
      char const* cpy = str_cpy (lexer->ident, npos);
      lexer->linev =
          mem_grow (lexer->linev, sizeof (char const*), lexer->linec, &cpy, 1);
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

void ful_freeTokens (fu_lexer* lexer) {
  fu_dword i;
  if (lexer->tokenv) {
    for (i = 0; i < lexer->tokenc; ++i) {
      if (lexer->tokenv[i].ident)
        free (lexer->tokenv[i].ident);
    }
    free (lexer->tokenv);
    lexer->tokenv = NULL;
    lexer->tokenc = 0;
  }
}

void ful_freeLexer (fu_lexer* lexer) {
  fu_dword i;
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