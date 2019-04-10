SECTION .data
	FALSE equ 0 
	TRUE equ 1 
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
	aguana : dd 0
	ae : dd 0
	t1 : dd 2.5 
	e : dd 2.5
	t2 : db `\n Testing String Literal \n`,0 
	t3 : db `AFIFNEI`,0 
	t4 : db `\n Testing Char Literal \n`,0 
	t5 : db `a`,0 
	t6 : db `\n Testing Decimal Variable \n`,0 
	t7 : db `\n Testing Decimal Literal \n`,0 
	t8 : dd 2.8
	t9 : db `\n Testing Decimal and Number Operations \n`,0 
	t10 : dd 2 
	t11 : dd 0 
	t12 : db `\t`,0 
	t13 : dd 4 
	t14 : dd 0 
	t15 : db `\t`,0 
	t16 : dd 2.5 
	t17 : dd 0 
	t18 : db `\t`,0 
	t19 : dd 6.1 
	t20 : dd 0 
	t21 : db `\t`,0 
	t22 : dd 7.1 
	t23 : dd 0 
	t24 : db `\t`,0 
	t25 : dd 8.1 
	t26 : dd 0 
	t27 : db `\t`,0 
	t28 : dd 2.5 
	t29 : dd 0 
	t30 : db `\t`,0 
	t31 : dd 2.5 
	t32 : dd 0 
	t33 : db `\t`,0 
	t34 : dd 0 
	t35 : db `\t`,0 
	t36 : dd 0 
	t37 : db `\t`,0 
	t38 : dd 0 
	t39 : db `\n`,0 
	t40 : db `\n Testing Number Variable \n`,0 
	t41 : db `\n Testing Number Literal \n`,0 
	t42 : dd 2
	t43 : db `\t`,0 
	t44 : dd 0 
	t45 : db `\t`,0 
	t46 : dd 0 
	t47 : dd 0 
	t48 : db `\t`,0 
	t49 : dd 0 
	t50 : dd 0 
	t51 : dd 0 
	x : dd 0
	t52 : db `\n Testing Boolean Variable \n`,0 
	t53 : db `\n Testing Boolean Literal \n`,0 
	t54 : db `\n Testing Boolean Literal \n`,0 


SECTION .text
	global _main
	extern _printf
	extern _scanf
	_main:

		mov eax, 0
		mov dword[aguana], eax

		mov eax, 16
		mov dword[ae], eax

		mov eax, 8
		mov dword[a], eax

		mov eax, 6
		mov dword[a], eax

		mov eax, 6
		mov dword[a], eax

		fld	dword[t1]; need to convert 32 - bit to 64 - bit
		fstp dword[e]

		push t2
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t3
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t4
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t5
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t6
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

		push t7
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t8
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push format into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t9
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fild dword[t10] ; Moving First Operand Number
		fld dword[e] ; Moving Second Operand Decimal Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t11] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t11
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t12
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[e] ; Moving First Operand Decimal Var
		fild dword[t13] ; Moving Second Operand Number
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t14] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t14
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t15
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[e] ; Moving First Operand Decimal Var
		fld dword[t16] ; Moving Second Operand Decimal Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t17] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t17
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t18
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[t19] ; Moving First Operand Decimal Var
		fld dword[e] ; Moving Second Operand Decimal Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t20] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t20
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t21
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fild dword[a] ; Moving First Operand Number Var
		fld dword[t22] ; Moving Second Operand Decimal Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t23] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t23
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t24
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[t25] ; Moving First Operand Decimal Var
		fild dword[a] ; Moving Second Operand Number Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t26] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t26
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t27
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fild dword[a] ; Moving First Operand Number Var
		fld dword[t28] ; Moving Second Operand Decimal Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t29] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t29
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t30
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[t31] ; Moving First Operand Decimal Var
		fild dword[a] ; Moving Second Operand Number Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t32] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t32
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t33
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[e] ; Moving First Operand Decimal Var
		fld dword[e] ; Moving Second Operand Decimal Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t34] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t34
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t35
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fild dword[a] ; Moving First Operand Number Var
		fld dword[e] ; Moving Second Operand Decimal Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t36] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t36
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t37
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[e] ; Moving First Operand Decimal Var
		fild dword[a] ; Moving Second Operand Number Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t38] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t38
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t39
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t40
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[a]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t41
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[t42]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t43
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[a] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t44] , eax ;Result 

		push dword[t44]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t45
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving First Operand Number Var
		mov ebx, dword[a] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t46] , eax ;Result 

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[t46] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t47] , eax ;Result 

		push dword[t47]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t48
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving First Operand Number Var
		mov ebx, dword[a] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t49] , eax ;Result 

		mov eax, dword[a] ; Moving First Operand Number Var
		mov ebx, dword[t49] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t50] , eax ;Result 

		mov eax, 2 ; Moving First Operand Number
		mov ebx, dword[t50] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t51] , eax ;Result 

		push dword[t51]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, TRUE
		mov dword[x], eax

		push t52
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[x]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t53
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push TRUE
		push formatoutbool; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t54
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push FALSE
		push formatoutbool; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

