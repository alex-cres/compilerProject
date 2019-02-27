#pragma once

#include <stdio.h>

#define MAX_LEXEME_SIZE 255

#define	DIGIT	01
#define	ALPHA_SMALL	02
#define	ALPHA_BIG	03
#define	UNKNOWN	99

#define	IDENTIFIER	10
#define	LITERAL_NUMBER	11
#define LITERAL_DECIMAL 12

#define IDN_NUMBER 13
#define IDN_DECIMAL 14
#define IDN_CHAR 15
#define IDN_BOOL 16
#define IDN_VOID 17
#define IDN_STRING 18

#define	OP_MINUS	21
#define	OP_ADD	22
#define	OP_DIV	23
#define	OP_MUL	24
#define	OP_MOD	25
#define OP_SUM 26

#define LEFT_PARENTESIS 27
#define OPEN_PARENTESIS LEFT_PARENTESIS
#define RIGHT_PARENTESIS 28
#define CLOSE_PARENTESIS RIGHT_PARENTESIS

#define LINE_ENDER 29

typedef struct nextChar {
	char value;
	int type;
} NextChar;

typedef struct lexeme {
	char* value;
	int type;
	Lexeme *next;
}Lexeme;

//used to get the next char on the file
NextChar getChar(FILE* file);

//used to get the next non whitespace char on the file
NextChar getNonBlankChar(FILE* file, NextChar nextChar);

//add a char to a lexeme
int addChar(char* lexeme, int lexemeLenght, char ch);

//lookup char
int lookup(char ch, char* lexeme, int lexemeLength);

//lex
int lex(FILE* file, NextChar* nextChar, char* nextLexeme);

void addLexemeToEndOfList(Lexeme * lexemeList, Lexeme * lexeme);


