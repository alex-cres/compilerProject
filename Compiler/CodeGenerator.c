#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "CodeGenerator.h"
#include "ErrorHandling.h"
int registerCounter = 1;
int registerLoopsCounter = 1;
int registerParamsCounter = 1;
char bufferdec[1024];
char buffer[1024];
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
	Element * rescode = CreateList();
	Element * funcCode = CreateList();
	char lofFileName[600] = "";
	strcat(lofFileName, originalfilename);
	lofFileName[strlen(lofFileName) - 3] = 0;
	strcat(lofFileName, ".asm");
	asmFile = fopen(lofFileName, "w");
	char cmd1[2048];
	char cmd2[2048];
	datacode = InsertList(datacode, "SECTION .data\n");
	datacode = InsertList(datacode, "	FALSE equ 0 \n");
	datacode = InsertList(datacode, "	TRUE equ 1 \n");
	datacode = InsertList(datacode, "	stringFalse: db `False`, 0\n");
	datacode = InsertList(datacode, "	stringTrue: db `True`, 0\n");
	datacode = InsertList(datacode, "	formatoutnumber: db \"%d\", 0\n");
	datacode = InsertList(datacode, "	formatoutdecimal: db \"%f\", 0\n");
	datacode = InsertList(datacode, "	formatoutchar: db \"%s\", 0\n");
	datacode = InsertList(datacode, "	formatoutstring: db \"%s\", 0\n");
	datacode = InsertList(datacode, "	formatoutbool: db \"%s\", 0\n\n");
	datacode = InsertList(datacode, "	formatinnumber: db \"%d\", 0\n");
	datacode = InsertList(datacode, "	formatindecimal: db \"%f\", 0\n");
	datacode = InsertList(datacode, "	formatinchar: db \"%c\", 0\n");
	datacode = InsertList(datacode, "	formatinstring: db \"%s\", 0\n");
	datacode = InsertList(datacode, "	formatinbool: db \"%d\", 0\n\n");
	rescode = InsertList(rescode, "SECTION .bss\n");
	insertSymbolToken(IDN_STRING, ast, "stringFalse", table);
	insertSymbolToken(IDN_STRING, ast, "stringTrue", table);
	insertSymbolToken(IDN_STRING, ast, "formatoutnumber", table);
	insertSymbolToken(IDN_STRING, ast, "formatoutdecimal", table);
	insertSymbolToken(IDN_STRING, ast, "formatoutchar", table);
	insertSymbolToken(IDN_STRING, ast, "formatoutstring", table);
	insertSymbolToken(IDN_STRING, ast, "formatoutbool", table);
	insertSymbolToken(IDN_STRING, ast, "formatinnumber", table);
	insertSymbolToken(IDN_STRING, ast, "formatindecimal", table);
	insertSymbolToken(IDN_STRING, ast, "formatinchar", table);
	insertSymbolToken(IDN_STRING, ast, "formatinstring", table);
	insertSymbolToken(IDN_STRING, ast, "formatinbool", table);
	funcCode = InsertList(funcCode, "SECTION .text\n");
	funcCode = InsertList(funcCode, "	global _main\n");
	funcCode = InsertList(funcCode, "	extern _printf\n");
	funcCode = InsertList(funcCode, "	extern _scanf\n");
	funcCode = InsertList(funcCode, "	extern _strcat\n");
	funcCode = InsertList(funcCode, "	extern _strcpy\n");
	//**functions here*//
	progcode = InsertList(progcode, "	_main:\n\n");
	GenerateIntermidiateCode(ast, logFile, datacode, progcode, table, registerLoopsCounter, rescode, funcCode);
	progcode = InsertList(progcode, "\n\n	mov eax, 0 \n");
	progcode = InsertList(progcode, "	ret\n\n");
	datacode = InsertList(datacode, "\n");
	datacode = InsertList(datacode, "\n");
	rescode = InsertList(rescode, "\n");
	rescode = InsertList(rescode, "\n");
	Element*data = datacode;
	Element*text = progcode;
	Element* res = rescode;
	Element* func = funcCode;
	while (data != NULL)
	{
		printf("%s", data->code);
		fprintf(asmFile, "%s", data->code);
		data = data->next;
	}
	while (res != NULL)
	{
		printf("%s", res->code);
		fprintf(asmFile, "%s", res->code);
		res = res->next;
	}
	while (func != NULL)
	{
		printf("%s", func->code);
		fprintf(asmFile, "%s", func->code);
		func = func->next;
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
	_getcwd(directory, sizeof(directory));
	_chdir("C:\\MinGW\\bin\\");
	strcat(directory, "\\");
	strcpy(fileCreator, directory);
	strcat(fileCreator, lofFileName);
	sprintf(cmd1, "nasm -f win32 %s", fileCreator);
	system(cmd1);
	fprintf(logFile, "Object File Created : %s\n", cmd1);
	printf("Object File Created : %s\n", cmd1);
	strcpy(fileCreator, directory);
	strcat(fileCreator, lofFileName);
	fileCreator[strlen(fileCreator) - 4] = 0;
	sprintf(cmd2, "gcc %s.obj -o %s.exe", fileCreator, fileCreator);
	system(cmd2);
	fprintf(logFile, "Exe File Created : %s\n", cmd2);
	printf("Exe File Created : %s\n", cmd2);
}
int  GenerateIntermidiateCode(Node * ast, FILE* logFile, Element* datacode, Element* progcode, SymbolToken * table, int breakerLoop, Element* rescode, Element* funcCode)
{
	int loopnumber = registerLoopsCounter;
	int i = 0;
	int regsub = 0;
	bool floatingpointed = False;
	bool concatString = False;
	if (ast == NULL) {
		return;
	}
	//loopers, ifs, things that encompass other encapsualtions
	/*if (ast->type == DECLARE && ast->kids[1] !=NULL && ast->kids[1]->kids[0] != NULL && ast->kids[1]->kids[0]->type==IDENTIFIER_FUNCTION) {
		funcCode = InsertList(funcCode, "	%s:\n", ast->kids[1]->kids[0]->info);
		GenerateIntermidiateCode(ast->kids[1]->kids[1], logFile, datacode, funcCode, table, breakerLoop, rescode, funcCode);
		funcCode = InsertList(funcCode, "	ret\n");
	}*/
	if (ast->type == DOUBLE_POINT) {
		sprintf(buffer, "		%s:\n", ast->kids[0]->info);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
	}
	if (ast->type == RESERVED_GOTO) {
		if (ast->kids[1] != NULL && ast->kids[1]->type == RESERVED_IF) {
			if (ast->kids[1]->kids[0]->type == RESERVED_BOOL_TRUE) {
				sprintf(buffer, "		jmp %s\n", ast->kids[0]->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else if (ast->kids[1]->kids[0]->type == RESERVED_BOOL_FALSE) {
			}
			else
			{
				if (ast->kids[1]->kids[0]->type == IDENTIFIER) {
					sprintf(buffer, "\n		mov ebx, dword[%s]\n", ast->kids[1]->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else {
					int reg = GenerateIntermidiateCode(ast->kids[1]->kids[0], logFile, datacode, progcode, table, breakerLoop, rescode, funcCode);
					sprintf(buffer, "\n		mov ebx, dword[t%d]\n", reg);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				sprintf(buffer, "\n		cmp  ebx, TRUE\n");
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "\n		je t%d_if_then\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "\n		jmp t%d_if_else\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "\n		t%d_if_then:\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		jmp %s\n", ast->kids[0]->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "\n		t%d_if_else:\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
			}
		}
		else {
			sprintf(buffer, "		jmp %s\n", ast->kids[0]->kids[0]->info);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
	}
	else if (ast->type == RESERVED_DO && ast->kids[1] != NULL && ast->kids[1]->type == RESERVED_WHILE)
	{
		registerLoopsCounter++;
		char looperID[100];
		sprintf(looperID, "l%d", loopnumber);
		sprintf(buffer, "		%s_begin:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		GenerateIntermidiateCode(ast->kids[0], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
		char iffer[100];
		if (ast->kids[1]->kids[0]->type != RESERVED_BOOL_FALSE && ast->kids[1]->kids[0]->type != RESERVED_BOOL_TRUE) {
			GenerateIntermidiateCode(ast->kids[1]->kids[0], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
			sprintf(iffer, "t%d", registerCounter - 1);
			sprintf(buffer, "		mov ebx, dword[%s]\n		mov eax, TRUE\n		cmp eax, ebx\n", iffer);
		}
		else {
			strupper(ast->kids[1]->kids[0]->info);
			sprintf(buffer, "		mov ebx, %s\n		cmp ebx, TRUE\n", ast->kids[1]->kids[0]->info);
		}
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		je %s_looper_true\n		jmp %s_looper_end\n", looperID, looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		%s_looper_true:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		%s_looper_continue:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		jmp %s_begin\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		%s_looper_end:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
	}
	else if (ast->type == RESERVED_WHILE)
	{
		registerLoopsCounter++;
		char looperID[100];
		sprintf(looperID, "l%d", loopnumber);
		sprintf(buffer, "		%s_begin:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		char iffer[100];
		if (ast->kids[0]->type != RESERVED_BOOL_FALSE && ast->kids[0]->type != RESERVED_BOOL_TRUE) {
			GenerateIntermidiateCode(ast->kids[0], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
			sprintf(iffer, "t%d", registerCounter - 1);
			sprintf(buffer, "		mov ebx, dword[%s]\n		mov eax, TRUE\n		cmp eax, ebx\n", iffer);
		}
		else {
			strupper(ast->kids[0]->info);
			sprintf(buffer, "		mov ebx, %s\n		cmp ebx, TRUE\n", ast->kids[0]->info);
		}
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		je %s_looper_true\n		jmp %s_looper_end\n", looperID, looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		%s_looper_true:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		GenerateIntermidiateCode(ast->kids[1], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
		sprintf(buffer, "		%s_looper_continue:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		jmp %s_begin\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		%s_looper_end:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
	}
	else if (ast->type == RESERVED_LOOP) {
		registerLoopsCounter++;
		char looperID[100];
		sprintf(looperID, "l%d", loopnumber);
		insertSymbolToken(IDN_NUMBER, ast, looperID, table);
		sprintf(buffer, "	%s : dd  0\n", looperID);
		datacode = InsertList(datacode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		;looper %s begin\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		if (ast->kids[0] != NULL && ast->kids[1] != NULL) {
			GenerateIntermidiateCode(ast->kids[0], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
			if (ast->kids[0]->type == IDENTIFIER) {
				int type_of_var = 0;
				type_of_var = searchTable(table, ast->kids[0]->info, logFile);
				if (type_of_var == IDN_NUMBER) {
					sprintf(buffer, "		mov eax, dword[%s]\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else {
					printfAndExit(logFile, "ERROR: SYNTAX ERROR LOOP DOENS'T ACCEPT NON NUMBER, %s", ERROR_NON_NUMBER_LOOP, ast->kids[0]->info);
				}
			}
			else if (ast->kids[0]->type == LITERAL_NUMBER) {
				sprintf(buffer, "		mov eax, %s\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else {
				int type_of_var = 0;
				char buffersearcher[256];
				sprintf(buffersearcher, "t%d", registerCounter - 1);
				type_of_var = searchTable(table, buffersearcher, logFile);
				if (type_of_var == IDN_NUMBER) {
					sprintf(buffer, "		mov eax, dword[%s]\n", buffersearcher);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else {
					printfAndExit(logFile, "ERROR: SYNTAX ERROR LOOP DOENS'T ACCEPT NON NUMBER, %s", ERROR_NON_NUMBER_LOOP, ast->info);
				}
			}
			sprintf(buffer, "		mov dword[%s],eax\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		%s_looper:\n\n\n\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			GenerateIntermidiateCode(ast->kids[1], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
			sprintf(buffer, "\n\n		%s_looper_continue:\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "\n\n		mov eax , dword[%s]\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		sub eax,1\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		mov dword[%s], eax\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		cmp eax, 0\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		jle %s_looper_end\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		jmp %s_looper\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		%s_looper_end:\n		;looper %s end\n", looperID, looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
		else {
			printfAndExit(logFile, "ERROR: SYNTAX ERROR INCOMPLETE LOOP, %s", ERROR_INCOMPLETE_LOOP, ast->info);
		}
	}
	else if (ast->type == RESERVED_FOR) {
		registerLoopsCounter++;
		char looperID[100];
		sprintf(looperID, "l%d", loopnumber);
		char idToStepOver[256];
		GenerateIntermidiateCode(ast->kids[0], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
		char stepper[100];
		if (ast->kids[0]->type != OP_ATTRIBUTION) {
			printfAndExit(logFile, "ERROR: SYNTAX ERROR IN ATTRIBUTION, %s", ERROR_SYNTAX_ERROR_ATTRIBUTION, "\n");
		}
		else {
			sprintf(idToStepOver, "%s", ast->kids[0]->kids[0]->info);
		}
		if (ast->kids[1]->kids[0]->type == LITERAL_DECIMAL || ast->kids[1]->kids[0]->type == LITERAL_NUMBER) {
			sprintf(stepper, "t%d", registerCounter);
			insertSymbolToken(((ast->kids[1]->kids[0]->type == LITERAL_DECIMAL) ? IDN_DECIMAL : IDN_NUMBER), ast, stepper, table);
			sprintf(buffer, "	%s : dd %s ; literal stepper for loop %s\n", stepper, ast->kids[1]->kids[0]->info, looperID);
			datacode = InsertList(datacode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter++;
		}
		else if (ast->kids[1]->kids[0]->type == IDENTIFIER) {
			sprintf(stepper, "%s", ast->kids[1]->kids[0]->info);
		}
		else {
			GenerateIntermidiateCode(ast->kids[1]->kids[0], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
			sprintf(stepper, "t%d", registerCounter - 1);
		}
		int type_of_var = 0, type_of_var2 = 0;
		type_of_var = searchTable(table, stepper, logFile);
		type_of_var2 = searchTable(table, idToStepOver, logFile);
		if (type_of_var != type_of_var2) {
			printfAndExit(logFile, "ERROR: SYNTAX ERROR DIFFERENT TYPES IN FOR LOOP AND STEP, %s", ERROR_SYNTAX_ERROR_FOR_LOOP, "\n");
		}
		sprintf(buffer, "		%s_begin:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		char iffer[100];
		if (ast->kids[2]->kids[0]->type != RESERVED_BOOL_FALSE && ast->kids[2]->kids[0]->type != RESERVED_BOOL_TRUE) {
			GenerateIntermidiateCode(ast->kids[2]->kids[0], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
			sprintf(iffer, "t%d", registerCounter - 1);
			sprintf(buffer, "		mov ebx, dword[%s]\n		mov eax, TRUE\n		cmp eax, ebx\n", iffer);
		}
		else {
			strupper(ast->kids[2]->kids[0]->info);
			sprintf(buffer, "		mov ebx, %s\n		cmp ebx, TRUE\n", ast->kids[2]->kids[0]->info);
		}
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		je %s_looper_true\n		jmp %s_looper_end\n", looperID, looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		%s_looper_true:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		GenerateIntermidiateCode(ast->kids[3], logFile, datacode, progcode, table, loopnumber, rescode, funcCode);
		if (type_of_var == IDN_NUMBER) {
			sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number\n", idToStepOver);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number\n", stepper);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		add eax, ebx\n ");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		mov dword[%s],eax\n ", idToStepOver);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
		else {
			sprintf(buffer, "		fld dword[%s] ; Moving First Operand Number\n", idToStepOver);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Number\n", stepper);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		fadd\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		fstp dword[%s]\n", idToStepOver);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
		sprintf(buffer, "		%s_looper_continue:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		jmp %s_begin\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		%s_looper_end:\n", looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
	}
	else if (ast->type == RESERVED_BREAKIF) {
		if (ast->kids[0]->type == RESERVED_BOOL_TRUE) {
			char looperID[200];
			sprintf(looperID, "l%d", breakerLoop);
			sprintf(buffer, "\n		jmp %s_looper_end\n\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
		else if (ast->kids[0]->type == RESERVED_BOOL_FALSE) {
			//no action empty -no instructions will be executed
		}
		else {
			GenerateIntermidiateCode(ast->kids[0], logFile, datacode, progcode, table, breakerLoop, rescode, funcCode);
			sprintf(bufferdec, "t%d", registerCounter - 1);
			char looperID[200];
			sprintf(looperID, "l%d", breakerLoop);
			char ifferId[200];
			sprintf(ifferId, "t%d", registerCounter);
			sprintf(buffer, "		mov eax , 0 \n		mov ebx, dword[%s]\n		cmp ebx, TRUE\n", bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		je %s_if_then\n		jmp %s_if_else\n", ifferId, ifferId);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		%s_if_then:\n\n\n", ifferId);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		jmp %s_looper_end\n\n\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		%s_if_else:\n\n\n", ifferId);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter++;
		}
	}
	else if (ast->type == RESERVED_CONTINUEIF) {
		if (ast->kids[0]->type == RESERVED_BOOL_TRUE) {
			char looperID[200];
			sprintf(looperID, "l%d", breakerLoop);
			sprintf(buffer, "\n		jmp %s_looper_continue\n\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
		else if (ast->kids[0]->type == RESERVED_BOOL_FALSE) {
			//no action empty -no instructions will be executed
		}
		else {
			GenerateIntermidiateCode(ast->kids[0], logFile, datacode, progcode, table, breakerLoop, rescode, funcCode);
			sprintf(bufferdec, "t%d", registerCounter - 1);
			char looperID[200];
			sprintf(looperID, "l%d", breakerLoop);
			char ifferId[200];
			sprintf(ifferId, "t%d", registerCounter);
			sprintf(buffer, "		mov eax , 0 \n		mov ebx, dword[%s]\n		cmp ebx, TRUE\n", bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		je %s_if_then\n		jmp %s_if_else\n", ifferId, ifferId);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		%s_if_then:\n\n\n", ifferId);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		jmp %s_looper_continue\n\n\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		%s_if_else:\n\n\n", ifferId);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter++;
		}
	}
	else if (ast->type == RESERVED_IF && ast->kids[0] != NULL && ast->kids[1] != NULL) {
		if (ast->kids[0]->type != IDENTIFIER) {
			GenerateIntermidiateCode(ast->kids[0], logFile, datacode, progcode, table, breakerLoop, rescode, funcCode);
			sprintf(bufferdec, "t%d", registerCounter - 1);
			sprintf(buffer, "\n		mov ebx, dword[%s]\n", bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
		else {
			sprintf(bufferdec, "t%d", registerCounter);
			insertSymbolToken(IDN_BOOL, ast, bufferdec, table);
			sprintf(buffer, "	t%d : dd 0\n", registerCounter);
			datacode = InsertList(datacode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "\n		mov ebx, dword[%s]\n", ast->kids[0]->info);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "\n		mov dword[%s],ebx\n", bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "\n		mov ebx, dword[%s]\n", bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter++;
		}
		sprintf(buffer, "\n		cmp  ebx, TRUE\n");
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "\n		je %s_if_then\n", bufferdec);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "\n		jmp %s_if_else\n", bufferdec);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "\n		%s_if_then:\n", bufferdec);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		GenerateIntermidiateCode(ast->kids[1], logFile, datacode, progcode, table, breakerLoop, rescode, funcCode);
		sprintf(buffer, "\n		jmp %s_if_end\n", bufferdec);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "\n		%s_if_else:\n", bufferdec);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		if (ast->kids[2] != NULL) {
			GenerateIntermidiateCode(ast->kids[2], logFile, datacode, progcode, table, breakerLoop, rescode, funcCode);
		}
		sprintf(buffer, "\n		%s_if_end:\n", bufferdec);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
	}
	else {
		int directRegisters[128];
		while (ast->kids[i] != NULL && i < MAX_CHILDREN)
		{
			directRegisters[i] = GenerateIntermidiateCode(ast->kids[i], logFile, datacode, progcode, table, breakerLoop, rescode, funcCode);
			i++;
		}
		if (ast->type == RESERVED_BREAK) {
			char looperID[200];
			sprintf(looperID, "l%d", breakerLoop);
			sprintf(buffer, "\n		jmp %s_looper_end\n\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
		else if (ast->type == RESERVED_CONTINUE) {
			char looperID[200];
			sprintf(looperID, "l%d", breakerLoop);
			sprintf(buffer, "\n		jmp %s_looper_continue\n\n", looperID);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
		}
		else if ((ast->kids[0] != NULL && ast->kids[1] != NULL) && (ast->kids[0]->type == LITERAL_CHAR || ast->kids[0]->type == LITERAL_STRING) &&
			(ast->kids[1]->type == LITERAL_CHAR || ast->kids[1]->type == LITERAL_STRING) &&
			(ast->type == OP_ADD || ast->type == OP_MINUS || ast->type == OP_MUL || ast->type == OP_DIV)) {
			concatString = True;
			if (ast->type != OP_ADD) {
				char buffererr[1000];
				sprintf(buffererr, "ERROR: : SYNTAX ERROR STRINGS IN OPERATION NOT <string> + <string> , %s %s %s %s", ast->kids[0]->info, ast->info, ast->kids[1]->info);
				printfAndExit(logFile, buffererr, ERROR_STRING_OP_NOT_VALID, "\n");
			}
			sprintf(bufferdec, "t%d", registerCounter + 2);
			insertSymbolToken(IDN_STRING, ast, bufferdec, table);
			sprintf(buffer, "	t%d : times  256  db ``, 0\n", registerCounter + 2);
			datacode = InsertList(datacode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			strcpy(bufferdec, "");
			sprintf(bufferdec, "t%d", registerCounter + 1);
			insertSymbolToken(IDN_STRING, ast, bufferdec, table);
			sprintf(buffer, "	t%d : times  256  db `%s`, 0\n", registerCounter + 1, ast->kids[1]->info);
			datacode = InsertList(datacode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			strcpy(bufferdec, "");
			sprintf(bufferdec, "t%d", registerCounter);
			insertSymbolToken(IDN_STRING, ast, bufferdec, table);
			sprintf(buffer, "	t%d : times  256  db `%s`, 0\n", registerCounter, ast->kids[0]->info);
			datacode = InsertList(datacode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			strcpy(bufferdec, "");
			sprintf(buffer, "		push	t%d ; Concat Begin\n", registerCounter);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		push	t%d ; Concat Dest\n", registerCounter + 2);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		call _strcat\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		add esp, 8\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		push	t%d ; Concat Second\n", registerCounter + 1);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		push	t%d ; Concat Dest\n", registerCounter + 2);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		call _strcat\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		add esp, 8\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter += 3;
		}
		else if (ast->type == OP_ADD || ast->type == OP_MINUS || ast->type == OP_MUL || ast->type == OP_DIV || ast->type == OP_MOD) {// http://www.website.masmforum.com/tutorials/fptute/fpuchap5.htm // integer in float op
			int type_of_var = 0;
			int type_of_var2 = 0;
			if (ast->kids[0]->type == LITERAL_NUMBER && ast->kids[1]->type == LITERAL_NUMBER)
			{
				if (ast->type == OP_MOD) {
					sprintf(buffer, "		mov edx, 0\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else if (ast->kids[0]->type == LITERAL_DECIMAL && ast->kids[1]->type == LITERAL_DECIMAL)
			{
				if (ast->type == OP_MOD) {
					printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
				}
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
				strcpy(bufferdec, "");
				floatingpointed = True;
				sprintf(buffer, "		fld	%s ; Moving First Operand Number\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		fld	%s ; Moving Second Operand Number\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else if (ast->kids[0]->type == IDENTIFIER && ast->kids[1]->type == IDENTIFIER) {
				sprintf(bufferdec, "t%d", registerCounter);
				type_of_var = searchTable(table, ast->kids[0]->info, logFile);
				type_of_var2 = searchTable(table, ast->kids[1]->info, logFile);
				if (type_of_var == type_of_var2) {
					insertSymbolToken(type_of_var, ast, bufferdec, table);
					strcpy(bufferdec, "");
					switch (type_of_var) {
					case IDN_NUMBER:
						if (ast->type == OP_MOD) {
							sprintf(buffer, "		mov edx, 0\n");
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						break;
					case IDN_DECIMAL:
						if (ast->type == OP_MOD) {
							printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
						}
						floatingpointed = True;
						sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						break;
					}
				}
				else {
					switch (type_of_var) {
					case IDN_NUMBER:
						if (type_of_var2 == IDN_DECIMAL) {
							if (ast->type == OP_MOD) {
								printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
							}
							insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
							strcpy(bufferdec, "");
							floatingpointed = True;
							sprintf(buffer, "		fild dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", ast->kids[1]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						else {
							if (type_of_var2 == IDN_BOOL) {
								insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
								strcpy(bufferdec, "");
								floatingpointed = False;
								sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number\n", ast->kids[0]->info);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
								sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
						}
						break;
					case IDN_DECIMAL:
						if (type_of_var2 == IDN_NUMBER) {
							if (ast->type == OP_MOD) {
								printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
							}
							insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
							strcpy(bufferdec, "");
							floatingpointed = True;
							sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		fild dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						break;
					case IDN_BOOL:
						if (type_of_var2 == IDN_NUMBER) {
							if (ast->type == OP_MOD) {
								sprintf(buffer, "		mov edx, 0\n");
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
							strcpy(bufferdec, "");
							floatingpointed = False;
							sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						else if (type_of_var2 == IDN_BOOL) {
							if (ast->type == OP_MOD) {
								sprintf(buffer, "		mov edx, 0\n");
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
							strcpy(bufferdec, "");
							floatingpointed = False;
							sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						break;
					}
				}
			}
			else if (ast->kids[0]->type == LITERAL_NUMBER)
			{
				if (ast->kids[1]->type == IDENTIFIER) {
					type_of_var = searchTable(table, ast->kids[1]->info, logFile);
					if (type_of_var == IDN_DECIMAL) {
						if (ast->type == OP_MOD) {
							printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
						}
						floatingpointed = True;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(bufferdec, "t%d", registerCounter + 1);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		fild dword[t%d] ; Moving First Operand Number\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", ast->kids[1]->info);
						registerCounter++;
					}
					else if (type_of_var == IDN_NUMBER) {
						if (ast->type == OP_MOD) {
							sprintf(buffer, "		mov edx, 0\n");
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						floatingpointed = False;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
					}
				}
				else {
					if (ast->type == OP_MOD) {
						sprintf(buffer, "		mov edx, 0\n");
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					floatingpointed = False;
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					regsub++;
					sprintf(buffer, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", directRegisters[1]);
				}
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else if (ast->kids[1]->type == LITERAL_NUMBER)
			{
				if (ast->kids[0]->type == IDENTIFIER) {
					type_of_var = searchTable(table, ast->kids[0]->info, logFile);
					if (type_of_var == IDN_DECIMAL) {
						if (ast->type == OP_MOD) {
							printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
						}
						floatingpointed = True;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(bufferdec, "t%d", registerCounter + 1);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		fild dword[t%d] ; Moving Second Operand Number\n", registerCounter);
						registerCounter++;
					}
					else if (type_of_var == IDN_NUMBER) {
						if (ast->type == OP_MOD) {
							sprintf(buffer, "		mov edx, 0\n");
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						floatingpointed = False;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(buffer, "		mov eax, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		mov ebx, %s ; Moving First Operand Number\n", ast->kids[1]->info);
					}
				}
				else {
					if (ast->type == OP_MOD) {
						sprintf(buffer, "		mov edx, 0\n");
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					floatingpointed = False;
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					regsub++;
					sprintf(buffer, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", directRegisters[1]);
				}
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else if (ast->kids[0]->type == LITERAL_DECIMAL) {
				if (ast->kids[1]->type == IDENTIFIER) {
					type_of_var = searchTable(table, ast->kids[1]->info, logFile);
					if (ast->type == OP_MOD) {
						printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
					}
					floatingpointed = True;
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(bufferdec, "t%d", registerCounter + 1);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[0]->info);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		f%sld dword[%s] ; Moving Second Operand Decimal Var\n", (type_of_var == IDN_NUMBER || type_of_var == IDN_BOOL) ? "i" : "", ast->kids[1]->info);
					registerCounter++;


				}
				else {
					char searcherbuf[150];
					sprintf(searcherbuf, "t%d", registerCounter - 1);
					type_of_var = searchTable(table, searcherbuf, logFile);
					if (ast->type == OP_MOD) {
						printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
					}
					floatingpointed = True;
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(bufferdec, "t%d", registerCounter + 1);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");

					sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[0]->info);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		f%sld dword[t%d] ; Moving Second Operand Decimal Var\n", (type_of_var == IDN_NUMBER || type_of_var == IDN_BOOL) ? "i" : "", registerCounter - 1);


					registerCounter++;
				}
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else if (ast->kids[1]->type == LITERAL_DECIMAL) {
				if (ast->type == OP_MOD) {
					printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
				}
				char varnamebuffer[1024];

				if (ast->kids[0]->type == IDENTIFIER) {
					sprintf(varnamebuffer, "%s", ast->kids[0]->info);
				}
				else {
					sprintf(varnamebuffer, "t%d", registerCounter - 1);
				}
				type_of_var = searchTable(table, varnamebuffer, logFile);
				floatingpointed = True;
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(bufferdec, "t%d", registerCounter + 1);
				insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		f%sld dword[%s] ; Moving First Operand Decimal/Number Var\n", (type_of_var == IDN_NUMBER || type_of_var == IDN_BOOL) ? "i" : "", varnamebuffer);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		fld dword[t%d] ; Moving Second Operand Decimal Var\n", registerCounter);
				registerCounter++;


				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else {
				sprintf(bufferdec, "t%d", registerCounter);
				if (ast->kids[0]->type == IDENTIFIER) {
					char  searchbuf[100];
					sprintf(searchbuf, "t%d", registerCounter - 1);
					type_of_var = searchTable(table, ast->kids[0]->info, logFile);
					type_of_var2 = searchTable(table, searchbuf, logFile);
					if (type_of_var == type_of_var2) {
						insertSymbolToken(type_of_var, ast, bufferdec, table);
						strcpy(bufferdec, "");
						switch (type_of_var) {
						case IDN_NUMBER:
							if (ast->type == OP_MOD) {
								sprintf(buffer, "		mov edx, 0\n");
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - 1);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							break;
						case IDN_DECIMAL:
							if (ast->type == OP_MOD) {
								printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
							}
							floatingpointed = True;
							sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		fld dword[t%d] ; Moving Second Operand Decimal Var\n", registerCounter - 1);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							break;
						}
					}
					else {
						switch (type_of_var) {
						case IDN_NUMBER:
							if (type_of_var2 == IDN_DECIMAL) {
								if (ast->type == OP_MOD) {
									printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
								}
								insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
								strcpy(bufferdec, "");
								floatingpointed = True;
								sprintf(buffer, "		fild dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
								sprintf(buffer, "		fld dword[t%d] ; Moving Second Operand Decimal Var\n", registerCounter - 1);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							break;
						case IDN_DECIMAL:
							if (type_of_var == IDN_DECIMAL) {
								if (ast->type == OP_MOD) {
									printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
								}
								insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
								strcpy(bufferdec, "");
								floatingpointed = True;
								sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", ast->kids[0]->info);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
								sprintf(buffer, "		fild dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - 1);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							break;
						}
					}
				}
				else if (ast->kids[1]->type == IDENTIFIER) {
					char  searchbuf[100];
					sprintf(searchbuf, "t%d", registerCounter - 1);
					type_of_var = searchTable(table, searchbuf, logFile);
					type_of_var = searchTable(table, ast->kids[1]->info, logFile);
					if (type_of_var == type_of_var2) {
						insertSymbolToken(type_of_var, ast, bufferdec, table);
						strcpy(bufferdec, "");
						switch (type_of_var) {
						case IDN_NUMBER:
							if (ast->type == OP_MOD) {
								sprintf(buffer, "		mov edx, 0\n");
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							sprintf(buffer, "		mov eax, dword[t%d] ; Moving First Operand Number Var\n", registerCounter - 1);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							break;
						case IDN_DECIMAL:
							if (ast->type == OP_MOD) {
								printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
							}
							floatingpointed = True;
							sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter - 1);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", ast->kids[1]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							break;
						}
					}
					else {
						switch (type_of_var) {
						case IDN_NUMBER:
							if (type_of_var2 == IDN_DECIMAL) {
								if (ast->type == OP_MOD) {
									printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
								}
								insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
								strcpy(bufferdec, "");
								floatingpointed = True;
								sprintf(buffer, "		fild dword[t%d] ; Moving First Operand Number Var\n", registerCounter - 1);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
								sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", ast->kids[1]->info);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							break;
						case IDN_DECIMAL:
							if (type_of_var == IDN_DECIMAL) {
								if (ast->type == OP_MOD) {
									printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
								}
								insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
								strcpy(bufferdec, "");
								floatingpointed = True;
								sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter - 1);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
								sprintf(buffer, "		fild dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							break;
						}
					}
				}
				else {
					char  searchbuf[100];
					char  searchbuf2[100];
					sprintf(searchbuf, "t%d", directRegisters[0]);
					sprintf(searchbuf2, "t%d", directRegisters[1]);
					type_of_var = searchTable(table, searchbuf, logFile);
					type_of_var2 = searchTable(table, searchbuf2, logFile);
					if (type_of_var == type_of_var2) {
						insertSymbolToken(type_of_var, ast, bufferdec, table);
						strcpy(bufferdec, "");
						switch (type_of_var) {
						case IDN_NUMBER:
							if (ast->type == OP_MOD) {
								sprintf(buffer, "		mov edx, 0\n");
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							sprintf(buffer, "		mov eax, dword[%s] ; Moving First Operand Number Var\n", searchbuf);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", searchbuf2);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							break;
						case IDN_DECIMAL:
							if (ast->type == OP_MOD) {
								printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, searchbuf);
							}
							floatingpointed = True;
							sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", searchbuf);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", searchbuf2);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							break;
						}
					}
					else {
						switch (type_of_var) {
						case IDN_NUMBER:
							if (type_of_var2 == IDN_DECIMAL) {
								if (ast->type == OP_MOD) {
									printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
								}
								insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
								strcpy(bufferdec, "");
								floatingpointed = True;
								sprintf(buffer, "		fild dword[%s] ; Moving First Operand Number Var\n", searchbuf);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
								sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", searchbuf2);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							break;
						case IDN_DECIMAL:
							if (type_of_var == IDN_DECIMAL) {
								if (ast->type == OP_MOD) {
									printfAndExit(logFile, "ERROR: OP_MOD IS JUST FOR NUMBERS", ERROR_OP_MOD_IS_JUST_FOR_INTEGERS, ast->kids[0]->info);
								}
								insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
								strcpy(bufferdec, "");
								floatingpointed = True;
								sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", searchbuf);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
								sprintf(buffer, "		fild dword[%s] ; Moving Second Operand Number Var\n", searchbuf2);
								progcode = InsertList(progcode, buffer);
								printf(buffer);
								fprintf(logFile, buffer);
							}
							break;
						}
					}
				}
			}
			//Operation
			if (!floatingpointed && !concatString) {
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
				case OP_MOD:
				case OP_DIV:
					progcode = InsertList(progcode, "		idiv ebx ; Dividing First and Second Operand Number\n");
					printf("		idiv ebx ; Dividing First and Second Operand Number\n");
					fprintf(logFile, "		idiv ebx ; Dividing First and Second Operand Number\n");
					break;
				}
			}
			else if (floatingpointed) {
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
			sprintf(buffer, "	t%d : dd 0 \n", registerCounter);
			datacode = InsertList(datacode, buffer);
			if (!floatingpointed && !ast->type == OP_MOD) {
				sprintf(buffer, "		mov dword[t%d] , eax ;Result \n\n", registerCounter);
			}
			else if (!floatingpointed && ast->type == OP_MOD) {
				sprintf(buffer, "		mov dword[t%d] , edx ;Result \n\n", registerCounter);
			}
			else if (floatingpointed)
			{
				sprintf(buffer, "		fstp dword[t%d] ;Result \n\n", registerCounter);
			}
			else {
				sprintf(buffer, "		mov dword[t%d] , eax ;Result \n\n", registerCounter);
			}
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter++;
		}
		else if (ast->type == OP_MINOR || ast->type == OP_BIGGER || ast->type == OP_BIGGER_EQUAL ||
			ast->type == OP_MINOR_EQUAL || ast->type == OP_EQUAL || ast->type == OP_NOT_EQUAL) {
			if (ast->kids[0]->type == IDENTIFIER) {
				sprintf(buffer, "		mov eax, dword[%s]\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else  if (ast->kids[0]->type == LITERAL_NUMBER || ast->kids[0]->type == LITERAL_DECIMAL || ast->kids[0]->type == LITERAL_CHAR) {
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken((ast->kids[0]->type == LITERAL_NUMBER) ? IDN_NUMBER : ((ast->kids[0]->type == LITERAL_DECIMAL) ? IDN_DECIMAL : IDN_CHAR), ast, bufferdec, table);
				sprintf(buffer, (ast->kids[0]->type == LITERAL_NUMBER) ? "	t%d : dd %s\n" : ((ast->kids[0]->type == "	t%d : dd %s\n") ? IDN_DECIMAL : "	t%d : times 10 db `%s`\n"), registerCounter, ast->kids[0]->info);
				datacode = InsertList(datacode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
			}
			else  if (ast->kids[0]->type == RESERVED_BOOL_FALSE || ast->kids[0]->type == RESERVED_BOOL_TRUE) {
				sprintf(buffer, "		mov eax, %s\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else {
				sprintf(buffer, "		mov eax, dword[t%d]\n", directRegisters[0]);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			if (ast->kids[1]->type == IDENTIFIER) {
				sprintf(buffer, "		mov eax, dword[%s]\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else  if (ast->kids[1]->type == LITERAL_NUMBER || ast->kids[1]->type == LITERAL_DECIMAL || ast->kids[1]->type == LITERAL_CHAR) {
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken((ast->kids[1]->type == LITERAL_NUMBER) ? IDN_NUMBER : ((ast->kids[1]->type == LITERAL_DECIMAL) ? IDN_DECIMAL : IDN_CHAR), ast, bufferdec, table);
				sprintf(buffer, (ast->kids[1]->type == LITERAL_NUMBER) ? "	t%d : dd %s\n" : ((ast->kids[1]->type == "	t%d : dd %s\n") ? IDN_DECIMAL : "	t%d : times 10 db `%s`\n"), registerCounter, ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		mov ebx, dword[t%d]\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
			}
			else  if (ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_TRUE) {
				strupper(ast->kids[1]->info);
				sprintf(buffer, "		mov ebx, %s\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else {
				sprintf(buffer, "		mov ebx, dword[t%d]\n", directRegisters[1]);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			sprintf(bufferdec, "t%d", registerCounter);
			insertSymbolToken(IDN_BOOL, ast, bufferdec, table);
			sprintf(buffer, "	t%d : dd 0\n", registerCounter);
			datacode = InsertList(datacode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		cmp eax, ebx\n");
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			switch (ast->type)
			{
			case OP_MINOR:
				sprintf(buffer, "		jl	%s_true\n", bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				break;
			case OP_BIGGER:
				sprintf(buffer, "		jg	%s_true\n", bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				break;
			case OP_BIGGER_EQUAL:
				sprintf(buffer, "		jge	%s_true\n", bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				break;
			case OP_MINOR_EQUAL:
				sprintf(buffer, "		jle	%s_true\n", bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				break;
			case OP_EQUAL:
				sprintf(buffer, "		je	%s_true\n", bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				break;
			case OP_NOT_EQUAL:
				sprintf(buffer, "		jne	%s_true\n", bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				break;
			}
			sprintf(buffer, "		mov eax,FALSE\n		mov dword[%s],eax\n		jmp %s_false\n", bufferdec, bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		%s_true:\n		mov eax,TRUE\n		mov dword[%s],eax\n		%s_false:\n\n", bufferdec, bufferdec, bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter++;
		}
		else if (ast->type == OP_AND || ast->type == OP_OR || ast->type == OP_XOR || ast->type == OP_NOT) {
			if (ast->type == OP_NOT && ast->kids[0] != NULL) {
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_BOOL, ast, bufferdec, table);
				sprintf(buffer, "	%s : dd 0\n", bufferdec);
				datacode = InsertList(datacode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				if (ast->kids[0]->type == IDENTIFIER) {
					int type_of_var = 0;
					type_of_var = searchTable(table, ast->kids[0]->info, logFile);
					if (type_of_var == IDN_BOOL) {
						sprintf(buffer, "		mov eax, dword[%s]\n		cmp eax, FALSE\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		je %s_true\n		mov eax, FALSE		mov dword[%s], eax\n", bufferdec, bufferdec);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		jmp %s_false\n		%s_true:\n", bufferdec, bufferdec);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		mov eax,TRUE\n		mov dword[%s],eax\n		%s_false:\n", bufferdec, bufferdec);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					else {
						printfAndExit(logFile, "ERROR: NON BOOL FOR OP_NOT", ERROR_OP_NOT_NON_BOOL, lineNumber, ast->kids[0]->info);
					}
					registerCounter++;
				}
				else {
					char bufferdecx[200];
					sprintf(bufferdecx, "t%d", directRegisters[0]);
					int type_of_var = 0;
					type_of_var = searchTable(table, bufferdecx, logFile);
					if (type_of_var == IDN_BOOL) {
						sprintf(buffer, "		mov eax, dword[%s]\n		cmp eax, FALSE\n", bufferdecx);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		je %s_true\n		mov dword[%s], FALSE\n", bufferdec, bufferdec);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		jmp %s_false\n		%s_true:\n", bufferdec, bufferdec);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		mov dword[%s],TRUE\n		%s_false:\n", bufferdec, bufferdec);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					else {
						printfAndExit(logFile, "ERROR: NON BOOL FOR OP_NOT", ERROR_OP_NOT_NON_BOOL, lineNumber, ast->kids[0]->info);
					}
					registerCounter++;
				}
			}
			else {
				char buffer1[200];
				char buffer2[200];
				if (ast->kids[0]->type == IDENTIFIER) {
					sprintf(buffer1, "%s", ast->kids[0]->info);
				}
				else if (ast->kids[0]->type == LITERAL_NUMBER || ast->kids[0]->type == LITERAL_DECIMAL
					|| ast->kids[0]->type == RESERVED_BOOL_FALSE || ast->kids[0]->type == RESERVED_BOOL_TRUE) {
					sprintf(buffer1, "t%d", registerCounter);
					insertSymbolToken(IDN_BOOL, ast, buffer1, table);
					sprintf(buffer, "	%s : dd %s\n", buffer1, ast->kids[0]->info);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer1, "t%d", registerCounter);
					registerCounter++;
				}
				else {
					regsub++;
					sprintf(buffer1, "t%d", directRegisters[0]);
				}
				if (ast->kids[1]->type == IDENTIFIER) {
					sprintf(buffer2, "%s", ast->kids[1]->info);
				}
				else if (ast->kids[1]->type == LITERAL_NUMBER || ast->kids[1]->type == LITERAL_DECIMAL
					|| ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_TRUE) {
					sprintf(buffer2, "t%d", registerCounter);
					insertSymbolToken(IDN_BOOL, ast, buffer2, table);
					sprintf(buffer, "	%s : dd %s\n", buffer2, ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);
					printf(buffer2);
					fprintf(logFile, buffer2);
					sprintf(buffer2, "t%d", registerCounter);
					registerCounter++;
				}
				else {
					regsub++;
					sprintf(buffer2, "t%d", directRegisters[1]);
				}
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_BOOL, ast, bufferdec, table);
				sprintf(buffer, "	%s : dd 0\n", bufferdec);
				datacode = InsertList(datacode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		mov eax, dword[%s]\n		mov ebx, dword[%s]\n", buffer1, buffer2);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		%s eax,ebx\n		cmp eax, TRUE\n", (ast->type == OP_AND) ? "and" : ((ast->type == OP_OR) ? "or" : "xor"));
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		je %s_true\n		mov dword[%s], FALSE\n", bufferdec, bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		jmp %s_false\n		%s_true:\n", bufferdec, bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		mov dword[%s],TRUE\n		%s_false:\n", bufferdec, bufferdec);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
			}
		}
		else if (ast->type == OP_ATTRIBUTION)
		{
			sprintf(buffer, "%s", ast->kids[0]->info);
			int type_of_the_var_space = 0;
			type_of_the_var_space = searchTable(table, buffer, logFile);
			int type_of_var = 0;
			if (ast->kids[1]->type == LITERAL_NUMBER || ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_TRUE) {
				switch (type_of_the_var_space) {
				case IDN_NUMBER:
				case IDN_BOOL:
					strupper(ast->kids[1]->info);
					sprintf(buffer, "		mov eax, %s\n", ast->kids[1]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		mov dword[%s], eax\n\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					break;
				case IDN_DECIMAL:
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "	t%d :  dd %s \n", registerCounter, ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);
					registerCounter++;
					sprintf(buffer, "		fild dword[t%d]\n", registerCounter - 1);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fstp dword[%s]\n\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					break;
				}
			}
			else if (ast->kids[1]->type == LITERAL_DECIMAL) {
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
				datacode = InsertList(datacode, buffer);
				switch (type_of_the_var_space) {
				case IDN_NUMBER:
					sprintf(buffer, "		fld	dword[t%d]; need to convert 32 - bit to 64 - bit\n", registerCounter);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fistp dword[%s]\n\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					break;
				case IDN_DECIMAL:
					sprintf(buffer, "		fld	dword[t%d]; need to convert 32 - bit to 64 - bit\n", registerCounter);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fstp dword[%s]\n\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					break;
				}
				registerCounter++;
			}
			else if (ast->kids[1]->type == LITERAL_CHAR || ast->kids[1]->type == LITERAL_STRING) {
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_STRING, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(buffer, "	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
				printf(buffer);
				fprintf(logFile, buffer);
				datacode = InsertList(datacode, buffer);
				sprintf(buffer, "		push t%d\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
				sprintf(buffer, "		push %s\n\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		call _strcpy\n\n");
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		add esp, 8; params * 4\n\n");
				progcode = InsertList(progcode, buffer);
			}
			else {
				sprintf(buffer, "t%d", registerCounter - 1);
				type_of_var = searchTable(table, buffer, logFile);
				if (type_of_var == type_of_the_var_space) {
					switch (type_of_var)
					{
					case IDN_NUMBER: case IDN_BOOL:
						sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		mov dword[%s], eax\n\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						break;
					case IDN_DECIMAL:
						sprintf(buffer, "		fld dword[t%d]\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		fstp dword[%s]\n\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						break;
					case IDN_STRING:case IDN_CHAR:
						sprintf(buffer, "		push t%d\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push %s\n\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		call _strcpy\n\n");
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		add esp, 8; params * 4\n\n");
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						break;
					}
				}
				else {
					switch (type_of_var) {
					case IDN_NUMBER: case IDN_BOOL:
						if (type_of_the_var_space == IDN_DECIMAL) {
							sprintf(buffer, "		fild dword[t%d]\n", registerCounter - 1);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		fstp dword[%s]\n\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						else {
							sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter - 1);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		mov dword[%s], eax\n\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						break;
					case IDN_DECIMAL:
						if (type_of_the_var_space == IDN_NUMBER || type_of_the_var_space == IDN_BOOL) {
							sprintf(buffer, "		fld dword[t%d]\n", registerCounter - 1);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		fistp dword[%s]\n\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						else {
							sprintf(buffer, "		fld dword[t%d]\n", registerCounter - 1);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
							sprintf(buffer, "		fistp dword[%s]\n\n", ast->kids[0]->info);
							progcode = InsertList(progcode, buffer);
							printf(buffer);
							fprintf(logFile, buffer);
						}
						break;
					}
				}
			}
		}
		else if (ast->type == DECLARE)
		{
			if (ast->kids[0]->type == IDN_NUMBER || ast->kids[0]->type == IDN_BOOL)
			{
				int element = 1;
				while (ast->kids[element] != NULL) {
					if (ast->kids[element]->type == OP_ATTRIBUTION) {
						sprintf(bufferdec, "%s", ast->kids[element]->kids[0]->info);
						insertSymbolToken(ast->kids[0]->type, ast, bufferdec, table);
						sprintf(buffer, "	%s : dd 0\n", ast->kids[element]->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					else if (ast->kids[element]->type == IDENTIFIER) {
						sprintf(bufferdec, "%s", ast->kids[element]->info);
						insertSymbolToken(ast->kids[0]->type, ast, bufferdec, table);
						sprintf(buffer, "	%s : dd 0\n", ast->kids[element]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					element++;
				}
			}
			else if (ast->kids[0]->type == IDN_DECIMAL)
			{
				int element = 1;
				while (ast->kids[element] != NULL) {
					if (ast->kids[element]->type == OP_ATTRIBUTION) {
						sprintf(bufferdec, "%s", ast->kids[element]->kids[0]->info);
						insertSymbolToken(ast->kids[0]->type, ast, bufferdec, table);
						sprintf(buffer, "	%s : dd 0.0\n", ast->kids[element]->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					else if (ast->kids[1]->type == IDENTIFIER) {
						sprintf(bufferdec, "%s", ast->kids[element]->info);
						insertSymbolToken(ast->kids[0]->type, ast, bufferdec, table);
						sprintf(buffer, "	%s : dd 0.0\n", ast->kids[element]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					element++;
				}
			}
			else if (ast->kids[0]->type == IDN_STRING) {
				int element = 1;
				while (ast->kids[element] != NULL) {
					if (ast->kids[element]->type == OP_ATTRIBUTION) {
						sprintf(bufferdec, "%s", ast->kids[element]->kids[0]->info);
						insertSymbolToken(ast->kids[0]->type, ast, bufferdec, table);
						sprintf(buffer, "	%s : times  256  db ``, 0\n", ast->kids[element]->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					else if (ast->kids[1]->type == IDENTIFIER) {
						sprintf(bufferdec, "%s", ast->kids[element]->info);
						insertSymbolToken(ast->kids[0]->type, ast, bufferdec, table);
						sprintf(buffer, "	%s : times  256 db ``, 0\n", ast->kids[element]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					element++;
				}
			}
			else if (ast->kids[0]->type == IDN_CHAR) {
				int element = 1;
				while (ast->kids[element] != NULL) {
					if (ast->kids[element]->type == OP_ATTRIBUTION) {
						sprintf(bufferdec, "%s", ast->kids[element]->kids[0]->info);
						insertSymbolToken(ast->kids[0]->type, ast, bufferdec, table);
						sprintf(buffer, "	%s : times  10  db ``, 0\n", ast->kids[element]->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					else if (ast->kids[1]->type == IDENTIFIER) {
						sprintf(bufferdec, "%s", ast->kids[element]->info);
						insertSymbolToken(ast->kids[0]->type, ast, bufferdec, table);
						sprintf(buffer, "	%s : times  10 db ``, 0\n", ast->kids[element]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
					}
					element++;
				}
			}
		}
		else if (ast->type == RESERVED_ON) {
			if (ast->kids[0]->type == RESERVED_CONSOLE) {
				int type_of_var = 0;
				if (ast->kids[1]->type == IDENTIFIER && ast->kids[1] != NULL)
				{
					type_of_var = searchTable(table, ast->kids[1]->info, logFile);
					switch (type_of_var)
					{
					case IDN_NUMBER:
						sprintf(buffer, "		push dword[%s]\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf("		push dword[%s]\n", ast->kids[1]->info);
						fprintf(logFile, "		push dword[%s]\n", ast->kids[1]->info);
						sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
						break;
					case IDN_DECIMAL:
						progcode = InsertList(progcode, "		sub esp, 8;reserve stack for a double in stack\n");
						printf("		sub esp, 8;reserve stack for a double in stack\n");
						fprintf(logFile, "		sub esp, 8;reserve stack for a double in stack\n");
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
						sprintf(buffer, "		push formatoutdecimal; push message into ESP\n");
						break;
					case IDN_STRING:
						sprintf(buffer, "		push %s\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutstring; push message into ESP\n");
						break;
					case IDN_CHAR:
						sprintf(buffer, "		push %s\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutchar; push message into ESP\n");
						break;
					case IDN_BOOL:
						sprintf(buffer, "		mov eax, dword[%s]\n		cmp eax, 0\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		je t%d_false\n		push stringTrue\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		jmp t%d_true\n		t%d_false:\n", registerCounter, registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push stringFalse\n		t%d_true:\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutbool; push message into ESP\n");
						registerCounter++;
						break;
					default:
						sprintf(buffer, "		push dword[%s]\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf("		push dword[%s]\n", ast->kids[1]->info);
						fprintf(logFile, "		push dword[%s]\n", ast->kids[1]->info);
						sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
						break;
					}
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else if (ast->kids[1]->type == LITERAL_NUMBER)
				{
					sprintf(buffer, "	t%d : dd %s\n", registerCounter, ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);
					sprintf(buffer, "		push dword[t%d]\n", registerCounter);
					progcode = InsertList(progcode, buffer);
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
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_STRING, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);
					sprintf(buffer, "		push t%d\n", registerCounter);
					progcode = InsertList(progcode, buffer);
					sprintf(buffer, "		push formatoutstring; push format into ESP\n");
					progcode = InsertList(progcode, buffer);
					printf("		push t%d\n", registerCounter);
					fprintf(logFile, "		push t%d\n", registerCounter);
					printf("	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
					printf("		push formatoutstring; push format into ESP\n");
					fprintf(logFile, "		push formatoutstring; push format into ESP\n");
					registerCounter++;
				}
				else if (ast->kids[1]->type == LITERAL_CHAR)
				{
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_CHAR, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);
					sprintf(buffer, "		push t%d\n", registerCounter);
					progcode = InsertList(progcode, buffer);
					sprintf(buffer, "		push formatoutchar; push format into ESP\n");
					progcode = InsertList(progcode, buffer);
					printf("		push t%d\n", registerCounter);
					fprintf(logFile, "		push t%d\n", registerCounter);
					printf("	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
					printf("		push formatoutchar; push format into ESP\n");
					fprintf(logFile, "		push formatoutchar; push format into ESP\n");
					registerCounter++;
				}
				else if (ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_TRUE)
				{
					strupper(ast->kids[1]->info);
					sprintf(buffer, "		push %s\n", ast->kids[1]->info);
					progcode = InsertList(progcode, buffer);
					sprintf(buffer, "		push formatoutbool; push format into ESP\n");
					progcode = InsertList(progcode, buffer);
					printf("		push %s\n", ast->kids[1]->info);
					fprintf(logFile, "		push %s\n", ast->kids[1]->info);
					printf("		push formatoutbool; push format into ESP\n");
					fprintf(logFile, "		push formatoutbool; push format into ESP\n");
				}
				else if (ast->kids[1]->type == LITERAL_DECIMAL)
				{
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "	t%d : dd %s\n", registerCounter, ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);
					progcode = InsertList(progcode, "		sub esp, 8;reserve stack for a double in stack\n");
					printf("		sub esp, 8;reserve stack for a double in stack\n");
					fprintf(logFile, "		sub esp, 8;reserve stack for a double in stack\n");
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
					sprintf(buffer, "		push formatoutdecimal; push format into ESP\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					registerCounter++;
				}
				else {
					sprintf(buffer, "t%d", registerCounter - 1);
					type_of_var = searchTable(table, buffer, logFile);
					switch (type_of_var)
					{
					case IDN_NUMBER:
						sprintf(buffer, "		push dword[t%d]\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
						break;
					case IDN_DECIMAL:
						sprintf(buffer, "		sub esp, 8;reserve stack for a double in stack\n");
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		mov ebx, t%d\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		fld dword[ebx];load float\n");
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		fstp qword[esp];store double(8087 does the conversion internally)\n");
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutdecimal; push message into ESP\n");
						break;
					case IDN_STRING:
						sprintf(buffer, "		push t%d\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutstring; push message into ESP\n");
						break;
					case IDN_CHAR:
						sprintf(buffer, "		push t%d\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutchar; push message into ESP\n");
						break;
					case IDN_BOOL:
						sprintf(buffer, "		mov eax, dword[t%d]\n		cmp eax, 0\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		je t%d_false\n		push stringTrue\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		jmp t%d_true\n		t%d_false:\n", registerCounter, registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push stringFalse\n		t%d_true:\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutbool; push message into ESP\n");
						registerCounter++;
						break;
					default:
						sprintf(buffer, "		push dword[t%d]\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
						break;
					}
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				sprintf(buffer, "		call _printf; call defined function\n");
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				if (ast->kids[1]->type == LITERAL_DECIMAL || type_of_var == IDN_DECIMAL)
				{
					sprintf(buffer, "		add esp, 12; params * 4\n\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else {
					sprintf(buffer, "		add esp, 8; params * 4\n\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
			}
		}
		else if (ast->type == RESERVED_IN) {
			if (ast->kids[0]->type == RESERVED_CONSOLE) {
				sprintf(bufferdec, "t%d", registerCounter);
				sprintf(buffer, "		push t%d ; Temporary Read\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				if (ast->kids[1] == NULL || ast->kids[1]->type == RESERVED_CAST_STRING) {
					insertSymbolToken(IDN_STRING, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resb 256 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		push formatinstring; number reading\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else if (ast->kids[1]->type == RESERVED_CAST_NUMBER) {
					insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resd 1 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		push formatinnumber; number reading\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else if (ast->kids[1]->type == RESERVED_CAST_DECIMAL) {
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resd 1 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		push formatindecimal; number reading\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else if (ast->kids[1]->type == RESERVED_CAST_CHAR) {
					insertSymbolToken(IDN_STRING, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resb 256 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		push formatinchar; number reading\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else if (ast->kids[1]->type == RESERVED_CAST_BOOL) {
					insertSymbolToken(IDN_BOOL, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resd 1 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		push formatinbool; number reading\n");
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				sprintf(buffer, "		call _scanf; call defined function\n");
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		add esp, 8; params * 4\n\n");
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
			}
		}
		else if (ast->type == RESERVED_CAST_DECIMAL && ast->kids[0] != NULL) {
			if (ast->kids[0]->type == LITERAL_NUMBER) {
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(bufferdec, "t%d", registerCounter + 1);
				insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(buffer, "	t%d : dd %s\n", registerCounter, ast->kids[0]->info);
				datacode = InsertList(datacode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "	t%d : dd 0.0\n", registerCounter + 1);
				datacode = InsertList(datacode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		fild dword[t%d]\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				sprintf(buffer, "		fstp dword[t%d]\n\n", registerCounter + 1);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
			}
			else if (ast->kids[0]->type == IDENTIFIER) {
				int type_of_var = 0;
				type_of_var = searchTable(table, ast->kids[0]->info, logFile);
				switch (type_of_var) {
				case IDN_NUMBER:
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "	t%d : dd 0.0\n", registerCounter);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fild dword[%s]\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fstp dword[t%d]\n\n", registerCounter);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					break;
				}
			}
			else {
				int type_of_var = 0;
				char buffersearcher[200];
				sprintf(buffersearcher, "t%d", registerCounter - 1);
				type_of_var = searchTable(table, buffersearcher, logFile);
				switch (type_of_var) {
				case IDN_NUMBER:
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(buffer, "	t%d : dd 0.0\n", registerCounter);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fild dword[%s]\n", buffersearcher);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					sprintf(buffer, "		fstp dword[t%d]\n\n", registerCounter);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					break;
				}
			}
			registerCounter++;
		}
	}
	return registerCounter - 1;
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