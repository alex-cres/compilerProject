SECTION .data
	False equ 0 
	True equ 1 
	formatoutnumber: db "%d"
	formatoutnumber: db "%d"
	formatoutdecimal: db "%f"
	formatoutchar: db "%c"
	formatoutstring: db "%s"
	formatoutbool: db "%s"

	formatinnumber: db "%d"
	formatindecimal: db "%f"
	formatinchar: db "%c"
	formatinstring: db "%s"
	formatinbool: db "%d"

	x : dd 0
	resultado : dd 0
	anagua : dd 0

	t0 : dd 0 
	t1 : dd 0 

SECTION .text
	global _main
	extern _printf
	extern _scanf
	_main:

