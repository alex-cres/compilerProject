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
	t3 : times  256  db `\n\tReading from Console Tests:\n`,0 
	t4 : times  256  db `\n\t\tNumber: `,0 
	number : dd 0
	t6 : times  256  db `\n\t\tDecimal: `,0 
	decimal : dd 0.0
	t8 : times  256  db `\n\t\tString: `,0 
	string : times  256  db ``, 0
	t11 : times  256  db `\n\t\tChar: `,0 
	char : times  10  db ``, 0
	t13 : times  256  db `\n\t\tBoolean 1[True] or 0[False] : `,0 
	boolean : dd 0
	t15 : times  256  db `\n\n\tRead Results:\n`,0 
	t16 : times  256  db `\n\t\tNumber: `,0 
	t17 : times  256  db `\n\t\tDecimal: `,0 
	t18 : times  256  db `\n\t\tString: `,0 
	t19 : times  256  db `\n\t\tChar: `,0 
	t20 : times  256  db `\n\t\tBoolean 1[True] or 0[False] : `,0 
	t22 : times  256  db `\n\n\n`,0 
	t23 : times  256  db `\nFinished Tests\n`,0 


SECTION .bss
	t5 : resd 1 
	t7 : resd 1 
	t9 : resb 256 
	t10 : resb 256 
	t12 : resb 256 
	t14 : resd 1 


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

		push t5 ; Temporary Read
		push formatinnumber; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		mov eax, dword[t5]
		mov dword[number], eax

		push t6
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t7 ; Temporary Read
		push formatindecimal; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		fld dword[t7]
		fstp dword[decimal]

		push t8
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t9 ; Temporary Read
		push formatinstring; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		push t9
		push string

		call _strcpy

		add esp, 8; params * 4

		push t10 ; Temporary Read
		push formatinchar; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		push t11
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t12 ; Temporary Read
		push formatinchar; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		push t12
		push char

		call _strcpy

		add esp, 8; params * 4

		push t13
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t14 ; Temporary Read
		push formatinbool; number reading
		call _scanf; call defined function
		add esp, 8; params * 4

		mov eax, dword[t14]
		mov dword[boolean], eax

		push t15
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t16
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push dword[number]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t17
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		sub esp, 8;reserve stack for a double in stack
		mov ebx, decimal
		fld dword[ebx];load float
		fstp qword[esp];store double(8087 does the conversion internally)
		push formatoutdecimal; push message into ESP
		call _printf; call defined function
		add esp, 12; params * 4

		push t18
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push string
		push formatoutstring; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t19
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push char
		push formatoutchar; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t20
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[boolean]
		cmp eax, 0
		je t21_false
		push stringTrue
		jmp t21_true
		t21_false:
		push stringFalse
		t21_true:
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t22
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t23
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

