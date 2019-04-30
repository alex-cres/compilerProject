#pragma once
#include<stdio.h>
#define ERROR_FILE_EMPTY 99
#define ERROR_FILE_NOT_FOUND 2
#define ERROR_NUMBER_WITH_LETTERS_IN_IT 3
#define ERROR_DECIMAL_WITH_LETTERS_OR_SECOND_POINTS_IN_IT 4
#define ERROR_MAX_LEXEME_SIZE_EXCEDED 5
#define ERROR_COMMENT_NOT_FORMATED_CORRECTLY 6



#define ERROR_SYNTAX_ERROR_NEAR 7
#define ERROR_SYNTAX_ERROR_VOID_PARAM 8
#define ERROR_SYNTAX_ERROR_FILE_MODE 9
#define ERROR_SYNTAX_ERROR_FOR_TYPE_NOT_SUPPORTED 10
#define ERROR_SYNTAX_ERROR_ATTRIBUTION 11
#define ERROR_SYNTAX_ERROR_PARENTISIS_NOT_CLOSED 12
#define ERROR_SYNTAX_ERROR_PARENTISIS_NOT_OPENED 13
#define ERROR_SYNTAX_ERROR_FUNCTION_NOT_FUNCTION 14
#define ERROR_SYNTAX_ERROR_CALL_FUNCTION 15
#define ERROR_SYNTAX_ERROR_BAD_NUMBER_FORMAT 16
#define ERROR_SYNTAX_ERROR_ARRAY_NOT_CLOSED 17
#define ERROR_SYNTAX_ERROR_ELSE_NOT_FOUND 18
#define ERROR_SYNTAX_ERROR_POINT_NOT_FOUND 19
#define ERROR_SYNTAX_ERROR_THEN_NOT_OPENED 20
#define ERROR_SYNTAX_ERROR_THEN_NOT_FOUND 21
#define ERROR_SYNTAX_ERROR_DO_NOT_FOUND 22
#define ERROR_SYNTAX_ERROR_IF_NOT_FOUND 23
#define ERROR_SYNTAX_ERROR_STEP_NOT_FOUND 24
#define ERROR_SYNTAX_ERROR_COMMA_NOT_FOUND 25
#define ERROR_SYNTAX_ERROR_CAST_NOT_FOUND 26
#define ERROR_SYNTAX_ERROR_IDENTIFIER_NOT_FOUND 27
#define ERROR_SYNTAX_ERROR_PARAM_BAD_FORMAT 28
#define ERROR_SYNTAX_ERROR_BAD_DECLARATION 29
#define ERROR_SYNTAX_ERROR_MISSING_POINT_COMMA 30
#define ERROR_SYNTAX_ERROR_MISSING_CLOSING_BRACKETS 31
#define ERROR_INVALID_CHAR 32
#define ERROR_MAX_CHILDREN_REACHED 50
#define ERROR_RESERVED_VARIABLES_USED 51
#define ERROR_STRING_OP_NOT_VALID 52
#define ERROR_NON_NUMBER_LOOP 53
#define ERROR_INCOMPLETE_LOOP 54
#define ERROR_OP_NOT_NON_BOOL 55
#define ERROR_SYNTAX_ERROR_FOR_LOOP 56
#define ERROR_OP_MOD_IS_JUST_FOR_INTEGERS 57

void errorColor();
void normalColor();
void marineColor();
void marine2Color();

void printfAndExitWithLine(FILE * logfile, char* printableString, int errorNumber, int lineNumber, char* lexeme);
void printfAndExit(FILE * logfile, char* printableString, int errorNumber, char* lexeme);
