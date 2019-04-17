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

	j : dd 0
	i : dd 0
	t1 : dd 1.0 
	t2 : dd 1.0 
	e : dd 0.0
	q : dd 0.0
	t3 : dd 2 ; literal stepper for loop l1
	t4 : dd 10
	t5 : dd 0
	t6 : times  256  db `\t`,0 
	t7 : dd 0 
	t8 : times  256  db `\n`,0 
	t9 : dd 2 ; literal stepper for loop l2
	t10 : times  256  db `\t`,0 
	t11 : dd 0 
	t12 : times  256  db `\n`,0 
	t13 : dd 10
	t14 : dd 0


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		fld	dword[t1]; need to convert 32 - bit to 64 - bit
		fstp dword[e]

		fld	dword[t2]; need to convert 32 - bit to 64 - bit
		fstp dword[q]

		mov eax, 2
		mov dword[i], eax

		l1_begin:
		mov eax, dword[i]
		mov eax, dword[t4]
		cmp eax, ebx
		jle	t5_true
		mov dword[t5],FALSE
		jmp t5_false
		t5_true:
		mov dword[t5],TRUE
		t5_false:

		mov eax, dword[t5]
		cmp eax, TRUE
		je l1_true
		jmp l1_looper_end
		l1_true:
		push dword[i]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t6
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[i] ; Moving First Operand Number Var
		mov ebx, dword[i] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t7] , eax ;Result 

		push dword[t7]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t8
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[i] ; Moving First Operand Number
		mov ebx, dword[t3] ; Moving Second Operand Number
		add eax, ebx
 		mov dword[i],eax
 		l1_looper_continue:
		jmp l1_begin
		l1_looper_end:
		mov eax, 2
		mov dword[j], eax

		l2_begin:
		mov eax, TRUE
		cmp eax, TRUE
		je l2_true
		jmp l2_looper_end
		l2_true:
		push dword[j]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t10
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t11] , eax ;Result 

		push dword[t11]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t12
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j]
		mov eax, dword[t13]
		cmp eax, ebx
		jle	t14_true
		mov dword[t14],FALSE
		jmp t14_false
		t14_true:
		mov dword[t14],TRUE
		t14_false:


		mov ebx, dword[t14]

		mov eax, TRUE

		cmp eax, ebx

		je t14_if_then

		jmp t14_if_else

		t14_if_then:

		jmp l2_looper_end


		jmp t14_if_end

		t14_if_else:

		t14_if_end:
		mov eax, dword[j] ; Moving First Operand Number
		mov ebx, dword[t9] ; Moving Second Operand Number
		add eax, ebx
 		mov dword[j],eax
 		l2_looper_continue:
		jmp l2_begin
		l2_looper_end:


	mov eax, 0 
	ret

