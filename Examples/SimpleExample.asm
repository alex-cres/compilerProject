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

	i : dd 0
	j : dd 0
	t1 : dd 3.14 
	pi : dd 0.0
	bool : dd 0
	nbool : dd 0
	t2 : times  256  db `\nBegin Tests:\n`,0 
	t3 : times  256  db `\n\tCalculating tests:`,0 
	t4 : times  256  db `\n\t\tWith: N i<<1,j<<1,e; AND D pi<<3.14 AND B bool << True, nbool << False\n\n`,0 
	t5 : times  256  db `\n\t\t i << `,0 
	t6 : times  256  db `\n`,0 
	t7 : times  256  db `\n\t\t j << `,0 
	t8 : times  256  db `\n`,0 
	t9 : times  256  db `\n\t\t e << `,0 
	t10 : times  256  db `\n`,0 
	t11 : times  256  db `\n\t\t pi << `,0 
	t12 : times  256  db `\n`,0 
	t13 : times  256  db `\n\t\t i*j << `,0 
	t14 : dd 0 
	t15 : times  256  db `\n`,0 
	t16 : times  256  db `\n\t\t j/i << `,0 
	t17 : dd 0 
	t18 : times  256  db `\n`,0 
	t19 : times  256  db `\n\t\t e+j << `,0 
	t20 : dd 0 
	t21 : times  256  db `\n`,0 
	t22 : times  256  db `\n\t\t e-j << `,0 
	t23 : dd 0 
	t24 : times  256  db `\n`,0 
	t25 : times  256  db `\n\t\t pi-i << `,0 
	t26 : dd 0 
	t27 : times  256  db `\n`,0 
	t28 : times  256  db `\n\t\t pi/j << `,0 
	t29 : dd 0 
	t30 : times  256  db `\n`,0 
	t31 : times  256  db `\n\t\t pi*j << `,0 
	t32 : dd 0 
	t33 : times  256  db `\n`,0 
	t34 : times  256  db `\n\t\t pi+pi << `,0 
	t35 : dd 0 
	t36 : times  256  db `\n`,0 
	t37 : times  256  db `\nFinished Tests\n`,0 


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, 1
		mov dword[i], eax

		mov eax, 1
		mov dword[j], eax

		fld	dword[t1]; need to convert 32 - bit to 64 - bit
		fstp dword[pi]

		mov eax, TRUE
		mov dword[bool], eax

		mov eax, FALSE
		mov dword[nbool], eax

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
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[i]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t6
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t7
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[j]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t8
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t9
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[e]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t10
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t11
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		sub esp, 8;reserve stack for a double in stack
		mov ebx, pi
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t12
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t13
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[i] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t14] , eax ;Result 

		push dword[t14]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t15
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t16
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[i] ; Moving Second Operand Number Var
		idiv ebx ; Dividing First and Second Operand Number
		mov dword[t17] , eax ;Result 

		push dword[t17]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t18
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t19
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t20] , eax ;Result 

		push dword[t20]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t21
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t22
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		sub eax, ebx ; Subtracting First and Second Operand Number
		mov dword[t23] , eax ;Result 

		push dword[t23]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t24
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t25
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fild dword[i] ; Moving Second Operand Number Var
		fsub ; Subtracting First and Second Operand Number
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

		push t28
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fild dword[j] ; Moving Second Operand Number Var
		fdiv ; Dividing First and Second Operand Number
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

		push t31
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fild dword[j] ; Moving Second Operand Number Var
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

		push t34
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fld dword[pi] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t35] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t35
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t36
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t37
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

