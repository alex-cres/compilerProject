#include <stdio.h>
#include <stdlib.h>
#include "ErrorHandling.h"
#include "LexicalAnalyser.h"
#include "SyntaxAnalyser.h"

#define _countof(array) (sizeof(array) / sizeof(array[0]))
typedef int bool;
#define True 1
#define False 0

void pad(char *s, int length);

int main()
{
	FILE * filePointer = NULL;
	NextChar nextChar;
	int nextToken;
	char nextLexeme[MAX_LEXEME_SIZE];
	char fileName[500];
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
		int i = 0;
		int inlinetabs = 0;
		do {
			//color change in output
			if (i%2) {
				marine2Color();
			}
			else {
				normalColor();
			}
			//identifying the lexemes
			nextToken = lex(filePointer, &nextChar, nextLexeme);

			//getting the string of the tokenType and formatting it
			char nameToken[60];
			strcpy(nameToken, tokenDescription(nextToken));
			pad(nameToken, 30);
			
			//printing with tabs
			printf("Token:\t%03d --> %s\tLex:\t", nextToken, nameToken);
			//tabs formating
			if (nextToken == CLOSE_PARENTESIS) {
				inlinetabs--;
			}
			//tabs formating
			for (int j = 0; j < inlinetabs;j++) {
				printf("   ");
			}
			//lexeme printing
			printf("%s\n", nextLexeme);
			//tabs formating
			if (nextToken == OPEN_PARENTESIS) {
				inlinetabs++;
			}
			
			i++;
		} while (nextToken != EOF);
		normalColor();
	}
	else {
		errorColor();
		printf("\n\nError: File not found");
		normalColor();
		return ERROR_FILE_NOT_FOUND;
	}


	return 0;
}




void pad(char *s, int length)
{
	int l;

	l = strlen(s); /* its length */

	while (l < length) {
		s[l] = ' '; /* insert a space */
		l++;
	}
	s[l] = '\0'; /* strings need to be terminated in a null */

}
