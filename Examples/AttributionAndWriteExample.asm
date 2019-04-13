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

	b : dd 0
	a : dd 0
	t1 : times  256  db `\n\n\n\n`,0 
	t2 : dd 2
	t3 : dd 0
	t5 : times  256  db `\n`,0 
	t6 : dd 0
	t8 : times  256  db `\n`,0 
	t9 : dd 0
	t11 : times  256  db `\n`,0 


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, TRUE
		mov dword[b], eax

		mov eax, 3
		mov dword[a], eax

		push t1
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[t2]
		mov eax, dword[a]
		cmp eax, ebx
		jl	t3_true
		mov dword[t3],FALSE
		jmp t3_false
		t3_true:
		mov dword[t3],TRUE
		t3_false:

		mov eax, dword[t3]
		mov dword[b], eax

		mov eax, dword[b]
		cmp eax, 0
		je t4_false
		push stringTrue
		jmp t4_true
		t4_false:
		push stringFalse
		t4_true:
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t5
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[b]
		cmp eax, FALSE
		je t6_true
		mov dword[t6], FALSE
		jmp t6_false
		t6_true:
		mov dword[t6],TRUE
		t6_false:
		mov eax, dword[t6]
		mov dword[b], eax

		mov eax, dword[b]
		cmp eax, 0
		je t7_false
		push stringTrue
		jmp t7_true
		t7_false:
		push stringFalse
		t7_true:
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t8
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[b]
		cmp eax, FALSE
		je t9_true
		mov dword[t9], FALSE
		jmp t9_false
		t9_true:
		mov dword[t9],TRUE
		t9_false:
		mov eax, dword[t9]
		mov dword[b], eax

		mov eax, dword[b]
		cmp eax, 0
		je t10_false
		push stringTrue
		jmp t10_true
		t10_false:
		push stringFalse
		t10_true:
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t11
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

