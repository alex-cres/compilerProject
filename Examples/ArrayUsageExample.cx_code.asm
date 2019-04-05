SECTION .data
	formatoutnumber: db "%d",10
	formatoutdecimal: db "%f",10
	formatoutchar: db "%c",10
	formatoutstring: db "%s",10
	formatoutbool: db "%s",10
SECTION .text
	global _main
	extern _printf
	_main:
