#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ListTokens.h"
#include "lexer.h"
#include "AST.h"

typedef struct Parser {
    ListTokens* Tokens;
    ListTokens* lookahead;
    int count;
    struct AST *root;
} Parser;

Parser* parser;

struct AST* initParser(ListTokens* Tokens);
void S(Parser* parser);
void StList( );
void input1(struct AST* inputNode);
void input2(struct AST* inputNode);
void input3();
void commaid(struct AST* inputNode);
void headcommaid( );
void tailcommaid(struct AST* node);
void semiid();
void headsemiid();
void tailsemiid();
void print1(struct AST *node);
void print2(struct AST *node);
void initi(struct AST* node);
void varormas(struct AST* node);
void idornum();
void type();
void callOrAriph(struct AST* node);
void cOar(struct AST* node);
void term(struct AST* node);
void add_prior1(struct AST* node);
void add_prior2(struct AST* node);
void mult_prior1(struct AST* node);
void mult_prior2(struct AST* node);
void group(struct AST* node);
void mult_prior_oper(struct AST* node);
void add_prior_oper(struct AST* node);
void id_or_sign_number(struct AST* node);
void method_or_array(struct AST* node);
void call_method(struct AST* node);
void pass_arg(struct AST* node);
void tail_pass_arg(struct AST* node);
void sign_number(struct AST* node);
void sign(struct AST* node);
void vetvlenie(struct AST* StartNode);
void expression(struct AST* node);
void first_prior_expr1(struct AST* node);
void first_prior_expr2(struct AST* node);
void group_expr (struct AST* node);
void condition(struct AST* node);
void tsicl(struct AST* StartNode);
void tsicl1(struct AST* StartNode);
void func(struct AST* StartNode);

void match(char* x);
void consume();
ListTokens* nextToken();
ListTokens* getLookahead();
void printErrorMessage(int row, int column, char *x);

#endif