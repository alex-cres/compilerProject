#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "CodeGenerator.h"

int registerCounter = 1;

typedef int bool;
#define True 1
#define False 0

Element * CreateList()
{
	return NULL;
}

Element * InsertList(Element * list, char * code)
{
	Element * elem = list;
	Element * nelem = (Element*)malloc(sizeof(Element));
	nelem->code = (char*)malloc(strlen(code) + 1);
	strcpy(nelem->code, code);
	nelem->next = NULL;
	if (elem == NULL) {
		nelem->prior = NULL;
		return nelem;
	}
	else {
		while (elem->next != NULL) {
			elem = elem->next;
		}
		elem->next = nelem;
		nelem->prior = elem;
	}
	return list;
}

void GenerateMachineCode(Node * ast, FILE* logFile, char*originalfilename, SymbolToken * table)
{
	int tvariables = 0;
	FILE*asmFile = NULL;
	Element * datacode = CreateList();
	Element * progcode = CreateList();
	char lofFileName[600] = "";
	strcat(lofFileName, originalfilename);
	lofFileName[strlen(lofFileName) - 3] = 0;
	strcat(lofFileName, ".asm");
	asmFile = fopen(lofFileName, "w");


	char cmd1[1024];
	char cmd2[1024];
	datacode = InsertList(datacode, "SECTION .data\n");
	datacode = InsertList(datacode, "	FALSE equ 0 \n");
	datacode = InsertList(datacode, "	TRUE equ 1 \n");
	datacode = InsertList(datacode, "	formatoutnumber: db \"%d\", 0\n");
	datacode = InsertList(datacode, "	formatoutdecimal: db \"%f\", 0\n");
	datacode = InsertList(datacode, "	formatoutchar: db \"%c\", 0\n");
	datacode = InsertList(datacode, "	formatoutstring: db \"%s\", 0\n");
	datacode = InsertList(datacode, "	formatoutbool: db \"%s\", 0\n\n");
	datacode = InsertList(datacode, "	formatinnumber: db \"%d\", 0\n");
	datacode = InsertList(datacode, "	formatindecimal: db \"%f\", 0\n");
	datacode = InsertList(datacode, "	formatinchar: db \"%c\", 0\n");
	datacode = InsertList(datacode, "	formatinstring: db \"%s\", 0\n");
	datacode = InsertList(datacode, "	formatinbool: db \"%d\", 0\n\n");

	insertSymbolToken(IDN_STRING, ast,"formatoutnumber", table);
	insertSymbolToken(IDN_STRING, ast,"formatoutdecimal", table);
	insertSymbolToken(IDN_STRING, ast,"formatoutchar", table);
	insertSymbolToken(IDN_STRING, ast,"formatoutstring", table);
	insertSymbolToken(IDN_STRING, ast,"formatoutbool", table);
	insertSymbolToken(IDN_STRING, ast,"formatinnumber", table);
	insertSymbolToken(IDN_STRING, ast,"formatindecimal", table);
	insertSymbolToken(IDN_STRING, ast,"formatinchar", table);
	insertSymbolToken(IDN_STRING, ast,"formatinstring", table);
	insertSymbolToken(IDN_STRING, ast,"formatinbool", table);
	
	progcode = InsertList(progcode, "SECTION .text\n");
	progcode = InsertList(progcode, "	global _main\n");
	progcode = InsertList(progcode, "	extern _printf\n");
	progcode = InsertList(progcode, "	extern _scanf\n");
	progcode = InsertList(progcode, "	_main:\n\n");

	GenerateIntermidiateCode(ast, logFile, datacode, progcode, table);
	progcode = InsertList(progcode, "	ret\n\n");
	datacode = InsertList(datacode, "\n");
	datacode = InsertList(datacode, "\n");
	Element*data = datacode;
	Element*text = progcode;
	while (data != NULL)
	{
		printf("%s", data->code);
		fprintf(asmFile, "%s", data->code);
		data = data->next;
	}
	while (text != NULL)
	{
		printf("%s", text->code);
		fprintf(asmFile, "%s", text->code);
		text = text->next;
	}
	fclose(asmFile);
	char directory[1800];
	char fileCreator[2000];
	_getcwd(directory,sizeof(directory));
	_chdir("C:\\MinGW\\bin\\");
	strcat(directory, "\\");
	strcpy(fileCreator,directory);
	strcat(fileCreator, lofFileName);
	sprintf(cmd1, "nasm -f win32 %s", fileCreator);
	system(cmd1);
	fprintf(logFile,"Object File Created : %s\n", cmd1);
	printf("Object File Created : %s\n", cmd1);

	strcpy(fileCreator, directory);
	strcat(fileCreator, lofFileName);

	fileCreator[strlen(fileCreator) - 4] = 0;
	sprintf(cmd2, "gcc %s.obj -o %s.exe", fileCreator, fileCreator);
	system(cmd2);
	fprintf(logFile, "Exe File Created : %s\n", cmd2);
	printf("Exe File Created : %s\n", cmd2);
}

