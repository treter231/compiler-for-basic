#include "ListChild.h"

struct ListChild *chinit() {
    struct ListChild *ListChildren;
    ListChildren = (struct ListChild*)malloc(sizeof(struct ListChild));
    ListChildren->next = NULL;
    ListChildren->Node = NULL;
    return ListChildren;
}

struct ListChild *addChild(struct ListChild *List, struct AST *Node) {
    if (List->Node == NULL) {
        List->Node = Node;
        return List;
    } else {
        struct ListChild *List2;
        List2 = (struct ListChild*)malloc(sizeof(struct ListChild));
        List->next = List2;
        List->Node = Node;
        List2->next = NULL;
        return List2;
    }
    return NULL;
}

struct ListChild *getChild(struct ListChild *list, int ind)
{
    struct ListChild *child = list;
    for (int i = 0; i != ind; i++) {
        if (child != NULL)
            child = child->next;
        else
            return NULL;
    }

    return child;
}
