#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SymbolTable.h"


SymbolToken* generateSymbolTable(Node* tree, SymbolToken* next, SymbolToken* previous, FILE * logfile)
{
	SymbolToken * table = generateSymbolToken(TABLE, tree, "TABLE",next,previous);
	return table;
}

SymbolToken* generateSymbolToken(int type_of_symbol, Node * scope, char* name, SymbolToken* next, SymbolToken* previous)
{
	SymbolToken* token =(SymbolToken*) malloc(sizeof(SymbolToken));
	token->type_of_symbol = type_of_symbol;
	token->scope = scope;
	token->next = next;
	token->previous = previous;
	token->name = (char*)malloc(sizeof(char*)*(strlen(name) + 1));
	strcpy(token->name, name);
	
	return token;

}

void printSymbolTable(SymbolToken* table, FILE * logfile)
{
	if (table->type_of_symbol==TABLE) {
		printf("| Symbol | Scope \t | Name\n");
		fprintf(logfile, "| Symbol | Scope \t | Name\n");
	}
	printf("| %d \t | %s \t | %s\n",table->type_of_symbol, table->scope->info, table->name);
	fprintf(logfile, "| %d \t | %s \t | %s\n", table->type_of_symbol, table->scope->info, table->name);
	if(table->next !=NULL)
		printSymbolTable(table->next, logfile);
}

