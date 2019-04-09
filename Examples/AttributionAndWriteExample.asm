SECTION .data
	FALSE equ 0 
	TRUE equ 1 
	formatoutnumber: db "%d", 0
	formatoutdecimal: db "%f", 0
	formatoutchar: db "%c", 0
	formatoutstring: db "%s", 0
	formatoutbool: db "%s", 0

	formatinnumber: db "%d", 0
	formatindecimal: db "%f", 0
	formatinchar: db "%c", 0
	formatinstring: db "%s", 0
	formatinbool: db "%d", 0

	a : dd 0
	aguana : dd 0
	t1 : dd 0 
	t2 : dd 0 
	t3 : dd 0 
	ae : dd 0
	x : dd 0
	t4 : dd 0 
	t5 : dd 0 
	t6 : dd 0 
	t7 : dd 0 
	t8 : dd 0 
	t9 : dd 0 
	t10 : dd 0 
	e : dd *
	t11 : db `\n Testing String Literal \n`,0 
	t12 : db `AFIFNEI`,0 
	t13 : db `\n Testing Char Literal \n`,0 
	t14 : db `a`,0 
	t15 : db `\n Testing Decimal Variable \n`,0 
	t16 : db `\n Testing Decimal Literal \n`,0 
	t17 : dd 2.55
	t18 : db `\n Testing Number Variable \n`,0 
	t19 : db `\n Testing Number Literal \n`,0 
	t20 : dd 2
	t21 : db `\t`,0 
	t22 : dd 0 
	t23 : db `\t`,0 
	t24 : dd 0 
	t25 : dd 0 
	t26 : db `\t`,0 
	t27 : dd 0 
	t28 : dd 0 
	t29 : dd 0 
	t30 : db `\n Testing Boolean Variable \n`,0 
	t31 : db `\n Testing Boolean Literal \n`,0 
	t32 : db `\n Testing Boolean Literal \n`,0 


SECTION .text
	global _main
	extern _printf
	extern _scanf
	_main:

		mov eax, 0
		mov dword[aguana], eax

		mov eax, 2 ; Moving First Operand Number
		mov ebx, 2 ; Moving Second Operand Number
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t1] , eax ;Result 

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[t1] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t2] , eax ;Result 

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[t2] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t3] , eax ;Result 

		mov eax, dword[t3]
		mov dword[ae], eax

		mov eax, TRUE
		mov dword[x], eax

		mov eax, 2 ; Moving First Operand Number
		mov ebx, 2 ; Moving Second Operand Number
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t4] , eax ;Result 

		mov eax, dword[t4] ; Moving First Operand Number Var
		mov ebx, 2 ; Moving Second Operand Number
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t5] , eax ;Result 

		mov eax, dword[t5]
		mov dword[a], eax

		mov eax, 2 ; Moving First Operand Number
		mov ebx, 2 ; Moving Second Operand Number
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t6] , eax ;Result 

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[t6] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t7] , eax ;Result 

		mov eax, dword[t7]
		mov dword[a], eax

		mov eax, 2 ; Moving First Operand Number
		mov ebx, 2 ; Moving Second Operand Number
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t8] , eax ;Result 

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[t8] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t9] , eax ;Result 

		mov eax, dword[t9]
		mov dword[a], eax

		mov eax, dword[t9] ; Moving First Operand Number Var
		mov ebx, 2 ; Moving Second Operand Number
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t10] , eax ;Result 

		mov eax, dword[t10]
		mov dword[e], eax

		push t11
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t12
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t13
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t14
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t15
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		sub esp, 8;reserve stack for a double in stack
		mov ebx, e
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t16
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t17
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push format into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t18
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[a]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t19
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[t20]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t21
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[a] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t22] , eax ;Result 

		push dword[t22]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t23
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving First Operand Number Var
		mov ebx, dword[a] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t24] , eax ;Result 

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[t24] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t25] , eax ;Result 

		push dword[t25]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t26
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving First Operand Number Var
		mov ebx, dword[a] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t27] , eax ;Result 

		mov eax, dword[a] ; Moving First Operand Number Var
		mov ebx, dword[t27] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t28] , eax ;Result 

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[t28] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t29] , eax ;Result 

		push dword[t29]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t30
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[x]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t31
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push TRUE
		push formatoutbool; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t32
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push FALSE
		push formatoutbool; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

	ret

