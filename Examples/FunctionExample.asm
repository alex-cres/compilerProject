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

	t1 : dd 0 
	t2 : dd 0.30 
	t3 : dd 0.5 
	x : dd 0.0
	y : dd 0.0
	t4 : times  256  db `\nGive me the x: `,0 
	t6 : times  256  db `\nGive me the y: `,0 
	area : dd 0.0
	t8 : times  256  db `\nThe Area: `,0 


SECTION .bss
	t5 : resd 1 
	t7 : resd 1 


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		imul eax, ebx ; Multiplying First and Second Operand Number
		mov dword[t1] , eax ;Result 

		fld	dword[t2]; need to convert 32 - bit to 64 - bit
		fstp dword[x]

		fld	dword[t3]; need to convert 32 - bit to 64 - bit
		fstp dword[y]

		push t4
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t5 ; Temporary Read
		push formatindecimal; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		fld dword[t5]
		fstp dword[x]

		push t6
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t7 ; Temporary Read
		push formatindecimal; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		fld dword[t7]
		fstp dword[y]

		fld dword[t7]
		fstp dword[area]

		push t8
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t8
		push formatoutstring; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

