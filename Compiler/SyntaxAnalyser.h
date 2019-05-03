#pragma once
#include <stdio.h>
#include "LexicalAnalyser.h"
#include "ErrorHandling.h"
#include "SyntaxTree.h"

int instructionList(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme,Node* tree, FILE* logFile);
int instruction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedLooper(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedFor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedExit(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedContinue(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedContinueIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedBreakIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedBreak(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedIn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedOn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int params(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int bexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int cexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int declaration(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int functionDec(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int declareExp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int attribution(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedCallFunction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int params_call(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int exp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int term(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int factor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reservedCastingVar(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
int reserved_while(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);

int reservedArrayexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree, FILE* logFile);
void syntaxError(char* nextLexeme, FILE* logFile);


