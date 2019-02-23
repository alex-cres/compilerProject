// compilerProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <stdio.h>
#include <string.h>
#define _countof(array) (sizeof(array) / sizeof(array[0]))
#define _CRT_SECURE_NO_WARNINGS

char *readFile(char *fileName) {
	FILE *file = fopen(fileName, "r");
	char *code;
	size_t n = 0;
	int c;

	if (file == NULL) return NULL; //could not open file
	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	code = malloc(f_size);

	while ((c = fgetc(file)) != EOF) {
		code[n++] = (char)c;
	}

	code[n] = '\0';

	return code;
}
void escapedPrint(char ch) {
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
		// and so on
	default:
		if (iscntrl(ch)) printf("\\%03o", ch);
		else printf("%c", ch);
	}
}

int main()
{ 
	FILE *fp;
	char fnamer[100] = "";		//Storing File Path/Name of Image to Display
	printf("\n\nPlease Enter the Full Path of the Image file you want to view: \n");
	scanf("%s", &fnamer);
	
	char* content = readFile(fnamer);
	size_t i = 0;
	if (content != NULL) {
		while (content[i] != '\0') {
			char ch = content[i];
			escapedPrint(ch);
			printf(" ");
			i++;
		}
		printf("\\0 ");

		// Closing the file using fclose() 


		printf("\n\n\n\nData successfully read from file\n");
		printf("The file is now closed.");
	}
	else {
		printf("Error in reading from file\n");
	}
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file



//C:\Users\Alex\Documents\GitHub\compilerProject\testee.txt

