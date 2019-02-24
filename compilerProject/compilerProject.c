// compilerProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define _countof(array) (sizeof(array) / sizeof(array[0]))
#define True 1
#define False 0



char *readFile(char *fileName) { //reads the entire file and puts it in a char*
	FILE *file = fopen(fileName, "r");//opens the file in reading mode
	char *code;
	size_t n = 0;
	int c;

	if (file == NULL) return NULL; //could not open file
	fseek(file, 0, SEEK_END);//goto the end of the file
	long f_size = ftell(file);//gets the size of the file with the pointer at the end of the file
	fseek(file, 0, SEEK_SET);//goto the beginning of the file
	code = malloc(f_size);//allocate the memory for the char* to fit the whole file

	while ((c = fgetc(file)) != EOF) {//go until the end of the file, reading char by char
		code[n++] = (char)c;
	}
	
	code[n] = '\0';//terminate the char* with a string terminator
	fclose(file);//close the file
	return code;
}
void escapedPrint(char ch) {
	marineColor();
	switch (ch) {
	case '\"':
		printf("\\\"");
		break;
	case '\'':
		printf("\\\'");
		break;
	case '\\':
		printf("\\\\");
		break;
	case '\a':
		printf("\\a");
		break;
	case '\b':
		printf("\\b");
		break;
	case '\n':
		printf("\\n");
		break;
	case '\t':
		printf("\\t");
		break;
		
	default:
		if (iscntrl(ch)) { printf("\\%03o", ch); }
		else {
			normalColor();
			printf("%c", ch);
		}
		normalColor();
	}
}

int main()
{ 
	normalColor();
	char fileName[300] = "";		//Storing File Path/Name of File to Display
	printf("\n\nPlease Enter the Full Path of the File to read: \n");
	scanf("%s", &fileName);
	
	char* content = readFile(fileName);//gets the file and reads it
	size_t i = 0;
	if (content != NULL) {
		printf("\n\n\n\nData successfully read from file\n");
		printf("The file is now closed.\n\n\n");

		while (content[i] != '\0') {
			char ch = content[i];
			escapedPrint(ch);
			i++;
		}
		marineColor();
		printf("\\0");
		normalColor();
	}
	else {
		errorColor();
		printf("Error: File could not be opened\n");
		normalColor();
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


