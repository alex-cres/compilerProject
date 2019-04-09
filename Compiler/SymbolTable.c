#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "SymbolTable.h"
#include "LexicalAnalyser.h"
#include "ErrorHandling.h"


SymbolToken* generateSymbolTable(Node* tree, SymbolToken* table, FILE * logfile)
{
	int i = 0;
	SymbolToken* syntemp = table;
	SymbolToken* syntempnext = NULL;
	while (tree->kids[i] != NULL && i < MAX_CHILDREN)
	{
		generateSymbolTable(tree->kids[i], table, logfile);
		i++;
	}
	while (syntemp->next!=NULL)
	{
		syntemp = syntemp->next;
	}
	if (tree->type == DECLARE)
	{
		if (tree->kids[1]->type == OP_ATTRIBUTION) {
			char* contents_chopped = tree->kids[1]->kids[0]->info + 1;

			if ((isNumeric(contents_chopped)&&tree->kids[1]->kids[0]->info[0]=='t')||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatoutnumber") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatoutdecimal") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatoutchar") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatoutstring") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatoutbool") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatinnumber") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatindecimal") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatinchar") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatinstring") ||
				0 == strcmp(tree->kids[1]->kids[0]->info, "formatinbool"))
			{
				errorColor();
				printf("\n\nError: Reserved Internal Variables Used %s\n", tree->kids[1]->kids[0]->info);
				fprintf(logfile, "\n\nError: Reserved Internal Variables Used %s\n",  tree->kids[1]->kids[0]->info);
				normalColor();
				exit(ERROR_RESERVED_VARIABLES_USED);
			}
			syntempnext = generateSymbolToken(tree->kids[0]->type, tree, tree->kids[1]->kids[0]->info, NULL, syntemp);

		}
		else if (tree->kids[1]->type == IDENTIFIER) {
			char* contents_chopped = tree->kids[1]->info + 1;
			if ((isNumeric(contents_chopped) && tree->kids[1]->info[0] == 't') ||
				0 == strcmp(tree->kids[1]->info, "formatoutnumber") ||
				0 == strcmp(tree->kids[1]->info, "formatoutdecimal") ||
				0 == strcmp(tree->kids[1]->info, "formatoutchar") ||
				0 == strcmp(tree->kids[1]->info, "formatoutstring") ||
				0 == strcmp(tree->kids[1]->info, "formatoutbool") ||
				0 == strcmp(tree->kids[1]->info, "formatinnumber") ||
				0 == strcmp(tree->kids[1]->info, "formatindecimal") ||
				0 == strcmp(tree->kids[1]->info, "formatinchar") ||
				0 == strcmp(tree->kids[1]->info, "formatinstring") ||
				0 == strcmp(tree->kids[1]->info, "formatinbool"))
			{
				errorColor();
				printf("\n\nError: Reserved Internal Variables Used %s\n", tree->kids[1]->kids[0]->info);
				fprintf(logfile, "\n\nError: Reserved Internal Variables Used %s\n", tree->kids[1]->kids[0]->info);
				normalColor();
				exit(ERROR_RESERVED_VARIABLES_USED);
			}
			syntempnext = generateSymbolToken(tree->kids[0]->type, tree, tree->kids[1]->info, NULL, syntemp);
		}
	}
	return syntemp;
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
	if (previous!=NULL) {
		previous->next = token;
	}
	return token;

}

SymbolToken* insertSymbolToken(int type_of_symbol, Node * scope, char* name, SymbolToken*table)
{
	SymbolToken* token = (SymbolToken*)malloc(sizeof(SymbolToken));
	token->type_of_symbol = type_of_symbol;
	token->scope = scope;
	token->next = NULL;
	SymbolToken* syntemp = table;
	while (syntemp->next != NULL)
	{
		syntemp = syntemp->next;
	}
	token->previous = syntemp;


	token->name = (char*)malloc(sizeof(char*)*(strlen(name) + 1));
	strcpy(token->name, name);
	if (syntemp != NULL) {
		syntemp->next = token;
	}
	return token;

}

void printSymbolTable(SymbolToken* table, FILE * logfile)
{
	if (table->type_of_symbol==TABLE) {
		printf("| Symbol | Scope \t | Name\n");
		fprintf(logfile, "| Symbol | Scope \t | Name\n");
	}
	printf("| %s \t | %s \t | %s\n", tokenDescription(table->type_of_symbol), table->scope->info, table->name);
	fprintf(logfile, "| %d \t | %s \t | %s\n", table->type_of_symbol, table->scope->info, table->name);
	if(table->next !=NULL)
		printSymbolTable(table->next, logfile);
}

int isNumeric(const char * s)
{
	if (s == NULL || *s == '\0' || isspace(*s))
		return 0;
	char * p;
	strtod(s, &p);
	return *p == '\0';
}