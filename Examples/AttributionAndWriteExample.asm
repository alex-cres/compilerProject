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

	t1 : times  256  db `Starting Tests:\n`,0 
	t2 : times  256  db `	Declaring Variables:\n`,0 
	b : dd 0
	notb : dd 0
	t3 : times  256  db `Hello World!!!!`,0 
	str : times  256  db ``, 0
	t4 : times  256  db `\n`,0 
	cn : times  10  db ``, 0
	a : dd 0
	t5 : dd 3.14 
	pi : dd 0
	t6 : times  256  db `\n		Boolean b<<`,0 
	t8 : times  256  db `\n		String str<<`,0 
	t9 : times  256  db `\n		Char cn<<`,0 
	t10 : times  256  db `\n		Number a<<`,0 
	t11 : times  256  db `\n		Decimal pi<<`,0 
	t12 : times  256  db `\n\n\n\n`,0 
	t13 : dd 0
	t14 : dd 0


SECTION .bss


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

		push t2
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, TRUE
		mov dword[b], eax

		mov eax, FALSE
		mov dword[notb], eax

		push t3
		push str

		call _strcpy

		add esp, 8; params * 4

		push t4
		push cn

		call _strcpy

		add esp, 8; params * 4

		mov eax, 2
		mov dword[a], eax

		fld	dword[t5]; need to convert 32 - bit to 64 - bit
		fstp dword[pi]

		push t6
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[b]
		cmp eax, 0
		je t7_false
		push stringTrue
		jmp t7_true
		t7_false:
		push stringFalse
		t7_true:
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t8
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push str
		push formatoutstring; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t9
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push cn
		push formatoutchar; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t10
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[a]
		push formatoutnumber; push message into ESP
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
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t12
		push b

		call _strcpy

		add esp, 8; params * 4

		mov eax, dword[b]
		cmp eax, FALSE
		je t13_true
		mov dword[t13], FALSE
		jmp t13_false
		t13_true:
		mov dword[t13],TRUE
		t13_false:
		mov eax, dword[t13]
		mov dword[b], eax

		mov eax, dword[t13]
		mov dword[b], eax

		mov eax, dword[b]
		cmp eax, 0
		je t14_false
		push stringTrue
		jmp t14_true
		t14_false:
		push stringFalse
		t14_true:
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

