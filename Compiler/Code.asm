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
	y : dd 0
	z : dd 0
	t1 : dd 0
	t2 : times  256  db `MAIOR\n`,0 
	t3 : dd 0
	t4 : times  256  db `IGUAL\n`,0 
	t5 : dd 0
	t6 : times  256  db `MAIOR OU IGUAL\n`,0 
	t7 : dd 0
	t8 : times  256  db `MENOR\n`,0 
	t9 : dd 0
	t10 : times  256  db `NE\n`,0 
	t11 : dd 0
	t12 : times  256  db `MENOR IGUAL\n`,0 
	t13 : times  256  db `Nao`,0 


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, 4
		mov dword[y], eax

		mov eax, 4
		mov dword[z], eax

		mov eax, 3
		mov dword[x], eax

		mov eax, dword[y]
		mov ebx, dword[x]
		cmp eax, ebx
		jg	t1_true
		mov eax,FALSE
		mov dword[t1],eax
		jmp t1_false
		t1_true:
		mov eax,TRUE
		mov dword[t1],eax
		t1_false:


		mov eax, dword[t1]

		cmp  eax, TRUE

		je t1_if_then

		jmp t1_if_else

		t1_if_then:
		push t2
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[y]
		mov ebx, dword[z]
		cmp eax, ebx
		je	t3_true
		mov eax,FALSE
		mov dword[t3],eax
		jmp t3_false
		t3_true:
		mov eax,TRUE
		mov dword[t3],eax
		t3_false:


		mov eax, dword[t3]

		cmp  eax, TRUE

		je t3_if_then

		jmp t3_if_else

		t3_if_then:
		push t4
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t3_if_end

		t3_if_else:

		t3_if_end:
		mov eax, dword[y]
		mov ebx, dword[z]
		cmp eax, ebx
		jge	t5_true
		mov eax,FALSE
		mov dword[t5],eax
		jmp t5_false
		t5_true:
		mov eax,TRUE
		mov dword[t5],eax
		t5_false:


		mov eax, dword[t5]

		cmp  eax, TRUE

		je t5_if_then

		jmp t5_if_else

		t5_if_then:
		push t6
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t5_if_end

		t5_if_else:

		t5_if_end:
		mov eax, dword[x]
		mov ebx, dword[z]
		cmp eax, ebx
		jl	t7_true
		mov eax,FALSE
		mov dword[t7],eax
		jmp t7_false
		t7_true:
		mov eax,TRUE
		mov dword[t7],eax
		t7_false:


		mov eax, dword[t7]

		cmp  eax, TRUE

		je t7_if_then

		jmp t7_if_else

		t7_if_then:
		push t8
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t7_if_end

		t7_if_else:

		t7_if_end:
		mov eax, dword[x]
		mov ebx, dword[z]
		cmp eax, ebx
		jne	t9_true
		mov eax,FALSE
		mov dword[t9],eax
		jmp t9_false
		t9_true:
		mov eax,TRUE
		mov dword[t9],eax
		t9_false:


		mov eax, dword[t9]

		cmp  eax, TRUE

		je t9_if_then

		jmp t9_if_else

		t9_if_then:
		push t10
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t9_if_end

		t9_if_else:

		t9_if_end:
		mov eax, dword[x]
		mov ebx, dword[z]
		cmp eax, ebx
		jle	t11_true
		mov eax,FALSE
		mov dword[t11],eax
		jmp t11_false
		t11_true:
		mov eax,TRUE
		mov dword[t11],eax
		t11_false:


		mov eax, dword[t11]

		cmp  eax, TRUE

		je t11_if_then

		jmp t11_if_else

		t11_if_then:
		push t12
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t11_if_end

		t11_if_else:

		t11_if_end:

		jmp t1_if_end

		t1_if_else:
		push t13
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		t1_if_end:


	mov eax, 0 
	ret

