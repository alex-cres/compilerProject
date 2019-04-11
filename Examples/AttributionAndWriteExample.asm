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



	mov eax, 0 
	ret

