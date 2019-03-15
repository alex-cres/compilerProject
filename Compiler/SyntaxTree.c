#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SyntaxTree.h"
#include "ErrorHandling.h"



Node * createNode(char * info, int type)
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

Node * addChildNode(Node * fatherNode, char * info, int type)
{
	int i = 0;

	while (fatherNode->kids[i] != NULL && i < MAX_CHILDREN)
	{
		i++;
	}
	if (i < MAX_CHILDREN) {
		fatherNode->kids[i] = createNode(info, type);;
	}
	else {
		errorColor();
		printf("ERROR: MAX CHILDREN REACHED");
		normalColor();
		exit(ERROR_MAX_CHILDREN_REACHED);

	}

	return fatherNode->kids[i];
}

void printTree(Node * tree, int tabDepth)
{
	int i = 0;
	for (i = 0; i < tabDepth; i++)
	{
		printf("\t");
	}
	printf("%s\n", tree->info);
	for (i = 0; (i < MAX_CHILDREN && tree->kids[i] != NULL); i++)
	{
		printTree(tree->kids[i], tabDepth + 1);
	}
}
