#include "ErrorHandling.h"
#include <Windows.h>


void errorColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY );
}
void normalColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}
void marineColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
}

void marine2Color() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void printfAndExitWithLine(FILE * logfile, char * printableString, int errorNumber, int lineNumber, char * lexeme)
{
	errorColor();
	printf(printableString, lineNumber, lexeme);
	fprintf(logfile, printableString, lineNumber, lexeme);
	normalColor();
	exit(errorNumber);
}

void printfAndExit(FILE * logfile, char * printableString, int errorNumber, char * lexeme)
{
	errorColor();
	printf(printableString, lexeme);
	fprintf(logfile, printableString, lexeme);
	normalColor();
	exit(errorNumber);
}
