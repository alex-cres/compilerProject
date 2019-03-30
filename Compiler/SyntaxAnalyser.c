#include <stdio.h>
#include <string.h>
#include "SyntaxAnalyser.h"
#include "LexicalAnalyser.h"
#include "SyntaxTree.h"





int reservedArrayexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	printf("Entering <ARRAYEXP>\n");
	fprintf(logFile, "Entering <ARRAYEXP>\n");
	Node* reservedArrayexpNode = addChildNode(tree, "ARRAYEXP", -1, logFile);
	addChildNode(reservedArrayexpNode, nextLexeme, nextToken, logFile);

	do
	{
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == CLOSE_BRACKETS) {
			addChildNode(reservedArrayexpNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			printf("Exiting <ARRAYEXP>\n");
			fprintf(logFile, "Exiting <ARRAYEXP>\n");
			return nextToken;
		}
		nextToken = exp(file, nextToken, nextChar, nextLexeme, reservedArrayexpNode, logFile);
	} while (nextToken == COMMA);
	if (nextToken == CLOSE_BRACKETS) {
		addChildNode(reservedArrayexpNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		printf("Exiting <ARRAYEXP>\n");
		fprintf(logFile, "Exiting <ARRAYEXP>\n");
		return nextToken;
	}
	else {
		errorColor();
		printf("ERROR: At Line %i : SYNTAX ERROR MISSING CLOSING BRACKETS, %s", lineNumber, nextLexeme);
		fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR MISSING  CLOSING BRACKETS, %s", lineNumber, nextLexeme);
		normalColor();
		exit(ERROR_SYNTAX_ERROR_MISSING_CLOSING_BRACKETS);
	}



	printf("Exiting <ARRAYEXP>\n");
	fprintf(logFile, "Exiting <ARRAYEXP>\n");
	return nextToken;
}

