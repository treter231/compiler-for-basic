#include "parser.h"

struct AST* initParser(ListTokens* Tokens) {
	parser = (Parser*) calloc(1, sizeof(Parser));
	parser->Tokens = Tokens;
	parser->count = 0;
	parser->lookahead = Tokens;
	parser->root = NULL;

	S(parser);
	printf("End parsing\n");
	return parser->root;
}

void S(Parser* parser) {
	struct AST* StartNode = initASTNode();
	setStroka(StartNode, "start");
	parser->root = StartNode;
	
	StList(StartNode);
	
	match("end");
}

void StList(struct AST* StartNode) {
	if (strcmp(parser->lookahead->token, "input") == 0) {
		struct AST* InputNode = initASTNode();
		setStroka(InputNode, "input");
		add_child(InputNode, StartNode);

		input1(InputNode);
		StList(StartNode);
	} else if (strcmp(parser->lookahead->token, "print") == 0) {
		struct AST* printNode = initASTNode();
		setStroka(printNode, "print");
		add_child(printNode, StartNode);
		
		print1(printNode);
		StList(StartNode);
	} else if (strcmp(parser->lookahead->token, "dim") == 0) {
		struct AST* dimNode = initASTNode();
		setStroka(dimNode, "dim");
		add_child(dimNode, StartNode);

		initi(dimNode);
		StList(StartNode);
	} else if (strcmp(parser->lookahead->token, "id") == 0) {
		callOrAriph(StartNode);
		StList(StartNode);
	} else if (strcmp(parser->lookahead->token, "if") == 0) {
		vetvlenie(StartNode);
		StList(StartNode);
	} else if (strcmp(parser->lookahead->token, "do") == 0) {
		tsicl(StartNode);
		StList(StartNode);
	} else if (strcmp(parser->lookahead->token, "function") == 0) {
		func(StartNode);
		StList(StartNode);
	}
}

void input1(struct AST* inputNode) {
	match("input");
	input2(inputNode);
}

