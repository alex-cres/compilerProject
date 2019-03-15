#pragma once

#define MAX_CHILDREN 128


typedef struct node
{
	char* info;
	int type;
	struct node* kids[MAX_CHILDREN];
} Node;

Node* generateNode(char* info, int type);

Node* addChildNode(Node* fatherNode, char* info, int type);

void printTree(Node * tree, int tabDepth,int lastChild);

int getTreeMaxDepth(Node * tree);