int instructionList(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <INSTRUCTIONLIST>\n");
	fprintf(logFile, "Entering <INSTRUCTIONLIST>\n");
	Node* instructionListNode = addChildNode(tree, "INSTRUCTIONLIST", -1, logFile);

	/*
	<instructionList> --> <instruction>
						| <instruction>;<instructionList>
	*/
	nextToken = instruction(file, nextToken, nextChar, nextLexeme, instructionListNode, logFile);
	if (nextToken == POINT_COMMA) {
		addChildNode(instructionListNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);
		if (nextToken == EOF) {
			addChildNode(instructionListNode, nextLexeme, nextToken, logFile);
			printf("Exiting <INSTRUCTIONLIST>\n");
			fprintf(logFile, "Exiting <INSTRUCTIONLIST>\n");
			return nextToken;
		}
		else if (nextToken == CLOSE_PARENTESIS) {
			printf("Exiting <INSTRUCTIONLIST>\n");
			fprintf(logFile, "Exiting <INSTRUCTIONLIST>\n");
			return nextToken;
		}

		nextToken = instructionList(file, nextToken, nextChar, nextLexeme, instructionListNode, logFile);
	}
	else if (nextToken != POINT_COMMA) {
		errorColor();
		printf("ERROR: At Line %i : SYNTAX ERROR MISSING POINT COMMA, %s", lineNumber, nextLexeme);
		fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR MISSING POINT COMMA, %s", lineNumber, nextLexeme);
		normalColor();
		exit(ERROR_SYNTAX_ERROR_MISSING_POINT_COMMA);
	}
	else if (nextToken == EOF) {
		addChildNode(instructionListNode, nextLexeme, nextToken, logFile);
		printf("Exiting <INSTRUCTIONLIST>\n");
		fprintf(logFile, "Exiting <INSTRUCTIONLIST>\n");
		return nextToken;
	}

	printf("Exiting <INSTRUCTIONLIST>\n");
	fprintf(logFile, "Exiting <INSTRUCTIONLIST>\n");
	return nextToken;
}
int instruction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <INSTRUCTION>\n");
	fprintf(logFile, "Entering <INSTRUCTION>\n");
	Node* instructionNode = addChildNode(tree, "INSTRUCTION", -1, logFile);
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
		nextToken = declaration(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case IDENTIFIER: //attribution
		nextToken = attribution(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_IF:
		nextToken = reservedIf(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;

	case RESERVED_LOOP:
		nextToken = reservedLooper(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_COMMENT://Ignore Comments
		addChildNode(instructionNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		break;
	case RESERVED_FOR:
		nextToken = reservedFor(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_CALL_FUNCTION:
		nextToken = reservedCallFunction(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_EXIT_FUNCTION:
		nextToken = reservedExit(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_CONTINUE:
		nextToken = reservedContinue(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_CONTINUEIF:
		nextToken = reservedContinueIf(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_BREAK:
		nextToken = reservedBreak(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_BREAKIF:
		nextToken = reservedBreakIf(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_IN:
		nextToken = reservedIn(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case RESERVED_ON:
		nextToken = reservedOn(file, nextToken, nextChar, nextLexeme, instructionNode, logFile);
		break;
	case EOF: break;
	}
	printf("Exiting <INSTRUCTION>\n");
	fprintf(logFile, "Exiting <INSTRUCTION>\n");
	return nextToken;
}
int reservedIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <IF>\n");
	fprintf(logFile, "Entering <IF>\n");
	Node* ifNode = addChildNode(tree, "IF", -1, logFile);
	/*

<if_struct> --> If(<bool>).Then(<instructionList>)
			  | If(<bool>).Then(<instructionList>).Else(<instructionList>)

	*/
	if (nextToken == RESERVED_IF) {
		addChildNode(ifNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(ifNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			nextToken = boolstruct(file, nextToken, nextChar, nextLexeme, ifNode, logFile);
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(ifNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == POINT) {
					addChildNode(ifNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					if (nextToken == RESERVED_THEN) {

						Node * thenNode = addChildNode(ifNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						if (nextToken == OPEN_PARENTESIS) {
							addChildNode(thenNode, nextLexeme, nextToken, logFile);
							nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
							nextToken = instructionList(file, nextToken, nextChar, nextLexeme, thenNode, logFile);
							if (nextToken == CLOSE_PARENTESIS) {
								addChildNode(thenNode, nextLexeme, nextToken, logFile);
								nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
								if (nextToken == POINT) {
									addChildNode(ifNode, nextLexeme, nextToken, logFile);
									nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
									if (nextToken == RESERVED_ELSE) {
										Node * elseNode = addChildNode(ifNode, nextLexeme, nextToken, logFile);

										nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
										if (nextToken == OPEN_PARENTESIS) {
											addChildNode(elseNode, nextLexeme, nextToken, logFile);
											nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
											nextToken = instructionList(file, nextToken, nextChar, nextLexeme, elseNode, logFile);
											if (nextToken == CLOSE_PARENTESIS) {
												addChildNode(elseNode, nextLexeme, nextToken, logFile);
												nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
											}
											else {
												errorColor();
												printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
												fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
												normalColor();
												exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
											}
										}
										else {
											errorColor();
											printf("ERROR: At Line %i : SYNTAX ERROR ARRAY NOT OPENED, %s", lineNumber, nextLexeme);
											fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR ARRAY NOT OPENED, %s", lineNumber, nextLexeme);
											normalColor();
											exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
										}
									}
									else {
										errorColor();
										printf("ERROR: At Line %i : SYNTAX ERROR ELSE NOT FOUND, %s", lineNumber, nextLexeme);
										fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR ELSE NOT FOUND, %s", lineNumber, nextLexeme);
										normalColor();
										exit(ERROR_SYNTAX_ERROR_ELSE_NOT_FOUND);
									}
								}
							}
						}
						else {
							errorColor();
							printf("ERROR: At Line %i : SYNTAX ERROR THEN NOT OPENED, %s", lineNumber, nextLexeme);
							fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR THEN NOT OPENED, %s", lineNumber, nextLexeme);
							normalColor();
							exit(ERROR_SYNTAX_ERROR_THEN_NOT_OPENED);
						}
					}
					else {
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR THEN NOT FOUND, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR THEN NOT FOUND, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_THEN_NOT_FOUND);
					}
				}
				else {
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
				}
			}
			else {
				errorColor();
				printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
			}
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}
	}
	printf("Exiting <IF>\n");
	fprintf(logFile, "Exiting <IF>\n");
	return nextToken;
}
int reservedLooper(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	/*
	<looper> --> Loop(<exp>).Do(<instructionList>)

	*/
	printf("Entering <LOOPER>\n");
	fprintf(logFile, "Entering <LOOPER>\n");
	Node* looperNode = addChildNode(tree, "LOOPER", -1, logFile);

	if (nextToken == RESERVED_LOOP) {
		addChildNode(looperNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(looperNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			nextToken = exp(file, nextToken, nextChar, nextLexeme, looperNode, logFile);
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(looperNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == POINT) {
					addChildNode(looperNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					if (nextToken == RESERVED_DO) {
						Node * doNode = addChildNode(looperNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						if (nextToken == OPEN_PARENTESIS) {
							addChildNode(doNode, nextLexeme, nextToken, logFile);
							nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
							nextToken = instructionList(file, nextToken, nextChar, nextLexeme, doNode, logFile);
							if (nextToken == CLOSE_PARENTESIS) {
								addChildNode(doNode, nextLexeme, nextToken, logFile);
								nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
							}
							else
							{
								errorColor();
								printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
								fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
								normalColor();
								exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
							}
						}
						else
						{
							errorColor();
							printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
							fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
							normalColor();
							exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
						}
					}
					else
					{
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR DO NOT FOUND, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR DO NOT FOUND, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_DO_NOT_FOUND);
					}
				}
				else
				{
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
				}
			}
			else
			{
				errorColor();
				printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
			}

		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}
	}

	printf("Exiting <LOOPER>\n");
	fprintf(logFile, "Exiting <LOOPER>\n");
	return nextToken;

}
int reservedFor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	/*
<for> --> For(<idn_number> <attribution>).Step(<exp>).If(<boolstruct>).Do(<instructionList>)  //missing

	*/
	printf("Entering <FOR>\n");
	fprintf(logFile, "Entering <FOR>\n");
	Node* forNode = addChildNode(tree, "FOR", -1, logFile);

	if (nextToken == RESERVED_FOR)
	{
		addChildNode(forNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term

		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(forNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			if (nextToken == IDN_DECIMAL || nextToken == IDN_NUMBER) {
				addChildNode(forNode, nextLexeme, nextToken, logFile);

				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				nextToken = attribution(file, nextToken, nextChar, nextLexeme, forNode, logFile);
				if (nextToken == CLOSE_PARENTESIS) {
					addChildNode(forNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					if (nextToken == POINT) {
						addChildNode(forNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						if (nextToken == RESERVED_STEP) {
							addChildNode(forNode, nextLexeme, nextToken, logFile);
							nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
							if (nextToken == OPEN_PARENTESIS) {
								addChildNode(forNode, nextLexeme, nextToken, logFile);
								nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
								nextToken = exp(file, nextToken, nextChar, nextLexeme, forNode, logFile);
								if (nextToken == CLOSE_PARENTESIS) {
									addChildNode(forNode, nextLexeme, nextToken, logFile);
									nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
									if (nextToken == POINT) {
										addChildNode(forNode, nextLexeme, nextToken, logFile);
										nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
										if (nextToken == RESERVED_IF) {
											addChildNode(forNode, nextLexeme, nextToken, logFile);
											nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
											if (nextToken == OPEN_PARENTESIS) {
												addChildNode(forNode, nextLexeme, nextToken, logFile);
												nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
												nextToken = boolstruct(file, nextToken, nextChar, nextLexeme, forNode, logFile);
												if (nextToken == CLOSE_PARENTESIS) {
													addChildNode(forNode, nextLexeme, nextToken, logFile);
													nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
													if (nextToken == POINT) {
														addChildNode(forNode, nextLexeme, nextToken, logFile);
														nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
														if (nextToken == RESERVED_DO) {
															Node * doNode = addChildNode(forNode, nextLexeme, nextToken, logFile);
															nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
															if (nextToken == OPEN_PARENTESIS) {
																addChildNode(doNode, nextLexeme, nextToken, logFile);
																nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
																nextToken = instructionList(file, nextToken, nextChar, nextLexeme, doNode, logFile);
																if (nextToken == CLOSE_PARENTESIS) {
																	addChildNode(doNode, nextLexeme, nextToken, logFile);
																	nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
																}
																else {
																	errorColor();
																	printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
																	fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
																	normalColor();
																	exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
																}
															}
															else {
																errorColor();
																printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
																fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
																normalColor();
																exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
															}
														}
													}
													else {
														errorColor();
														printf("ERROR: At Line %i : SYNTAX ERROR DO NOT FOUND, %s", lineNumber, nextLexeme);
														fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR DO NOT FOUND, %s", lineNumber, nextLexeme);
														normalColor();
														exit(ERROR_SYNTAX_ERROR_DO_NOT_FOUND);
													}
												}
												else {
													errorColor();
													printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
													fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
													normalColor();
													exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
												}
											}
											else {
												errorColor();
												printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
												fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
												normalColor();
												exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
											}
										}
										else {
											errorColor();
											printf("ERROR: At Line %i : SYNTAX ERROR IF NOT FOUND, %s", lineNumber, nextLexeme);
											fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR IF NOT FOUND, %s", lineNumber, nextLexeme);
											normalColor();
											exit(ERROR_SYNTAX_ERROR_IF_NOT_FOUND);
										}
									}
									else {
										errorColor();
										printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT CLOSED, %s", lineNumber, nextLexeme);
										fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT CLOSED, %s", lineNumber, nextLexeme);
										normalColor();
										exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
									}
								}
								else {
									errorColor();
									printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
									fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
									normalColor();
									exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
								}
							}
							else {
								errorColor();
								printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
								fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
								normalColor();
								exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
							}
						}
						else {
							errorColor();
							printf("ERROR: At Line %i : SYNTAX ERROR STEP NOT FOUND, %s", lineNumber, nextLexeme);
							fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR STEP NOT FOUND, %s", lineNumber, nextLexeme);
							normalColor();
							exit(ERROR_SYNTAX_ERROR_STEP_NOT_FOUND);
						}
					}
					else {
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
					}
				}
				else {
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
				}
			}
			else {
				errorColor();
				printf("ERROR:At Line %i : SYNTAX ERROR TYPE SET ON FOR LOOP NOT SUPPORTED, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR:At Line %i : SYNTAX ERROR TYPE SET ON FOR LOOP NOT SUPPORTED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_FOR_TYPE_NOT_SUPPORTED);
			}
		}
	}

	printf("Exiting <FOR>\n");
	fprintf(logFile, "Exiting <FOR>\n");
	return nextToken;
}
int reservedExit(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	/*
	<exit> --> Exit(<exp>)
			| Exit()
	*/

	printf("Entering <EXIT>\n");
	fprintf(logFile, "Entering <EXIT>\n");
	Node* exitNode = addChildNode(tree, "EXIT", -1, logFile);



	if (nextToken == RESERVED_EXIT_FUNCTION)
	{
		addChildNode(exitNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(exitNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(exitNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			}
			else
			{
				nextToken = exp(file, nextToken, nextChar, nextLexeme, exitNode, logFile);

				if (nextToken == CLOSE_PARENTESIS) {
					addChildNode(exitNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				}
				else {
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
				}
			}

		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}
	}

	printf("Exiting <EXIT>\n");
	fprintf(logFile, "Exiting <EXIT>\n");
	return nextToken;
}
int reservedContinue(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	/*
	<continue> --> Continue()
	*/
	printf("Entering <CONTINUE>\n");
	fprintf(logFile, "Entering <CONTINUE>\n");
	Node* continueNode = addChildNode(tree, "CONTINUE", -1, logFile);


	if (nextToken == RESERVED_CONTINUE)
	{
		addChildNode(continueNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(continueNode, nextLexeme, nextToken, logFile);

			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(continueNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			}
			else
			{
				errorColor();
				printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
			}

		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}
	}

	printf("Exiting <CONTINUE>\n");
	fprintf(logFile, "Exiting <CONTINUE>\n");
	return nextToken;
}
int reservedContinueIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	/*
	<continueif> -->ContinueIf(<boolstruct>)
	*/
	printf("Entering <CONTINUEIF>\n");
	fprintf(logFile, "Entering <CONTINUEIF>\n");
	Node* continueIfNode = addChildNode(tree, "CONTINUEIF", -1, logFile);

	if (nextToken == RESERVED_CONTINUEIF)
	{
		addChildNode(continueIfNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(continueIfNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			nextToken = boolstruct(file, nextToken, nextChar, nextLexeme, continueIfNode, logFile);
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(continueIfNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			}
			else
			{
				errorColor();
				printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
			}

		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}
	}

	printf("Exiting <CONTINUEIF>\n");
	fprintf(logFile, "Exiting <CONTINUEIF>\n");
	return nextToken;
}
int reservedBreakIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	/*
	<breakif> --> BreakIf(<boolstruct>)
	*/
	printf("Entering <BREAKIF>\n");
	fprintf(logFile, "Entering <BREAKIF>\n");
	Node* breakIfNode = addChildNode(tree, "BREAKIF", -1, logFile);

	if (nextToken == RESERVED_BREAKIF)
	{
		addChildNode(breakIfNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(breakIfNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			nextToken = boolstruct(file, nextToken, nextChar, nextLexeme, breakIfNode, logFile);
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(breakIfNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			}
			else
			{
				errorColor();
				printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
			}

		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}
	}

	printf("Exiting <BREAKIF>\n");
	fprintf(logFile, "Exiting <BREAKIF>\n");
	return nextToken;
}
int reservedBreak(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	/*
<break> --> Break()
	*/
	printf("Entering <BREAK>\n");
	fprintf(logFile, "Entering <BREAK>\n");
	Node* breakNode = addChildNode(tree, "BREAK", -1, logFile);

	if (nextToken == RESERVED_BREAK)
	{
		addChildNode(breakNode, nextLexeme, nextToken, logFile);

		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(breakNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(breakNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			}
			else
			{
				errorColor();
				printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
			}

		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}
	}

	printf("Exiting <BREAK>\n");
	fprintf(logFile, "Exiting <BREAK>\n");
	return nextToken;
}
int reservedIn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	printf("Entering <IN>\n");
	fprintf(logFile, "Entering <IN>\n");

	/*
<in> --> <fscan>
		| <scan>
		| <fscanEnder>
		| <fscanSize>

<fscan> --> In.File(<exp>,<exp>)

<fscanEnder> --> In.FileEnder(<exp>) //missing

<fscanSize> --> In.FileSize(<exp>,<exp>)

<scan> --> In.Console()
<Scasting> --> <in>.toString()

<in> --> <in>.toDecimal()

<in> --> <in>.toNumber()

<in> --> <in>.toChar()

<in> --> <in>.toBool()
	*/

	Node* inNode = addChildNode(tree, "IN", -1, logFile);

	if (nextToken == RESERVED_IN) {
		addChildNode(inNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == POINT) {
			addChildNode(inNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			switch (nextToken) {
			case RESERVED_CONSOLE:
				addChildNode(inNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					addChildNode(inNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					if (nextToken == CLOSE_PARENTESIS) {
						addChildNode(inNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					}
					else
					{
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
					}
				}
				else
				{
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
				}
				break;
			case RESERVED_FILE_ENDER:
				addChildNode(inNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					addChildNode(inNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					nextToken = exp(file, nextToken, nextChar, nextLexeme, inNode, logFile);
					if (nextToken == CLOSE_PARENTESIS) {
						addChildNode(inNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					}
					else
					{
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
					}
				}
				else
				{
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
				}
				break;
			case RESERVED_FILE:case RESERVED_FILE_SIZE:
				addChildNode(inNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					addChildNode(inNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					nextToken = exp(file, nextToken, nextChar, nextLexeme, inNode, logFile);
					if (nextToken == COMMA) {
						addChildNode(inNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						nextToken = exp(file, nextToken, nextChar, nextLexeme, inNode, logFile);
						if (nextToken == CLOSE_PARENTESIS) {
							addChildNode(inNode, nextLexeme, nextToken, logFile);
							nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						}
						else
						{
							errorColor();
							printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
							fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
							normalColor();
							exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
						}
					}
					else
					{
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR COMMA NOT FOUND, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR COMMA NOT FOUND, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_COMMA_NOT_FOUND);
					}
				}
				else
				{
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
				}
				break;
			default: syntaxError(nextLexeme, logFile); break;
			}
			if (nextToken == POINT) { // casting
				printf("Entering <CASTIN>\n");
				fprintf(logFile, "Entering <CASTIN>\n");
				Node* castInNode = addChildNode(inNode, "CASTIN", -1, logFile);
				addChildNode(castInNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == RESERVED_CAST_CHAR || nextToken == RESERVED_CAST_DECIMAL
					|| nextToken == RESERVED_CAST_NUMBER || nextToken == RESERVED_CAST_STRING) {
					addChildNode(castInNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					if (nextToken == OPEN_PARENTESIS) {
						addChildNode(castInNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						if (nextToken == CLOSE_PARENTESIS) {
							addChildNode(castInNode, nextLexeme, nextToken, logFile);
							nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
							printf("Exiting <CASTIN>\n");
							fprintf(logFile, "Exiting <CASTIN>\n");
						}
						else
						{
							errorColor();
							printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
							fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
							normalColor();
							exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
						}
					}
					else
					{
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
					}
				}
				else
				{
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR CAST NOT FOUND, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR CAST NOT FOUND, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_CAST_NOT_FOUND);
				}
			}
		}
		else
		{
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
		}
	}

	printf("Exiting <IN>\n");
	fprintf(logFile, "Exiting <IN>\n");
	return nextToken;
}
int reservedOn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	printf("Entering <ON>\n");
	fprintf(logFile, "Entering <ON>\n");
	/*
<on> --> <fprint>
			|<print>
<fprint> --> On.File(<exp>,<exp>,<mode>,<exp>)  //filename, numberofline(ifin append adds to the end of the line), mode, stringtowrite
<print> --> On.Console(<exp>)
<mode> --> "W" | "A" | "w" | "a"
	*/
	Node* onNode = addChildNode(tree, "ON", -1, logFile);


	if (nextToken == RESERVED_ON) {
		addChildNode(onNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == POINT) {
			addChildNode(onNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			switch (nextToken) {
			case RESERVED_CONSOLE:
				addChildNode(onNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					addChildNode(onNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					nextToken = exp(file, nextToken, nextChar, nextLexeme, onNode, logFile);

					if (nextToken == CLOSE_PARENTESIS) {

						addChildNode(onNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					}
					else {
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
					}
				}
				else {
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
				}


				break;
			case RESERVED_FILE:
				addChildNode(onNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					addChildNode(onNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					nextToken = exp(file, nextToken, nextChar, nextLexeme, onNode, logFile);
					if (nextToken == CLOSE_PARENTESIS) {
						addChildNode(onNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					}
					else {
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
					}
				}
				else {
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
				}
				break;
			default: syntaxError(nextLexeme, logFile); break;
			}
		}
		else
		{
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
		}
	}
	printf("Exiting <ON>\n");
	fprintf(logFile, "Exiting <ON>\n");
	return nextToken;
}
int params(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	printf("Entering <PARAMS>\n");
	fprintf(logFile, "Entering <PARAMS>\n");
	/*
	<params> --> <type_identifier> <var>
			| <type_identifier> <var>, <params>
			*/
	Node* paramsNode = addChildNode(tree, "PARAMS", -1, logFile);
	if (nextToken == IDN_BOOL || nextToken == IDN_NUMBER || nextToken == IDN_DECIMAL || nextToken == IDN_CHAR || nextToken == IDN_STRING) {
		addChildNode(paramsNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == IDENTIFIER) {
			addChildNode(paramsNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			if (nextToken == COMMA) {
				addChildNode(paramsNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				nextToken = params(file, nextToken, nextChar, nextLexeme, paramsNode, logFile);
			}
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR IDENTIFIER NOT FOUND, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR IDENTIFIER NOT FOUND, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_IDENTIFIER_NOT_FOUND);
		}
	}
	else if (nextToken == IDN_VOID) {
		errorColor();
		printf("ERROR:At Line %i : SYNTAX ERROR VOID PARAMS ARE FORBIDDEN, %s", lineNumber, nextLexeme);
		fprintf(logFile, "ERROR:At Line %i : SYNTAX ERROR VOID PARAMS ARE FORBIDDEN, %s", lineNumber, nextLexeme);
		normalColor();
		exit(ERROR_SYNTAX_ERROR_VOID_PARAM);
	}
	else {
		errorColor();
		printf("ERROR:At Line %i : SYNTAX ERROR PARAM BAD FORMAT, %s", lineNumber, nextLexeme);
		fprintf(logFile, "ERROR:At Line %i : SYNTAX ERROR PARAM BAD FORMAT, %s", lineNumber, nextLexeme);
		normalColor();
		exit(ERROR_SYNTAX_ERROR_PARAM_BAD_FORMAT);
	}

	printf("Exiting <PARAMS>\n");
	fprintf(logFile, "Exiting <PARAMS>\n");
	return nextToken;
}
int boolstruct(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <BOOLSTRUCT>\n");
	fprintf(logFile, "Entering <BOOLSTRUCT>\n");
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
	Node* boolstructNode = addChildNode(tree, "BOOLSTRUCT", -1, logFile);
	switch (nextToken) {
	case OP_NOT:
		addChildNode(boolstructNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
	case OPEN_PARENTESIS:
		addChildNode(boolstructNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		nextToken = boolstruct(file, nextToken, nextChar, nextLexeme, boolstructNode, logFile);
		if (nextToken == CLOSE_PARENTESIS) {
			addChildNode(boolstructNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			printf("Exiting <BOOLSTRUCT>\n");
			fprintf(logFile, "Exiting <BOOLSTRUCT>\n");
			return nextToken;
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
		}


		break;
	case RESERVED_BOOL_TRUE: case RESERVED_BOOL_FALSE:
		addChildNode(boolstructNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		printf("Exiting <BOOLSTRUCT>\n");
		fprintf(logFile, "Exiting <BOOLSTRUCT>\n");
		return nextToken;
		break;
	default:
		nextToken = boolexp(file, nextToken, nextChar, nextLexeme, boolstructNode, logFile);
		if (nextToken == OP_AND || nextToken == OP_OR || nextToken == OP_XOR) {
			addChildNode(boolstructNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			nextToken = boolexp(file, nextToken, nextChar, nextLexeme, boolstructNode, logFile);
		}
		printf("Exiting <BOOLSTRUCT>\n");
		fprintf(logFile, "Exiting <BOOLSTRUCT>\n");
		return nextToken;
		break;
	}
	printf("Exiting <BOOLSTRUCT>\n");
	fprintf(logFile, "Exiting <BOOLSTRUCT>\n");
	return nextToken;
}
int boolexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <BOOLEXP>\n");
	fprintf(logFile, "Entering <BOOLEXP>\n");
	/*
	<boolexp> --> <exp> < <exp>
		| <exp> > <exp>
		| <exp> >= <exp>
		| <exp> <= <exp>
		| <exp> = <exp>
		| <exp> != <exp>
		| <exp>



	*/
	Node* boolexpNode = addChildNode(tree, "BOOLEXP", -1, logFile);
	nextToken = exp(file, nextToken, nextChar, nextLexeme, boolexpNode, logFile);
	if (nextToken == OP_EQUAL || nextToken == OP_MINOR_EQUAL || nextToken == OP_MINOR || nextToken == OP_BIGGER || nextToken == OP_BIGGER_EQUAL || nextToken == OP_NOT_EQUAL)
	{
		addChildNode(boolexpNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		nextToken = exp(file, nextToken, nextChar, nextLexeme, boolexpNode, logFile);
	}


	printf("Exiting <BOOLEXP>\n");
	fprintf(logFile, "Exiting <BOOLEXP>\n");
	return nextToken;
}
int declaration(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <DECLARATION>\n");
	fprintf(logFile, "Entering <DECLARATION>\n");

	/*

<declaration> --> <type_identifier> <declareExpr>
				| <type_identifier> <functionDec>
				| <type_identifier><array_def> <declareExpr>
				| <type_identifier><array_def> <functionDec>

	*/
	Node* attributionNode = addChildNode(tree, "DECLARATION", -1, logFile);
	Node * typeNode = addChildNode(attributionNode, nextLexeme, nextToken, logFile);
	nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
	switch (nextToken) {
	case OPEN_BRACKETS:
		nextToken = reservedArrayexp(file, nextToken, nextChar, nextLexeme, typeNode, logFile);
		if (nextToken == IDENTIFIER_FUNCTION) {
			nextToken = functionDec(file, nextToken, nextChar, nextLexeme, attributionNode, logFile);
			break;
		}
		else if (nextToken == IDENTIFIER) {
			nextToken = declareExp(file, nextToken, nextChar, nextLexeme, attributionNode, logFile);
			break;
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR BAD DECLARATION, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR BAD DECLARATION, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_BAD_DECLARATION);
		}

	case IDENTIFIER_FUNCTION:
		nextToken = functionDec(file, nextToken, nextChar, nextLexeme, attributionNode, logFile);
		break;

	case IDENTIFIER:
		nextToken = declareExp(file, nextToken, nextChar, nextLexeme, attributionNode, logFile);
		break;
	}
	printf("Exiting <DECLARATION>\n");
	fprintf(logFile, "Exiting <DECLARATION>\n");
	return nextToken;
}
int functionDec(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <FUNCTIONDEC>\n");
	fprintf(logFile, "Entering <FUNCTIONDEC>\n");

	/*
	<functionDec> -- > <function_name>(<params>).Do(<instructionList>)
	<functionDec> -- > <function_name>().Do(<instructionList>)

	*/
	Node* functionDecNode = addChildNode(tree, "FUNCTIONDEC", -1, logFile);

	if (nextToken == IDENTIFIER_FUNCTION) {
		Node* nameNode = addChildNode(functionDecNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(functionDecNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(functionDecNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == POINT) {
					addChildNode(functionDecNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					if (nextToken == RESERVED_DO) {
						Node * doNode = addChildNode(functionDecNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						if (nextToken == OPEN_PARENTESIS) {
							addChildNode(doNode, nextLexeme, nextToken, logFile);
							nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
							nextToken = instructionList(file, nextToken, nextChar, nextLexeme, doNode, logFile);
							if (nextToken == CLOSE_PARENTESIS) {
								addChildNode(doNode, nextLexeme, nextToken, logFile);
								nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
							}
							else {
								errorColor();
								printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
								fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
								normalColor();
								exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
							}
						}
						else {
							errorColor();
							printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
							fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
							normalColor();
							exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
						}
					}
					else {
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR DO NOT FOUNF, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR DO NOT FOUNF, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_DO_NOT_FOUND);
					}
				}
				else {
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
				}
			}
			else {
				nextToken = params(file, nextToken, nextChar, nextLexeme, nameNode, logFile);
				if (nextToken == CLOSE_PARENTESIS) {
					addChildNode(functionDecNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					if (nextToken == POINT) {
						addChildNode(functionDecNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						if (nextToken == RESERVED_DO) {
							Node * doNode = addChildNode(functionDecNode, nextLexeme, nextToken, logFile);
							nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
							if (nextToken == OPEN_PARENTESIS) {
								addChildNode(doNode, nextLexeme, nextToken, logFile);
								nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
								nextToken = instructionList(file, nextToken, nextChar, nextLexeme, doNode, logFile);
								if (nextToken == CLOSE_PARENTESIS) {
									addChildNode(doNode, nextLexeme, nextToken, logFile);
									nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
								}
								else {
									errorColor();
									printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
									fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
									normalColor();
									exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
								}
							}
							else {
								errorColor();
								printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
								fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
								normalColor();
								exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
							}
						}
						else {
							errorColor();
							printf("ERROR: At Line %i : SYNTAX ERROR DO NOT FOUND, %s", lineNumber, nextLexeme);
							fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR DO NOT FOUND, %s", lineNumber, nextLexeme);
							normalColor();
							exit(ERROR_SYNTAX_ERROR_DO_NOT_FOUND);
						}
					}
					else {
						errorColor();
						printf("ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
						fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR POINT NOT FOUND, %s", lineNumber, nextLexeme);
						normalColor();
						exit(ERROR_SYNTAX_ERROR_POINT_NOT_FOUND);
					}
				}
				else {
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
				}
			}
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}

	}


	printf("Exiting <FUNCTIONDEC>\n");
	fprintf(logFile, "Exiting <FUNCTIONDEC>\n");
	return nextToken;
}
int declareExp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <DECLAREEXP>\n");
	fprintf(logFile, "Entering <DECLAREEXP>\n");

	/*

		<declareExpr> --> <var>
						| <var> << <exp>
						| <declareExpr>,<declareExpr>
	*/
	Node* declareExpNode = addChildNode(tree, "DECLAREEXP", -1, logFile);

	if (nextToken == IDENTIFIER) {
		addChildNode(declareExpNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OP_ATTRIBUTION) {
			addChildNode(declareExpNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			nextToken = exp(file, nextToken, nextChar, nextLexeme, declareExpNode, logFile);//analyze term
			if (nextToken == COMMA) {
				addChildNode(declareExpNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				nextToken = declareExp(file, nextToken, nextChar, nextLexeme, declareExpNode, logFile);//analyze term
			}
		}
	}


	printf("Exiting <DECLAREEXP>\n");
	fprintf(logFile, "Exiting <DECLAREEXP>\n");
	return nextToken;
}
int attribution(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <ATTRIBUTION>\n");
	fprintf(logFile, "Entering <ATTRIBUTION>\n");

	/*
		<attribution> --> <var> << <exp>
	*/
	Node*	attributionNode = addChildNode(tree, "ATTRIBUTION", -1, logFile);

	if (nextToken == IDENTIFIER) {
		addChildNode(attributionNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OP_ATTRIBUTION) {
			addChildNode(attributionNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			nextToken = exp(file, nextToken, nextChar, nextLexeme, attributionNode, logFile);//analyze term

		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR IN ATTRIBUTION, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR IN ATTRIBUTION, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_ATTRIBUTION);
		}
	}

	printf("Exiting <ATTRIBUTION>\n");
	fprintf(logFile, "Exiting <ATTRIBUTION>\n");
	return nextToken;
}
int reservedCallFunction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <CALLFUNCTION>\n");
	fprintf(logFile, "Entering <CALLFUNCTION>\n");
	/*
<call_function> --> F.<function_name>(<params_call>)
				| F.<function_name>()
	*/
	Node*	callFunctionNode = addChildNode(tree, "CALLFUNCTION", -1, logFile);

	if (nextToken == RESERVED_CALL_FUNCTION) {
		addChildNode(callFunctionNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == POINT) {
			addChildNode(callFunctionNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			if (nextToken == IDENTIFIER_FUNCTION) {
				addChildNode(callFunctionNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
				if (nextToken == OPEN_PARENTESIS) {
					addChildNode(callFunctionNode, nextLexeme, nextToken, logFile);
					nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					if (nextToken == CLOSE_PARENTESIS) {
						addChildNode(callFunctionNode, nextLexeme, nextToken, logFile);
						nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
					}
					else {
						nextToken = params_call(file, nextToken, nextChar, nextLexeme, callFunctionNode, logFile);
						if (nextToken == CLOSE_PARENTESIS) {
							addChildNode(callFunctionNode, nextLexeme, nextToken, logFile);
							nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
						}
						else {
							errorColor();
							printf("ERROR: At Line %i : SYNTAX ERROR CALL PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
							fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR CALL PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
							normalColor();
							exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
						}
					}

				}
				else {
					errorColor();
					printf("ERROR: At Line %i : SYNTAX ERROR CALL PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR CALL PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
					normalColor();
					exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
				}
			}
			else {
				errorColor();
				printf("ERROR: At Line %i : SYNTAX ERROR CALL FUNCTION NOT FUNCTION, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR CALL FUNCTION NOT FUNCTION, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_FUNCTION_NOT_FUNCTION);
			}
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR CALL FUNCTION, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR CALL FUNCTION, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_CALL_FUNCTION);
		}
	}


	printf("Exiting <CALLFUNCTION>\n");
	fprintf(logFile, "Exiting <CALLFUNCTION>\n");
	return nextToken;
}
int params_call(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	printf("Entering <PARAMSCALL>\n");
	fprintf(logFile, "Entering <PARAMSCALL>\n");
	/*
<params_call> -->  <exp>
			| <exp>, <params_call>
	*/
	Node*	paramsCallNode = addChildNode(tree, "PARAMSCALL", -1, logFile);

	nextToken = exp(file, nextToken, nextChar, nextLexeme, paramsCallNode, logFile);
	if (nextToken == COMMA) {
		Node * callingNode = addChildNode(paramsCallNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);

		nextToken = params_call(file, nextToken, nextChar, nextLexeme, callingNode, logFile);
	}

	printf("Exiting <PARAMSCALL>\n");
	fprintf(logFile, "Exiting <PARAMSCALL>\n");
	return nextToken;;
}
int exp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	printf("Entering <EXP>\n");
	fprintf(logFile, "Entering <EXP>\n");
	Node* expNode = addChildNode(tree, "EXP", -1, logFile);
	/*
		<exp> --> <term> + <term>
				| <term> - <term>
				| <term>
	*/
	nextToken = term(file, nextToken, nextChar, nextLexeme, expNode, logFile);//analyze term
	if (nextToken == OP_ADD || nextToken == OP_MINUS) {//permits multiple + and - in the program
		addChildNode(expNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		nextToken = exp(file, nextToken, nextChar, nextLexeme, expNode, logFile);//analyze term
	}
	printf("Exiting <EXP>\n");
	fprintf(logFile, "Exiting <EXP>\n");
	return nextToken;

}
int term(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {

	/*
		<term> -- > <factor> * <factor>
				| <factor> / <factor>
				| <factor> % <factor>
				| <factor>
	*/
	printf("Entering <TERM>\n");
	fprintf(logFile, "Entering <TERM>\n");
	Node* termNode = addChildNode(tree, "TERM", -1, logFile);
	nextToken = factor(file, nextToken, nextChar, nextLexeme, termNode, logFile);//analyze term
	if (nextToken == OP_MUL || nextToken == OP_MOD || nextToken == OP_DIV) {//permits multiple * , %, / in the program
		addChildNode(termNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		nextToken = term(file, nextToken, nextChar, nextLexeme, termNode, logFile);//analyze term
	}
	printf("Exiting <TERM>\n");
	fprintf(logFile, "Exiting <TERM>\n");
	return nextToken;
}
int factor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile) {
	/*
	<factor> -- > (<exp>)
			| <var>
			| <var>[<number>]
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
			| <arrayexp>
	*/
	printf("Entering <FACTOR>\n");
	fprintf(logFile, "Entering <FACTOR>\n");
	Node* factorNode = addChildNode(tree, "FACTOR", -1, logFile);
	if (nextToken == OPEN_BRACKETS) {
		nextToken = reservedArrayexp(file, nextToken, nextChar, nextLexeme, factorNode, logFile);//analyze term
	}
	else if (nextToken == IDENTIFIER) {
		Node* identNode = addChildNode(factorNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_BRACKETS) {
			nextToken = reservedArrayexp(file, nextToken, nextChar, nextLexeme, identNode, logFile);//analyze term
		}

	}
	else if (nextToken == LITERAL_CHAR || nextToken == LITERAL_STRING
		|| nextToken == LITERAL_NUMBER || nextToken == LITERAL_DECIMAL) {
		addChildNode(factorNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
	}
	else if (nextToken == OP_ADD || nextToken == OP_MINUS) {
		printf("Entering <SIGNEDNUMBER>\n");
		fprintf(logFile, "Entering <SIGNEDNUMBER>\n");
		Node* signedNumberNode = addChildNode(factorNode, "SIGNEDNUMBER", -1, logFile);
		addChildNode(signedNumberNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == LITERAL_DECIMAL || nextToken == LITERAL_NUMBER) {
			addChildNode(signedNumberNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			printf("Exiting <SIGNEDNUMBER>\n");
			fprintf(logFile, "Exiting <SIGNEDNUMBER>\n");
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR BAD NUMBER FORMAT, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR BAD NUMBER FORMAT, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_BAD_NUMBER_FORMAT);
		}
	}
	else if (
		nextToken == RESERVED_CAST_NUMBER ||
		nextToken == RESERVED_CAST_CHAR ||
		nextToken == RESERVED_CAST_BOOL ||
		nextToken == RESERVED_CAST_STRING ||
		nextToken == RESERVED_CAST_DECIMAL) {
		nextToken = reservedCastingVar(file, nextToken, nextChar, nextLexeme, factorNode, logFile);
	}
	else if (nextToken == RESERVED_CALL_FUNCTION) {
		nextToken = reservedCallFunction(file, nextToken, nextChar, nextLexeme, factorNode, logFile);
	}
	else if (nextToken == RESERVED_IN) {
		nextToken = reservedIn(file, nextToken, nextChar, nextLexeme, factorNode, logFile);
	}
	else if (nextToken == OPEN_PARENTESIS) {
		addChildNode(factorNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		nextToken = exp(file, nextToken, nextChar, nextLexeme, factorNode, logFile);
		if (nextToken == CLOSE_PARENTESIS) {
			addChildNode(factorNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
		}
	}
	else {
		nextToken = boolstruct(file, nextToken, nextChar, nextLexeme, factorNode, logFile);
	}

	printf("Exiting <FACTOR>\n");
	fprintf(logFile, "Exiting <FACTOR>\n");
	return nextToken;
}
int reservedCastingVar(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile)
{
	printf("Entering <CASTINGVAR>\n");
	fprintf(logFile, "Entering <CASTINGVAR>\n");
	/*
<casting> --> toString(<exp>)
<casting> --> toDecimal(<exp>)
<casting> --> toNumber(<exp>)
<casting> --> toChar(<exp>)
<casting> --> toBool(<exp>)
	*/
	Node* castingVarNode = addChildNode(tree, "CASTINGVAR", -1, logFile);


	if (
		nextToken == RESERVED_CAST_NUMBER || nextToken == RESERVED_CAST_CHAR ||
		nextToken == RESERVED_CAST_BOOL || nextToken == RESERVED_CAST_STRING ||
		nextToken == RESERVED_CAST_DECIMAL) {
		addChildNode(castingVarNode, nextLexeme, nextToken, logFile);
		nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
		if (nextToken == OPEN_PARENTESIS) {
			addChildNode(castingVarNode, nextLexeme, nextToken, logFile);
			nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			nextToken = exp(file, nextToken, nextChar, nextLexeme, castingVarNode, logFile);
			if (nextToken == CLOSE_PARENTESIS) {
				addChildNode(castingVarNode, nextLexeme, nextToken, logFile);
				nextToken = lex(file, nextChar, nextLexeme, logFile);//gets next term
			}
			else {
				errorColor();
				printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT CLOSED, %s", lineNumber, nextLexeme);
				normalColor();
				exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED);
			}
		}
		else {
			errorColor();
			printf("ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			fprintf(logFile, "ERROR: At Line %i : SYNTAX ERROR PARENTISIS NOT OPENED, %s", lineNumber, nextLexeme);
			normalColor();
			exit(ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED);
		}
	}

	printf("Exiting <CASTINGVAR>\n");
	fprintf(logFile, "Exiting <CASTINGVAR>\n");
	return nextToken;
}

void syntaxError(char* nextLexeme, FILE* logFile) {
	errorColor();
	printf("ERROR:At Line %i : SYNTAX ERROR NEAR, %s", lineNumber, nextLexeme);
	fprintf(logFile, "ERROR:At Line %i : SYNTAX ERROR NEAR, %s", lineNumber, nextLexeme);
	normalColor();
	exit(ERROR_SYNTAX_ERROR_NEAR);
}

/*



int name_of_non_terminal(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme){
	printf("Entering <name_of_non_terminal>\n");
fprintf(logFile,"Entering <name_of_non_terminal>\n");
	printf("Exiting <name_of_non_terminal>\n");
fprintf(logFile,"Exiting <name_of_non_terminal>\n");
	return nextToken;
}
*/