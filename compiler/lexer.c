#include "lexer.h"

char *DefineToken(char *lexeme) {
    if(strcmp(lexeme, "INPUT") == 0)
        return "input";
    if(strcmp(lexeme, "PRINT") == 0)
        return "print";
    if(strcmp(lexeme, "+") == 0)
        return "plus";
    if(strcmp(lexeme, "-") == 0)
        return "minus";
    if(strcmp(lexeme, "*") == 0)
        return "multiply";
    if(strcmp(lexeme, "/") == 0)
        return "divide";
    if(strcmp(lexeme, "\\") == 0)
        return "div";
    if(strcmp(lexeme, "^") == 0)
        return "pow";
    if(strcmp(lexeme, "MOD") == 0)
        return "mod";
    if(strcmp(lexeme, "=") == 0)
        return "equal";
    if(strcmp(lexeme, "<>") == 0)
        return "not_equal";
    if(strcmp(lexeme, ">") == 0)
        return "more";
    if(strcmp(lexeme, "<") == 0)
        return "less";
    if(strcmp(lexeme, ">=") == 0)
        return "more_or_equal";
    if(strcmp(lexeme, "<=") == 0)
        return "less_or_equal";
    if(strcmp(lexeme, "IF") == 0)
        return "if";
    if(strcmp(lexeme, "THEN") == 0)
        return "then";
    if(strcmp(lexeme, "END") == 0)
       return "end";
    if(strcmp(lexeme, "ELSE") == 0)
        return "else";
    if(strcmp(lexeme, "DO") == 0)
        return "do";
    if(strcmp(lexeme, "WHILE") == 0)
        return "while";
    if(strcmp(lexeme, "LOOP") == 0)
        return "loop";
    if(strcmp(lexeme, "UNTIL") == 0)
        return "until";
    if(strcmp(lexeme, "DIM") == 0)
        return "dim";
    if(strcmp(lexeme, "AS") == 0)
        return "as";
    if(strcmp(lexeme, "INTEGER") == 0)
        return "integer";
    if(strcmp(lexeme, "STRING") == 0)
        return "string";
    if(strcmp(lexeme, "TO") == 0)
        return "to";
    if(strcmp(lexeme, "FUNCTION") == 0)
        return "function";
    if(strcmp(lexeme, "(") == 0)
        return "l_paren";
    if(strcmp(lexeme, ")") == 0)
        return "r_paren";
    if(strcmp(lexeme, ",") == 0) {
        return "comma";
	}
    if(strcmp(lexeme, ";") == 0) {
		return "semi";
	}
	if(lexeme[0] == '"') {
		if (lexeme[strlen(lexeme)-1] == '"')
			return "literal";
		else
			return "unknown";
	}

	if (lexeme[0] >= '0' && lexeme[0] <= '9') {
		for (int i = 1; i < strlen(lexeme); i++) {
			if (!(lexeme[i] >= '0' && lexeme[i] <= '9'))
				return "unknown";	
		} 
		return "numeric";
	}

	if ((lexeme[0] >= 'A' && lexeme[0] <= 'Z') ||
		(lexeme[0] >= 'a' && lexeme[0] <= 'z') || (lexeme[0] == '_')) {
	
		for (int i = 1; i < strlen(lexeme); i++) {
			if (!((lexeme[i] >= 'A' && lexeme[i] <= 'Z') ||
				(lexeme[i] >= 'a' && lexeme[i] <= 'z') || (lexeme[i] == '_') ||
				(lexeme[i] >= '0' && lexeme[i] <= '9')))
				return "unknown";
		}
		return "id";
	}
	return "unknown";
}

