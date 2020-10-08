#include "semantics.h"

void init_semantic(struct AST *root) {
	root->table = idTable_init(0);
	travel_tree(root->table, root);
	countError = 0;
}

void travel_tree(struct idTable *table, struct AST *node) {
	struct idTable* currTable = table;
	if (strcmp(node->Stroka, "dim") == 0) {
		struct ListChild* Children = node->ListChildren;
		while (Children != NULL) {
			char* name = Children->Node->Token->lexeme;

			struct listnode *foundNode = hashtab_lookup(table->hashtab, name);

			if (foundNode == NULL) {
				int value = hashtab_hash(name);
	
				int baseType;

				if (strcmp(Children->Node->Stroka, "var") == 0) {
					baseType = 1;
				} else if (strcmp(Children->Node->Stroka, "arr") == 0) {
					baseType = 2;
				} 

				int type;

				if (Children->Node->type == 1) {
					type = 1;
				} else if (Children->Node->type == 2) {
					type = 2;
				}

				hashtab_add(currTable->hashtab, name, value, baseType, type);
				addSizeTable(currTable);
				hashtab_setOffset(currTable->hashtab, name, table->sizeTable);
			} else {
				redeclMessage(Children->Node);
			}
			Children = Children->next;
		}
		return;
	} else if ((strcmp(node->Stroka, "if") == 0)
		|| (strcmp(node->Stroka, "while") == 0)) {

		int lvl = currTable->level + 1;
		node->table = idTable_init(lvl);

		node->table->next = currTable;

		currTable = node->table;    
	} else if (strcmp(node->Stroka, "func") == 0) {
		char* name = node->Token->lexeme;

		struct listnode *foundNode = hashtab_lookup(table->hashtab, name);

		if (foundNode == NULL) {
			int value = hashtab_hash(name);

			hashtab_add(currTable->hashtab, name, value, 3, 0);

			int lvl = currTable->level + 1;
			node->table = idTable_init(lvl);

			node->table->next = currTable;

			currTable = node->table;
		} else {
			redeclMessage(node);
		}
	} else if (strcmp(node->Stroka, "id") == 0
			|| strcmp(node->Stroka, "callFunc") == 0) {
		char* name = node->Token->lexeme;

		struct listnode *foundNode = findInAllTable(currTable, name);

		if (foundNode == NULL) {
			notdeclMessage(node);
		}
	}

	struct ListChild* Children = node->ListChildren;
	while (Children != NULL) {
		travel_tree(currTable, Children->Node);
		Children = Children->next;
	}
}

void redeclMessage(struct AST *node)
{
	countError++;
	printf("%d:", node->Token->row);
	printf("%d:", node->Token->column);
	printf("error: redeclorte ");
	printf("%s\n", node->Token->lexeme);
}

void notdeclMessage(struct AST *node)
{
	countError++;
	printf("%d:", node->Token->row);
	printf("%d:", node->Token->column);
	printf("error: notdeclorate ");
	printf("%s\n", node->Token->lexeme);
}

void print_table(struct AST *node) {
	if (node->table != NULL) {
		printf("level = %d\n", node->table->level);
		for (int i=0; i < HASHTAB_SIZE; i++) {
			struct listnode *hashnode = node->table->hashtab[i];
			if (hashnode != NULL) {
				printf("id=%s\t", hashnode->key);
				printf("baseType=%d\t", hashnode->baseType);
				printf("type=%d\t", hashnode->type);
				printf("offset=%d\n", hashnode->offset);
			}
		}
		printf("Size tab: %d\n", node->table->sizeTable);
	}
	struct ListChild* Children = node->ListChildren;
	while (Children != NULL) {
		print_table(Children->Node);
		Children = Children->next;
	}
}