void GenerateIntermidiateCode(Node * ast, FILE* logFile, Element* datacode, Element* progcode, SymbolToken * table)
{
	int i = 0;
	int regsub = 0;
	bool floatingpointed = False;
	char buffer[1000];
	if (ast == NULL) {
		return;
	}
	while (ast->kids[i] != NULL && i < MAX_CHILDREN)
	{
		GenerateIntermidiateCode(ast->kids[i], logFile, datacode, progcode, table);
		i++;
	}
	if (ast->type == OP_ADD || ast->type == OP_MINUS || ast->type == OP_MUL || ast->type == OP_DIV) {// http://www.website.masmforum.com/tutorials/fptute/fpuchap5.htm // integer in float op
		char bufferdec[100];
		sprintf(bufferdec, "t%d", registerCounter);
		if (ast->kids[0]->type == LITERAL_NUMBER && ast->kids[1]->type == LITERAL_NUMBER)
		{
			insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
			strcpy(bufferdec, "");
			
			sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
			progcode = InsertList(progcode, buffer);
			strcpy(buffer,"");

			sprintf(buffer, "		mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
			progcode = InsertList(progcode, buffer);

			printf("		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
			fprintf(logFile, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
			printf("		mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
			fprintf(logFile, "		mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
		
		}
		else if (ast->kids[0]->type == LITERAL_DECIMAL && ast->kids[1]->type == LITERAL_DECIMAL)
		{
			insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
			strcpy(bufferdec, "");
			floatingpointed = True;

			
			strcpy(buffer, "");
			sprintf(buffer, "		fld	%s ; Moving First Operand Number\n", ast->kids[0]->info);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);

			strcpy(buffer, "");
			sprintf(buffer, "		fld	%s ; Moving Second Operand Number\n", ast->kids[1]->info);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		
		}
		else if (ast->kids[0]->type == LITERAL_NUMBER)
		{
			insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
			strcpy(bufferdec, "");
			strcpy(buffer, "");

			sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
			progcode = InsertList(progcode, buffer);
			strcpy(buffer, "");

			printf("		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
			fprintf(logFile, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);

			if (ast->kids[1]->type == IDENTIFIER) {

				sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);

				printf("		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
				fprintf(logFile, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
			}
			else {
				regsub++;
				/*char bufferdec[100];
				sprintf(bufferdec, "t%d", registerCounter - regsub);
				insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
				strcpy(bufferdec, "");
				strcpy(buffer, "");
				sprintf(buffer, "	t%d : dd 0 \n", registerCounter - regsub);
				datacode = InsertList(datacode, buffer);
				strcpy(buffer, "");
				*/
				sprintf(buffer, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);
				progcode = InsertList(progcode, buffer);
							   				
				printf("		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);
				fprintf(logFile, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);
			}

		}
		else if (ast->kids[1]->type == LITERAL_NUMBER)
		{
			insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
			strcpy(bufferdec, "");
			strcpy(buffer, "");

			if (ast->kids[0]->type == IDENTIFIER) {

				sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);

				printf("		mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
				fprintf(logFile, "		mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
			}
			else {
				regsub++;
				/*char bufferdec[100];
				sprintf(bufferdec, "t%d", registerCounter - regsub);
				insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(buffer, "	t%d : dd 0 \n", registerCounter - regsub);
				datacode = InsertList(datacode, buffer);*/
				strcpy(buffer, "");
				sprintf(buffer, "		mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - regsub);
				progcode = InsertList(progcode, buffer);

				printf("		mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - regsub);
				fprintf(logFile, "		mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - regsub);
			}


			strcpy(buffer, "");
			sprintf(buffer, "		mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
			progcode = InsertList(progcode, buffer);


			printf("		mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
			fprintf(logFile, "		mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
		}
		else {
				insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
				strcpy(bufferdec, "");
			strcpy(buffer, "");

			if (ast->kids[0]->type == IDENTIFIER) {

				sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);

				printf("		mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
				fprintf(logFile, "		mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
			}
			else {
				regsub++;
				strcpy(buffer, "");
				sprintf(buffer, "		mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - regsub);
				progcode = InsertList(progcode, buffer);

				printf("		mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - regsub);
				fprintf(logFile, "		mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - regsub);
			}
			strcpy(buffer, "");
			if (ast->kids[1]->type == IDENTIFIER) {

				sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);

				printf("		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
				fprintf(logFile, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
			}
			else {
				regsub++;

			/*char bufferdec[100];
				sprintf(bufferdec, "t%d", registerCounter - regsub);
				
				insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(buffer, "	t%d : dd 0 \n", registerCounter - regsub);
				datacode = InsertList(datacode, buffer);
			*/
			strcpy(buffer, "");
				sprintf(buffer, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);
				progcode = InsertList(progcode, buffer);

				printf("		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);
				fprintf(logFile, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);
			}

		}

		//Operation
		strcpy(buffer, "");
		if (!floatingpointed) {
			switch (ast->type)
			{
			case OP_ADD:
				progcode = InsertList(progcode, "		add eax, ebx ; Adding First and Second Operand Number\n");
				printf("		add eax, ebx ; Adding First and Second Operand Number\n");
				fprintf(logFile, "		add eax, ebx ; Adding First and Second Operand Number\n");
				break;
			case OP_MINUS:
				progcode = InsertList(progcode, "		sub eax, ebx ; Subtracting First and Second Operand Number\n");
				printf("		sub eax, ebx ; Subtracting First and Second Operand Number\n");
				fprintf(logFile, "		sub eax, ebx ; Subtracting First and Second Operand Number\n");
				break;
			case OP_MUL:
				progcode = InsertList(progcode, "		imul eax, ebx ; Multiplying First and Second Operand Number\n");
				printf("		imul eax, ebx ; Multiplying First and Second Operand Number\n");
				fprintf(logFile, "		imul eax, ebx ; Multiplying First and Second Operand Number\n");
				break;
			case OP_DIV:
				progcode = InsertList(progcode, "		idiv ebx ; Dividing First and Second Operand Number\n");
				printf("		idiv ebx ; Dividing First and Second Operand Number\n");
				fprintf(logFile, "		idiv ebx ; Dividing First and Second Operand Number\n");
				break;
			}
		}
		else {
			switch (ast->type)
			{
			case OP_ADD:
				progcode = InsertList(progcode, "		fadd ; Adding First and Second Operand Number\n");
				printf("		fadd ; Adding First and Second Operand Number\n");
				fprintf(logFile, "		fadd ; Adding First and Second Operand Number\n");
				break;
			case OP_MINUS:
				progcode = InsertList(progcode, "		fsub ; Subtracting First and Second Operand Number\n");
				printf("		fsub ; Subtracting First and Second Operand Number\n");
				fprintf(logFile, "		fsub  ; Subtracting First and Second Operand Number\n");
				break;
			case OP_MUL:
				progcode = InsertList(progcode, "		fmul ; Multiplying First and Second Operand Number\n");
				printf("		fmul ; Multiplying First and Second Operand Number\n");
				fprintf(logFile, "		fmul ; Multiplying First and Second Operand Number\n");
				break;
			case OP_DIV:
				progcode = InsertList(progcode, "		fdiv ; Dividing First and Second Operand Number\n");
				printf("		fdiv ; Dividing First and Second Operand Number\n");
				fprintf(logFile, "		fdiv ; Dividing First and Second Operand Number\n");
				break;
			}
		}
	


		
		strcpy(buffer, "");
		sprintf(buffer, "	t%d : dd 0 \n", registerCounter);
		datacode = InsertList(datacode, buffer);
		if (!floatingpointed) {
			strcpy(buffer, "");
			sprintf(buffer, "		mov dword[t%d] , eax ;Result \n\n", registerCounter);
		}
		else
		{
			strcpy(buffer, "");
			sprintf(buffer, "		fstp dword[t%d] ;Result \n\n", registerCounter);
		}

		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);

		registerCounter++;
	}
	else if (ast->type == OP_ATTRIBUTION)
	{
		if (ast->kids[1]->type == LITERAL_NUMBER ||	ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_TRUE ) {
			strupper(ast->kids[1]->info);
			strcpy(buffer, "");
			sprintf(buffer, "		mov eax, %s\n", ast->kids[1]->info);
			progcode = InsertList(progcode, buffer);

			printf("		mov eax, %s\n", ast->kids[1]->info);
			fprintf(logFile, "		mov eax, %s\n", ast->kids[1]->info);
		}
		else if (ast->kids[1]->type == LITERAL_DECIMAL) {
			char bufferdec[100];
			sprintf(bufferdec, "t%d", registerCounter);
			insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
			strcpy(bufferdec, "");
			strcpy(buffer, "");
			sprintf(buffer, "	t%d : db %s \n", registerCounter, ast->kids[1]->info);
			datacode = InsertList(datacode, buffer);
			strcpy(buffer, "");
			
			sprintf(buffer, "		fld	dword[t%d]; need to convert 32 - bit to 64 - bit\n", registerCounter);
			progcode = InsertList(progcode, buffer);

			printf(buffer);
			fprintf(logFile, buffer);
			
			
			registerCounter++;
		}
		else if (ast->kids[1]->type == LITERAL_CHAR || ast->kids[1]->type == LITERAL_STRING) {
			char bufferdec[100];
			sprintf(bufferdec, "t%d", registerCounter);
			insertSymbolToken(IDN_STRING, ast, bufferdec, table);
			strcpy(bufferdec, "");
			strcpy(buffer, "");
			sprintf(buffer, "	t%d : db `%s`,0 \n", registerCounter, ast->kids[1]->info);
			datacode = InsertList(datacode, buffer);
			strcpy(buffer, "");
			sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter);
			progcode = InsertList(progcode, buffer);

			printf("		mov eax, dword[t%d]\n", registerCounter);
			fprintf(logFile, "		mov eax, dword[t%d]\n", registerCounter);
			registerCounter++;

		}
		else {
			strcpy(buffer, "");
			sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter - 1);
			progcode = InsertList(progcode, buffer);

			printf("		mov eax, dword[t%d]\n", registerCounter - 1);
			fprintf(logFile, "		mov eax, dword[t%d]\n", registerCounter - 1);
		}

		if (ast->kids[1]->type ==LITERAL_DECIMAL) {
			strcpy(buffer, "");
			sprintf(buffer, "		fstp	dword[t%d]\n\n",registerCounter-1);
			progcode = InsertList(progcode, buffer);
			
		}
		else {
			strcpy(buffer, "");
			sprintf(buffer, "		mov dword[%s], eax\n\n", ast->kids[0]->info);
			progcode = InsertList(progcode, buffer);
		}
		printf(buffer);
		fprintf(logFile, buffer);
	}
	else if (ast->type == DECLARE)
	{
		if (ast->kids[0]->type == IDN_NUMBER ||  ast->kids[0]->type == IDN_CHAR || ast->kids[0]->type == IDN_BOOL)
		{
			strcpy(buffer, "");
			if (ast->kids[1]->type == OP_ATTRIBUTION) {
				sprintf(buffer, "	%s : dd 0\n", ast->kids[1]->kids[0]->info);
				datacode = InsertList(datacode, buffer);
				printf("	%s : dd 0\n", ast->kids[1]->kids[0]->info);
				fprintf(logFile, "	%s : dd 0\n", ast->kids[1]->kids[0]->info);
			}
			else if (ast->kids[1]->type == IDENTIFIER) {
				sprintf(buffer, "	%s : dd 0\n", ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);
				printf("	%s : dd 0\n", ast->kids[1]->info);
				fprintf(logFile, "	%s : dd 0\n", ast->kids[1]->info);

			}

		}
		else if (ast->kids[0]->type == IDN_DECIMAL)
		{
			strcpy(buffer, "");
			if (ast->kids[1]->type == OP_ATTRIBUTION) {
				sprintf(buffer, "	%s : dd %s\n", ast->kids[1]->kids[0]->info, ast->kids[1]->kids[1]->info);
				datacode = InsertList(datacode, buffer);
				printf("	%s : dd 0\n", ast->kids[1]->kids[0]->info);
				fprintf(logFile, "	%s : dd 0\n", ast->kids[1]->kids[0]->info);
			}
			else if (ast->kids[1]->type == IDENTIFIER) {
				sprintf(buffer, "	%s : dd 0\n", ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);
				printf("	%s : dd 0\n", ast->kids[1]->info);
				fprintf(logFile, "	%s : dd 0\n", ast->kids[1]->info);

			}

		}
		else if (ast->kids[0]->type == IDN_CHAR || ast->kids[0]->type == IDN_STRING) {

			strcpy(buffer, "");
			if (ast->kids[1]->type == OP_ATTRIBUTION) {
				sprintf(buffer, "	%s : db ``, 0\n", ast->kids[1]->kids[0]->info);
				datacode = InsertList(datacode, buffer);
				printf("	%s : db %s, 0\n", ast->kids[1]->kids[0]->info);
				fprintf(logFile, "	%s : db ``, 0\n", ast->kids[1]->kids[0]->info);
			}
			else if (ast->kids[1]->type == IDENTIFIER) {
				sprintf(buffer, "	%s : db ``, 0\n", ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);
				printf("	%s : db ``, 0\n", ast->kids[1]->info);
				fprintf(logFile, "	%s : db ``, 0\n", ast->kids[1]->info);

			}

			

		}
	}
	else if (ast->type == RESERVED_ON) {
		if (ast->kids[0]->type == RESERVED_CONSOLE) {
			int type_of_var = 0;
				
			if (ast->kids[1]->type == IDENTIFIER)
			{

				
				SymbolToken*current = table;
				while (current->next != NULL) {
					if (0 == strcmp(ast->kids[1]->info, current->name)) {
						type_of_var = current->type_of_symbol;
						break;
					}
					current = current->next;
				}
				switch (type_of_var)
				{
				case IDN_NUMBER:

					strcpy(buffer, "");
					sprintf(buffer, "		push dword[%s]\n", ast->kids[1]->info);
					progcode = InsertList(progcode, buffer);
					printf("		push dword[%s]\n", ast->kids[1]->info);
					fprintf(logFile, "		push dword[%s]\n", ast->kids[1]->info);

					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
					break;
				case IDN_DECIMAL:

					progcode = InsertList(progcode, "		sub esp, 8;reserve stack for a double in stack\n");
					printf("		sub esp, 8;reserve stack for a double in stack\n");
					fprintf(logFile, "		sub esp, 8;reserve stack for a double in stack\n");

					strcpy(buffer, "");
					sprintf(buffer, "		mov ebx, %s\n", ast->kids[1]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);

					progcode = InsertList(progcode, "		fld dword[ebx];load float\n");
					printf("		fld dword[ebx];load float\n");
					fprintf(logFile, "		fld dword[ebx];load float\n");

					progcode = InsertList(progcode, "		fstp qword[esp];store double(8087 does the conversion internally)\n");
					printf("		fstp qword[esp];store double(8087 does the conversion internally)\n");
					fprintf(logFile, "		fstp qword[esp];store double(8087 does the conversion internally)\n");

					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutdecimal; push message into ESP\n");
					break;
				case IDN_STRING:

					strcpy(buffer, "");
					sprintf(buffer, "		push dword[%s]\n", ast->kids[1]->info);
					progcode = InsertList(progcode, buffer);
					printf("		push dword[%s]\n", ast->kids[1]->info);
					fprintf(logFile, "		push dword[%s]\n", ast->kids[1]->info);

					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutstring; push message into ESP\n");
					break;
				case IDN_CHAR:

					strcpy(buffer, "");
					sprintf(buffer, "		push dword[%s]\n", ast->kids[1]->info);
					progcode = InsertList(progcode, buffer);
					printf("		push dword[%s]\n", ast->kids[1]->info);
					fprintf(logFile, "		push dword[%s]\n", ast->kids[1]->info);

					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutchar; push message into ESP\n");
					break;
				case IDN_BOOL:

					strcpy(buffer, "");
					sprintf(buffer, "		push dword[%s]\n", ast->kids[1]->info);
					progcode = InsertList(progcode, buffer);
					printf("		push dword[%s]\n", ast->kids[1]->info);
					fprintf(logFile, "		push dword[%s]\n", ast->kids[1]->info);

					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutbool; push message into ESP\n");
					break;

				default:

					strcpy(buffer, "");
					sprintf(buffer, "		push dword[%s]\n", ast->kids[1]->info);
					progcode = InsertList(progcode, buffer);
					printf("		push dword[%s]\n", ast->kids[1]->info);
					fprintf(logFile, "		push dword[%s]\n", ast->kids[1]->info);

					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
					break;
				}
				
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				strcpy(buffer, "");
				
			}
			else if (ast->kids[1]->type == LITERAL_NUMBER)
			{
				strcpy(buffer, "");
				sprintf(buffer, "	t%d : dd %s\n", registerCounter, ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		push dword[t%d]\n", registerCounter);
				progcode = InsertList(progcode, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
				progcode = InsertList(progcode, buffer);

				printf("		push dword[t%d]\n", registerCounter);
				fprintf(logFile, "		push dword[t%d]\n", registerCounter);
				printf("		push formatoutnumber; push format into ESP\n");
				fprintf(logFile, "		push formatoutnumber; push format into ESP\n");
				registerCounter++;
			}
			else if (ast->kids[1]->type == LITERAL_STRING)
			{
				char bufferdec[100];
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_STRING, ast, bufferdec, table);
				strcpy(bufferdec, "");
				strcpy(buffer, "");
				sprintf(buffer, "	t%d : db `%s`,0 \n", registerCounter, ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		push t%d\n", registerCounter);
				progcode = InsertList(progcode, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		push formatoutstring; push format into ESP\n");
				progcode = InsertList(progcode, buffer);

				printf("		push t%d\n", registerCounter );
				fprintf(logFile, "		push t%d]\n", registerCounter );
				printf("	t%d : db `%s`,0 \n", registerCounter, ast->kids[1]->info);
				printf("		push formatoutstring; push format into ESP\n");
				fprintf(logFile, "		push formatoutstring; push format into ESP\n");
				registerCounter++;
			}
			else if (ast->kids[1]->type == LITERAL_CHAR)
			{
				char bufferdec[100];
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_CHAR, ast, bufferdec, table);
				strcpy(bufferdec, "");
				strcpy(buffer, "");
				sprintf(buffer, "	t%d : db `%s`,0 \n", registerCounter, ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		push t%d\n", registerCounter );
				progcode = InsertList(progcode, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		push formatoutchar; push format into ESP\n");
				progcode = InsertList(progcode, buffer);

				printf("		push t%d\n", registerCounter);
				fprintf(logFile, "		push t%d\n", registerCounter);
				printf("	t%d : db `%s`,0 \n", registerCounter, ast->kids[1]->info);
				printf("		push formatoutchar; push format into ESP\n");
				fprintf(logFile, "		push formatoutchar; push format into ESP\n");
				registerCounter++;
			}
			else if (ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_TRUE)
			{
			strupper(ast->kids[1]->info);
				strcpy(buffer, "");
				sprintf(buffer, "		push %s\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		push formatoutbool; push format into ESP\n");
				progcode = InsertList(progcode, buffer);

				printf("		push %s\n", ast->kids[1]->info);
				fprintf(logFile, "		push %s\n", ast->kids[1]->info);
				printf("		push formatoutbool; push format into ESP\n");
				fprintf(logFile, "		push formatoutbool; push format into ESP\n");
			}
			else if (ast->kids[1]->type == LITERAL_DECIMAL)
			{

			
			char bufferdec[100];
			sprintf(bufferdec, "t%d", registerCounter);
			insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
			strcpy(bufferdec, "");
			strcpy(buffer, "");
			sprintf(buffer, "	t%d : dd %s\n", registerCounter, ast->kids[1]->info);
			datacode = InsertList(datacode, buffer);


			progcode = InsertList(progcode, "		sub esp, 8;reserve stack for a double in stack\n");
			printf("		sub esp, 8;reserve stack for a double in stack\n");
			fprintf(logFile, "		sub esp, 8;reserve stack for a double in stack\n");

			strcpy(buffer, "");
			sprintf(buffer, "		mov ebx, t%d\n", registerCounter);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			
			progcode = InsertList(progcode, "		fld dword[ebx];load float\n");
			printf("		fld dword[ebx];load float\n");
			fprintf(logFile, "		fld dword[ebx];load float\n");

			progcode = InsertList(progcode, "		fstp qword[esp];store double(8087 does the conversion internally)\n");
			printf("		fstp qword[esp];store double(8087 does the conversion internally)\n");
			fprintf(logFile, "		fstp qword[esp];store double(8087 does the conversion internally)\n");

			strcpy(buffer, "");
			sprintf(buffer, "		push formatoutdecimal; push format into ESP\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter++;
			}
			else {

				strcpy(buffer, "");
				sprintf(buffer, "		push dword[t%d]\n", registerCounter - 1);
				progcode = InsertList(progcode, buffer);
				printf("		push dword[t%d]\n", registerCounter - 1);
				fprintf(logFile, "		push dword[t%d]\n", registerCounter - 1);

				SymbolToken*current = table;
				while (current->next != NULL) {
					if (0 == strcmp(ast->kids[1]->info, current->name)) {
						type_of_var = current->type_of_symbol;
						break;
					}
					current = current->next;
				}
				switch (type_of_var)
				{
				case IDN_NUMBER:
					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
					break;
				case IDN_DECIMAL:
					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutdecimal; push message into ESP\n");
					break;
				case IDN_STRING:
					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutstring; push message into ESP\n");
					break;
				case IDN_CHAR:
					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutchar; push message into ESP\n");
					break;
				case IDN_BOOL:
					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutbool; push message into ESP\n");
					break;

				default:
					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
					break;
				}

				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				strcpy(buffer, "");

				
			}

			strcpy(buffer, "");
			sprintf(buffer, "		call _printf; call defined function\n");
			progcode = InsertList(progcode, buffer);
			printf("		call _printf; call defined function\n");
			fprintf(logFile, "		call _printf; call defined function\n");

			if (ast->kids[1]->type == LITERAL_DECIMAL || type_of_var == IDN_DECIMAL)
			{
				strcpy(buffer, "");
				sprintf(buffer, "		add esp, 12; params * 4\n\n");
				progcode = InsertList(progcode, buffer);
				printf("		add esp, 12; params * 4\n\n");
				fprintf(logFile, "		add esp, 12; params * 4\n\n");
			}
			else {
				strcpy(buffer, "");
				sprintf(buffer, "		add esp, 8; params * 4\n\n");
				progcode = InsertList(progcode, buffer);
				printf("		add esp, 8; params * 4\n\n");
				fprintf(logFile, "		add esp, 8; params * 4\n\n");
			}
			
			
			
		}
	}
	else if (ast->type == RESERVED_IN) {
		if (ast->kids[0]->type == RESERVED_CONSOLE) {
			regsub++;

			strcpy(buffer, "");
			sprintf(buffer, "		push dword[t%d] ; Temporary Read\n", registerCounter - regsub);
			progcode = InsertList(progcode, buffer);

			printf("		push dword[t%d] ; Temporary Read\n", registerCounter - regsub);
			fprintf(logFile, "		push dword[t%d] ; Temporary Read\n", registerCounter - regsub);
		 if (ast->kids[1] == NULL||ast->kids[1]->type == RESERVED_CAST_STRING ) {

			strcpy(buffer, "");
			sprintf(buffer, "		push formatinstring; number reading\n");
			progcode = InsertList(progcode, buffer);

			printf("		push formatinstring; number reading\n");
			fprintf(logFile, "		push formatinstring; number reading\n");
		}
		 else	if (ast->kids[1]->type == RESERVED_CAST_NUMBER) {

				strcpy(buffer, "");
				sprintf(buffer, "		push formatinnumber; number reading\n");
				progcode = InsertList(progcode, buffer);

				printf("		push formatinnumber; number reading\n");
				fprintf(logFile, "		push formatinnumber; number reading\n");
			}
			else if (ast->kids[1]->type == RESERVED_CAST_DECIMAL) {

				strcpy(buffer, "");
				sprintf(buffer, "		push formatindecimal; number reading\n");
				progcode = InsertList(progcode, buffer);

				printf("		push formatindecimal; number reading\n");
				fprintf(logFile, "		push formatindecimal; number reading\n");
			}
			else if (ast->kids[1]->type == RESERVED_CAST_CHAR) {
				
				strcpy(buffer, "");
				sprintf(buffer, "		push formatinchar; number reading\n");
				progcode = InsertList(progcode, buffer);

				printf("		push formatinchar; number reading\n");
				fprintf(logFile, "		push formatinchar; number reading\n");
			}
			else if (ast->kids[1]->type == RESERVED_CAST_BOOL) {
				
				strcpy(buffer, "");
				sprintf(buffer, "		push formatinbool; number reading\n");
				progcode = InsertList(progcode, buffer);

				printf("		push formatinbool; number reading\n");
				fprintf(logFile, "		push formatinbool; number reading\n");
			}

			strcpy(buffer, "");
			sprintf(buffer, "		call _scanf; call defined function\n");
			progcode = InsertList(progcode, buffer);

			strcpy(buffer, "");
			sprintf(buffer, "		add esp, 8; params * 4\n\n");
			progcode = InsertList(progcode, buffer);

			printf("		call _scanf; call defined function\n");
			fprintf(logFile, "		call _scanf; call defined function\n");
			printf("		add esp, 8; params * 4\n\n");
			fprintf(logFile, "		add esp, 8; params * 4\n\n");
		}
	}
}
void strupper(char * temp) {
	char * name;
	name = strtok(temp, ":");

	// Convert to upper case
	char *s = name;
	while (*s) {
		*s = toupper((unsigned char)*s);
		s++;
	}

}