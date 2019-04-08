SECTION .data
	False equ 0 
	True equ 1 
	formatoutnumber: db "%d", 0
	formatoutdecimal: db "%f", 0
	formatoutchar: db "%c", 0
	formatoutstring: db "%s", 0
	formatoutbool: db "%s", 0

	formatinnumber: db "%d", 0
	formatindecimal: db "%f", 0
	formatinchar: db "%c", 0
	formatinstring: db "%s", 0
	formatinbool: db "%d", 0

	a : dd 0
	aguana : dd 0
	ae : dd 0
	x : dd 0
	t1 : db `\n`,0 


SECTION .text
	global _main
	extern _printf
	extern _scanf
	_main:

		mov eax, 0
		mov dword[aguana], eax

		mov eax, 16
		mov dword[ae], eax

		mov eax, True
		mov dword[x], eax

		mov eax, 8
		mov dword[a], eax

		mov eax, 6
		mov dword[a], eax

		mov eax, 6
		mov dword[a], eax

		push dword[a]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t1
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

