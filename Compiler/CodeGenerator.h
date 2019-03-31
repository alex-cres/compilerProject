#pragma once
#include "LexicalAnalyser.h"

#include "SyntaxTree.h"


void GenerateMachineCode(Node * ast, FILE* logFile);
void GenerateIntermidiateCode(Node * ast, FILE* logFile);