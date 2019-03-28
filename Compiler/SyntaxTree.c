#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ErrorHandling.h"
#include "SyntaxTree.h"
#include "LexicalAnalyser.h"




Node * generateNode(char * info, int type)
{
	int i;
	Node * p = (Node*)malloc(sizeof(Node));
	p->info = (char*)malloc(sizeof(char*)*(strlen(info) + 1));
	strcpy(p->info, info);
	p->type = type;
	for (i = 0; i < MAX_CHILDREN; i++)
	{
		p->kids[i] = NULL;
	}
	return p;
}

Node* addChildNode(Node* fatherNode, char* info, int type, FILE* logFile)
{
	int i = 0;

	while (fatherNode->kids[i] != NULL && i < MAX_CHILDREN)
	{
		i++;
	}
	if (i < MAX_CHILDREN) {
		fatherNode->kids[i] = generateNode(info, type);
	}
	else {
		errorColor();
		printf("ERROR: MAX CHILDREN REACHED");
		fprintf(logFile,"ERROR: MAX CHILDREN REACHED");
		normalColor();
		exit(ERROR_MAX_CHILDREN_REACHED);

	}

	return fatherNode->kids[i];
}


void printTree(Node * tree, int tabDepth,int lastChild,int* arrayDepthTab, FILE * logFile)
{
	int i = 0;
	for (i = 0; i < tabDepth-1; i++)
	{
		int last = arrayDepthTab[i+1];
		if (!last) {
			printf("%c",179);
			fprintf(logFile,"|");
		}
		else {
			printf(" ");
			fprintf(logFile," ");
		}
		printf(" ");
		fprintf(logFile," ");
	}
	if (tabDepth != 0 && !lastChild) {
	//	printf("%c%c", 195, 196);
	//	fprintf(logFile,"%c%c", 195, 196);
	//	printf("%c%c%c", 195, 196,196);
    //	fprintf(logFile,"%c%c%c", 195, 196,196);
		printf("%c", 195);
		fprintf(logFile,"|-");
	}
	if (tabDepth != 0 && lastChild) {
	//	printf("%c%c", 192, 196);
	//	fprintf(logFile,"%c%c", 192, 196);
	//	printf("%c%c%c", 192, 196, 196);
	//	fprintf(logFile,"%c%c%c", 192, 196, 196);
		printf("%c", 192);
		fprintf(logFile,"|_");
	}
	printf("%s\n", tree->info);
	fprintf(logFile,"%s\n", tree->info);
	
	arrayDepthTab[tabDepth] = lastChild;
	
	for (i = 0; (i < MAX_CHILDREN && tree->kids[i] != NULL); i++)
	{
		printTree(tree->kids[i], tabDepth + 1,(tree->kids[i+1]==NULL), arrayDepthTab, logFile);
	}
}


int getTreeMaxDepth(Node * tree,int initialDepth)
{
	int max = 0,i=0;
	if (tree->kids[0]==NULL) {
		return initialDepth;
	}
	for (i = 0; (i < MAX_CHILDREN && tree->kids[i] != NULL); i++)
	{
		int childmax = getTreeMaxDepth(tree->kids[i], initialDepth+1);
		if (max<childmax) {
			max = childmax;
		}
	}
	return max;
}

