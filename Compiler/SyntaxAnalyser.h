#pragma once
#include <stdio.h>
#include "LexicalAnalyser.h"
#include "ErrorHandling.h"
int program(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int instructionList(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int instruction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);



int declaration(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);

int functionDec(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);

int declareExp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);

int attribution(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int exp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int term(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int factor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
void syntaxError(char* nextLexeme);

