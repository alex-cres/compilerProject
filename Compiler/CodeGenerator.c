#include <stdio.h>
#include "CodeGenerator.h"

int registerCounter = 1;


void GenerateMachineCode(Node * ast, FILE* logFile)
{
	GenerateIntermidiateCode(ast,logFile);
}

void GenerateIntermidiateCode(Node * ast, FILE* logFile)
{
	int i = 0;
	if (ast == NULL) {
		return;
	}
	while (ast->kids[i]!=NULL && i<MAX_CHILDREN)
	{
		GenerateIntermidiateCode(ast->kids[i],logFile);
		i++;
	}
	if (ast->type == OP_ADD || ast->type == OP_MINUS || ast->type == OP_MUL || ast->type == OP_DIV) {
	/*
	
		mov eax, 4
		mov ebx, 2
		add eax, ebx
		mov dword[variable], eax
	*/
	}
}
