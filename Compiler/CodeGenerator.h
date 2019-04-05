#pragma once
#include "LexicalAnalyser.h"

#include "SyntaxTree.h"
typedef struct element{
	char * code;
	struct element *next;
	struct element *prior;

}Element;
Element * CreateList();
Element * InsertList(Element* list, char* code);
void GenerateMachineCode(Node * ast, FILE* logFile, char*originalfilename);
void GenerateIntermidiateCode(Node * ast, FILE* logFile, Element* datacode, Element* progcode);