#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ErrorHandling.h"
#include "SyntaxTree.h"
#include "LexicalAnalyser.h"
#include "SyntaxAnalyser.h"
#include "SymbolTable.h"
#include "CodeGenerator.h"


typedef int bool;
#define True 1
#define False 0

void pad(char *s, int length);

int main()
{

	Node * cst = generateNode("PROGRAM", -1);
	
	FILE * filePointer = NULL;
	FILE * logFile = NULL;
	NextChar nextChar;
	int nextToken;
	char nextLexeme[MAX_LEXEME_SIZE];
	char fileName[500];
	char lofFileName[512] = "";
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
	strcat(lofFileName, fileName);
	strcat(lofFileName, "_logFile.txt");
	logFile = fopen(lofFileName, "w");
	if (filePointer != NULL) {
		printf("\nREADING FROM : %s\n", fileName);
		fprintf(logFile, "\nREADING FROM : %s\n", fileName);
		printf("WRITING TO : %s\n\n", lofFileName);
		fprintf(logFile, "WRITING TO : %s\n\n", lofFileName);
		nextChar = getChar(filePointer);

		do {

			//identifying the lexemes
			nextToken = lex(filePointer, &nextChar, nextLexeme, logFile);
			if (nextToken==EOF) {
				errorColor();
				printf("\n\nError: File Empty");
				fprintf(logFile, "\n\nError: File Empty");
				normalColor();
				return ERROR_FILE_EMPTY;
			}
			nextToken = instructionList(filePointer, nextToken, &nextChar, nextLexeme, cst, logFile);


		} while (nextToken != EOF);
		normalColor();

		int MAX_DEPTH_TREE = getTreeMaxDepth(cst);
		int* arrayDepthTab = (int*)malloc(MAX_DEPTH_TREE * sizeof(int));
		arrayDepthTab[0] = True;
		printf("\nMAX_DEPTH_TREE FOUND : %d\n", MAX_DEPTH_TREE);
		fprintf(logFile, "\nMAX_DEPTH_TREE FOUND : %d\n", MAX_DEPTH_TREE);

		printTree(cst, 0, True, arrayDepthTab, logFile);

		cst = CSTtoAST(cst, logFile);
		printTree(cst, 0, True, arrayDepthTab, logFile);

		cst = Optimization(cst, logFile);
		printTree(cst, 0, True, arrayDepthTab, logFile);
		SymbolToken * table = generateSymbolToken(TABLE, cst, "TABLE", NULL, NULL);
		generateSymbolTable(cst,table,logFile);
		printSymbolTable(table,logFile);

		GenerateMachineCode(cst,logFile,fileName);
	}
	else {
		errorColor();
		printf("\n\nError: File not found");
		fprintf(logFile, "\n\nError: File not found");
		normalColor();
		return ERROR_FILE_NOT_FOUND;
	}
	fclose(filePointer);
	fclose(logFile);


	return 0;

	//printf\((.*)\);
	//printf($1);\r\nfprintf(logFile,$1);






}



