#include <stdio.h>
#include <string.h>
#include "LexicalAnalyser.h"
#include "ErrorHandling.h"
int lineNumber = 1;
int isLETTER(int type) {
	return type == LETTER_SMALL || type == LETTER_BIG;
}

NextChar getChar(FILE * file)
{
	NextChar nextCh;
	if ((nextCh.ch = getc(file)) != EOF) {
		if (isdigit(nextCh.ch))
			nextCh.tp_code = DIGIT;
		else if (isupper(nextCh.ch))
			nextCh.tp_code = LETTER_BIG;
		else if (islower(nextCh.ch))
			nextCh.tp_code = LETTER_SMALL;
		else if (nextCh.ch == '.')
			nextCh.tp_code = POINT;
		else if (nextCh.ch == '"')
			nextCh.tp_code = DOUBLE_QUOTE;
		else if (nextCh.ch == '\'')
			nextCh.tp_code = QUOTE;
		else if (nextCh.ch == '\\')
			nextCh.tp_code = BACKLASH;
		else
			nextCh.tp_code = UNKNOWN;
	}
	else {
		nextCh.tp_code = EOF;
	}
	if (nextCh.ch == '\n') {
		lineNumber++;
	}
	return nextCh;
}

NextChar getNonBlankChar(FILE * file, NextChar nextChar)
{
	while (isspace(nextChar.ch)) {

		nextChar = getChar(file);
	}
	return nextChar;
}
//identify and build the lexemes 
int lex(FILE* file, NextChar* nextChar, char* nextLexeme, FILE* logFile)
{
	int sizeOfLexeme = -1;//according to the add char function, it first adds to the length, so the first char needs to be on the 0
	int nextToken = 0;
	*nextChar = getNonBlankChar(file, *nextChar);
	switch (nextChar->tp_code) {

	case DOUBLE_QUOTE: //STRINGS
		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
			*nextChar = getChar(file);
			if (nextChar->tp_code == DOUBLE_QUOTE) {
				if (nextLexeme[strlen(nextLexeme) - 1] == '\\') {
					sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
					*nextChar = getChar(file);
				}
			}
		} while (nextChar->tp_code != DOUBLE_QUOTE);
		sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
		*nextChar = getChar(file);
		nextToken = LITERAL_STRING;
		break;
	case QUOTE:
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
			*nextChar = getChar(file);
				if (nextChar->tp_code == BACKLASH) {
					sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
					*nextChar = getChar(file);
					sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
					*nextChar = getChar(file);
					if (nextChar->tp_code == QUOTE) {
						sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
						*nextChar = getChar(file);
					}
					else {
						sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
						printfAndExitWithLine(logFile, "ERROR:At Line %i : INVALID CHAR %s", ERROR_INVALID_CHAR, lineNumber, nextLexeme);
					}
				}
				else {
					sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
					*nextChar = getChar(file);
					if (nextChar->tp_code == QUOTE) {
						sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
						*nextChar = getChar(file);
					}
					else {
						sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
						printfAndExitWithLine(logFile, "ERROR:At Line %i : INVALID CHAR %s", ERROR_INVALID_CHAR, lineNumber, nextLexeme);

					}
				}
		nextToken = LITERAL_CHAR;
		break;
	case LETTER_SMALL:
	case LETTER_BIG:
		if (nextChar->tp_code == LETTER_BIG) {
			nextToken = IDENTIFIER_FUNCTION;
		}
		else if (nextChar->tp_code == LETTER_SMALL) {
			nextToken = IDENTIFIER;
		}
		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
			*nextChar = getChar(file);

		} while (isLETTER(nextChar->tp_code) || nextChar->tp_code == DIGIT);
		if (0 == strcmp(nextLexeme, LITERAL_BOOL_TRUE_LEXEME)) {
			nextToken = RESERVED_BOOL_TRUE;
			break;
		}
		else if (0 == strcmp(nextLexeme, LITERAL_BOOL_FALSE_LEXEME)) {
			nextToken = RESERVED_BOOL_FALSE;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_EXIT_FUNCTION_LEXEME)) {
			nextToken = RESERVED_EXIT_FUNCTION;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CONSOLE_LEXEME)) {
			nextToken = RESERVED_CONSOLE;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_IF_LEXEME)) {
			nextToken = RESERVED_IF;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_THEN_LEXEME)) {
			nextToken = RESERVED_THEN;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_ELSE_LEXEME)) {
			nextToken = RESERVED_ELSE;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_LOOP_LEXEME)) {
			nextToken = RESERVED_LOOP;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_DO_LEXEME)) {
			nextToken = RESERVED_DO;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_IN_LEXEME)) {
			nextToken = RESERVED_IN;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_ON_LEXEME)) {
			nextToken = RESERVED_ON;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CONSOLE_LEXEME)) {
			nextToken = RESERVED_CONSOLE
				;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_FOR_LEXEME)) {
			nextToken = RESERVED_FOR;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_STEP_LEXEME)) {
			nextToken = RESERVED_STEP;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CALL_FUNCTION_LEXEME)) {
			nextToken = RESERVED_CALL_FUNCTION;
			break;
		}
		else if ((0 == strcmp(nextLexeme, RESERVED_COMMENT_VARIANT3_LEXEME)) || (0 == strcmp(nextLexeme, RESERVED_COMMENT_VARIANT2_LEXEME)) || (0 == strcmp(nextLexeme, RESERVED_COMMENT_VARIANT1_LEXEME))) {
			if (lookup(nextChar->ch, nextLexeme, &sizeOfLexeme, logFile) != OPEN_PARENTESIS) {
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				printfAndExitWithLine(logFile,"ERROR:At Line %i : COMMENT NOT FORMATED CORRECTLY %s",ERROR_COMMENT_NOT_FORMATED_CORRECTLY,lineNumber,nextLexeme);

			}
			*nextChar = getChar(file);

			while (lookup(nextChar->ch, nextLexeme, &sizeOfLexeme, logFile) != CLOSE_PARENTESIS) {
				*nextChar = getChar(file);
			}

			*nextChar = getChar(file);
			nextToken = RESERVED_COMMENT;

			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_BREAK_LEXEME)) {
			nextToken = RESERVED_BREAK;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_BREAKIF_LEXEME)) {
			nextToken = RESERVED_BREAKIF;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CONTINUE_LEXEME)) {
			nextToken = RESERVED_CONTINUE;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CONTINUEIF_LEXEME)) {
			nextToken = RESERVED_CONTINUEIF;
			break;
		}
		else if (0 == strcmp(nextLexeme, IDN_NUMBER_LEXEME)) {
			nextToken = IDN_NUMBER;
			break;

		}
		else if (0 == strcmp(nextLexeme, IDN_DECIMAL_LEXEME)) {
			nextToken = IDN_DECIMAL;
			break;
		}
		else if (0 == strcmp(nextLexeme, IDN_STRING_LEXEME)) {
			nextToken = IDN_STRING;
			break;
		}
		else if (0 == strcmp(nextLexeme, IDN_BOOL_LEXEME)) {
			nextToken = IDN_BOOL;
			break;
		}
		else if (0 == strcmp(nextLexeme, IDN_VOID_LEXEME)) {
			nextToken = IDN_VOID;
			break;
		}
		else if (0 == strcmp(nextLexeme, IDN_CHAR_LEXEME)) {
			nextToken = IDN_CHAR;
			break;
		}
		else if (0 == strcmp(nextLexeme, OP_XOR_LEXEME)) {
			nextToken = OP_XOR;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_FILE_LEXEME)) {
			nextToken = RESERVED_FILE;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_FILE_ENDER_LEXEME)) {
			nextToken = RESERVED_FILE_ENDER;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_FILE_SIZE_LEXEME)) {
			nextToken = RESERVED_FILE_SIZE;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CAST_NUMBER_LEXEME)) {
			nextToken = RESERVED_CAST_NUMBER;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CAST_CHAR_LEXEME)) {
			nextToken = RESERVED_CAST_CHAR;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CAST_BOOL_LEXEME)) {
			nextToken = RESERVED_CAST_BOOL;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CAST_STRING_LEXEME)) {
			nextToken = RESERVED_CAST_STRING;
			break;
		}
		else if (0 == strcmp(nextLexeme, RESERVED_CAST_DECIMAL_LEXEME)) {
			nextToken = RESERVED_CAST_DECIMAL;
			break;
		}
		break;
	case DIGIT:

		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
			*nextChar = getChar(file);
		} while (nextChar->tp_code == DIGIT);
		if (isLETTER(nextChar->tp_code)) {//error handling
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
			printfAndExitWithLine(logFile,"ERROR:At Line %i : NUMBER_WITH_LETTERS_IN_IT, %s",ERROR_NUMBER_WITH_LETTERS_IN_IT,lineNumber,nextLexeme);
		}
		if (nextChar->tp_code == POINT) {//decimal handling
			do {
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				*nextChar = getChar(file);
			} while (nextChar->tp_code == DIGIT);
			if (isLETTER(nextChar->tp_code) || nextChar->tp_code == POINT) {//error handling
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				printfAndExitWithLine(logFile,"ERROR:At Line %i : ERROR_DECIMAL_WITH_LETTERS_OR_SECOND_POINTS_IN_IT, %s",ERROR_DECIMAL_WITH_LETTERS_OR_SECOND_POINTS_IN_IT,lineNumber,nextLexeme);
			}
			nextToken = LITERAL_DECIMAL;
			break;
		}
		nextToken = LITERAL_NUMBER;
		break;
	case POINT:
		sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, '0', logFile);
		sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
		nextToken = POINT;
		*nextChar = getChar(file);
		if (nextChar->tp_code == DIGIT) {
			do {
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				*nextChar = getChar(file);
			} while (nextChar->tp_code == DIGIT);
			if (isLETTER(nextChar->tp_code) || nextChar->tp_code == POINT) {//error handling
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				printfAndExitWithLine(logFile,"ERROR:At Line %i : ERROR_DECIMAL_WITH_LETTERS_OR_SECOND_POINTS_IN_IT, %s",ERROR_DECIMAL_WITH_LETTERS_OR_SECOND_POINTS_IN_IT,lineNumber,nextLexeme);
			}
			nextToken = LITERAL_DECIMAL;
			break;
		}
		break;
	case UNKNOWN:
		nextToken = lookup(nextChar->ch, nextLexeme, &sizeOfLexeme, logFile);
		*nextChar = getChar(file);
		if (nextToken == OP_MINOR) {
			if (nextChar->ch == '=') {
				nextToken = OP_MINOR_EQUAL;
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				*nextChar = getChar(file);
			}
			if (nextChar->ch == '<') {
				nextToken = OP_ATTRIBUTION;
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				*nextChar = getChar(file);
			}
			
		}
		else if (nextToken == OP_BIGGER) {
			if (nextChar->ch == '=') {
				nextToken = OP_BIGGER_EQUAL;
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				*nextChar = getChar(file);
			}

		}
		else if (nextToken == OP_NOT) {
			if (nextChar->ch == '=') {
				nextToken = OP_NOT_EQUAL;
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch, logFile);
				*nextChar = getChar(file);
			}

		}

		break;
	case EOF:
		nextToken = EOF;
		nextLexeme[0] = 'E';
		nextLexeme[1] = 'O';
		nextLexeme[2] = 'F';
		nextLexeme[3] = '\0';
		break;
	}
	char nameToken[60];
	strcpy(nameToken, tokenDescription(nextToken));
	pad(nameToken, 30);
	printf("Token:\t%03d --> %s\tLex:\t", nextToken, nameToken);
	fprintf(logFile, "Token:\t%03d --> %s\tLex:\t", nextToken, nameToken);
	printf("%s\n", nextLexeme);
	fprintf(logFile, "%s\n", nextLexeme);

	return nextToken;
}

