#pragma once
#include <stdio.h>
#include "LexicalAnalyser.h"
#include "ErrorHandling.h"


int instructionList(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int instruction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedLooper(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedFor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedExit(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedContinue(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedContinueIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedBreakIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedBreak(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedIn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedOn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int params(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int boolstruct(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int boolexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int declaration(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int functionDec(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int declareExp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int attribution(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedCallFunction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int params_call(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int exp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int term(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int factor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
int reservedCastingVar(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme);
void syntaxError(char* nextLexeme);

