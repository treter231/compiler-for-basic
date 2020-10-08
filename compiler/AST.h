#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ListTokens.h"
#include "ListChild.h"
#include "hashtable.h"

struct AST {
    char *Stroka;
    ListTokens* Token;
    struct ListChild* ListChildren;
    int idNode;
    int type;
    struct idTable* table;
};

struct AST* initASTNode();

void add_child(struct AST* Node, struct AST* parent);

void setStroka(struct AST* node, char *str);
void setToken(struct AST* node, ListTokens *token);

struct ListChild* searchLastChild(struct AST* node);
struct AST* getLastChilde(struct AST* node);
void deleteLastChild(struct AST *node);
void swapChild(struct AST *parent, struct AST *newChild);
void addNewChild(struct AST *parent, struct AST *newChild);

void createTree(struct AST* Node);
void createConnect(FILE *graph, struct AST* Node);
void createBox(FILE *graph, struct AST* Node);
void toGvNode(FILE *graph, struct AST* Node);

#endif