int addChar(char* lexeme, int lexemeLength, char nextChar, FILE* logFile)
{
	if (lexemeLength < MAX_LEXEME_SIZE) {
		lexemeLength++;
		lexeme[lexemeLength] = nextChar;
		lexeme[lexemeLength + 1] = 0;
	}
	else {
		errorColor();
		printf("ERROR: At Line %i : MAX_LEXEME_SIZE_EXCEDED (%d) : %s \n", lineNumber, MAX_LEXEME_SIZE, lexeme);
		fprintf(logFile, "ERROR: At Line %i : MAX_LEXEME_SIZE_EXCEDED (%d) : %s \n", lineNumber, MAX_LEXEME_SIZE, lexeme);
		normalColor();
		exit(ERROR_MAX_LEXEME_SIZE_EXCEDED);
	}
	return lexemeLength;
}

int lookup(char ch, char* lexeme, int * lexemeLength, FILE* logFile)
{
	int nextToken = UNKNOWN;
	switch (ch) {
	case '\\':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = BACKLASH;
		break;
	case '(':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OPEN_PARENTESIS;
		break;
	case ')':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = CLOSE_PARENTESIS;
		break;
	case '[':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OPEN_BRACKETS;
		break;
	case ']':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = CLOSE_BRACKETS;
		break;
	case '+':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_ADD;
		break;
	case '-':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_MINUS;
		break;
	case '/':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_DIV;
		break;
	case '*':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_MUL;
		break;
	case '%':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_MOD;
		break;
	case ';':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = POINT_COMMA;
		break;
	case ',':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = COMMA;
		break;
	case '<':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_MINOR;
		break;
	case '>':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_BIGGER;
		break;
	case '=':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_EQUAL;
		break;
	case '&':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_AND;
		break;
	case '|':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_OR;
		break;
	case '!':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = OP_NOT;
		break;
	default:
		*lexemeLength = addChar(lexeme, *lexemeLength, ch, logFile);
		nextToken = EOF;
		break;
	}
	return nextToken;
}


