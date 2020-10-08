#include "hashtable.h"

int hashtab_hash(char *key) {
    int h = 0;
    char *p;

    for (p = key; *p != '\0'; p++) {
        h = h * HASHTAB_MUL + (int)*p;
    }
    return h % HASHTAB_SIZE;
}

void hashtab_init(struct listnode **hashtab) {
    int i;

    for (i = 0; i < HASHTAB_SIZE; i++) {
        hashtab[i] = NULL;
    }
}

struct idTable *idTable_init(int level) {
    struct idTable *table = (struct idTable*)calloc(1, sizeof(struct idTable));
    table->level = level;
    table->next = NULL;
    table->sizeTable = 0;
    hashtab_init(table->hashtab);
    return table;
}

void addSizeTable(struct idTable *table)
{
    table->sizeTable += 8;
}

void hashtab_add(struct listnode **hashtab, char *key, int value, int baseType, int type) {
    struct listnode *node;

    int index = hashtab_hash(key);

    node = malloc(sizeof(*node));
    if (node != NULL) {
        node->key = key;
        node->value = value;
        node->size = 8;
        node->offset = 0;
        node->baseType = baseType;
        node->type = type;
        node->next = hashtab[index];
        hashtab[index] = node;
    }
}

struct listnode *findInAllTable(struct idTable *table, char *key)
{
    struct listnode **currHashTab = table->hashtab;

    struct listnode *node = hashtab_lookup(currHashTab, key);

    if (node == NULL && table->next != NULL) {
        node = findInAllTable(table->next, key);
    }

    return node;
}

struct listnode *hashtab_lookup(struct listnode **hashtab, char *key) {
    int index;
    struct listnode *node;

    index = hashtab_hash(key);
    for (node = hashtab[index]; node != NULL; node = node->next) {
        if (strcmp(node->key, key) == 0) {
            return node;
        }
    }
    return NULL;
}

void hashtab_delete(struct listnode **hashtab, char *key) {
    int index;
    struct listnode *p, *prev = NULL;

    index = hashtab_hash(key);
    for (p = hashtab[index]; p != NULL; p = p->next) {
        if (strcmp(p->key, key) == 0) {
            if (prev == NULL) {
                hashtab[index] = p->next;
            } else {
                prev->next = p->next;
            }
            free(p);
            return;
        }
        prev = p;
    }
}

void hashtab_setOffset(struct listnode **hashtab, char *key, int offset)
{
    struct listnode *node = hashtab_lookup(hashtab, key);
    node->offset = offset;
}