Node * CSTtoAST(Node * current, FILE* logFile)
{	
	
	int numberOfKidsFound = 0;
	int j = 0;
	if (current==NULL)
		return NULL;
	printf("Entering %s\n", current->info);
	fprintf(logFile, "Entering %s\n", current->info);
	//Subir nos que so têm um filho
	//Tirar detalhes sintaticos
	//Subir operadores 
	//Subir operadores

	//recursivity for the cst to ast
	while (numberOfKidsFound < MAX_CHILDREN && current->kids[numberOfKidsFound] != NULL)
	{
		current->kids[numberOfKidsFound] = CSTtoAST(current->kids[numberOfKidsFound], logFile);
		numberOfKidsFound++;
	}
	/*Reorganizar nodes*/

	while (j<numberOfKidsFound-1)
	{
		if (current->kids[j] == NULL) {
			current->kids[j] = current->kids[j+1];
			current->kids[j+1] = NULL;
		}
		j++;
	}
	reorderTree(current);
	//Raise ops 
	numberOfKidsFound = 0;
	while (numberOfKidsFound < MAX_CHILDREN && current->kids[numberOfKidsFound] != NULL)
	{
		if (current->kids[numberOfKidsFound]->type== OP_ATTRIBUTION ||
			current->kids[numberOfKidsFound]->type == OP_ADD ||
			current->kids[numberOfKidsFound]->type == OP_MINUS ||
			current->kids[numberOfKidsFound]->type == OP_MUL ||
			current->kids[numberOfKidsFound]->type == OP_DIV ||
			current->kids[numberOfKidsFound]->type == OP_MOD ||
			current->kids[numberOfKidsFound]->type == OP_NOT ||
			current->kids[numberOfKidsFound]->type == OP_XOR ||
			current->kids[numberOfKidsFound]->type == OP_MINOR ||
			current->kids[numberOfKidsFound]->type == OP_BIGGER ||
			current->kids[numberOfKidsFound]->type == OP_BIGGER_EQUAL ||
			current->kids[numberOfKidsFound]->type == OP_MINOR_EQUAL ||
			current->kids[numberOfKidsFound]->type == OP_NOT_EQUAL ||
			current->kids[numberOfKidsFound]->type == OP_EQUAL ||
			current->kids[numberOfKidsFound]->type == OP_AND ||
			current->kids[numberOfKidsFound]->type == OP_OR ||
			current->kids[numberOfKidsFound]->type == RESERVED_EXIT_FUNCTION ||
			current->kids[numberOfKidsFound]->type == RESERVED_CALL_FUNCTION ||
			current->kids[numberOfKidsFound]->type == RESERVED_IF ||
			current->kids[numberOfKidsFound]->type == RESERVED_FOR ||
			current->kids[numberOfKidsFound]->type == RESERVED_LOOP ||
			current->kids[numberOfKidsFound]->type == RESERVED_IN ||
			current->kids[numberOfKidsFound]->type == RESERVED_ON ||
			0==strcmp(current->kids[numberOfKidsFound]->info , "EOF") ||			
			current->kids[numberOfKidsFound]->type == RESERVED_CAST_NUMBER ||
			current->kids[numberOfKidsFound]->type == RESERVED_CAST_CHAR ||
			current->kids[numberOfKidsFound]->type == RESERVED_CAST_BOOL ||
			current->kids[numberOfKidsFound]->type == RESERVED_CAST_STRING ||
			current->kids[numberOfKidsFound]->type == RESERVED_CAST_DECIMAL
			){

			if (current->kids[numberOfKidsFound]->kids[0]==NULL)
			{
				current->info = (char*)malloc(sizeof(char)*(1 + strlen(current->kids[numberOfKidsFound]->info)));
				strcpy(current->info, current->kids[numberOfKidsFound]->info);
				current->type = current->kids[numberOfKidsFound]->type;
				free(current->kids[numberOfKidsFound]);
				current->kids[numberOfKidsFound] = NULL;
			}

		
		}
		numberOfKidsFound++;
	}
	
	reorderTree(current);

	//remove non needed symbols like brackets etc
	if (current->type == OPEN_PARENTESIS ||
		current->type == CLOSE_PARENTESIS ||
		current->type == OPEN_BRACKETS ||
		current->type == CLOSE_BRACKETS ||
		current->type == POINT_COMMA ||
		current->type == COMMA ||
		current->type == POINT ||
		current->type == RESERVED_COMMENT
		) {
		free(current);
		return NULL;
	}
	reorderTree(current);
	//raise one child only with non special syntax
	if (current->type==-1) {
		int i = 0;
		int foundKids = 0;
		int foundWhere = 0;
		for (i=0;i<MAX_CHILDREN;i++)
		{
			if (current->kids[i] != NULL) {
				foundKids++; foundWhere = i;
			}
		}
		if (foundKids == 1) {
			
				current->info = (char*)malloc(sizeof(char)*(1 + strlen(current->kids[foundWhere]->info)));
				strcpy(current->info, current->kids[foundWhere]->info);
				current->type = current->kids[foundWhere]->type;
				Node * c = current->kids[foundWhere];
				int re = 0;
				for (re = 0; re < MAX_CHILDREN;re++) {
					current->kids[re] = c->kids[re];
				}
				
			
		}else if(foundKids==0){
			return NULL;
		}
	}
	reorderTree(current);
	
	printf("Exiting %s\n", current->info);
	fprintf(logFile, "Exiting %s\n", current->info);
	return current;
}

void reorderTree(Node * current){

	/*Reorganizar nodes*/
	int j = 0;
	int i = 0;
	while (i < MAX_CHILDREN - 1)
	{
		while (j < MAX_CHILDREN - 1)
		{
			if (current->kids[j] == NULL) {
				current->kids[j] = current->kids[j + 1];
				current->kids[j + 1] = NULL;
			}
			j++;
		}
		i++;
	}
}