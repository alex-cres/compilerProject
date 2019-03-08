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
		| <if_struct>
		| <looper> //missing
		| <comment>
		| <for> //missing
		| <scan> //missing
		| <print> //missing
		| <function> 
		| <call_function>
		| <return> //missing
		| <continue> //missing
		| <break> //missing
		| <fprint> //missing
		| <fscan> //missing
		| <fscanEnder> //missing
		| <fscanSize> //missing
		| EOF 
		*/
	
	switch (nextToken)
	{
	case RESERVED_IF:
		nextToken=reservedIf(file,nextToken,nextChar,nextLexeme);
		break;
	case RESERVED_COMMENT:
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		break;
	case RESERVED_CALL_FUNCTION:
		nextToken = callFunction(file, nextToken, nextChar, nextLexeme);
		break;
	case IDENTIFIER: //attribution
		nextToken = attribution(file,nextToken,nextChar,nextLexeme);
		break;
	
	case IDN_BOOL: case IDN_NUMBER:	case IDN_DECIMAL: case IDN_CHAR: case IDN_VOID:	case IDN_STRING:
		nextToken=declaration(file, nextToken, nextChar, nextLexeme);
		break;
	case EOF: break;
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
		nextToken = boolstruct(file, nextToken, nextChar, nextLexeme);
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

int params(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	printf("Entering <PARAMS>\n");
	/*
	<params> --> <type_identifier> <var>
			| <type_identifier> <var>, <params> 
			*/
	do {
		if (nextToken == IDN_BOOL || nextToken == IDN_NUMBER || nextToken == IDN_DECIMAL || nextToken == IDN_CHAR || nextToken == IDN_STRING) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken==IDENTIFIER) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
			}
			else {
				syntaxError(nextLexeme);
			}
		}
		else if (nextToken == IDN_VOID) {
			errorColor();
			printf("ERROR:At Line %i : SYNTAX ERROR VOID PARAMS ARE FORBIDDEN, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_VOID_PARAM);
		}
		else {
			syntaxError(nextLexeme);
		}
	} while (nextToken==COMMA) ;
		printf("Exiting <PARAMS>\n");
	return nextToken;
}

int boolstruct(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <BOOLSTRUCT>\n");
	/*
	<boolstruct> -->  !(<boolstruct>)
		| (<boolstruct>)
		| <boolexp>
		| <boolstruct> & <boolstruct>
		| <boolstruct> | <boolstruct>
		| <boolstruct> X <boolstruct>
		| True
		| False
		| <Bcasting>//missing
	*/
	switch (nextToken) {
	case OP_NOT:	
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
	case OPEN_PARENTESIS:
			if (nextToken == OPEN_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				nextToken = boolexp(file, nextToken, nextChar, nextLexeme);
				if (nextToken == CLOSE_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					printf("Exiting <BOOLSTRUCT>\n");
					return nextToken;
				}
				else {
					syntaxError(nextLexeme);
				}
			}
			else {
				syntaxError(nextLexeme);
			}
			break;
	case RESERVED_BOOL_TRUE: case RESERVED_BOOL_FALSE:
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			printf("Exiting <BOOLSTRUCT>\n");
			return nextToken;
		break;
	default:
		nextToken = boolstruct(file, nextToken, nextChar, nextLexeme);
		if (nextToken == OP_AND || nextToken == OP_OR || nextToken == OP_XOR) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken = boolstruct(file, nextToken, nextChar, nextLexeme);
		}
		printf("Exiting <BOOLSTRUCT>\n");
		return nextToken;
		break;
	}
	printf("Exiting <BOOLSTRUCT>\n");
	return nextToken;
}
int boolexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <BOOLEXP>\n");
	/*
	<boolexp> --> <exp> < <exp>
		| <exp> > <exp>
		| <exp> >= <exp>
		| <exp> <= <exp>
		| <exp> = <exp>
		| <exp> != <exp>
		
	*/
	
	
	nextToken = exp(file, nextToken, nextChar, nextLexeme);
	if (nextToken==OP_EQUAL|| nextToken == OP_MINOR_EQUAL || nextToken == OP_MINOR || nextToken == OP_BIGGER || nextToken == OP_BIGGER_EQUAL || nextToken == OP_NOT_EQUAL)
	{
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		nextToken = exp(file, nextToken, nextChar, nextLexeme);
	}

	printf("Exiting <BOOLEXP>\n");
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
	<functionDec> -- > <function_name>().Do(<instructionList>)

	*/
	if (nextToken==IDENTIFIER_FUNCTION) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == CLOSE_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == POINT) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					if (nextToken == RESERVED_DO) {
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
				else {
					syntaxError(nextLexeme);
				}
			}
			else {
				nextToken = params(file, nextToken, nextChar, nextLexeme);
				if (nextToken == CLOSE_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					if (nextToken == POINT) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
						if (nextToken == RESERVED_DO) {
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
					else {
						syntaxError(nextLexeme);
					}
				}
				else {
					syntaxError(nextLexeme);
				}
			}
		}
		else {
		syntaxError(nextLexeme);
	}
		
	}
	
	
	printf("Exiting <FUNCTIONDEC>\n");
	return nextToken;
}

int params_call(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	printf("Entering <PARAMSCALL>\n");
	/*
<params_call> -->  <exp>
			| <exp>, <params_call>
	*/
	nextToken = exp(file, nextToken, nextChar, nextLexeme);
	while (nextToken == COMMA) {
		nextToken = exp(file, nextToken, nextChar, nextLexeme);
		nextToken = lex(file, nextChar, nextLexeme);
	}
	printf("Exiting <PARAMSCALL>\n");
	return nextToken;;
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
int callFunction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <CALLFUNCTION>\n");
	/*
<call_function> --> F.<function_name>(<params_call>)
				| F.<function_name>()
	*/
	if (nextToken == RESERVED_CALL_FUNCTION) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == POINT) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == IDENTIFIER_FUNCTION) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					if (nextToken == CLOSE_PARENTESIS) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
					}
					else {
						nextToken = params_call(file, nextToken, nextChar, nextLexeme);
						if (nextToken == CLOSE_PARENTESIS) {
							nextToken = lex(file, nextChar, nextLexeme);//gets next term
						}
						else {
							syntaxError(nextLexeme);
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

	printf("Exiting <CALLFUNCTION>\n");
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
			| +<number>
			| -<number>
			| +<decimal>
			| -<decimal>
			| <number>
			| <decimal>
			| <string>
			| <char>
			| <boolstruct>
			| <call_function>
	*/
	printf("Entering <FACTOR>\n");
	if (nextToken == IDENTIFIER || nextToken == LITERAL_CHAR || nextToken == LITERAL_STRING 
		|| nextToken == LITERAL_NUMBER || nextToken == LITERAL_DECIMAL) {

		nextToken = lex(file, nextChar, nextLexeme);//gets next term

	}
	else if (nextToken == OP_ADD || nextToken == OP_MINUS ) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken==LITERAL_DECIMAL || nextToken == LITERAL_NUMBER) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else if (nextToken==RESERVED_CALL_FUNCTION) {
		nextToken = callFunction(file, nextToken, nextChar, nextLexeme);
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
			nextToken = boolstruct(file, nextToken, nextChar, nextLexeme);
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