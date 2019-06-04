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
	t4 : dd 0
	t5 : dd 0
	t6 : dd 0
	t7 : times  256  db `IGUAL\n`,0 
	t8 : dd 0
	t9 : times  256  db `MAIOR OU IGUAL\n`,0 
	t10 : dd 0
	t11 : times  256  db `MENOR\n`,0 
	t12 : dd 0
	t13 : times  256  db `NE\n`,0 
	t14 : dd 0
	t15 : times  256  db `MENOR IGUAL\n`,0 
	t16 : times  256  db `Nao`,0 
	xx : dd 0.0
	yy : dd 0
	resultado : dd 0.0
	t18 : dd 0 


SECTION .bss
	t17 : resd 1 


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

		mov eax, dword[y]
		mov ebx, dword[z]
		cmp eax, ebx
		jge	t4_true
		mov eax,FALSE
		mov dword[t4],eax
		jmp t4_false
		t4_true:
		mov eax,TRUE
		mov dword[t4],eax
		t4_false:

		mov eax, dword[t4]
		cmp eax, FALSE
		je t5_true
		mov dword[t5], FALSE
		jmp t5_false
		t5_true:
		mov dword[t5],TRUE
		t5_false:
		mov eax, dword[t3]
		mov ebx, dword[t5]
		xor eax,ebx
		cmp eax, TRUE
		je t6_true
		mov dword[t6], FALSE
		jmp t6_false
		t6_true:
		mov dword[t6],TRUE
		t6_false:

		mov eax, dword[t6]

		cmp  eax, TRUE

		je t6_if_then

		jmp t6_if_else

		t6_if_then:
		push t7
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t6_if_end

		t6_if_else:

		t6_if_end:
		mov eax, dword[y]
		mov ebx, dword[z]
		cmp eax, ebx
		jge	t8_true
		mov eax,FALSE
		mov dword[t8],eax
		jmp t8_false
		t8_true:
		mov eax,TRUE
		mov dword[t8],eax
		t8_false:


		mov eax, dword[t8]

		cmp  eax, TRUE

		je t8_if_then

		jmp t8_if_else

		t8_if_then:
		push t9
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t8_if_end

		t8_if_else:

		t8_if_end:
		mov eax, dword[x]
		mov ebx, dword[z]
		cmp eax, ebx
		jl	t10_true
		mov eax,FALSE
		mov dword[t10],eax
		jmp t10_false
		t10_true:
		mov eax,TRUE
		mov dword[t10],eax
		t10_false:


		mov eax, dword[t10]

		cmp  eax, TRUE

		je t10_if_then

		jmp t10_if_else

		t10_if_then:
		push t11
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t10_if_end

		t10_if_else:

		t10_if_end:
		mov eax, dword[x]
		mov ebx, dword[z]
		cmp eax, ebx
		jne	t12_true
		mov eax,FALSE
		mov dword[t12],eax
		jmp t12_false
		t12_true:
		mov eax,TRUE
		mov dword[t12],eax
		t12_false:


		mov eax, dword[t12]

		cmp  eax, TRUE

		je t12_if_then

		jmp t12_if_else

		t12_if_then:
		push t13
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t12_if_end

		t12_if_else:

		t12_if_end:
		mov eax, dword[x]
		mov ebx, dword[z]
		cmp eax, ebx
		jle	t14_true
		mov eax,FALSE
		mov dword[t14],eax
		jmp t14_false
		t14_true:
		mov eax,TRUE
		mov dword[t14],eax
		t14_false:


		mov eax, dword[t14]

		cmp  eax, TRUE

		je t14_if_then

		jmp t14_if_else

		t14_if_then:
		push t15
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t14_if_end

		t14_if_else:

		t14_if_end:

		jmp t1_if_end

		t1_if_else:
		push t16
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		t1_if_end:
		push t17 ; Temporary Read
		push formatindecimal; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		fld dword[t17]
		fstp dword[xx]

		mov eax, 2
		mov dword[yy], eax

		fild dword[yy] ; Moving First Operand Number Var
		fld dword[xx] ; Moving Second Operand Decimal Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t18] ;Result 

		fld dword[t18]
		fstp dword[resultado]

		sub esp, 8;reserve stack for a double in stack
		mov ebx, resultado
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4



	mov eax, 0 
	ret

