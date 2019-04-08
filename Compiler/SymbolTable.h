#pragma once
#include <stdio.h>
#include "SyntaxTree.h"



typedef struct symbolToken {
	int type_of_symbol;
	Node* scope;
	char* name;
	struct symbolToken * next;
	struct symbolToken * previous;
} SymbolToken;

#define TABLE  1000
#define IDENTIFIER_NUMBER  1400
#define IDENTIFIER_ARRAY_NUMBER 1401
#define IDENTIFIER_BOOL 1410
#define IDENTIFIER_ARRAY_BOOL 1411
#define IDENTIFIER_STRING 1420
#define IDENTIFIER_ARRAY_STRING 1421
#define IDENTIFIER_CHAR 1430
#define IDENTIFIER_ARRAY_CHAR 1431
#define IDENTIFIER_DECIMAL 1440
#define IDENTIFIER_ARRAY_DECIMAL 1441

SymbolToken* generateSymbolTable(Node* tree, SymbolToken* table, FILE * logfile);
SymbolToken* generateSymbolToken(int type_of_symbol,Node * scope, char* name, SymbolToken* next, SymbolToken* previous);
void printSymbolTable(SymbolToken* table, FILE * logfile);
SymbolToken* insertSymbolToken(int type_of_symbol, Node * scope, char* name, SymbolToken*table);
int isNumeric(const char * s);