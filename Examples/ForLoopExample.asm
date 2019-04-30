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
	t1 : dd 0.09 
	sses : dd 0.0
	t2 : dd 2 ; literal stepper for loop l1
	t3 : dd 10
	t4 : dd 0
	t5 : times  256  db `\t`,0 
	t6 : dd 0 
	t7 : times  256  db `\n`,0 
	t8 : dd 2 ; literal stepper for loop l2
	t9 : times  256  db `\t`,0 
	t10 : dd 0 
	t11 : times  256  db `\n`,0 
	t12 : dd 10
	t13 : dd 0
	t14 : dd 2 ; literal stepper for loop l3
	t15 : times  256  db `\t`,0 
	t16 : dd 0 
	t17 : times  256  db `\n`,0 
	t18 : dd 10
	t19 : dd 0
	t21 : times  256  db `\n\n\nFinished`,0 


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		fld	dword[t1]; need to convert 32 - bit to 64 - bit
		fstp dword[sses]

		mov eax, 2
		mov dword[i], eax

		l1_begin:
		mov eax, dword[i]
		mov ebx, dword[t3]
		cmp eax, ebx
		jle	t4_true
		mov eax,FALSE
		mov dword[t4],eax
		jmp t4_false
		t4_true:
		mov eax,TRUE
		mov dword[t4],eax
		t4_false:

		mov ebx, dword[t4]
		mov eax, TRUE
		cmp eax, ebx
		je l1_looper_true
		jmp l1_looper_end
		l1_looper_true:
		push dword[i]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t5
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[i] ; Moving First Operand Number Var
		mov ebx, dword[i] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t6] , eax ;Result 

		push dword[t6]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t7
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[i] ; Moving First Operand Number
		mov ebx, dword[t2] ; Moving Second Operand Number
		add eax, ebx
 		mov dword[i],eax
 		l1_looper_continue:
		jmp l1_begin
		l1_looper_end:
		mov eax, 2
		mov dword[j], eax

		l2_begin:
		mov ebx, TRUE
		cmp ebx, TRUE
		je l2_looper_true
		jmp l2_looper_end
		l2_looper_true:
		push dword[j]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t9
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t10] , eax ;Result 

		push dword[t10]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t11
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j]
		mov ebx, dword[t12]
		cmp eax, ebx
		jg	t13_true
		mov eax,FALSE
		mov dword[t13],eax
		jmp t13_false
		t13_true:
		mov eax,TRUE
		mov dword[t13],eax
		t13_false:


		mov ebx, dword[t13]

		cmp  ebx, TRUE

		je t13_if_then

		jmp t13_if_else

		t13_if_then:

		jmp l2_looper_end


		jmp t13_if_end

		t13_if_else:

		t13_if_end:
		mov eax, dword[j] ; Moving First Operand Number
		mov ebx, dword[t8] ; Moving Second Operand Number
		add eax, ebx
 		mov dword[j],eax
 		l2_looper_continue:
		jmp l2_begin
		l2_looper_end:
		mov eax, 2
		mov dword[j], eax

		l3_begin:
		mov ebx, TRUE
		cmp ebx, TRUE
		je l3_looper_true
		jmp l3_looper_end
		l3_looper_true:
		push dword[j]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t15
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t16] , eax ;Result 

		push dword[t16]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t17
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j]
		mov ebx, dword[t18]
		cmp eax, ebx
		jg	t19_true
		mov eax,FALSE
		mov dword[t19],eax
		jmp t19_false
		t19_true:
		mov eax,TRUE
		mov dword[t19],eax
		t19_false:

		mov eax , 0 
		mov ebx, dword[t19]
		cmp ebx, TRUE
		je t20_if_then
		jmp t20_if_else
		t20_if_then:


		jmp l3_looper_end


		t20_if_else:


		mov eax, dword[j] ; Moving First Operand Number
		mov ebx, dword[t14] ; Moving Second Operand Number
		add eax, ebx
 		mov dword[j],eax
 		l3_looper_continue:
		jmp l3_begin
		l3_looper_end:
		push t21
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

