#include "AST.h"

int countNode = 1;

struct AST* initASTNode() {
	struct AST* ast;
	ast = (struct AST*) calloc(1, sizeof(struct AST));
	ast->Stroka = NULL;
	ast->Token = NULL;
	ast->ListChildren = NULL;
	ast->idNode = countNode;
	countNode++;
	ast->type = 0;
	ast->table = NULL;

	return ast;
}

void add_child(struct AST* Node, struct AST* parent) {
	struct ListChild* child;
	child = (struct ListChild*) calloc(1, sizeof(struct ListChild));
	child->next = NULL;
	child->Node = Node;
	struct ListChild* pnode = parent->ListChildren;  
	if (pnode == NULL) {
		parent->ListChildren = child;
	} else {
		while (pnode->next != NULL)
		{
			pnode = pnode->next;
		}
		pnode->next = child;
	}
}

void setStroka(struct AST *node, char* str)
{
	node->Stroka = (char*) calloc(strlen(str), sizeof(char*));
	strcpy(node->Stroka, str);
}

void setToken(struct AST *node, ListTokens *token) {
	node->Token = token;
}

struct ListChild* searchLastChild(struct AST* node)
{
	struct ListChild* listChild = node->ListChildren;
	while (listChild->next != NULL)
		listChild = listChild->next;
	return listChild;
}

struct AST* getLastChilde(struct AST* node)
{
	return searchLastChild(node)->Node;
}

void deleteLastChild(struct AST *node)
{
	struct ListChild* listChild = searchLastChild(node);
	listChild->Node = NULL;
}

void swapChild(struct AST *parent, struct AST *newChild)
{
	struct AST *oldChild = getLastChilde(parent);
	deleteLastChild(parent);
	addNewChild(parent, newChild);
	add_child(oldChild, newChild);
}

void addNewChild(struct AST *parent, struct AST *newChild)
{
	struct ListChild *listChild = searchLastChild(parent);
	listChild->Node = newChild;
}

void createTree(struct AST* Node) {
	FILE *graph;
	if ((graph = fopen("graph.gv", "w")) == NULL) {
		printf ("Cannot open file.\n");
		exit(1); 
	}
	fprintf(graph, "digraph {\n");
	createConnect(graph, Node);
	fprintf(graph, "}\n");
}

void createConnect(FILE *graph, struct AST* Node) {
	createBox(graph, Node);
	struct ListChild* child = Node->ListChildren;
	while (child != NULL) {
		fprintf(graph, "\t");
		toGvNode(graph, Node);
		fprintf(graph, " -> ");
		toGvNode(graph, child->Node);
		fprintf(graph, "\n");
		createConnect(graph, child->Node);
		child = child->next;
	}
}

void createBox(FILE *graph, struct AST* Node) {
	fprintf(graph, "\t\"node%d\" [label=\"", Node->idNode);
	if (Node->Stroka != NULL) {
		fprintf(graph, "TypeNode=%s",Node->Stroka);
	} else {
		fprintf(graph, "TypeNode=unknown");
	}

	if (Node->type == 0) {
		fprintf(graph, "\\ntype=unknown");
	} else if (Node->type == 1) {
		fprintf(graph, "\\ntype=int");
	} else if (Node->type == 2) {
		fprintf(graph, "\\ntype=string");
	}

	if (Node->Token != NULL) {
		fprintf(graph, "\\nlexeme=%s", Node->Token->lexeme);
	}
	
	fprintf(graph, "\"]\n");
}

void toGvNode(FILE *graph, struct AST* Node) {
	fprintf(graph, "\"node%d\"", Node->idNode);
}