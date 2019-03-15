#pragma once
#include <stdio.h>
#include "LexicalAnalyser.h"
#include "ErrorHandling.h"
#include "SyntaxTree.h"

int instructionList(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme,Node* tree);
int instruction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedLooper(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedFor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedExit(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedContinue(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedContinueIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedBreakIf(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedBreak(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedIn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedOn(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int params(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int boolstruct(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int boolexp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int declaration(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int functionDec(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int declareExp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int attribution(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedCallFunction(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int params_call(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int exp(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int term(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int factor(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
int reservedCastingVar(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);
void syntaxError(char* nextLexeme);
int reservedArray(FILE* file, int nextToken, NextChar* nextChar, char* nextLexeme, Node* tree);

