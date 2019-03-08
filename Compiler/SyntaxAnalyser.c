

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
		| <looper> 
		| <comment>
		| <for> 
		| <call_function>
		| <exit> 
		| <continue>
		| <continueif>
		| <break> 
		| <breakif>
		| <in>
		| <on>
		| EOF 
		*/
	
	switch (nextToken)
	{
	case IDN_BOOL: case IDN_NUMBER:	case IDN_DECIMAL: case IDN_CHAR: case IDN_VOID:	case IDN_STRING:
		nextToken = declaration(file, nextToken, nextChar, nextLexeme);
		break;
	case IDENTIFIER: //attribution
		nextToken = attribution(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_IF:
		nextToken=reservedIf(file,nextToken,nextChar,nextLexeme);
		break;

	case RESERVED_LOOP:
		nextToken = reservedLooper(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_COMMENT://Ignore Comments
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		break;
	case RESERVED_FOR:
		nextToken = reservedFor(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_CALL_FUNCTION:
		nextToken = reservedCallFunction(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_EXIT_FUNCTION:
		nextToken = reservedExit(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_CONTINUE:
		nextToken = reservedContinue(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_CONTINUEIF:
		nextToken = reservedContinueIf(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_BREAK:
		nextToken = reservedBreak(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_BREAKIF:
		nextToken = reservedBreakIf(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_IN:
		nextToken = reservedIn(file, nextToken, nextChar, nextLexeme);
		break;
	case RESERVED_ON:
		nextToken = reservedOn(file, nextToken, nextChar, nextLexeme);
		break;
	case EOF: break;
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
	if (nextToken==RESERVED_IF) {
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
	}
	printf("Exiting <IF>\n");
	return nextToken;
}
int reservedLooper(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	/*
	<looper> --> Loop(<exp>).Do(<instructionList>) 

	*/
	printf("Entering <LOOPER>\n");

	if (nextToken == RESERVED_LOOP){
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken = exp(file, nextToken, nextChar, nextLexeme);
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
							else
							{
								syntaxError(nextLexeme);
							}
						}
						else
						{
							syntaxError(nextLexeme);
						}
					}
					else
					{
						syntaxError(nextLexeme);
					}
				}
				else
				{
					syntaxError(nextLexeme);
				}
			}
			else
			{
				syntaxError(nextLexeme);
			}

		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else
	{
		syntaxError(nextLexeme);
	}

	printf("Exiting <LOOPER>\n");
	return nextToken;
	
}
int reservedFor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	/*
<for> --> For(<idn_number> <attribution>).Step(<exp>).If(<boolstruct>).Do(<instructionList>)  //missing

	*/
	printf("Entering <FOR>\n");
	if (nextToken == RESERVED_FOR)
	{
		nextToken = lex(file, nextChar, nextLexeme);//gets next term

		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == IDN_DECIMAL || nextToken == IDN_NUMBER) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				nextToken = attribution(file, nextToken, nextChar, nextLexeme);
				if (nextToken == CLOSE_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					if (nextToken == OPEN_PARENTESIS) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
						if (nextToken == RESERVED_STEP) {
							nextToken = lex(file, nextChar, nextLexeme);//gets next term
							nextToken = exp(file, nextToken, nextChar, nextLexeme);
							if (nextToken == CLOSE_PARENTESIS) {
								nextToken = lex(file, nextChar, nextLexeme);//gets next term
								if (nextToken == POINT) {
									nextToken = lex(file, nextChar, nextLexeme);//gets next term
									if (nextToken == RESERVED_IF) {
										nextToken = lex(file, nextChar, nextLexeme);//gets next term
										if (nextToken == OPEN_PARENTESIS) {
											nextToken = lex(file, nextChar, nextLexeme);//gets next term
											nextToken = boolstruct(file, nextToken, nextChar, nextLexeme);
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
				errorColor();
				printf("ERROR:At Line %i : SYNTAX ERROR TYPE SET ON FOR LOOP NOT SUPPORTED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_FOR_TYPE_NOT_SUPPORTED);
			}
		}
	}		

	printf("Exiting <FOR>\n");
	return nextToken;
}
int reservedExit(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	/*
	<exit> --> Exit(<exp>)
			| Exit() 
	*/
	printf("Entering <EXIT>\n");
	if (nextToken==RESERVED_EXIT_FUNCTION)
	{
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == CLOSE_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
			}
			else
			{
				nextToken = exp(file, nextToken, nextChar, nextLexeme);

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
	else
	{
		syntaxError(nextLexeme);
	}
	printf("Exiting <EXIT>\n");
	return nextToken;
}
int reservedContinue(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	/*
	<continue> --> Continue() 
	*/
	printf("Entering <CONTINUE>\n");


	if (nextToken == RESERVED_CONTINUE)
	{
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == CLOSE_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
			}
			else
			{
				syntaxError(nextLexeme);
			}

		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else
	{
		syntaxError(nextLexeme);
	}
	printf("Exiting <CONTINUE>\n");
	return nextToken;
}
int reservedContinueIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	/*
	<continueif> -->ContinueIf(<boolstruct>)
	*/
	printf("Entering <CONTINUEIF>\n");

	if (nextToken == RESERVED_CONTINUEIF)
	{
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken = boolstruct(file, nextToken, nextChar, nextLexeme);
			if (nextToken == CLOSE_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
			}
			else
			{
				syntaxError(nextLexeme);
			}

		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else
	{
		syntaxError(nextLexeme);
	}
	printf("Exiting <CONTINUEIF>\n");
	return nextToken;
}
int reservedBreakIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	/*
	<breakif> --> BreakIf(<boolstruct>) 
	*/
	printf("Entering <BREAKIF>\n");
	if (nextToken == RESERVED_BREAKIF)
	{
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken = boolstruct(file, nextToken, nextChar, nextLexeme);
			if (nextToken == CLOSE_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
			}
			else
			{
				syntaxError(nextLexeme);
			}

		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else
	{
		syntaxError(nextLexeme);
	}
	printf("Exiting <BREAKIF>\n");
	return nextToken;
}
int reservedBreak(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	/*
<break> --> Break()
	*/
	printf("Entering <BREAK>\n");
	if (nextToken == RESERVED_BREAK)
	{
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == CLOSE_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
			}
			else
			{
				syntaxError(nextLexeme);
			}

		}
		else {
			syntaxError(nextLexeme);
		}
	}
	else
	{
		syntaxError(nextLexeme);
	}
	printf("Exiting <BREAK>\n");
	return nextToken;
}
int reservedIn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	printf("Entering <IN>\n");

	/*
<in> --> <fscan> 
		| <scan> 
		| <fscanEnder> 
		| <fscanSize> 

<fscan> --> In.File(<exp>,<number>) 

<fscanEnder> --> In.FileEnder(<exp>) //missing

<fscanSize> --> In.FileSize(<exp>,<number>) 

<scan> --> In.Console() 
<Scasting> --> <in>.toString()  

<in> --> <in>.toDecimal()  

<in> --> <in>.toNumber()

<in> --> <in>.toChar()  

<in> --> <in>.toBool()  
	*/
	if (nextToken == RESERVED_IN) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == POINT) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			switch (nextToken) {
			case RESERVED_CONSOLE:
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					if (nextToken == CLOSE_PARENTESIS) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
					}
					else
					{
						syntaxError(nextLexeme);
					}
				}
				else
				{
					syntaxError(nextLexeme);
				}
				break;
			case RESERVED_FILE_ENDER:
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					nextToken = exp(file, nextToken, nextChar, nextLexeme);
					if (nextToken == CLOSE_PARENTESIS) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
					}
					else
					{
						syntaxError(nextLexeme);
					}
				}
				else
				{
					syntaxError(nextLexeme);
				}
				break;
			case RESERVED_FILE:case RESERVED_FILE_SIZE:
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					nextToken = exp(file, nextToken, nextChar, nextLexeme);
					if (nextToken == COMMA) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
						if (nextToken == LITERAL_NUMBER) {
							nextToken = lex(file, nextChar, nextLexeme);//gets next term

							if (nextToken == CLOSE_PARENTESIS) {
								nextToken = lex(file, nextChar, nextLexeme);//gets next term
							}
							else
							{
								syntaxError(nextLexeme);
							}
						}
						else
						{
							syntaxError(nextLexeme);
						}
					}
					else
					{
						syntaxError(nextLexeme);
					}
				}
				else
				{
					syntaxError(nextLexeme);
				}
				break;
			default: syntaxError(nextLexeme); break;
			}
			if (nextToken==POINT) { // casting
				printf("Entering <CASTIN>\n");

				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if(nextToken == RESERVED_CAST_CHAR || nextToken == RESERVED_CAST_DECIMAL
					|| nextToken == RESERVED_CAST_NUMBER || nextToken == RESERVED_CAST_STRING){
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					if (nextToken == OPEN_PARENTESIS ) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
						if (nextToken == CLOSE_PARENTESIS) {
							nextToken = lex(file, nextChar, nextLexeme);//gets next term
							printf("Exiting <CASTIN>\n");

						}
						else
						{
							syntaxError(nextLexeme);
						}
					}
					else
					{
						syntaxError(nextLexeme);
					}
				}
				else
				{
					syntaxError(nextLexeme);
				}
			}
		}
		else
		{
			syntaxError(nextLexeme);
		}
	}

	printf("Exiting <IN>\n");
	return nextToken;
}
int reservedOn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	printf("Entering <ON>\n");
	/*
<on> --> <fprint> 
			|<print> 
<fprint> --> On.File(<exp>,<exp>,<mode>,<exp>)  
<print> --> On.Console(<exp>) 
<mode> --> "W" | "A" | "w" | "a"  
	*/

	if (nextToken==RESERVED_ON) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken==POINT) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			switch(nextToken){
			case RESERVED_CONSOLE: 
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					nextToken = exp(file, nextToken, nextChar, nextLexeme);
					
					if (nextToken==CLOSE_PARENTESIS) {
						nextToken = lex(file, nextChar, nextLexeme);//gets next term
					}
					else {
						syntaxError(nextLexeme);
					}
				}
				else {
					syntaxError(nextLexeme);
				}
					
				
				break;
			case RESERVED_FILE: 
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					nextToken = exp(file, nextToken, nextChar, nextLexeme);
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
				break;
			default: syntaxError(nextLexeme); break;
			}
		}
		else
		{
			syntaxError(nextLexeme);
		}
	}
	printf("Exiting <ON>\n");
	return nextToken;
}
int params(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	printf("Entering <PARAMS>\n");
	/*
	<params> --> <type_identifier> <var>
			| <type_identifier> <var>, <params> 
			*/
		if (nextToken == IDN_BOOL || nextToken == IDN_NUMBER || nextToken == IDN_DECIMAL || nextToken == IDN_CHAR || nextToken == IDN_STRING) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken==IDENTIFIER) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				if (nextToken == COMMA) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term	
					nextToken = params(file, nextToken, nextChar, nextLexeme);
				}
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

		printf("Exiting <PARAMS>\n");
	return nextToken;
}
int boolstruct(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <BOOLSTRUCT>\n");
	/*
	<boolstruct> -->  !(<boolstruct>)
		| (<boolstruct>)
		| <boolexp>
		| <boolexp> & <boolexp>
		| <boolexp> | <boolexp>
		| <boolexp> X <boolexp>
		| True
		| False
	*/
	switch (nextToken) {
	case OP_NOT:	
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
	case OPEN_PARENTESIS:
			if (nextToken == OPEN_PARENTESIS) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				nextToken = boolstruct(file, nextToken, nextChar, nextLexeme);
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
		nextToken = boolexp(file, nextToken, nextChar, nextLexeme);
		if (nextToken == OP_AND || nextToken == OP_OR || nextToken == OP_XOR) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken = boolexp(file, nextToken, nextChar, nextLexeme);
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
		| <exp>

		
		
	*/
	
			nextToken = exp(file, nextToken, nextChar, nextLexeme);
			if (nextToken == OP_EQUAL || nextToken == OP_MINOR_EQUAL || nextToken == OP_MINOR || nextToken == OP_BIGGER || nextToken == OP_BIGGER_EQUAL || nextToken == OP_NOT_EQUAL)
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
				| <type_identifier><array_def> <declareExpr>
				| <type_identifier><array_def> <functionDec>

	*/
	nextToken = lex(file, nextChar, nextLexeme);//gets next term
	switch (nextToken) {
	case OPEN_BRACKETS:
		nextToken = reservedArray(file, nextToken, nextChar, nextLexeme);
		
		if (nextToken== IDENTIFIER_FUNCTION) {
			nextToken = functionDec(file, nextToken, nextChar, nextLexeme);
			break;
		}
		else if (nextToken == IDENTIFIER) {
			nextToken = declareExp(file, nextToken, nextChar, nextLexeme);
			break;
		}
		else {
			syntaxError(nextLexeme);
		}
		
	case IDENTIFIER_FUNCTION:
		nextToken = functionDec(file, nextToken, nextChar, nextLexeme);
		break;

	case IDENTIFIER:
		nextToken = declareExp(file, nextToken, nextChar, nextLexeme);
		break;
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
int declareExp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <DECLAREEXP>\n");

	/*

		<declareExpr> --> <var>
						| <var> << <exp>
						| <declareExpr>,<declareExpr>
	*/
	
		if (nextToken == IDENTIFIER) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			if (nextToken == OP_ATTRIBUTION) {
				nextToken = lex(file, nextChar, nextLexeme);//gets next term
				nextToken = exp(file, nextToken, nextChar, nextLexeme);//analyze term
				if (nextToken==COMMA) {
					nextToken = lex(file, nextChar, nextLexeme);//gets next term
					nextToken=declareExp(file, nextToken, nextChar, nextLexeme);//analyze term
				}
			}
		}
		else {
			syntaxError(nextLexeme);
		}
	
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
int reservedCallFunction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
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
int params_call(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	printf("Entering <PARAMSCALL>\n");
	/*
<params_call> -->  <exp>
			| <exp>, <params_call>
	*/
	nextToken = exp(file, nextToken, nextChar, nextLexeme);
	if (nextToken==COMMA) {
		nextToken = lex(file, nextChar, nextLexeme);
		nextToken = params_call(file, nextToken, nextChar, nextLexeme);
	}
	
	printf("Exiting <PARAMSCALL>\n");
	return nextToken;;
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
			| <in>
			| <boolstruct>
			| <call_function>
			| <casting>
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
	else if (
		nextToken == RESERVED_CAST_NUMBER ||
		nextToken == RESERVED_CAST_CHAR ||
		nextToken == RESERVED_CAST_BOOL ||
		nextToken == RESERVED_CAST_STRING ||
		nextToken == RESERVED_CAST_DECIMAL) {
		nextToken = reservedCastingVar(file, nextToken, nextChar, nextLexeme);
	}else if (nextToken==RESERVED_CALL_FUNCTION) {
		nextToken = reservedCallFunction(file, nextToken, nextChar, nextLexeme);
	}
	else if (nextToken == RESERVED_IN) {
		nextToken = reservedIn(file, nextToken, nextChar, nextLexeme);
	}
	else if (nextToken == OPEN_PARENTESIS) {
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
	
	printf("Exiting <FACTOR>\n");
	return nextToken;
}
int reservedCastingVar(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme)
{
	printf("Entering <CASTINGVAR>\n");
	/*
<casting> --> toString(<exp>)  
<casting> --> toDecimal(<exp>) 
<casting> --> toNumber(<exp>)  
<casting> --> toChar(<exp>) 
<casting> --> toBool(<exp>) 	
	*/
	if (
		nextToken == RESERVED_CAST_NUMBER || nextToken == RESERVED_CAST_CHAR ||
		nextToken == RESERVED_CAST_BOOL || nextToken == RESERVED_CAST_STRING ||
		nextToken == RESERVED_CAST_DECIMAL) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
			nextToken = exp(file, nextToken, nextChar, nextLexeme);
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
	printf("Exiting <CASTINGVAR>\n");
	return nextToken;
}
int reservedArray(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme) {
	printf("Entering <ARRAYDEF>\n");
	/*
<array_def> --> [<exp>]  
	*/
	if (nextToken==OPEN_BRACKETS) {
		nextToken = lex(file, nextChar, nextLexeme);//gets next term
		nextToken = exp(file, nextToken, nextChar, nextLexeme);
		if (nextToken==CLOSE_BRACKETS) {
			nextToken = lex(file, nextChar, nextLexeme);//gets next term
		}
		else {
			syntaxError(nextLexeme);
		}
	}
	printf("Exiting <ARRAYDEF>\n");
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