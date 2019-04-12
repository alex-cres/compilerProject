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

	t1 : times  256  db `\n\n`,0 
	www : dd 0
	t3 : times  256  db `Finished`,0 


SECTION .bss
	t2 : resd 1 


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		push t1
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t2 ; Temporary Read
		push formatindecimal; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		fld dword[t2]
		fstp dword[www]

		sub esp, 8;reserve stack for a double in stack
		mov ebx, www
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t3
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

