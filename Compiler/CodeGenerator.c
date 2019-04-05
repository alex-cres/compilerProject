#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "CodeGenerator.h"

int registerCounter = 1;


Element * CreateList()
{
	return NULL;
}

Element * InsertList(Element * list, char * code)
{
	Element * elem = list;
	Element * nelem = (Element*)malloc(sizeof(Element));
	nelem->code = (char*)malloc(strlen(code)+1);
	strcpy(nelem->code, code);
	nelem->next = NULL;
	if (elem==NULL) {
		nelem->prior = NULL;
		return nelem;
	}
	else {
		while (elem->next!=NULL) {
			elem = elem->next;
		}
		elem->next = nelem;
		nelem->prior = elem;
	}
	return list;
}

void GenerateMachineCode(Node * ast, FILE* logFile, char * originalfilename)
{
	FILE*asmFile = NULL;
	Element * datacode = CreateList();
	Element * progcode = CreateList();
	char lofFileName[600] = "";
	strcat(lofFileName, originalfilename);
	strcat(lofFileName, "_code.asm");
	asmFile = fopen(lofFileName, "w");


	char cmd1[1024];
	char cmd2[1024];
	datacode = InsertList(datacode, "SECTION .data\n");
	datacode = InsertList(datacode, "	formatoutnumber: db \"%d\",10\n");
	datacode = InsertList(datacode, "	formatoutdecimal: db \"%f\",10\n");
	datacode = InsertList(datacode, "	formatoutchar: db \"%c\",10\n");
	datacode = InsertList(datacode, "	formatoutstring: db \"%s\",10\n");
	datacode = InsertList(datacode, "	formatoutbool: db \"%s\",10\n");

	progcode = InsertList(progcode, "SECTION .text\n");
	progcode = InsertList(progcode, "	global _main\n");
	progcode = InsertList(progcode, "	extern _printf\n");
	progcode = InsertList(progcode, "	_main:\n");
	
	GenerateIntermidiateCode(ast,logFile,datacode,progcode);
	Element*data = datacode;
	Element*text = progcode;
	while (data!=NULL)
	{
		fprintf(asmFile,"%s",data->code);
		data = data->next;
	}
	while (text != NULL)
	{
		fprintf(asmFile, "%s", text->code);
		text = text->next;
	}
	//sprintf(cmd1, "C:\\MinGW\\bin\\nasm.exe -f win32 %s", lofFileName);
	//system(cmd1);
	
//	sprintf(cmd2, "C:\\MinGW\\bin\\gcc.exe %s_code.obj %s_code.exe", originalfilename, originalfilename);
	//system(cmd2);
}

void GenerateIntermidiateCode(Node * ast, FILE* logFile, Element* datacode, Element* progcode)
{
	int i = 0;
	int regsub = 0;
	if (ast == NULL) {
		return;
	}
	while (ast->kids[i]!=NULL && i<MAX_CHILDREN)
	{
		GenerateIntermidiateCode(ast->kids[i],logFile,datacode,progcode);
		i++;
	}
	if (ast->type == OP_ADD || ast->type == OP_MINUS || ast->type == OP_MUL || ast->type == OP_DIV) {
		if (ast->kids[0]->type==LITERAL_NUMBER && ast->kids[1]->type == LITERAL_NUMBER)
		{
			printf("mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
			printf("mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
		} 
		else if(ast->kids[0]->type==LITERAL_NUMBER)
		{
			printf("mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
			if (ast->kids[1]->type==IDENTIFIER) {
				printf("mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
			}
			else {
				regsub++;
				printf("mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);
			}
			
		}
		else if (ast->kids[1]->type == LITERAL_NUMBER)
		{
			if (ast->kids[0]->type==IDENTIFIER) {
				printf("mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
			}
			else {
				regsub++;
				printf("mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - regsub);
			}

			printf("mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
		}
		else {
			if (ast->kids[0]->type == IDENTIFIER) {
				printf("mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
			}
			else {
				regsub++;
				printf("mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - regsub);
			}
			if (ast->kids[1]->type == IDENTIFIER) {
				printf("mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
			}
			else {
				regsub++;
				printf("mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);
			}
			
		}

		//Operation
		switch (ast->type)
		{
		case OP_ADD:
			printf("add eax, ebx ; Adding First and Second Operand Number\n");
			break;
		case OP_MINUS:
			printf("sub eax, ebx ; Subtracting First and Second Operand Number\n");
			break;
		case OP_MUL:
			printf("imul eax, ebx ; Multiplying First and Second Operand Number\n");
			break;
		case OP_DIV:
			printf("idiv ebx ; Dividing First and Second Operand Number\n");
			break;
		}
		/*
				mov eax, number1
				mov ebx, number2
				add eax, ebx //sub//imul   // idiv ebx
				mov dword[t1], eax
			*/
		printf("mov dword[t%d] , eax ;Result ", registerCounter);
		registerCounter++;
	}
	else if (ast->type==OP_ATTRIBUTION)
	{
		if (ast->kids[1]->type == LITERAL_NUMBER) {
			
			printf("mov eax, %s\n", ast->kids[1]->info); 
		}
		else {
			printf("mov eax, dword[%d]\n", registerCounter-1);
		}
		printf("mov dword[%s], eax\n", ast->kids[0]->info);
	}
	else if (ast->type == DECLARE)
	{
		if (ast->kids[0]->type== IDN_NUMBER)
		{
			if (ast->kids[1]->type==OP_ATTRIBUTION) {
				printf("%s : dd 0\n", ast->kids[1]->kids[0]->info);
			}
			else if (ast->kids[1]->type==IDENTIFIER) {
				printf("%s : dd 0\n", ast->kids[1]->info);
				
			}
			
		}
	}
	else if (ast->type == RESERVED_ON) {
		if (ast->kids[0]->type== RESERVED_CONSOLE) {
		if (ast->kids[1]->type == IDENTIFIER)
		{
			printf("push dword[%s]\n", ast->kids[1]->info);
			printf("push formatoutint; push message into ESP\n");
			//TODO: IDENTIFY THE TYPE OF THE VARIABLE WITH THE 

		}else if (ast->kids[1]->type== LITERAL_NUMBER)
		{
			printf("push %s\n", ast->kids[1]->info);
			printf("push formatoutint; push format into ESP\n");
		}
		else if (ast->kids[1]->type == LITERAL_DECIMAL)
		{
			printf("push %s\n", ast->kids[1]->info);
			printf("push formatoutdecimal; push format into ESP\n");
		}
		else if (ast->kids[1]->type == LITERAL_STRING)
		{
			printf("push %s\n", ast->kids[1]->info);
			printf("push formatoutstring; push format into ESP\n");
		}
		else if (ast->kids[1]->type == LITERAL_CHAR)
		{
			printf("push %s\n", ast->kids[1]->info);
			printf("push formatoutchar; push format into ESP\n");
		}

		else if (ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_FALSE)
		{
			printf("push \"%s\"\n", ast->kids[1]->info);
			printf("push formatoutbool; push format into ESP\n");
		}
		else {
			printf("push dword[t%d]\n", registerCounter-1);
			//TODO: FIND OUT THE TYPE OF THE tN in the SYMBOL TABLE
			printf("push formatoutnumber; push format into ESP\n");
		}

			printf("call _printf; call defined function\n");
			printf("add esp, 8; params * 4\n");
		}
	}
}
