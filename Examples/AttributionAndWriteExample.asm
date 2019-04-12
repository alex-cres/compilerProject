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

	t1 : dd 2
	t2 : dd 0.0
	xxx : dd 0
	t3 : times  256  db `\n`,0 
	a : dd 0
	t4 : dd 0.0
	t5 : times  256  db `\n`,0 
	t6 : dd 0 
	t7 : dd 0.0
	t8 : times  256  db `\n`,0 


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		fild dword[t1]
		fstp dword[t2]

		fld dword[t2]
		fstp dword[xxx]

		sub esp, 8;reserve stack for a double in stack
		mov ebx, xxx
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t3
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, 222
		mov dword[a], eax

		fild dword[a]
		fstp dword[t4]

		fld dword[t4]
		fstp dword[xxx]

		sub esp, 8;reserve stack for a double in stack
		mov ebx, xxx
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t5
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 22 ; Moving First Operand Number
		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t6] , eax ;Result 

		fild dword[t6]
		fstp dword[t7]

		fld dword[t7]
		fstp dword[xxx]

		sub esp, 8;reserve stack for a double in stack
		mov ebx, xxx
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t8
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

