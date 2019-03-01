#include <stdio.h>
#include <string.h>
#include "LexicalAnalyser.h"
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
		else if (nextCh.ch == '"')
			nextCh.tp_code = DOUBLE_QUOTE;
		else if (nextCh.ch == '\'')
			nextCh.tp_code = QUOTE;
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
	int sizeOfLexeme = -1;//according to the add char function, it first adds to the length, so the first char needs to be on the 0
	int nextToken=0;
	*nextChar = getNonBlankChar(file, *nextChar);
	switch (nextChar->tp_code) {

	case DOUBLE_QUOTE: //STRINGS
		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			*nextChar = getChar(file);
		} while (nextChar->tp_code != DOUBLE_QUOTE);
		sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
		*nextChar = getChar(file);
		nextToken = LITERAL_STRING;
		break;
	case QUOTE:
		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			*nextChar = getChar(file);
		} while (nextChar->tp_code != QUOTE);
		sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
		*nextChar = getChar(file);
		nextToken = LITERAL_CHAR;
		break;
	case LETTER_SMALL:
	case LETTER_BIG:
		if(nextChar->tp_code==LETTER_BIG){
			nextToken = IDENTIFIER_FUNCTION;
		}
		else if(nextChar->tp_code == LETTER_SMALL) {
			nextToken = IDENTIFIER;
		}
		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			*nextChar = getChar(file);

		} while (isLETTER(nextChar->tp_code)  || nextChar->tp_code == DIGIT);
		if (0==strcmp(nextLexeme, LITERAL_BOOL_TRUE_LEXEME)) {
			nextToken = RESERVED_BOOL_TRUE;
			break;
		}
		else if (0 == strcmp(nextLexeme, LITERAL_BOOL_FALSE_LEXEME)) {
			nextToken = RESERVED_BOOL_FALSE;
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
		else if ((0 == strcmp(nextLexeme, RESERVED_COMMENT_VARIANT3_LEXEME))||(0 == strcmp(nextLexeme, RESERVED_COMMENT_VARIANT2_LEXEME))||(0 == strcmp(nextLexeme, RESERVED_COMMENT_VARIANT1_LEXEME))) {
			*nextChar = getChar(file);
			if (lookup(nextChar->ch, nextLexeme, &sizeOfLexeme) != OPEN_PARENTESIS) {
				errorColor();
				printf("ERROR:COMMENT NOT FORMATED CORRECTLY %s", nextLexeme);
				normalColor();
				exit(ERROR_COMMENT_NOT_FORMATED_CORRECTLY);
			
			}
			*nextChar = getChar(file);
			while (nextChar->tp_code != CLOSE_PARENTESIS) {

				lookup(nextChar->ch, nextLexeme, &sizeOfLexeme);
				*nextChar = getChar(file);
			}
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			*nextChar = getChar(file);
			nextToken=RESERVED_COMMENT;

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
		

		break;
	case DIGIT:

		do {
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			*nextChar = getChar(file);
		} while (nextChar->tp_code == DIGIT);
		if (isLETTER(nextChar->tp_code)) {//error handling
			printf("ERROR: NUMBER_WITH_LETTERS_IN_IT, %s", nextLexeme);
			exit(ERROR_NUMBER_WITH_LETTERS_IN_IT);
		}
		if (nextChar->tp_code == POINT) {//decimal handling
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
	case POINT:
		sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
		nextToken = POINT;
		*nextChar = getChar(file);
		break;
	case UNKNOWN:
		nextToken = lookup(nextChar->ch, nextLexeme, &sizeOfLexeme);
		*nextChar = getChar(file);
		if (nextToken == OP_MINOR) {
			if (nextChar->ch == '=') {
				nextToken = OP_MINOR_EQUAL;
			}
			if (nextChar->ch == '<') {
				nextToken = OP_ATTRIBUTION;
			}
			sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			*nextChar = getChar(file);
		}
		else if (nextToken == OP_BIGGER) {
			if (nextChar->ch == '=') {
				nextToken = OP_BIGGER_EQUAL;
				sizeOfLexeme = addChar(nextLexeme, sizeOfLexeme, nextChar->ch);
			}
			*nextChar = getChar(file);
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
	int nextToken=UNKNOWN;
	switch (ch) {
	case '(':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OPEN_PARENTESIS;
		break;
	case ')':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = CLOSE_PARENTESIS;
		break;
	case '[':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OPEN_BRACKETS;
		break;
	case ']':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = CLOSE_BRACKETS;
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
	case ';':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = POINT_COMMA;
		break;
	case ',':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = COMMA;
		break;
	case '<':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_MINOR;
		break;
	case '>':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_BIGGER;
		break;
	case '=':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_EQUAL;
		break;
	case '&':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_AND;
		break;
	case '|':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_OR;
		break;
	case '!':
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = OP_NOT;
		break;
	default:
		*lexemeLength = addChar(lexeme, *lexemeLength, ch);
		nextToken = EOF;
		break;
	}
	return nextToken;
}

