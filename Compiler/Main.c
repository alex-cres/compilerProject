#include <stdio.h>
#include <stdlib.h>
#include "ErrorHandling.h"
#include "LexicalAnalyser.h"

#define _countof(array) (sizeof(array) / sizeof(array[0]))
typedef int bool;
#define True 1
#define False 0


int main()
{
	FILE * filePointer = NULL;
	NextChar nextChar;
	int nextToken;
	char nextLexeme[MAX_LEXEME_SIZE];
	filePointer = fopen("C:\\Users\\Alex\\Documents\\GitHub\\compilerProject\\Examples\\StringAndWriteInConsoleExample.cx", "r");
	if (filePointer != NULL) {
		nextChar = getChar(filePointer);
		do {
			nextToken = lex(filePointer, &nextChar, nextLexeme);
			printf("Token: %d , Lexeme: %s\n", nextToken, nextLexeme);
		} while (nextToken != EOF);
	}
	else {
		printf("\n\nError: File not found");
		return ERROR_FILE_NOT_FOUND;
	}


	return 0;
}