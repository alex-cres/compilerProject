#pragma once
#include <stdio.h>

#define MAX_LEXEME_SIZE 255
#define DIGIT 1
#define LETTER_BIG 2
#define LETTER_SMALL 3
#define POINT 4
#define POINT_COMMA 5
#define DOUBLE_QUOTE 6
#define QUOTE 7
#define COMMA 8
#define	UNKNOWN	999

#define IDENTIFIER_FUNCTION 10
#define	IDENTIFIER	11
#define IDN_NUMBER 12
#define IDN_DECIMAL 13
#define IDN_CHAR 14
#define IDN_BOOL 15
#define IDN_VOID 16
#define IDN_STRING 17

#define	LITERAL_NUMBER	18
#define LITERAL_DECIMAL 19
#define LITERAL_STRING 20
#define LITERAL_CHAR 21

#define LEFT_BRACKETS 26
#define OPEN_BRACKETS LEFT_BRACKETS
#define RIGHT_BRACKETS 27
#define CLOSE_BRACKETS RIGHT_BRACKETS

#define LEFT_PARENTESIS 28
#define OPEN_PARENTESIS LEFT_PARENTESIS
#define RIGHT_PARENTESIS 29
#define CLOSE_PARENTESIS RIGHT_PARENTESIS

#define OP_ATTRIBUTION 30
#define	OP_MINUS	31
#define	OP_ADD	32
#define	OP_DIV	33
#define	OP_MUL	34
#define	OP_MOD	35
#define OP_MINOR 36
#define OP_BIGGER 37
#define OP_BIGGER_EQUAL 38 
#define OP_MINOR_EQUAL 39
#define OP_EQUAL 40
#define OP_AND 41
#define OP_OR 42
#define OP_NOT 43
#define OP_XOR 44
#define OP_NOT_EQUAL 45

#define RESERVED_BOOL_TRUE 70
#define RESERVED_BOOL_FALSE 71
#define RESERVED_IF 72
#define RESERVED_THEN 73
#define RESERVED_ELSE 74
#define RESERVED_LOOP 75
#define RESERVED_DO 76
#define RESERVED_IN 77
#define RESERVED_ON 78
#define RESERVED_CONSOLE 79
#define RESERVED_CALL_FUNCTION 80
#define RESERVED_FOR 81
#define RESERVED_STEP 82
#define RESERVED_COMMENT 83
#define RESERVED_EXIT_FUNCTION 84
#define RESERVED_BREAK 85
#define RESERVED_BREAKIF 86
#define RESERVED_CONTINUE 87
#define RESERVED_CONTINUEIF 88
#define RESERVED_CAST_NUMBER 90
#define RESERVED_CAST_CHAR 91
#define RESERVED_CAST_BOOL 92
#define RESERVED_CAST_STRING 93
#define RESERVED_CAST_DECIMAL 94
#define RESERVED_FILE 95
#define RESERVED_FILE_ENDER 96
#define RESERVED_FILE_SIZE 97
//reserved strings

#define IDN_NUMBER_LEXEME "N"
#define IDN_DECIMAL_LEXEME "D"
#define IDN_CHAR_LEXEME "C"
#define IDN_BOOL_LEXEME "B"
#define IDN_VOID_LEXEME "V"
#define IDN_STRING_LEXEME "S"
#define LITERAL_BOOL_TRUE_LEXEME "True"
#define LITERAL_BOOL_FALSE_LEXEME "False"
#define RESERVED_IF_LEXEME "If"
#define RESERVED_THEN_LEXEME "Then"
#define RESERVED_ELSE_LEXEME "Else"
#define RESERVED_LOOP_LEXEME "Loop"
#define RESERVED_DO_LEXEME "Do"
#define RESERVED_IN_LEXEME "In"
#define RESERVED_ON_LEXEME "On"
#define RESERVED_CONSOLE_LEXEME "Console"



#define RESERVED_FOR_LEXEME "For"
#define RESERVED_STEP_LEXEME "Step"

#define RESERVED_OP_ATTRIBUTION_LEXEME "<<"
#define OP_MINOR_LEXEME "<"
#define OP_BIGGER_LEXEME ">"
#define OP_BIGGER_EQUAL_LEXEME ">=" 
#define OP_MINOR_EQUAL_LEXEME "<="
#define OP_NOT_EQUAL_LEXEME "!="
#define OP_EQUAL_LEXEME "="
#define OP_AND_LEXEME "&"
#define OP_OR_LEXEME "|"
#define OP_NOT_LEXEME "!"
#define OP_XOR_LEXEME "X"

#define RESERVED_CALL_FUNCTION_LEXEME "F"
#define RESERVED_EXIT_FUNCTION_LEXEME "Exit"
#define RESERVED_COMMENT_VARIANT1_LEXEME "Comment"
#define RESERVED_COMMENT_VARIANT2_LEXEME "Todo"
#define RESERVED_COMMENT_VARIANT3_LEXEME "Fix"

#define RESERVED_BREAK_LEXEME "Break"
#define RESERVED_BREAKIF_LEXEME "BreakIf"
#define RESERVED_CONTINUE_LEXEME "Continue"
#define RESERVED_CONTINUEIF_LEXEME "ContinueIf"

#define RESERVED_CAST_NUMBER_LEXEME "toNumber"
#define RESERVED_CAST_CHAR_LEXEME "toChar"
#define RESERVED_CAST_BOOL_LEXEME "toBool"
#define RESERVED_CAST_STRING_LEXEME "toString"
#define RESERVED_CAST_DECIMAL_LEXEME "toDecimal"
#define RESERVED_FILE_LEXEME "File"
#define RESERVED_FILE_ENDER_LEXEME "FileEnder"
#define RESERVED_FILE_SIZE_LEXEME "FileSize"





typedef struct nextChar {
	char ch;
	int tp_code;
} NextChar;

int lineNumber;

NextChar getChar(FILE* file);

NextChar getNonBlankChar(FILE* file, NextChar nextChar);

int lex(FILE* file, NextChar* nextChar, char* nextLexeme);

int addChar(char* lexeme, int lexemeLength, char nextChar);

int lookup(char ch, char* lexeme, int * lexemeLength);

int isLETTER(int type);

char * tokenDescription(int tokenType);

void pad(char *s, int length);