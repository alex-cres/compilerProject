#pragma once

#define MAX_CHILDREN 128


typedef struct node
{
	char* info;
	int type;
	struct node* kids[MAX_CHILDREN];
} Node;

Node* generateNode(char* info, int type);

Node* addChildNode(Node* fatherNode, char* info, int type, FILE* logFile);

void printTree(Node * tree, int tabDepth,int lastChild, int* arrayDepthTab, FILE * logFile);

int getTreeMaxDepth(Node * tree);




//Subir nos que so têm um filho
//Tirar detalhes sintaticos
//Subir operadores 
//Subir operadores 

Node * CSTtoAST(Node * current);
void reorderTree(Node * current);