void input2(struct AST* inputNode) {
	if (strcmp(parser->lookahead->token, "id") == 0) {
		commaid(inputNode);
	} else if (strcmp(parser->lookahead->token, "literal") == 0) {
		struct AST* literalNode = initASTNode();
		setStroka(literalNode, "literal");
		add_child(literalNode, inputNode);

		match("literal");
		input3(literalNode);
	} else {
		printf("error: %d:%d: expecting id or literal, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}   
}

void input3(struct AST* node) {
	if (strcmp(parser->lookahead->token, "comma") == 0) {
		match("comma");
		struct AST* idNode = initASTNode();
		setStroka(idNode, "var");
		setToken(idNode, getLookahead());
		
		add_child(idNode, node);
		match("id");
	} else if (strcmp(parser->lookahead->token, "semi") == 0) {
		match("semi");
		semiid(node);
	} else {
		printf("error: %d:%d: expecting comma or semi, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void commaid(struct AST* node) {
	headcommaid(node);
	tailcommaid(node);
}

void headcommaid(struct AST* node) {
	struct AST* idNode = initASTNode();
	setStroka(idNode, "var");
	setToken(idNode, getLookahead());
	
	add_child(idNode, node);
	match("id");
}

void tailcommaid(struct AST* node) {
	if (strcmp(parser->lookahead->token, "comma") == 0) {
		match("comma");

		struct AST* idNode = initASTNode();
		setStroka(idNode, "var");
		setToken(idNode, getLookahead());
	
		add_child(idNode, node);

		match("id");
		tailcommaid(node);
	}
}

void semiid(struct AST* node) {
	headsemiid(node);
	tailsemiid(node);
}

void headsemiid(struct AST* node) {
	struct AST* idNode = initASTNode();
	setStroka(idNode, "var");
	setToken(idNode, getLookahead());

	add_child(idNode, node);
	match("id");
}

void tailsemiid(struct AST* node) {
	if (strcmp(parser->lookahead->token, "semi") == 0) {
		match("semi");

		struct AST* idNode = initASTNode();
		setStroka(idNode, "var");
		setToken(idNode, getLookahead());
	
		add_child(idNode, node);
		
		match("id");
		tailsemiid(node);
	}
}

void print1(struct AST *node) {
	match("print");
	print2(node);
}

void print2(struct AST *node) {
	if (strcmp(parser->lookahead->token, "id") == 0) {
		commaid(node);
	} else if (strcmp(parser->lookahead->token, "literal") == 0) {
		struct AST* literalNode = initASTNode();
		setStroka(literalNode, "literal");
		add_child(literalNode, node);
		match("literal");
		match("comma");
		commaid(literalNode);
	} else {
		printf("error: %d:%d: expecting id or literal, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}  
}

void initi(struct AST* node) {
	match("dim");

	// struct AST* InputNode = initASTNode();
	// setStroka(InputNode, "input");
	// add_child(InputNode, StartNode);

	struct AST* idNode = initASTNode();
	//setStroka(idNode, "id");
	setToken(idNode, getLookahead());
	
	add_child(idNode, node);
	match("id");
	varormas(node);
}

void varormas(struct AST* node) {
	if ((strcmp(parser->lookahead->token, "comma") == 0) || 
		(strcmp(parser->lookahead->token, "as") == 0)) { 
		struct AST* glc = getLastChilde(node);
		setStroka(glc, "var");

		tailcommaid(node);
		match("as");
		type(node);
	} else if (strcmp(parser->lookahead->token, "l_paren") == 0) {
		match("l_paren");
		struct AST* glc = getLastChilde(node);
		setStroka(glc, "arr");
		idornum();
		match("r_paren");
		match("as");
		type(node);
	} else {
		printf("error: %d:%d: expecting l_paren or comma or as, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void idornum() {
	if (strcmp(parser->lookahead->token, "id") == 0) {
		match("id");
	} else if (strcmp(parser->lookahead->token, "numeric") == 0) {
		match("numeric");
	} else {
		printf("error: %d:%d: expecting id or numeric, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void type(struct AST* node) {
	int TypeNode;
	if (strcmp(parser->lookahead->token, "integer") == 0) {
		match("integer");
		TypeNode = 1;
	} else if (strcmp(parser->lookahead->token, "string") == 0) {
		match("string");
		TypeNode = 2;
	} else {
		printf("error: %d:%d: expecting integer or string, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
	struct ListChild* Node = node->ListChildren;
	while (Node != NULL) {
		Node->Node->type = TypeNode;
		Node = Node->next;
	}
}

void callOrAriph(struct AST* node) {
	struct AST* idNode = initASTNode();
	setToken(idNode, getLookahead());
	
	add_child(idNode, node);

	match("id");
	cOar(node);
}

void cOar(struct AST* node) {
	if (strcmp(parser->lookahead->token, "l_paren") == 0) {
		struct AST *child = getLastChilde(node);
		setStroka(child, "callFunc");
		struct AST* idNode = getLastChilde(node);
		call_method(idNode);
	} else if (strcmp(parser->lookahead->token, "equal") == 0) {
		struct AST *child = getLastChilde(node);
		setStroka(child, "id");

		struct AST* AssignNode = initASTNode();
		setStroka(AssignNode, "assign");
		swapChild(node, AssignNode);
		//setToken(idNode, getLookahead());

		match("equal");
		term(AssignNode);
	}  else {
		printf("error: %d:%d: expecting l_paren or equal, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void term(struct AST* node) {
	add_prior1(node);
}

void add_prior1(struct AST* node) {
	mult_prior1(node);
	add_prior2(node);
}

void add_prior2(struct AST* node) {
	if ((strcmp(parser->lookahead->token, "plus") == 0) || 
		(strcmp(parser->lookahead->token, "minus") == 0)) {
			struct AST *operNode = initASTNode();
			setStroka(operNode, parser->lookahead->token);
			swapChild(node, operNode);

			add_prior_oper(node);
			mult_prior1(operNode);
			add_prior2(node);
	}
}

void mult_prior1(struct AST* node) {
	group(node);
	mult_prior2(node);
}

void mult_prior2(struct AST* node) {
	if ((strcmp(parser->lookahead->token, "multiply") == 0) || 
		(strcmp(parser->lookahead->token, "divide") == 0)) {
			struct AST *operNode = initASTNode();
			setStroka(operNode, parser->lookahead->token);
			swapChild(node, operNode);

			mult_prior_oper(node);
			group(operNode);
			mult_prior2(node);
	}
}

void group(struct AST* node) {
	if (strcmp(parser->lookahead->token, "l_paren") == 0) {
		match ("l_paren");
		term(node);
		match ("r_paren");
	} else if ((strcmp(parser->lookahead->token, "id") == 0) ||
	(strcmp(parser->lookahead->token, "plus") == 0) ||
	(strcmp(parser->lookahead->token, "minus") == 0) ||
	(strcmp(parser->lookahead->token, "numeric") == 0)) {
		id_or_sign_number(node);
	}  else {
		printf("error: %d:%d: expecting l_paren or id or plus or minus or numeric, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void mult_prior_oper(struct AST* node) {
	if (strcmp(parser->lookahead->token, "multiply") == 0) {
		match ("multiply");
	} else if (strcmp(parser->lookahead->token, "divide") == 0) {
		match("divide");
	}  else {
		printf("error: %d:%d: expecting multiply or divide, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void add_prior_oper(struct AST* node) {
	if (strcmp(parser->lookahead->token, "plus") == 0) {
		match ("plus");
	} else if (strcmp(parser->lookahead->token, "minus") == 0) {
		match("minus");
	}  else {
		printf("error: %d:%d: expecting plus or minus, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void id_or_sign_number(struct AST* node) {
	if (strcmp(parser->lookahead->token, "id") == 0) {
		// struct AST *child = getLastChilde(node);
		// setToken(child, )
		
		struct AST *idNode = initASTNode();
		setStroka(idNode, parser->lookahead->token);
		setToken(idNode, parser->lookahead);
		add_child(idNode, node);

		match ("id");
		method_or_array(node);
	} else if ((strcmp(parser->lookahead->token, "plus") == 0) ||
	(strcmp(parser->lookahead->token, "minus") == 0) ||
	(strcmp(parser->lookahead->token, "numeric") == 0)) {

		sign_number(node);
	}  else {
		printf("error: %d:%d: expecting id or plus or minus or numeric, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void method_or_array(struct AST* node) {
	if (strcmp(parser->lookahead->token, "l_paren") == 0) {
		match("l_paren");
		pass_arg(node);
		match("r_paren");
	}
}

void call_method(struct AST* node) {
	if (strcmp(parser->lookahead->token, "l_paren") == 0) {
		match("l_paren");
		struct AST *passArgNode = initASTNode();
		setStroka(passArgNode, "passArg");
		add_child(passArgNode, node);

		pass_arg(passArgNode);
		match("r_paren");
	} else {
		printf("error: %d:%d: expecting l_paren, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void pass_arg(struct AST* node) {
	if ((strcmp(parser->lookahead->token, "l_paren") == 0) ||
	(strcmp(parser->lookahead->token, "id") == 0) ||
	(strcmp(parser->lookahead->token, "plus") == 0) ||
	(strcmp(parser->lookahead->token, "minus") == 0) ||
	(strcmp(parser->lookahead->token, "numeric") == 0)) {
		term(node);
		tail_pass_arg(node);
	}
}

void tail_pass_arg(struct AST* node) {
	if (strcmp(parser->lookahead->token, "comma") == 0) {
		match("comma");
		term(node);
		tail_pass_arg(node);
	}
}

void sign_number(struct AST* node) {
	if ((strcmp(parser->lookahead->token, "plus") == 0) ||
	(strcmp(parser->lookahead->token, "minus") == 0) ||
	(strcmp(parser->lookahead->token, "numeric") == 0)) {
		sign(node);

		struct AST *numNode = initASTNode();
		setStroka(numNode, "number");
		setToken(numNode, parser->lookahead);
		add_child(numNode, node);

		match("numeric");
	} else {
		printf("error: %d:%d: expecting plus or minus or numeric, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void sign(struct AST* node) {
	if (strcmp(parser->lookahead->token, "plus") == 0) {
		match("plus");
	} else if (strcmp(parser->lookahead->token, "minus") == 0) 
		match("minus");
}

void vetvlenie(struct AST* StartNode)
{	
	match("if");
	struct AST *ifNode = initASTNode();
	setStroka(ifNode, "if");
	add_child(ifNode, StartNode);

	struct AST *exprNode = initASTNode();
	setStroka(exprNode, "expr");
	add_child(exprNode, ifNode);
	
	expression(exprNode);
	
	match("then");
	struct AST *thenNode = initASTNode();
	setStroka(thenNode, "then");
	add_child(thenNode, ifNode);
	
	StList(thenNode);
	
	match("else");
	struct AST *elseNode = initASTNode();
	setStroka(elseNode, "then");
	add_child(elseNode, ifNode);

	StList(elseNode);
	
	match("end");
	match("if");
}

void expression(struct AST* node) {
	if ((strcmp(parser->lookahead->token, "l_paren") == 0) ||
	(strcmp(parser->lookahead->token, "id") == 0) ||
	(strcmp(parser->lookahead->token, "plus") == 0) ||
	(strcmp(parser->lookahead->token, "minus") == 0) ||
	(strcmp(parser->lookahead->token, "numeric") == 0)) {
		first_prior_expr1(node);
	} else {
		printf("error: %d:%d: expecting l_paren or id or plus or minus or numeric, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void first_prior_expr1(struct AST* node) {
	group_expr(node);
	first_prior_expr2(node);
}

void first_prior_expr2(struct AST* node) {
	if ((strcmp(parser->lookahead->token, "equal") == 0) ||
	(strcmp(parser->lookahead->token, "not_equal") == 0) ||
	(strcmp(parser->lookahead->token, "less") == 0) ||
	(strcmp(parser->lookahead->token, "less_or_equal") == 0) ||
	(strcmp(parser->lookahead->token, "more") == 0) ||
	(strcmp(parser->lookahead->token, "more_or_equal") == 0)) {
		struct AST *conditionNode = initASTNode();
		setStroka(conditionNode, parser->lookahead->token);
		swapChild(node, conditionNode);

		condition(node);
		group_expr(conditionNode);
		first_prior_expr2(node);
	}
}

void group_expr (struct AST* node) {
	term(node);
}

void condition(struct AST* node) {
	if (strcmp(parser->lookahead->token, "equal") == 0) {
		match("equal");
	} else if (strcmp(parser->lookahead->token, "not_equal") == 0) {
		match("not_equal");
	} else if (strcmp(parser->lookahead->token, "less") == 0) {
		match("less");
	} else if (strcmp(parser->lookahead->token, "less_or_equal") == 0) {
		match("less_or_equal");
	} else if (strcmp(parser->lookahead->token, "more") == 0) {
		match("more");
	} else if (strcmp(parser->lookahead->token, "more_or_equal") == 0) {
		match("more_or_equal");
	} else { 
		printf("error: %d:%d: expecting plus or sign condition, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void tsicl(struct AST* StartNode) {
	if (strcmp(parser->lookahead->token, "do") == 0) {
		match("do");
		tsicl1(StartNode);
	}
}

void tsicl1(struct AST* StartNode) {
	if (strcmp(parser->lookahead->token, "while") == 0) {
		struct AST *whileNode = initASTNode();
		setStroka(whileNode, "while");
		add_child(whileNode, StartNode);

		match("while");

		struct AST *exprNode = initASTNode();
		setStroka(exprNode, "expr");
		add_child(exprNode, whileNode);

		expression(exprNode);

		struct AST *stmNode = initASTNode();
		setStroka(stmNode, "bodyWhile");
		add_child(stmNode, whileNode);
		
		StList(stmNode);
		
		match("loop");
	} else if (strcmp(parser->lookahead->token, "until") == 0) {
		match("until");
		expression(StartNode);
		StList(StartNode);
		match("loop");
	} else {
		printf("error: %d:%d: expecting while or until, find %s\n", 
		parser->lookahead->row, parser->lookahead->column, parser->lookahead->token);
		exit(1);
	}
}

void func(struct AST* StartNode) {
	struct AST* FuncNode = initASTNode();
	setStroka(FuncNode, "func");
	add_child(FuncNode, StartNode);

	match("function");

	setToken(FuncNode, parser->lookahead);
	match("id");
	match("l_paren");
	struct AST *argListNode = initASTNode();
	setStroka(argListNode, "argList");
	add_child(argListNode, FuncNode);

	commaid(argListNode);
	match("r_paren");

	struct AST *bodyFuncNode = initASTNode();
	setStroka(bodyFuncNode, "bodyFunc");
	add_child(bodyFuncNode, FuncNode);

	StList(bodyFuncNode);
	match("end");
	match("function");
}

void match(char* x) {
	if(strcmp(parser->lookahead->token, x) == 0) {
		consume();
	} else {
		printErrorMessage(parser->lookahead->row, parser->lookahead->column, x);
		exit(1);
	}
}

void consume() {
	parser->lookahead = nextToken(); 
}

ListTokens* nextToken() {
	parser->count++;
	return parser->lookahead->next;
}

ListTokens* getLookahead()
{
	return parser->lookahead;
}

void printErrorMessage(int row, int column, char *x) {
	printf("%d:%d: error: expecting %s; find %s\n", row, column, x, parser->lookahead->token);
}