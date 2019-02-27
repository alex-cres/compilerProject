// compilerProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define _countof(array) (sizeof(array) / sizeof(array[0]))

#define True 1
#define False 0

#define ERR_FILE_NOT_OPENED 2





int main()
{ 
	FILE * fp = NULL;

	normalColor();
	char fileName[300] = "";		//Storing File Path/Name of File to Display
	printf("\n\nPlease Enter the Full Path of the File to read: \n");
	scanf("%s", &fileName);
	fp = fopen(fileName,"r");
	Lexeme * tokenList=NULL;
	if (fp!=NULL) {
		printf("\n\n File Opened, Starting Reading\n\n");
		char nextLexeme[MAX_LEXEME_SIZE];
		int nextToken;
		NextChar nextChar;

		nextChar = getChar(fp);
		do 
		{
			nextToken = lex(fp,&nextChar,nextLexeme);
			Lexeme lexeme;
			lexeme.value = nextLexeme;
			lexeme.type = nextToken;
			lexeme.next = NULL;
			addLexemeToEndOfList(tokenList,&lexeme);
		} while (nextToken!=EOF);
		//Printing the stored lexemes
		Lexeme * pointer = tokenList;
		while (pointer != NULL)
		{
			printf("Token: %d, Lexeme: %s", pointer->type, pointer->value);
			pointer = pointer->next;
		}
	}
	else {
		errorColor();
		printf("Error: File could not be opened\n");
		normalColor();
		return ERR_FILE_NOT_OPENED;
	}
}



//Text color functions
errorColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
}
normalColor(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}
marineColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//1. Use the Solution Explorer window to add/manage files
//2. Use the Team Explorer window to connect to source control
//3. Use the Output window to see build output and other messages
//4. Use the Error List window to view errors
//5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//6. In the future, to open this project again, go to File > Open > Project and select the .sln file


