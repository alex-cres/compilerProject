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
	t1 : dd 2 ; literal stepper for loop l1
	t2 : dd 10
	t3 : dd 0
	t4 : times  256  db `\t`,0 
	t5 : dd 0 
	t6 : times  256  db `\n`,0 
	t7 : dd 2 ; literal stepper for loop l2
	t8 : times  256  db `\t`,0 
	t9 : dd 0 
	t10 : times  256  db `\n`,0 
	t11 : dd 10
	t12 : dd 0
	t13 : dd 2 ; literal stepper for loop l3
	t14 : times  256  db `\t`,0 
	t15 : dd 0 
	t16 : times  256  db `\n`,0 
	t17 : dd 10
	t18 : dd 0
	t20 : times  256  db `\n\n\nFinished`,0 


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, 2
		mov dword[i], eax

		l1_begin:
		mov eax, dword[i]
		mov ebx, dword[t2]
		cmp eax, ebx
		jle	t3_true
		mov eax,FALSE
		mov dword[t3],eax
		jmp t3_false
		t3_true:
		mov eax,TRUE
		mov dword[t3],eax
		t3_false:

		mov ebx, dword[t3]
		mov eax, TRUE
		cmp eax, ebx
		je l1_looper_true
		jmp l1_looper_end
		l1_looper_true:
		push dword[i]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t4
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[i] ; Moving First Operand Number Var
		mov ebx, dword[i] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t5] , eax ;Result 

		push dword[t5]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t6
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[i] ; Moving First Operand Number
		mov ebx, dword[t1] ; Moving Second Operand Number
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

		push t8
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t9] , eax ;Result 

		push dword[t9]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t10
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j]
		mov ebx, dword[t11]
		cmp eax, ebx
		jg	t12_true
		mov eax,FALSE
		mov dword[t12],eax
		jmp t12_false
		t12_true:
		mov eax,TRUE
		mov dword[t12],eax
		t12_false:


		mov ebx, dword[t12]

		cmp  ebx, TRUE

		je t12_if_then

		jmp t12_if_else

		t12_if_then:

		jmp l2_looper_end


		jmp t12_if_end

		t12_if_else:

		t12_if_end:
		mov eax, dword[j] ; Moving First Operand Number
		mov ebx, dword[t7] ; Moving Second Operand Number
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

		push t14
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t15] , eax ;Result 

		push dword[t15]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t16
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j]
		mov ebx, dword[t17]
		cmp eax, ebx
		jg	t18_true
		mov eax,FALSE
		mov dword[t18],eax
		jmp t18_false
		t18_true:
		mov eax,TRUE
		mov dword[t18],eax
		t18_false:

		mov eax , 0 
		mov ebx, dword[t18]
		cmp ebx, TRUE
		je t19_if_then
		jmp t19_if_else
		t19_if_then:


		jmp l3_looper_end


		t19_if_else:


		mov eax, dword[j] ; Moving First Operand Number
		mov ebx, dword[t13] ; Moving Second Operand Number
		add eax, ebx
 		mov dword[j],eax
 		l3_looper_continue:
		jmp l3_begin
		l3_looper_end:
		push t20
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

