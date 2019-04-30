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

	a : dd 0
	t1 : dd 0 
	t2 : dd 0 
	t3 : dd 0
	t4 : dd 0
	t5 : times  256  db `a%2 is 0`,0 
	t6 : times  256  db `a%2 is 1`,0 


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, 2
		mov dword[a], eax

		mov edx, 0
		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 2 ; Moving First Operand Number
		idiv ebx ; Dividing First and Second Operand Number
		mov dword[t1] , edx ;Result 

		push dword[t1]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov edx, 0
		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 2 ; Moving First Operand Number
		idiv ebx ; Dividing First and Second Operand Number
		mov dword[t2] , edx ;Result 

		mov eax, dword[t2]
		mov ebx, dword[t3]
		cmp eax, ebx
		je	t4_true
		mov eax,FALSE
		mov dword[t4],eax
		jmp t4_false
		t4_true:
		mov eax,TRUE
		mov dword[t4],eax
		t4_false:


		mov ebx, dword[t4]

		cmp  ebx, TRUE

		je t4_if_then

		jmp t4_if_else

		t4_if_then:
		push t5
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t4_if_end

		t4_if_else:
		push t6
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		t4_if_end:


	mov eax, 0 
	ret

