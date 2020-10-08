#include "ListTokens.h"

ListTokens *init() {
    ListTokens *ListToken;
    ListToken = (ListTokens*)malloc(sizeof(ListTokens));
    ListToken->row = -1;
    ListToken->next = NULL;
    return ListToken;
}

ListTokens *addlexeme(ListTokens *List, char *token, char *lexeme, int row, int column) {
    if (List->row == -1) {
        strcpy(List->lexeme, lexeme);
        strcpy(List->token, token);
        List->row = row;
        List->column = column;
        return List;
    } else {
        ListTokens *List2;
        List2 = (ListTokens*)malloc(sizeof(ListTokens));
        List->next = List2;
        strcpy(List2->lexeme, lexeme);
        strcpy(List2->token, token);
        List2->row = row;
        List2->column = column;
        List2->next = NULL;
        return List2;
    }
    return NULL;
}

void ListPrint(ListTokens *head) {
    ListTokens *print;
    print = head;
    while (print != NULL) {
        printf("Loc=<%d:%d> %s '%s'\n", print->row, print->column, print->token, print->lexeme);
        print = print->next;
    }
}
