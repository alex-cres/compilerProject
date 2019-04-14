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

	q : dd 0
	t1 : dd 2
	t2 : dd 0
	t3 : dd 0
	b : dd 0
	t4 : dd 0
	t5 : dd 1


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, 2222
		mov dword[q], eax

		mov eax, dword[t1]
		mov eax, dword[q]
		cmp eax, ebx
		jl	t2_true
		mov dword[t2],FALSE
		jmp t2_false
		t2_true:
		mov dword[t2],TRUE
		t2_false:

		mov eax, dword[t2]
		cmp eax, FALSE
		je t3_true
		mov dword[t3], FALSE
		jmp t3_false
		t3_true:
		mov dword[t3],TRUE
		t3_false:
		mov eax, dword[t3]
		mov dword[b], eax


		mov ebx, dword[b]

		mov dword[t4],ebx

		mov ebx, dword[t4]

		mov eax, TRUE

		cmp eax, ebx

		je t4_if_then

		jmp t4_if_else

		t4_if_then:
		push dword[t5]
		push formatoutnumber; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4


		jmp t4_if_end

		t4_if_else:

		t4_if_end:


	mov eax, 0 
	ret

