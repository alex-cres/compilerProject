#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ErrorHandling.h"
#include "SyntaxTree.h"




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

Node * addChildNode(Node * fatherNode, char * info, int type)
{
	int i = 0;

	while (fatherNode->kids[i] != NULL && i < MAX_CHILDREN)
	{
		i++;
	}
	if (i < MAX_CHILDREN) {
		fatherNode->kids[i] = generateNode(info, type);;
	}
	else {
		errorColor();
		printf("ERROR: MAX CHILDREN REACHED");
		normalColor();
		exit(ERROR_MAX_CHILDREN_REACHED);

	}

	return fatherNode->kids[i];
}


void printTree(Node * tree, int tabDepth,int lastChild,int *arrayDepthTab)
{
	int i = 0;
	for (i = 0; i < tabDepth-1; i++)
	{
		int last = arrayDepthTab[i+1];
		if (!last) {
			printf("|");
		}
		else {
			printf(" ");
		}
		printf(" ");
	}
	if(tabDepth!=0 && !lastChild)
		printf("%c%c",195,196);
	if (tabDepth != 0 && lastChild)
		printf("%c%c", 192, 196);
	printf("%s\n", tree->info);
	
	arrayDepthTab[tabDepth] = lastChild;
	
	for (i = 0; (i < MAX_CHILDREN && tree->kids[i] != NULL); i++)
	{
		printTree(tree->kids[i], tabDepth + 1,(tree->kids[i+1]==NULL), arrayDepthTab);
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