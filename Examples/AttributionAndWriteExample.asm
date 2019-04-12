SECTION .data
	FALSE equ 0 
	TRUE equ 1 
	formatoutnumber: db "%d", 0
	formatoutdecimal: db "%f", 0
	formatoutchar: db "%s", 0
	formatoutstring: db "%s", 0
	formatoutbool: db "%d", 0

	formatinnumber: db "%d", 0
	formatindecimal: db "%f", 0
	formatinchar: db "%c", 0
	formatinstring: db "%s", 0
	formatinbool: db "%d", 0

	t1 : times  256  db `asa`,0 
	zx : times  256  db ``, 0
	t2 : times  256  db `\n`,0 
	t5 : times  256  db ``, 0
	t4 : times  256  db `aaaa`, 0
	t3 : times  256  db `qqqq`, 0
	t6 : times  256  db `\n3\n`,0 
	a : dd 0
	b : dd 0
	l1 : dd  0
	t7 : dd 0 
	t8 : times  256  db `\na\n`,0 
	l2 : dd  0
	t9 : dd 0 
	t10 : times  256  db `\na+2\n`,0 
	l3 : dd  0
	t11 : dd 0 
	t12 : dd 0 
	t13 : times  256  db `\nLop In Lop\n\n\n`,0 
	l4 : dd  0
	l5 : dd  0
	t14 : times  256  db `,`,0 
	t15 : times  256  db `\t`,0 
	t16 : dd 0 
	t17 : times  256  db `\n`,0 
	t18 : dd 0 
	t19 : times  256  db `\n\n`,0 
	t20 : dd 0.0
	www : dd In


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		push t1
		push zx

		call _strcpy

		add esp, 8; params * 4

		push zx
		push formatoutstring; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t2
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push	t3 ; Concat Begin
		push	t5 ; Concat Dest
		call _strcat
		add esp, 8
		push	t4 ; Concat Second
		push	t5 ; Concat Dest
		call _strcat
		add esp, 8
		push t5
		push zx

		call _strcpy

		add esp, 8; params * 4

		push zx
		push formatoutstring; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t6
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, 3
		mov dword[a], eax

		;looper l1 begin
		mov eax, 3
		mov dword[l1],eax
		l1_looper:



		push dword[a]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 1 ; Moving First Operand Number
		sub eax, ebx ; Subtracting First and Second Operand Number
		mov dword[t7] , eax ;Result 

		mov eax, dword[t7]
		mov dword[a], eax



		l1_looper_continue:


		mov eax , dword[l1]
		sub eax,1
		mov dword[l1], eax
		cmp eax, 0
		jle l1_looper_end
		jmp l1_looper
		l1_looper_end:
		;looper l1 end
		push t8
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, 3
		mov dword[a], eax

		;looper l2 begin
		mov eax, dword[a]
		mov dword[l2],eax
		l2_looper:



		push dword[a]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 1 ; Moving First Operand Number
		sub eax, ebx ; Subtracting First and Second Operand Number
		mov dword[t9] , eax ;Result 

		mov eax, dword[t9]
		mov dword[a], eax



		l2_looper_continue:


		mov eax , dword[l2]
		sub eax,1
		mov dword[l2], eax
		cmp eax, 0
		jle l2_looper_end
		jmp l2_looper
		l2_looper_end:
		;looper l2 end
		push t10
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, 1
		mov dword[a], eax

		;looper l3 begin
		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 2 ; Moving First Operand Number
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t11] , eax ;Result 

		mov eax, dword[t11]
		mov dword[l3],eax
		l3_looper:



		push dword[a]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 1 ; Moving First Operand Number
		sub eax, ebx ; Subtracting First and Second Operand Number
		mov dword[t12] , eax ;Result 

		mov eax, dword[t12]
		mov dword[a], eax



		l3_looper_continue:


		mov eax , dword[l3]
		sub eax,1
		mov dword[l3], eax
		cmp eax, 0
		jle l3_looper_end
		jmp l3_looper
		l3_looper_end:
		;looper l3 end
		push t13
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, 3
		mov dword[a], eax

		;looper l4 begin
		mov eax, dword[a]
		mov dword[l4],eax
		l4_looper:



		mov eax, 3
		mov dword[b], eax

		;looper l5 begin
		mov eax, dword[b]
		mov dword[l5],eax
		l5_looper:



		push dword[a]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t14
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[b]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t15
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[b] ; Moving Second Operand Number Var
		mov ebx, 1 ; Moving First Operand Number
		sub eax, ebx ; Subtracting First and Second Operand Number
		mov dword[t16] , eax ;Result 

		mov eax, dword[t16]
		mov dword[b], eax



		l5_looper_continue:


		mov eax , dword[l5]
		sub eax,1
		mov dword[l5], eax
		cmp eax, 0
		jle l5_looper_end
		jmp l5_looper
		l5_looper_end:
		;looper l5 end
		push t17
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 1 ; Moving First Operand Number
		sub eax, ebx ; Subtracting First and Second Operand Number
		mov dword[t18] , eax ;Result 

		mov eax, dword[t18]
		mov dword[a], eax



		l4_looper_continue:


		mov eax , dword[l4]
		sub eax,1
		mov dword[l4], eax
		cmp eax, 0
		jle l4_looper_end
		jmp l4_looper
		l4_looper_end:
		;looper l4 end
		push t19
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[t20] ; Temporary Read
		push formatindecimal; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		fld dword[t20]
		fstp dword[www]

		sub esp, 8;reserve stack for a double in stack
		mov ebx, www
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4



	mov eax, 0 
	ret

