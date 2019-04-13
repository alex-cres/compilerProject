#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "CodeGenerator.h"
#include "ErrorHandling.h"

int registerCounter = 1;
int registerLoopsCounter = 1;

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
	progcode = InsertList(progcode, "	extern _strcat\n");
	progcode = InsertList(progcode, "	extern _strcpy\n");
	progcode = InsertList(progcode, "	_main:\n\n");

	GenerateIntermidiateCode(ast, logFile, datacode, progcode, table, registerLoopsCounter, rescode);
	progcode = InsertList(progcode, "\n\n	mov eax, 0 \n");
	progcode = InsertList(progcode, "	ret\n\n");
	datacode = InsertList(datacode, "\n");
	datacode = InsertList(datacode, "\n");
	rescode = InsertList(rescode, "\n");
	rescode = InsertList(rescode, "\n");
	Element*data = datacode;
	Element*text = progcode;
	Element* res = rescode;
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

void GenerateIntermidiateCode(Node * ast, FILE* logFile, Element* datacode, Element* progcode, SymbolToken * table, int breakerLoop, Element* rescode)
{
	int loopnumber = registerLoopsCounter;
	int i = 0;
	int regsub = 0;
	bool floatingpointed = False;
	bool concatString = False;
	char buffer[1024];
	if (ast == NULL) {
		return;
	}
	

	//loopers, ifs, things that encompass other encapsualtions
	if (ast->type == RESERVED_LOOP) {
		registerLoopsCounter++;
		char looperID[100];
		sprintf(looperID, "l%d", loopnumber);
		
		insertSymbolToken(IDN_NUMBER, ast, looperID, table);
		sprintf(buffer, "	%s : dd  0\n", looperID);
		datacode = InsertList(datacode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		sprintf(buffer, "		;looper %s begin\n",looperID);
		progcode = InsertList(progcode, buffer);
		printf(buffer);
		fprintf(logFile, buffer);
		
		if (ast->kids[0]!=NULL && ast->kids[1]!=NULL) {
			GenerateIntermidiateCode(ast->kids[0], logFile, datacode, progcode, table, loopnumber, rescode);
			if (ast->kids[0]->type==IDENTIFIER) {
				int type_of_var = 0;
				SymbolToken*searcher = table;
				while (searcher != NULL) {
					if (0 == strcmp(ast->kids[0]->info, searcher->name))
					{
						type_of_var = searcher->type_of_symbol;
						break;
					}
					searcher = searcher->next;
				}
				if (type_of_var==IDN_NUMBER) {
					
					sprintf(buffer, "		mov eax, dword[%s]\n",ast->kids[0]->info);
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
				SymbolToken*searcher = table;
				char buffersearcher[100];
				sprintf(buffersearcher,"t%d", registerCounter - 1);
				while (searcher != NULL) {
					if (0 == strcmp(buffersearcher, searcher->name))
					{
						type_of_var = searcher->type_of_symbol;
						break;
					}
					searcher = searcher->next;
				}
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

			GenerateIntermidiateCode(ast->kids[1], logFile, datacode, progcode, table, loopnumber, rescode);

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
	
	else {
		while (ast->kids[i] != NULL && i < MAX_CHILDREN)
		{
			GenerateIntermidiateCode(ast->kids[i], logFile, datacode, progcode, table, breakerLoop, rescode);
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
			char bufferdec[100];
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
		else if (ast->type == OP_ADD || ast->type == OP_MINUS || ast->type == OP_MUL || ast->type == OP_DIV) {// http://www.website.masmforum.com/tutorials/fptute/fpuchap5.htm // integer in float op
			char bufferdec[100];
			int type_of_var = 0;
			int type_of_var2 = 0;
			if (ast->kids[0]->type == LITERAL_NUMBER && ast->kids[1]->type == LITERAL_NUMBER)
			{
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
				strcpy(bufferdec, "");

				sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				strcpy(buffer, "");

				sprintf(buffer, "		mov ebx, %s ; Moving Second Operand Number\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);

			}
			else if (ast->kids[0]->type == LITERAL_DECIMAL && ast->kids[1]->type == LITERAL_DECIMAL)
			{
				sprintf(bufferdec, "t%d", registerCounter);
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
			else if (ast->kids[0]->type == IDENTIFIER && ast->kids[1]->type == IDENTIFIER) {

				sprintf(bufferdec, "t%d", registerCounter);
				strcpy(buffer, "");

				SymbolToken*searcher = table;
				while (searcher != NULL) {
					if (0 == strcmp(ast->kids[0]->info, searcher->name))
					{
						type_of_var = searcher->type_of_symbol;

					}
					if (0 == strcmp(ast->kids[1]->info, searcher->name))
					{
						type_of_var2 = searcher->type_of_symbol;

					}
					searcher = searcher->next;
				}
				if (type_of_var == type_of_var2) {

					insertSymbolToken(type_of_var, ast, bufferdec, table);
					strcpy(bufferdec, "");
					switch (type_of_var) {
					case IDN_NUMBER:
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
					SymbolToken*searcher = table;
					while (searcher != NULL) {
						if (0 == strcmp(ast->kids[1]->info, searcher->name))
						{
							type_of_var = searcher->type_of_symbol;
							break;
						}
						searcher = searcher->next;
					}
					if (type_of_var == IDN_DECIMAL) {
						floatingpointed = True;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(bufferdec, "t%d", registerCounter + 1);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						strcpy(buffer, "");

						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fild dword[t%d] ; Moving First Operand Number\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", ast->kids[1]->info);
						registerCounter++;
					}
					else if (type_of_var == IDN_NUMBER) {
						floatingpointed = False;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
						strcpy(bufferdec, "");

						strcpy(buffer, "");
						sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");

						sprintf(buffer, "		mov ebx, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
					}

				}
				else {
					floatingpointed = False;
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
					strcpy(bufferdec, "");

					strcpy(buffer, "");
					sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					strcpy(buffer, "");
					regsub++;
					sprintf(buffer, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);

				}
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);

			}
			else if (ast->kids[1]->type == LITERAL_NUMBER)
			{
				if (ast->kids[0]->type == IDENTIFIER) {
					SymbolToken*searcher = table;
					while (searcher != NULL) {
						if (0 == strcmp(ast->kids[0]->info, searcher->name))
						{
							type_of_var = searcher->type_of_symbol;
							break;
						}
						searcher = searcher->next;
					}
					if (type_of_var == IDN_DECIMAL) {
						floatingpointed = True;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(bufferdec, "t%d", registerCounter + 1);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						strcpy(buffer, "");

						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fild dword[t%d] ; Moving Second Operand Number\n", registerCounter);
						registerCounter++;
					}
					else if (type_of_var == IDN_NUMBER) {
						floatingpointed = False;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
						strcpy(bufferdec, "");

						strcpy(buffer, "");
						sprintf(buffer, "		mov eax, dword[%s] ; Moving Second Operand Number Var\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		mov ebx, %s ; Moving First Operand Number\n", ast->kids[1]->info);

					}

				}
				else {
					floatingpointed = False;
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
					strcpy(bufferdec, "");

					strcpy(buffer, "");
					sprintf(buffer, "		mov eax, %s ; Moving First Operand Number\n", ast->kids[0]->info);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					strcpy(buffer, "");
					regsub++;
					sprintf(buffer, "		mov ebx, dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - regsub);

				}
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);

			}
			else if (ast->kids[0]->type == LITERAL_DECIMAL) {

				if (ast->kids[1]->type == IDENTIFIER) {

					SymbolToken*searcher = table;
					while (searcher != NULL) {
						if (0 == strcmp(ast->kids[1]->info, searcher->name))
						{
							type_of_var = searcher->type_of_symbol;
							break;
						}
						searcher = searcher->next;
					}
					if (type_of_var == IDN_DECIMAL) {
						floatingpointed = True;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(bufferdec, "t%d", registerCounter + 1);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						strcpy(buffer, "");

						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[%s] ; Moving Second Operand Decimal Var\n", ast->kids[1]->info);
						registerCounter++;
					}
					else if (type_of_var == IDN_NUMBER) {
						floatingpointed = True;
						strcpy(bufferdec, "");
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						sprintf(bufferdec, "t%d", registerCounter + 1);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						strcpy(buffer, "");

						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fild dword[%s] ; Moving Second Operand Number Var\n", ast->kids[1]->info);
						registerCounter++;
					}

				}
				else {
					SymbolToken*searcher = table;
					char searcherbuf[150];
					sprintf(searcherbuf, "t%d", registerCounter - 1);
					while (searcher != NULL) {
						if (0 == strcmp(searcherbuf, searcher->name))
						{
							type_of_var = searcher->type_of_symbol;
							break;
						}
						searcher = searcher->next;
					}
					floatingpointed = True;
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(bufferdec, "t%d", registerCounter + 1);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					strcpy(buffer, "");

					if (type_of_var == IDN_DECIMAL) {
						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving Second Operand Decimal Var\n", registerCounter - 1);
					}
					else if (type_of_var == IDN_NUMBER) {
						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[0]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fild dword[t%d] ; Moving Second Operand Number Var\n", registerCounter - 1);
					}


					registerCounter++;

				}
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else if (ast->kids[1]->type == LITERAL_DECIMAL) {
				if (ast->kids[0]->type == IDENTIFIER) {

					SymbolToken*searcher = table;
					while (searcher != NULL) {
						if (0 == strcmp(ast->kids[0]->info, searcher->name))
						{
							type_of_var = searcher->type_of_symbol;
							break;
						}
						searcher = searcher->next;
					}
					if (type_of_var == IDN_DECIMAL) {
						floatingpointed = True;
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						sprintf(bufferdec, "t%d", registerCounter + 1);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						strcpy(buffer, "");

						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");

						sprintf(buffer, "		fld dword[%s] ; Moving First Operand Decimal Var\n", ast->kids[0]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving Second Operand Decimal Var\n", registerCounter);
						registerCounter++;
					}
					else if (type_of_var == IDN_NUMBER) {
						floatingpointed = True;
						strcpy(bufferdec, "");
						sprintf(bufferdec, "t%d", registerCounter);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						sprintf(bufferdec, "t%d", registerCounter + 1);
						insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
						strcpy(bufferdec, "");
						strcpy(buffer, "");

						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fild dword[%s] ; Moving First Operand Number Var\n", ast->kids[0]->info);

						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving Second Operand Decimal Var\n", registerCounter);
						registerCounter++;
					}

				}
				else {
					SymbolToken*searcher = table;
					char searcherbuf[150];
					sprintf(searcherbuf, "t%d", registerCounter - 1);
					while (searcher != NULL) {
						if (0 == strcmp(searcherbuf, searcher->name))
						{
							type_of_var = searcher->type_of_symbol;
							break;
						}
						searcher = searcher->next;
					}
					floatingpointed = True;
					sprintf(bufferdec, "t%d", registerCounter);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					sprintf(bufferdec, "t%d", registerCounter + 1);
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					strcpy(bufferdec, "");
					strcpy(buffer, "");

					if (type_of_var == IDN_DECIMAL) {
						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving First Operand Decimal Var\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving Second Operand Decimal Var\n", registerCounter);
					}
					else if (type_of_var == IDN_NUMBER) {
						sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
						datacode = InsertList(datacode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fild dword[t%d] ; Moving First Operand Number Var\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		fld dword[t%d] ; Moving Second Operand Decimal Var\n", registerCounter);
					}


					registerCounter++;
				}
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);

			}
			else {
				sprintf(bufferdec, "t%d", registerCounter);
				strcpy(buffer, "");

				if (ast->kids[0]->type == IDENTIFIER) {
					SymbolToken*searcher = table;
					while (searcher != NULL) {
						char  searchbuf[100];
						sprintf(searchbuf, "t%d", registerCounter - 1);
						if (0 == strcmp(ast->kids[0]->info, searcher->name))
						{
							type_of_var = searcher->type_of_symbol;

						}
						if (0 == strcmp(searchbuf, searcher->name))
						{
							type_of_var2 = searcher->type_of_symbol;

						}
						searcher = searcher->next;
					}
					if (type_of_var == type_of_var2) {
						insertSymbolToken(type_of_var, ast, bufferdec, table);
						strcpy(bufferdec, "");
						switch (type_of_var) {
						case IDN_NUMBER:
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

				if (ast->kids[1]->type == IDENTIFIER) {

					SymbolToken*searcher = table;
					char  searchbuf[100];
					sprintf(searchbuf, "t%d", registerCounter - 1);
					while (searcher != NULL) {
						if (0 == strcmp(searchbuf, searcher->name))
						{
							type_of_var = searcher->type_of_symbol;

						}
						if (0 == strcmp(ast->kids[1]->info, searcher->name))
						{
							type_of_var2 = searcher->type_of_symbol;

						}
						searcher = searcher->next;
					}
					if (type_of_var == type_of_var2) {
						insertSymbolToken(type_of_var, ast, bufferdec, table);
						strcpy(bufferdec, "");
						switch (type_of_var) {
						case IDN_NUMBER:
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


			}

			//Operation
			strcpy(buffer, "");
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
			else if (concatString) {
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
		else if (ast->type == OP_MINOR || ast->type == OP_BIGGER || ast->type == OP_BIGGER_EQUAL || 
		ast->type == OP_MINOR_EQUAL || ast->type == OP_EQUAL || ast->type == OP_NOT_EQUAL) {
			char bufferdec[100];
			
			if (ast->kids[0]->type == IDENTIFIER) {
				sprintf(buffer, "		mov eax, dword[%s]\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else  if(ast->kids[0]->type == LITERAL_NUMBER || ast->kids[0]->type == LITERAL_DECIMAL || ast->kids[0]->type == LITERAL_CHAR ){
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken((ast->kids[0]->type==LITERAL_NUMBER)?IDN_NUMBER:((ast->kids[0]->type == LITERAL_DECIMAL)?IDN_DECIMAL:IDN_CHAR), ast, bufferdec, table);
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
				sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter-1);
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
				sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
			}
			else  if (ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_TRUE) {
				sprintf(buffer, "		mov eax, %s\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else {
				sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter - 1);
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
			sprintf(buffer, "		mov dword[%s],FALSE\n		jmp %s_false\n", bufferdec, bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			sprintf(buffer, "		%s_true:\n		mov dword[%s],TRUE\n		%s_false:\n\n", bufferdec, bufferdec, bufferdec);
			progcode = InsertList(progcode, buffer);
			printf(buffer);
			fprintf(logFile, buffer);
			registerCounter++;
		}
		else if (ast->type==OP_AND || ast->type == OP_OR || ast->type == OP_XOR || ast->type == OP_NOT) {
			if (ast->type== OP_NOT && ast->kids[0]!=NULL) {
				char bufferdec[200];
				sprintf(bufferdec, "t%d", registerCounter);
				strcpy(buffer, "");
				insertSymbolToken(IDN_BOOL, ast, bufferdec, table);
				sprintf(buffer, "	%s : dd 0\n", bufferdec);
				datacode = InsertList(datacode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				if (ast->kids[0]->type==IDENTIFIER) {
					int type_of_var = 0;
					SymbolToken*current = table;
					while (current != NULL) {
						if (0 == strcmp(ast->kids[0]->info, current->name)) {
							type_of_var = current->type_of_symbol;
							break;
						}
						current = current->next;

					}
					if (type_of_var==IDN_BOOL) {
				
						sprintf(buffer, "		mov eax, dword[%s]\n		cmp eax, FALSE\n",ast->kids[0]->info);
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
				else {
					char bufferdecx[200];
					sprintf(bufferdecx,"t%d",registerCounter-1);
					int type_of_var = 0;
					SymbolToken*current = table;
					while (current != NULL) {
						if (0 == strcmp(bufferdecx, current->name)) {
							type_of_var = current->type_of_symbol;
							break;
						}
						current = current->next;

					}
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

			}
		}
		else if (ast->type == OP_ATTRIBUTION)
		{
		int type_of_var = 0;
			if (ast->kids[1]->type == LITERAL_NUMBER ||	ast->kids[1]->type == RESERVED_BOOL_FALSE || ast->kids[1]->type == RESERVED_BOOL_TRUE ) {
				strupper(ast->kids[1]->info);
				strcpy(buffer, "");
				sprintf(buffer, "		mov eax, %s\n", ast->kids[1]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
			}
			else if (ast->kids[1]->type == LITERAL_DECIMAL) {
				char bufferdec[100];
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
				strcpy(bufferdec, "");
				strcpy(buffer, "");
				sprintf(buffer, "	t%d : dd %s \n", registerCounter, ast->kids[1]->info);
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
				sprintf(buffer, "	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
				printf(buffer);
				fprintf(logFile, buffer);

				datacode = InsertList(datacode, buffer);
				strcpy(buffer, "");
				sprintf(buffer, "		push t%d\n", registerCounter);
				progcode = InsertList(progcode, buffer);

				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;

			}
			else {
			
				strcpy(buffer, "");
				sprintf(buffer, "t%d", registerCounter - 1);
				SymbolToken*current = table;
				while (current != NULL) {
					if (0 == strcmp(buffer, current->name)) {
						type_of_var = current->type_of_symbol;
						break;
					}
					current = current->next;

				}
				strcpy(buffer, "");

				switch (type_of_var)
				{
				case IDN_NUMBER: case IDN_BOOL:
					strcpy(buffer, "");
					sprintf(buffer, "		mov eax, dword[t%d]\n", registerCounter - 1);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					break;
				case IDN_DECIMAL:
					strcpy(buffer, "");
					sprintf(buffer, "		fld dword[t%d]\n", registerCounter - 1);
					progcode = InsertList(progcode, buffer);

					printf(buffer);
					fprintf(logFile, buffer);

					break;

				case IDN_STRING:case IDN_CHAR:
					strcpy(buffer, "");
					sprintf(buffer, "		push t%d\n", registerCounter - 1);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				
					break;
				}
			}

			if (ast->kids[1]->type ==LITERAL_DECIMAL || type_of_var == IDN_DECIMAL) {
				strcpy(buffer, "");
				sprintf(buffer, "		fstp dword[%s]\n\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
			
			}
			else if(ast->kids[1]->type == LITERAL_STRING || type_of_var == IDN_STRING || ast->kids[1]->type == LITERAL_CHAR|| type_of_var == IDN_CHAR) {
				strcpy(buffer, "");
				sprintf(buffer, "		push %s\n\n", ast->kids[0]->info);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		call _strcpy\n\n");
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);

				strcpy(buffer, "");
				sprintf(buffer, "		add esp, 8; params * 4\n\n");
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
			if (ast->kids[0]->type == IDN_NUMBER ||  ast->kids[0]->type == IDN_BOOL)
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
			else if (ast->kids[0]->type == IDN_STRING) {

				strcpy(buffer, "");
				if (ast->kids[1]->type == OP_ATTRIBUTION) {
					sprintf(buffer, "	%s : times  256  db ``, 0\n", ast->kids[1]->kids[0]->info);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else if (ast->kids[1]->type == IDENTIFIER) {
					sprintf(buffer, "	%s : times  256 db ``, 0\n", ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);

				}

			

			}
			else if (ast->kids[0]->type == IDN_CHAR ) {

				strcpy(buffer, "");
				if (ast->kids[1]->type == OP_ATTRIBUTION) {
					sprintf(buffer, "	%s : times  10  db ``, 0\n", ast->kids[1]->kids[0]->info);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
				}
				else if (ast->kids[1]->type == IDENTIFIER) {
					sprintf(buffer, "	%s : times  10 db ``, 0\n", ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);

				}



			}
		}
		else if (ast->type == RESERVED_ON) {
			if (ast->kids[0]->type == RESERVED_CONSOLE) {
				int type_of_var = 0;
				
				if (ast->kids[1]->type == IDENTIFIER && ast->kids[1]!=NULL)
				{

				
					SymbolToken*current = table;
					while (current != NULL) {
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
						sprintf(buffer, "		push %s\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

						strcpy(buffer, "");
						sprintf(buffer, "		push formatoutstring; push message into ESP\n");
						break;
					case IDN_CHAR:

						strcpy(buffer, "");
						sprintf(buffer, "		push %s\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

						strcpy(buffer, "");
						sprintf(buffer, "		push formatoutchar; push message into ESP\n");
						break;
					case IDN_BOOL:
						
						sprintf(buffer, "		mov eax, dword[%s]\n		cmp eax, 0\n", ast->kids[1]->info);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						
						sprintf(buffer, "		je t%d_false\n		push stringTrue\n",registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

						sprintf(buffer, "		jmp t%d_true\n		t%d_false:\n", registerCounter,registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

						sprintf(buffer, "		push stringFalse\n		t%d_true:\n", registerCounter);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);
						
						strcpy(buffer, "");
						sprintf(buffer, "		push formatoutbool; push message into ESP\n");
						registerCounter++;
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
					sprintf(buffer, "	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);

					strcpy(buffer, "");
					sprintf(buffer, "		push t%d\n", registerCounter);
					progcode = InsertList(progcode, buffer);

					strcpy(buffer, "");
					sprintf(buffer, "		push formatoutstring; push format into ESP\n");
					progcode = InsertList(progcode, buffer);

					printf("		push t%d\n", registerCounter );
					fprintf(logFile, "		push t%d\n", registerCounter );
					printf("	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
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
					sprintf(buffer, "	t%d : times  256  db `%s`,0 \n", registerCounter, ast->kids[1]->info);
					datacode = InsertList(datacode, buffer);

					strcpy(buffer, "");
					sprintf(buffer, "		push t%d\n", registerCounter );
					progcode = InsertList(progcode, buffer);

					strcpy(buffer, "");
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
				sprintf(buffer, "t%d", registerCounter - 1);
					SymbolToken*current = table;
					while (current != NULL) {
						if (0 == strcmp(buffer, current->name)) {
							type_of_var = current->type_of_symbol;
							break;
						}
						current = current->next;
			
					}
					strcpy(buffer, "");

					switch (type_of_var)
					{
					case IDN_NUMBER:
						strcpy(buffer, "");
						sprintf(buffer, "		push dword[t%d]\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

						strcpy(buffer, "");
						sprintf(buffer, "		push formatoutnumber; push message into ESP\n");
						break;
					case IDN_DECIMAL:

						progcode = InsertList(progcode, "		sub esp, 8;reserve stack for a double in stack\n");
						printf("		sub esp, 8;reserve stack for a double in stack\n");
						fprintf(logFile, "		sub esp, 8;reserve stack for a double in stack\n");

						strcpy(buffer, "");
						sprintf(buffer, "		mov ebx, t%d\n", registerCounter-1);
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
						sprintf(buffer, "		push t%d\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

						strcpy(buffer, "");
						sprintf(buffer, "		push formatoutstring; push message into ESP\n");
						break;
					case IDN_CHAR:
						strcpy(buffer, "");
						sprintf(buffer, "		push t%d\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

						strcpy(buffer, "");
						sprintf(buffer, "		push formatoutchar; push message into ESP\n");
						break;
					case IDN_BOOL:
						strcpy(buffer, "");
						sprintf(buffer, "		push dword[t%d]\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

						strcpy(buffer, "");
						sprintf(buffer, "		push formatoutbool; push message into ESP\n");
						break;

					default:
						strcpy(buffer, "");
						sprintf(buffer, "		push dword[t%d]\n", registerCounter - 1);
						progcode = InsertList(progcode, buffer);
						printf(buffer);
						fprintf(logFile, buffer);

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
				char bufferdec[100];
				sprintf(bufferdec, "t%d", registerCounter);
				
				strcpy(buffer, "");
				sprintf(buffer, "		push t%d ; Temporary Read\n", registerCounter);
				progcode = InsertList(progcode, buffer);

				printf(buffer);
				fprintf(logFile, buffer);
				if (ast->kids[1] == NULL||ast->kids[1]->type == RESERVED_CAST_STRING ) {
					insertSymbolToken(IDN_STRING, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resb 256 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);

					strcpy(buffer, "");
					sprintf(buffer, "		push formatinstring; number reading\n");
					progcode = InsertList(progcode, buffer);

					printf("		push formatinstring; number reading\n");
					fprintf(logFile, "		push formatinstring; number reading\n");
				}
				else if (ast->kids[1]->type == RESERVED_CAST_NUMBER) {
					insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resd 1 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);

						strcpy(buffer, "");
						sprintf(buffer, "		push formatinnumber; number reading\n");
						progcode = InsertList(progcode, buffer);

						printf("		push formatinnumber; number reading\n");
						fprintf(logFile, "		push formatinnumber; number reading\n");
					}
				else if (ast->kids[1]->type == RESERVED_CAST_DECIMAL) {
					insertSymbolToken(IDN_DECIMAL, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resd 1 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		push formatindecimal; number reading\n");
						progcode = InsertList(progcode, buffer);

						printf("		push formatindecimal; number reading\n");
						fprintf(logFile, "		push formatindecimal; number reading\n");
					}
				else if (ast->kids[1]->type == RESERVED_CAST_CHAR) {
					insertSymbolToken(IDN_STRING, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resb 256 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		push formatinchar; number reading\n");
						progcode = InsertList(progcode, buffer);

						printf("		push formatinchar; number reading\n");
						fprintf(logFile, "		push formatinchar; number reading\n");
					}
				else if (ast->kids[1]->type == RESERVED_CAST_BOOL) {
					insertSymbolToken(IDN_BOOL, ast, bufferdec, table);
					sprintf(buffer, "	t%d : resd 1 \n", registerCounter);
					rescode = InsertList(rescode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
						strcpy(buffer, "");
						sprintf(buffer, "		push formatinbool; number reading\n");
						progcode = InsertList(progcode, buffer);

						printf("		push formatinbool; number reading\n");
						fprintf(logFile, "		push formatinbool; number reading\n");
					}

				strcpy(buffer, "");
				sprintf(buffer, "		call _scanf; call defined function\n");
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				strcpy(buffer, "");
				sprintf(buffer, "		add esp, 8; params * 4\n\n");
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
				registerCounter++;
			}
		}
		else if (ast->type == RESERVED_CAST_DECIMAL && ast->kids[0]!=NULL) {
		char bufferdec[200];
		if (ast->kids[0]->type==LITERAL_NUMBER) {
				
				
				sprintf(bufferdec, "t%d", registerCounter);
				insertSymbolToken(IDN_NUMBER, ast, bufferdec, table);
				strcpy(bufferdec, "");
				sprintf(bufferdec, "t%d", registerCounter+1);
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
				sprintf(buffer, "		fstp dword[t%d]\n\n", registerCounter+1);
				progcode = InsertList(progcode, buffer);
				printf(buffer);
				fprintf(logFile, buffer);
					
				registerCounter++;
				
				//integer
				//fild dword[t%d]
				//fstp dword[t%d]
			}
			else if (ast->kids[0]->type == IDENTIFIER) {
				int type_of_var = 0;
				SymbolToken*current = table;
				while (current != NULL) {
					if (0 == strcmp(ast->kids[0]->info, current->name)) {
						type_of_var = current->type_of_symbol;
						break;
					}
					current = current->next;
				}

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
					sprintf(buffer, "		fstp dword[t%d]\n\n",registerCounter);
					progcode = InsertList(progcode, buffer);
					printf(buffer);
					fprintf(logFile, buffer);
					break;
				}
					
			}
			else {

			int type_of_var = 0;
			SymbolToken*current = table;
			char buffersearcher[200];
			sprintf(buffersearcher,"t%d", registerCounter - 1);
			while (current != NULL) {
				if (0 == strcmp(buffersearcher, current->name)) {
					type_of_var = current->type_of_symbol;
					break;
				}
				current = current->next;
			}

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