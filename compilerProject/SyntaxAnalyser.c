#include "SyntaxAnalyser.h"
#include <stdio.h>
//used to get the next char on the file
NextChar getChar(FILE * file)
{
	NextChar nextCh;
	//reads the next char on the file and classifies it
	if (nextCh.value=getc(file)!=EOF) {
		if(isdigit(nextCh.value))
			nextCh.type = DIGIT;
		else if(isupper(nextCh.value))
			nextCh.type = ALPHA_BIG;
		else if (islower(nextCh.value))
			nextCh.type = ALPHA_SMALL;
		else
			nextCh.type = UNKNOWN;
	}
	else {
		nextCh.type = EOF;
	}

	return nextCh;
}
//used to get the next non whitespace char on the file
NextChar getNonBlankChar(FILE * file, NextChar nextChar)
{
	while (isspace(nextChar.value))
	{
		nextChar = getChar(file);
	}
	return nextChar;
		
}

int addChar(char * lexeme, int lexemeLenght, char ch)
{
	if (lexemeLenght<MAX_LEXEME_SIZE)
	{
		lexemeLenght++;
		lexeme[lexemeLenght] = ch;
		lexeme[lexemeLenght + 1] = 0;
		return lexemeLenght;
	} 
	else
	{
		return -1;
	}
	
}

int lookup(char ch, char * lexeme, int lexemeLength)
{
	return 0;
}

int lex(FILE * file, NextChar * nextChar, char * nextLexeme)
{
	return 0;
}


void addLexemeToEndOfList(Lexeme * lexemeList, Lexeme * lexeme) {
	Lexeme * pointer = lexemeList;
	if (lexemeList == NULL) {
		lexemeList = lexeme;
	}
	while (pointer->next != NULL)
	{
		pointer = pointer->next;
	}
	pointer->next = lexeme;
}