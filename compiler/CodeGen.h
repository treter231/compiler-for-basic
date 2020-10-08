#ifndef CODEGEN_H
#define CODEGEN_H

#include "AST.h"
#include "hashtable.h"
#include <stdlib.h>

FILE *asmb;

int regValAri[6];
int regValArg[6];

int countJmp;

struct idTable *currTable;

void code_gen(struct AST *root);
void treversal(struct AST *node);
char* rightPart(struct AST *node);

void genFunc(struct AST *node);
void createIf(struct AST *node);
void createWhile(struct AST *node);
void createPrint(struct AST *node);
void createInput(struct AST *node);

int getRegForAri();
void setRegForAri(int numReg, int value);
char* regAriNumToStr(int numReg);
int regAriStrToReg(char* strReg);
void freeRegValAri(char* strReg);
int getValRegAri(int numReg);

int getSizeNumber(int num);

void Mov(char *str1, char *str2);
void Add(char *str1, char *str2);
void Sub(char *str1, char *str2);
void Imul(char *str1, char *str2);
void Mul(char *str1, char *str2);
void Idiv(char *str1);
void Xor(char *str1, char *str2);
void Cmp(char *str1, char *str2);
void Loop(int jmp);
void Jmp(char *str1, int x);
void Push(char *reg);
void Pop(char *reg);

char* getOffset(struct AST *node);
char* createExpr(struct AST *node);

#endif