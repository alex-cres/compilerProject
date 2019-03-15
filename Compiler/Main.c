#include <stdio.h>
#include <stdlib.h>
#include "ErrorHandling.h"
#include "SyntaxTree.h"
#include "LexicalAnalyser.h"
#include "SyntaxAnalyser.h"


typedef int bool;
#define True 1
#define False 0

void pad(char *s, int length);

int main()
{
	
	Node * cst = generateNode("PROGRAM",-1);
	FILE * filePointer = NULL;
	NextChar nextChar;
	int nextToken;
	char nextLexeme[MAX_LEXEME_SIZE];
	char fileName[500];
	//..\Examples\ArrayUsageExample.cx
	//..\Examples\FunctionExample.cx
	//..\Examples\AttributionAndWriteExample.cx
	//..\Examples\BreakExample.cx
	//..\Examples\BreakIfExample.cx
	//..\Examples\ContinueExample.cx
	//..\Examples\FileReadingExample.cx
	//..\Examples\ForLoopExample.cx
	//..\Examples\SimpleExample.cx
	//..\Examples\SimpleLooperExample.cx
	//..\Examples\StringAndWriteInConsoleExample.cx
	

	scanf("%s", &fileName);
	filePointer = fopen(fileName, "r");
	if (filePointer != NULL) {
		nextChar = getChar(filePointer);

		do {
			
			//identifying the lexemes
			nextToken = lex(filePointer, &nextChar, nextLexeme);
			nextToken = instructionList(filePointer,nextToken, &nextChar, nextLexeme,cst);
			
			
		} while (nextToken != EOF);
		normalColor();
		printTree(cst, 0,False);
	}
	else {
		errorColor();
		printf("\n\nError: File not found");
		normalColor();
		return ERROR_FILE_NOT_FOUND;
	}


	return 0;

	




}



