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

	a : dd 0
	l1 : dd  0
	t1 : dd 0 


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, 0
		mov dword[a], eax

		;looper l1 begin
		mov eax, 5
		mov dword[l1],eax
		l1_looper:



		mov eax, dword[a] ; Moving Second Operand Number Var
		mov ebx, 1 ; Moving First Operand Number
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t1] , eax ;Result 

		mov eax, dword[t1]
		mov dword[a], eax



		l1_looper_continue:


		mov eax , dword[l1]
		sub eax,1
		mov dword[l1], eax
		cmp eax, 0
		jle l1_looper_end
		jmp l1_looper
		l1_looper_end:
		;looper l1 end
		push dword[a]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

