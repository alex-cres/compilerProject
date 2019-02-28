#include <stdio.h>
#include <string.h>
#include "SyntaxAnalyser.h"
#include "ErrorHandling.h"

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
		else
			nextCh.tp_code = UNKNOWN;
	}
	else {
		nextCh.tp_code = EOF;
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
int lex(FILE * file, NextChar * nextChar, char * nextLexeme)
{
	int sizeOfLexeme = -1;//acording to the addchar function, it first adds to the lenght, so the first char needs to be on the 0
	int nextToken;
	*nextChar = getNonBlankChar(file, *nextChar);
	switch (nextChar->tp_code) {

	case LETTER_SMALL:
		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			*nextChar = getChar(file);
			
		} while (nextChar->tp_code == LETTER_SMALL || nextChar->tp_code == DIGIT);
		if (strcmp(nextLexeme, "int")) {
			nextToken = IDN_NUMBER;
			break;
			
		}
		else if (strcmp(nextLexeme, "float")) {
			nextToken = IDN_DECIMAL;
			break;
		}
		nextToken = IDENTIFIER;
		break;
	case DIGIT:

		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			*nextChar = getChar(file);
		} while (nextChar->tp_code == DIGIT);
		if (isLETTER(nextChar->tp_code)) {//error handleing
			printf("ERROR: NUMBER_WITH_LETTERS_IN_IT, %s", nextLexeme);
			exit(ERROR_NUMBER_WITH_LETTERS_IN_IT);
		}
		if (nextChar->tp_code == POINT) {//decimal handleing
			do {
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
				*nextChar = getChar(file);
			} while (nextChar->tp_code == DIGIT);
			if (isLETTER(nextChar->tp_code) || nextChar->tp_code == POINT) {//error handleing
				printf("ERROR: DECIMAL_WITH_LETTERS_AND_SECOND_POINTS_IN_IT, %s", nextLexeme);
				exit(ERROR_DECIMAL_WITH_LETTERS_AND_SECOND_POINTS_IN_IT);
			}
			nextToken = LITERAL_DECIMAL;
			break;
		}
		nextToken = LITERAL_NUMBER;

		break;
	case UNKNOWN:
		nextToken = lookup(nextChar->ch, nextLexeme, &sizeOfLexeme);
		*nextChar = getChar(file);
		break;
	case EOF:
		nextToken = EOF;
		nextLexeme[0] = 'E';
		nextLexeme[1] = 'O';
		nextLexeme[2] = 'F';
		nextLexeme[3] = '\0';
		break;
	}

	return nextToken;
}

int addChar(char * lexeme, int lexemeLength, char nextChar)
{
	if (lexemeLength < MAX_LEXEME_SIZE) {
		lexemeLength++;
		lexeme[lexemeLength] = nextChar;
		lexeme[lexemeLength + 1] = 0;
	}
	else {
		printf("ERROR: MAX_LEXEME_SIZE_EXCEDED (255) : %s \n", lexeme);
		exit(ERROR_MAX_LEXEME_SIZE_EXCEDED);
	}
	return lexemeLength;
}

int lookup(char ch, char * lexeme, int * lexemeLength)
{
	int nextToken;
	switch (ch) {
	case '(':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OPEN_PARENTESIS;
		break;
	case ')':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = CLOSE_PARENTESIS;
		break;
	case '+':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_ADD;
		break;
	case '-':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_MINUS;
		break;
	case '/':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_DIV;
		break;
	case '*':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_MUL;
		break;
	case '%':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_MOD;
		break;
	case '=':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_ATTRIBUTION;
		break;
	case ';':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = POINT_COMMA;
		break;
	default:
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = EOF;
		break;
	}
	return nextToken;
}