char* tokenDescription(int tokenType) {
	switch (tokenType) {
	case EOF: return"EOF"; break;
	case DIGIT: return"DIGIT"; break;
	case LETTER_BIG: return"LETTER_BIG"; break;
	case LETTER_SMALL: return"LETTER_SMALL"; break;
	case POINT: return"POINT"; break;
	case POINT_COMMA: return"POINT_COMMA"; break;
	case DOUBLE_QUOTE: return"DOUBLE_QUOTE"; break;
	case QUOTE: return"QUOTE"; break;
	case COMMA: return"COMMA"; break;
	case UNKNOWN: return"UNKNOWN"; break;
	case IDENTIFIER_FUNCTION: return"IDENTIFIER_FUNCTION"; break;
	case IDENTIFIER: return"IDENTIFIER"; break;
	case IDN_NUMBER: return"IDN_NUMBER"; break;
	case IDN_DECIMAL: return"IDN_DECIMAL"; break;
	case IDN_CHAR: return"IDN_CHAR"; break;
	case IDN_BOOL: return"IDN_BOOL"; break;
	case IDN_VOID: return"IDN_VOID"; break;
	case IDN_STRING: return"IDN_STRING"; break;
	case LITERAL_NUMBER: return"LITERAL_NUMBER"; break;
	case LITERAL_DECIMAL: return"LITERAL_DECIMAL"; break;
	case LITERAL_STRING: return"LITERAL_STRING"; break;
	case LITERAL_CHAR: return"LITERAL_CHAR"; break;
	case OPEN_BRACKETS: return"OPEN_BRACKETS"; break;
	case CLOSE_BRACKETS: return"CLOSE_BRACKETS"; break;
	case OPEN_PARENTESIS: return"OPEN_PARENTESIS"; break;
	case CLOSE_PARENTESIS: return"CLOSE_PARENTESIS"; break;
	case OP_ATTRIBUTION: return"OP_ATTRIBUTION"; break;
	case OP_MINUS: return"OP_MINUS"; break;
	case OP_ADD: return"OP_ADD"; break;
	case OP_DIV: return"OP_DIV"; break;
	case OP_MUL: return"OP_MUL"; break;
	case OP_MOD: return"OP_MOD"; break;
	case OP_MINOR: return"OP_MINOR"; break;
	case OP_BIGGER: return"OP_BIGGER"; break;
	case OP_BIGGER_EQUAL: return"OP_BIGGER_EQUAL "; break;
	case OP_MINOR_EQUAL: return"OP_MINOR_EQUAL"; break;
	case OP_EQUAL: return"OP_EQUAL"; break;
	case OP_AND: return"OP_AND"; break;
	case OP_OR: return"OP_OR"; break;
	case OP_NOT: return"OP_NOT"; break;
	case OP_XOR: return"OP_XOR"; break;
	case OP_NOT_EQUAL: return "OP_NOT_EQUAL"; break;
	case RESERVED_BOOL_TRUE: return"RESERVED_BOOL_TRUE"; break;
	case RESERVED_BOOL_FALSE: return"RESERVED_BOOL_FALSE"; break;
	case RESERVED_IF: return"RESERVED_IF"; break;
	case RESERVED_THEN: return"RESERVED_THEN"; break;
	case RESERVED_ELSE: return"RESERVED_ELSE"; break;
	case RESERVED_LOOP: return"RESERVED_LOOP"; break;
	case RESERVED_DO: return"RESERVED_DO"; break;
	case RESERVED_IN: return"RESERVED_IN"; break;
	case RESERVED_ON: return"RESERVED_ON"; break;
	case RESERVED_CONSOLE: return"RESERVED_CONSOLE"; break;
	case RESERVED_CALL_FUNCTION: return"RESERVED_CALL_FUNCTION"; break;
	case RESERVED_FOR: return"RESERVED_FOR"; break;
	case RESERVED_STEP: return"RESERVED_STEP"; break;
	case RESERVED_COMMENT: return"RESERVED_COMMENT"; break;
	case RESERVED_EXIT_FUNCTION: return"RESERVED_EXIT_FUNCTION"; break;
	case RESERVED_BREAK: return"RESERVED_BREAK"; break;
	case RESERVED_BREAKIF: return"RESERVED_BREAKIF"; break;
	case RESERVED_CONTINUE: return"RESERVED_CONTINUE"; break;
	case RESERVED_CONTINUEIF: return"RESERVED_CONTINUEIF"; break;
	case RESERVED_CAST_NUMBER: return"RESERVED_CAST_NUMBER"; break;
	case RESERVED_CAST_CHAR: return"RESERVED_CAST_CHAR"; break;
	case RESERVED_CAST_BOOL: return"RESERVED_CAST_BOOL"; break;
	case RESERVED_CAST_STRING: return"RESERVED_CAST_STRING"; break;
	case RESERVED_CAST_DECIMAL: return"RESERVED_CAST_DECIMAL"; break;
	case RESERVED_FILE: return"RESERVED_FILE"; break;
	case RESERVED_FILE_ENDER: return"RESERVED_FILE_ENDER"; break;
	case RESERVED_FILE_SIZE: return"RESERVED_FILE_SIZE"; break;
	default: return"TOKEN_NOT_IDENTIFIED"; break;
	}
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

