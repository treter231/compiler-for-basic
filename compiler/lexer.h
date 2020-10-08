#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "lexer.c"
#include "ListTokens.h"

char *DefineToken(char *lexeme);
ListTokens* IdentifyTokens(int argc, char const *argv);

#endif