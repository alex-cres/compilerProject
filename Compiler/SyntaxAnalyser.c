#include <stdio.h>
#include <string.h>
#include "SyntaxAnalyser.h"
#include "LexicalAnalyser.h"

int instructionList(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <INSTRUCTIONLIST>\n");
/*
<instructionList> --> <instruction>
					| <instruction>;<instructionList>
*/
		nextToken = instruction(file, nextToken, nextChar, nextLexeme);
		while (nextToken == POINT_COMMA) {
			nextToken = lex(file, nextChar, nextLexeme);
			if (nextToken == EOF) {
				printf("Exiting <INSTRUCTIONLIST>\n");
				return nextToken;
			}
			nextToken=instructionList(file, nextToken, nextChar, nextLexeme);
		}
	
	printf("Exiting <INSTRUCTIONLIST>\n");
	return nextToken;
}
int instruction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <INSTRUCTION>\n");
/*
<instruction> -- > <declaration>
		| <attribution>
		| <if_struct> //missing
		| <looper> //missing
		| <comment> //missing
		| <for> //missing
		| <scan> //missing
		| <print> //missing
		| <function> //missing
		| <call_function> //missing
		| <return> //missing
		| <continue> //missing
		| <break> //missing
		| <fprint> //missing
		| <fscan> //missing
		| <fscanEnder> //missing
		| <fscanSize> //missing
		| EOF //missing
		*/
	
	switch (nextToken)
	{
	case RESERVED_IF:
		nextToken=reservedIf(file,nextToken,nextChar,nextLexeme);
		break;
		
	case IDENTIFIER: //attribution
		nextToken = attribution(file,nextToken,nextChar,nextLexeme);
		break;
	
	case IDN_BOOL: case IDN_NUMBER:	case IDN_DECIMAL: case IDN_CHAR: case IDN_VOID:	case IDN_STRING:
		nextToken=declaration(file, nextToken, nextChar, nextLexeme);
		break;
	default:syntaxError(nextLexeme); break;
	}
	printf("Exiting <INSTRUCTION>\n");
	return nextToken;
}
int reservedIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <IF>\n");

	/*

<if_struct> --> If(<bool>).Then(<instructionList>)
			  | If(<bool>).Then(<instructionList>).Else(<instructionList>)

	*/
	nextToken = lex(file, nextChar, nextLexeme);//gets next term
	if (nextToken == OPEN_PARENTESIS) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		nextToken = boolexp(file, nextToken, nextChar, nextLexeme);
		if (nextToken == CLOSE_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == POINT) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == RESERVED_THEN) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					if (nextToken == OPEN_PARENTESIS) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
						nextToken = instructionList(file, nextToken, nextChar, nextLexeme);
						if (nextToken == CLOSE_PARENTESIS) {
							nextToken = lex(file, nextChar, nextLexeme);//gets next term
							if (nextToken == POINT) {
								nextToken = lex(file, nextChar, nextLexeme);//gets next term
								if (nextToken == RESERVED_ELSE) {
									nextToken = lex(file, nextChar, nextLexeme);//gets next term
									if (nextToken == OPEN_PARENTESIS) {
										nextToken = lex(file, nextChar, nextLexeme);//gets next term
										nextToken = instructionList(file, nextToken, nextChar, nextLexeme);
										if (nextToken == CLOSE_PARENTESIS) {
											nextToken = lex(file, nextChar, nextLexeme);//gets next term
										}
										else {
											syntaxError(nextLexeme);
										}
									}
									else {
										syntaxError(nextLexeme);
									}
								}
								else {
									syntaxError(nextLexeme);
								}
							}
						}
					}
					else {
						syntaxError(nextLexeme);
					}
				}
				else {
					syntaxError(nextLexeme);
				}
			}
			else {
				syntaxError(nextLexeme);
			}
		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else {
		syntaxError(nextLexeme);
	}
	printf("Exiting <IF>\n");
	return nextToken;
}

int boolexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <BOOL>\n");
	/*
	<bool> --> <exp> < <exp>//missing
		| <exp> > <exp>//missing
		| <exp> >= <exp>//missing
		| <exp> <= <exp>//missing
		| <exp> = <exp>//missing
		| <exp> != <exp>//missing
		| !(<bool>)
		| (<bool>)
		| <bool> & <bool>//missing
		| <bool> | <bool>//missing
		| <bool> X <bool>//missing
		| True
		| False
		| <Bcasting>//missing
	*/
	
	if (nextToken == OP_NOT) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken = boolexp(file, nextToken, nextChar, nextLexeme);
			if (nextToken == CLOSE_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				printf("Exiting <BOOL>\n");
				return nextToken;
			}
		}
	}
	if (nextToken == OPEN_PARENTESIS) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		nextToken = boolexp(file, nextToken, nextChar, nextLexeme);
		if (nextToken == CLOSE_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			printf("Exiting <BOOL>\n");
			return nextToken;
		}
	}
	if (nextToken == RESERVED_BOOL_TRUE || nextToken == RESERVED_BOOL_FALSE) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		printf("Exiting <BOOL>\n");
		return nextToken;
	}
	

	printf("Exiting <BOOL>\n");
	return nextToken;
}
int declaration(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <ATTRIBUTION>\n");

	/*

<declaration> --> <type_identifier> <declareExpr>
				| <type_identifier> <functionDec>

	*/
	nextToken = lex(file, nextChar, nextLexeme);//gets next term
	if (nextToken == IDENTIFIER_FUNCTION) {
		nextToken = functionDec(file, nextToken, nextChar, nextLexeme);
	}
	else if (nextToken == IDENTIFIER) {
		nextToken = declareExp(file, nextToken, nextChar, nextLexeme);
	}

	printf("Exiting <ATTRIBUTION>\n");
	return nextToken;
}
int functionDec(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <FUNCTIONDEC>\n");

	/*
	<functionDec> -- > <function_name>(<params>).Do(<instructionList>)

	*/
	
	
	printf("Exiting <FUNCTIONDEC>\n");
	return nextToken;
}

int declareExp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <DECLAREEXP>\n");

	/*

		<declareExpr> --> <var>
						| <var> << <exp>
						| <declareExpr>,<declareExpr>
	*/
	do {
		if (nextToken == IDENTIFIER) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == OP_ATTRIBUTION) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				nextToken = exp(file, nextToken, nextChar, nextLexeme);//analyze term
			}
		}
		else {
			syntaxError(nextLexeme);
		}
	} while (nextToken==COMMA);
	printf("Exiting <DECLAREEXP>\n");
	return nextToken;
}
int attribution(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <ATTRIBUTION>\n");

	/*
		<attribution> --> <var> << <exp>
	*/
	if (nextToken == IDENTIFIER) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken==OP_ATTRIBUTION) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken = exp(file, nextToken, nextChar, nextLexeme);//analyze term
		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else {
		syntaxError(nextLexeme);
	}
	printf("Exiting <ATTRIBUTION>\n");
	return nextToken;
}

int exp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <EXP>\n");
/*
	<exp> --> <term> + <term>
			| <term> - <term>
			| <term>
*/
	nextToken = term(file,nextToken,nextChar,nextLexeme);//analyze term
	while(nextToken == OP_ADD || nextToken == OP_MINUS){//permits multiple + and - in the program
		nextToken=lex(file, nextChar, nextLexeme);//gets next term
		nextToken = term(file, nextToken, nextChar, nextLexeme);//analyze term
	}
	printf("Exiting <EXP>\n");
	return nextToken;

}
int term(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
/*
	<term> -- > <factor> * <factor>
			| <factor> / <factor>
			| <factor> % <factor>
			| <factor>
*/
	printf("Entering <TERM>\n");
	nextToken = factor(file, nextToken, nextChar, nextLexeme);//analyze term
	while (nextToken == OP_MUL || nextToken == OP_MOD || nextToken == OP_DIV) {//permits multiple * , %, / in the program
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		nextToken = factor(file, nextToken, nextChar, nextLexeme);//analyze term
	}
	printf("Exiting <TERM>\n");
	return nextToken;
}
int factor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme){
	/*
	<factor> -- > (<exp>)
			| <var>
			| <number_type>//missing
			| <decimal>
			| <string>
			| <char>
			| <call_function>//missing
	*/
	printf("Entering <FACTOR>\n");
	if (nextToken == IDENTIFIER || nextToken == LITERAL_CHAR || nextToken == LITERAL_STRING 
		|| nextToken == LITERAL_NUMBER || nextToken == LITERAL_DECIMAL) {

		nextToken = lex(file, nextChar, nextLexeme);//gets next term

	}
	else if (nextToken == OP_ADD || nextToken == OP_MINUS ) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken==LITERAL_DECIMAL|| nextToken == LITERAL_NUMBER) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else {
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken=exp(file, nextToken, nextChar, nextLexeme);
			if (nextToken == CLOSE_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
			}else{
				syntaxError(nextLexeme);
			}

		}
		else {
			syntaxError(nextLexeme);
		}
	}
	printf("Exiting <FACTOR>\n");
	return nextToken;
}

void syntaxError(char* nextLexeme) {
	errorColor();
	printf("ERROR:At Line %i : SYNTAX ERROR NEAR, %s", lineNumber, nextLexeme);
	normalColor();
	exit(ERROR_SYNTAX_ERROR_NEAR);
}

/*
	


			int name_of_non_terminal(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme){
				printf("Entering <name_of_non_terminal>\n");
				printf("Exiting <name_of_non_terminal>\n");
				return nextToken;
			}
*/