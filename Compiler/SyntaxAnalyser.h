#pragma once
#include <stdio.h>


#define MAX_LEXEME_SIZE 255

#define DIGIT 01
#define LETTER_BIG 02
#define LETTER_SMALL 03
#define POINT 04
#define POINT_COMMA 05
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

#define OP_ATTRIBUTION 20
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

typedef struct nextChar {
	char ch;
	int tp_code;
} NextChar;

NextChar getChar(FILE* file);

NextChar getNonBlankChar(FILE* file, NextChar nextChar);

int lex(FILE* file, NextChar* nextChar, char* nextLexeme);

int addChar(char* lexeme, int lexemeLength, char nextChar);

int lookup(char ch, char* lexeme, int * lexemeLength);

int isLETTER(int type);