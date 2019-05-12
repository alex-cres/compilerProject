SECTION .data
	FALSE equ 0 
	TRUE equ 1 
	stringFalse: db `False`, 0
	stringTrue: db `True`, 0
	formatoutnumber: db "%d", 0
	formatoutdecimal: db "%f", 0
	formatoutchar: db "%s", 0
	formatoutstring: db "%s", 0
	formatoutbool: db "%s", 0

	formatinnumber: db "%d", 0
	formatindecimal: db "%f", 0
	formatinchar: db "%c", 0
	formatinstring: db "%s", 0
	formatinbool: db "%d", 0

	x : dd 0
	resultado : dd 0
	t1 : dd 0 
	t2 : dd 0 
	t3 : dd 0 
	t4 : dd 0 
	t5 : times  256  db `\n`,0 
	t6 : dd 50
	t7 : dd 0


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, 5
		mov dword[x], eax

		label:
		mov eax, 5 ; Moving First Operand Number
		mov ebx, dword[x] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t1] , eax ;Result 

		mov eax, dword[resultado] ; Moving Second Operand Number Var
		mov ebx, 2 ; Moving First Operand Number
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t2] , eax ;Result 

		mov eax, 5 ; Moving First Operand Number
		mov ebx, dword[t2] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t3] , eax ;Result 

		mov eax, dword[t1] ; Moving First Operand Number Var
		mov ebx, dword[t3] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t4] , eax ;Result 

		mov eax, dword[t4]
		mov dword[resultado], eax

		push dword[resultado]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t5
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[resultado]
		mov ebx, dword[t6]
		cmp eax, ebx
		jl	t7_true
		mov eax,FALSE
		mov dword[t7],eax
		jmp t7_false
		t7_true:
		mov eax,TRUE
		mov dword[t7],eax
		t7_false:


		mov ebx, dword[t7]

		cmp  ebx, TRUE

		je t8_if_then

		jmp t8_if_else

		t8_if_then:
		jmp label

		t8_if_else:


	mov eax, 0 
	ret

