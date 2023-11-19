#include "include/lex.h"
#include <ctype.h>
#include <stdio.h>
#define NO_DIRENT
#include "include/common.h"

#define BRTBRED "\x1B[91;1m"
#define RESET "\x1B[0m"

int lx_next(lexer_t *lexer) {
  if (lexer->indx+1 >= lexer->size) {
    lexer->indx = lexer->size;
    lexer->cc = EOF;
    return EOF;
  }
  lexer->cc = lexer->content[++lexer->indx];
  return lexer->cc;
}

int lx_skip(lexer_t *lexer, int off) {
  int i = lexer->indx + off;
  if (i >= lexer->size || i < 0)
    return (i < 0) ? 0 : EOF;
  lexer->indx = i;
  lexer->cc = lexer->content[i];
  return lexer->cc;
}

int lx_peek(lexer_t *lexer, int off) {
  int i = lexer->indx + off;
  if (i >= lexer->size || i < 0)
    return (i < 0) ? 0 : EOF;
  return lexer->content[i];
}

int lx_back(lexer_t* lexer) {
  if (lexer->indx-1 < 0) {
    lexer->indx = 0;
    lexer->cc = 0;
    return 0;
  }
  lexer->cc = lexer->content[--lexer->indx];
  return lexer->cc;
}

const char *lx_save(lexer_t* lexer) {
  lexer->ident = str_append(lexer->ident, lexer->content+lexer->indx, utf8_charsize(lexer->cc));
  return lexer->ident;
}

void lx_delete(lexer_t* lexer) {
  if (lexer->ident)
    free(lexer->ident);
  lexer->ident = NULL;
}

#define validopc(c) (str_ffo("~+-*/!@$%%^&*[]<>|=", (char)c) != npos)

#define isnewline(c) (c == '\n' || c == '\r')
#define noteof(c) (c != EOF)
#define validc(c) (!c && c != EOF)

void lx_addtk(lexer_t* lexer, short tk) {
  token_t t = {0};
  t.ident = (char*)str_cpy(lexer->ident, npos);
  t.tk = tk;
  token_t *nts = malloc(sizeof(token_t)*(lexer->tokenc+1));
  memcpy(nts, lexer->tokenv, sizeof(token_t)*lexer->tokenc);
  if (lexer->tokenv)
    free(lexer->tokenv);
  memcpy(nts+lexer->tokenc, &t, sizeof(token_t));
  lexer->tokenv = nts;
  ++lexer->tokenc;
}

char lx_alpha(lexer_t* lexer) {
  const keyword_t keywords[] = {
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
    lx_save(lexer);
    lx_next(lexer);
  } while (isalnum(lexer->cc) || (utf8_charsize(lexer->cc) > 1 && validc(lexer->cc)));
  for (int i = 0; i < sizeof(keywords) / sizeof(keyword_t); ++i) {
    if (!strcmp(keywords[i].word, lexer->ident)) {
      lx_addtk(lexer, keywords[i].tk);
      return 2;
    }
  }
  lx_addtk(lexer, tk_ident);
  return 1;
}

char lx_oper(lexer_t* lexer) {
  do {
    lx_save(lexer);
    lx_next(lexer);
  } while (validopc(lexer->cc));
  lx_addtk(lexer, tk_oper);
  return 1;
}

char lx_string(lexer_t* lexer) {
  int q = lexer->cc;
  lx_next(lexer);
  while (lexer->cc != q && noteof(lexer->cc)) {
    /*if (lexer->cc == '\\') {
      int c = lx_peek(lexer, 1);
      
    }*/
    lx_save(lexer);
    lx_next(lexer);
  }
  if (!noteof(lexer->cc)) {
    printf(BRTBRED "String literal extends to EOF\n" RESET);
    return 0;
  }
  lx_addtk(lexer, tk_str);
  lx_next(lexer);
  return 1;
}

char lx_comment(lexer_t* lexer) {
  int c = lx_peek(lexer, 1);
  if (c == '-') {
    while (!isnewline(lexer->cc) && noteof(lexer->cc)) {
      lx_next(lexer);
    }
    lx_next(lexer);
    return 1;
  } else if (c == '*') {
    while (noteof(lexer->cc)) {
      lx_next(lexer);
      if (lexer->cc == '*' && lx_peek(lexer, 1) == '-')
       break;
    }
    lx_skip(lexer, 2);
    return 2;
  } else {
    lx_oper(lexer);
    return 3;
  }
}

char lx_number(lexer_t* lexer) {
  char dotex = '.';
  do {
    if (lexer->cc == '.')
      dotex = 0;
    lx_save(lexer);
  } while(isdigit(lx_next(lexer)) || lexer->cc == dotex);
  if (!strcmp(lexer->ident, ".")) {
    printf(BRTBRED "Expected a number literal, but got \".\"\n" RESET);
    return 0;
  }
  lx_addtk(lexer, tk_number);
  return 1;
}


int lex(lexer_t* lexer, const char* content) {
  lexer->content = content;
  lexer->size = strlen(content);
  while (1) {
    while (isspace(lexer->cc)) {
      lx_next(lexer);
    }
    if (lexer->cc == EOF || lexer->indx >= lexer->size) {
      break;
    }
    
    if (isalpha(lexer->cc) || utf8_charsize(lexer->cc) > 1) {
      // Alpha (ident)
      lx_alpha(lexer);
      lx_delete(lexer);
      continue;
    }

    if (lexer->cc == '\"' || lexer->cc == '`') {
      // String literal
      lx_string(lexer);
      lx_delete(lexer);
      continue;
    }

    if (isdigit(lexer->cc) || lexer->cc == '.') {
      lx_number(lexer);
      lx_delete(lexer);
      continue;
    }

    if (lexer->cc == '-') {
      // Comment?
      lx_comment(lexer);
      lx_delete(lexer);
      continue;
    }

    if (validopc(lexer->cc)) {
      lx_oper(lexer);
      lx_delete(lexer);
      continue;
    }

    lx_oper(lexer);
    lx_delete(lexer);
  }
  return 1;
}
