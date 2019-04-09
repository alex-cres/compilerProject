#pragma once
#include "LexicalAnalyser.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"
typedef struct element{
	char * code;
	struct element *next;
	struct element *prior;

}Element;
Element * CreateList();
Element * InsertList(Element* list, char* code);
void GenerateMachineCode(Node * ast, FILE* logFile, char*originalfilename, SymbolToken * table);
void GenerateIntermidiateCode(Node * ast, FILE* logFile, Element* datacode, Element* progcode, SymbolToken * table);
void strupper(char * temp);