ListTokens* IdentifyTokens(int argc, char const *argv) {
	char buf[100];
	if (argc > 1) {
		FILE *file;

		file = fopen (argv, "r");

		if (file == NULL) {
			printf("error\n");
			return NULL;
		}

		ListTokens *List = init();
		ListTokens *head = List;

		char sym;
		int i = 0;
		int row = 1, column = 1;
		char *token = (char *) malloc(sizeof(char) * 80);

		while ((sym = fgetc(file)) != EOF) {
			//литералы
			if (sym == '"') {
				buf[i] = sym;
				i++;
				column++;
				while ((sym = fgetc(file)) != '"' && sym != '\n') {
					buf[i] = sym;
					i++;
					column++;
				}
				if (sym == '"') {
					buf[i] = sym;
					i++;
					column++;
				}
				buf[i] = '\0';

				token = DefineToken(buf);
				List = addlexeme(List, token, buf, row, column-i);

				i = 0;
				continue;
			}
			//игнор пробелов
			else if (sym == ' ') {
				if (i == 0) {
					column++;
					continue;
				}

				buf[i] = '\0';
				//игнор комментов
				if (strcmp(buf, "REM") == 0) {
					while ((sym = fgetc(file)) != '\n')
						continue;
					i = 0;
					row++;
					column = 1;
					continue;
				}

				token = DefineToken(buf);
				List = addlexeme(List, token, buf, row, column-i);

				i = 0;
				column++;
				continue;
			}
			//новая строка
			else if (sym == '\n') {

				if (i == 0) {
					row++;
					continue;
				}

				buf[i] = '\0';
				//игнор комментов
				if (strcmp(buf, "REM") == 0) {
					while ((sym = fgetc(file)) != '\n')
						continue;
					i = 0;
					continue;
				}

				token = DefineToken(buf);
				List = addlexeme(List, token, buf, row, column-i);

				column = 1;
				row++;
				i = 0;
				continue;
			}
			//знаки в один символ
			else if (sym == ',' || sym == ';' || sym == '+' || sym == '-' ||
				sym == '=' || sym == '*' || sym == '/' || sym == '\\' || sym == '^' ||
				sym == '(' || sym == ')') {
				
				if (i != 0) {
					buf[i] = '\0';

					token = DefineToken(buf);
					List = addlexeme(List, token, buf, row, column-i);

					i = 0;
				}

				buf[i] = sym;
				i++;
				column++;
				buf[i] = '\0';

				token = DefineToken(buf);
				List = addlexeme(List, token, buf, row, column-i);

				i = 0;
				continue;
			}

			else if (sym == '>') {
				if (i != 0) {
					buf[i] = '\0';

					token = DefineToken(buf);
					List = addlexeme(List, token, buf, row, column-i);

					i = 0;
				}

				buf[i] = sym;
				i++;
				column++;

				if ((sym = fgetc(file)) == '=') {
					buf[i] = sym;
					i++;
					column++;
					buf[i] = '\0';

					token = DefineToken(buf);
					List = addlexeme(List, token, buf, row, column-i);

					i = 0;
					continue;
				} else {
					buf[i] = '\0';

					token = DefineToken(buf);
					List = addlexeme(List, token, buf, row, column-i);

					i = 0;

					if (sym == ' ') {
						column++;
						continue;
					}
				}
			}

			else if (sym == '<') {
				if (i != 0) {
					buf[i] = '\0';

					token = DefineToken(buf);
					List = addlexeme(List, token, buf, row, column-i);

					i = 0;
				}
				buf[i] = sym;
				i++;
				column++;

				sym = fgetc(file);

				if ((sym == '=') || (sym == '>')) {
					buf[i] = sym;
					i++;
					column++;
					buf[i] = '\0';

					token = DefineToken(buf);
					List = addlexeme(List, token, buf, row, column-i);

					i = 0;
					continue;
				} else {
					buf[i] = '\0';

					token = DefineToken(buf);
					List = addlexeme(List, token, buf, row, column-i);

					i = 0;

					if (sym == ' ') {
						column++;
						continue;
					}
				}
			}

			buf[i] = sym;
			i++;
			column++;
		}
		if (i != 0) {
			buf[i] = '\0';

			token = DefineToken(buf);
			List = addlexeme(List, token, buf, row, column-i);

			i = 0;
		}
		List = addlexeme(List, "eof", "", row, column-i);
		
		fclose (file);
	
		return head;
	}
	return NULL;
}