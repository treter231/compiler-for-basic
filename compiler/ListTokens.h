#ifndef LISTTOKENS_H
#define LISTTOKENS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ListTokens {
    char lexeme[50];
    char token[50];
    int row;
    int column;
    struct ListTokens *next;
} ListTokens;

ListTokens *init();
ListTokens *addlexeme(ListTokens *List, char *token, char *lexeme, int row, int column);
void ListPrint(ListTokens *head);

#endif
