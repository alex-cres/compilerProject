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
	e : dd 0
	t1 : dd 3.14 
	pi : dd 0.0
	bool : dd 0
	nbool : dd 0
	t2 : times  256  db `\nBegin Tests:\n`,0 
	t3 : times  256  db `\n\tCalculating tests:`,0 
	t4 : times  256  db `\n\t\tWith: N i<<1,j<<1; AND D pi<<3.14 AND B bool << True, nbool << False\n\n`,0 
	t5 : times  256  db `\n\t\t i << `,0 
	t6 : times  256  db `\n`,0 
	t7 : times  256  db `\n\t\t j << `,0 
	t8 : times  256  db `\n`,0 
	t9 : times  256  db `\n\t\t pi << `,0 
	t10 : times  256  db `\n`,0 
	t11 : times  256  db `\n\t\t i*j << `,0 
	t12 : dd 0 
	t13 : times  256  db `\n`,0 
	t14 : times  256  db `\n\t\t j/i << `,0 
	t15 : dd 0 
	t16 : times  256  db `\n`,0 
	t17 : times  256  db `\n\t\t j+j << `,0 
	t18 : dd 0 
	t19 : times  256  db `\n`,0 
	t20 : times  256  db `\n\t\t j-j << `,0 
	t21 : dd 0 
	t22 : times  256  db `\n`,0 
	t23 : times  256  db `\n\t\t pi-i << `,0 
	t24 : dd 0 
	t25 : times  256  db `\n`,0 
	t26 : times  256  db `\n\t\t pi/j << `,0 
	t27 : dd 0 
	t28 : times  256  db `\n`,0 
	t29 : times  256  db `\n\t\t pi*j << `,0 
	t30 : dd 0 
	t31 : times  256  db `\n`,0 
	t32 : times  256  db `\n\t\t pi+pi << `,0 
	t33 : dd 0 
	t34 : times  256  db `\n`,0 
	t35 : times  256  db `\nFinished Tests\n`,0 


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

		sub esp, 8;reserve stack for a double in stack
		mov ebx, pi
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t10
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t11
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[i] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t12] , eax ;Result 

		push dword[t12]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t13
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t14
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[i] ; Moving Second Operand Number Var
		idiv ebx ; Dividing First and Second Operand Number
		mov dword[t15] , eax ;Result 

		push dword[t15]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t16
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t17
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t18] , eax ;Result 

		push dword[t18]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t19
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t20
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[j] ; Moving First Operand Number Var
		mov ebx, dword[j] ; Moving Second Operand Number Var
		sub eax, ebx ; Subtracting First and Second Operand Number
		mov dword[t21] , eax ;Result 

		push dword[t21]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t22
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t23
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fild dword[i] ; Moving Second Operand Number Var
		fsub ; Subtracting First and Second Operand Number
		fstp dword[t24] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t24
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t25
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t26
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fild dword[j] ; Moving Second Operand Number Var
		fdiv ; Dividing First and Second Operand Number
		fstp dword[t27] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t27
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t28
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t29
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fild dword[j] ; Moving Second Operand Number Var
		fmul ; Multiplying First and Second Operand Number
		fstp dword[t30] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t30
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t31
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t32
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fld dword[pi] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t33] ;Result 

		sub esp, 8;reserve stack for a double in stack
		mov ebx, t33
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t34
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t35
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

