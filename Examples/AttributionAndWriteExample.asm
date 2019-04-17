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

	b : dd 0
	notb : dd 0
	a : dd 0
	t1 : dd True
	t2 : dd 0
	t3 : dd 2
	t4 : dd 0
	t5 : dd 0
	t6 : dd 0
	v : dd 0


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, TRUE
		mov dword[b], eax

		mov eax, FALSE
		mov dword[notb], eax

		mov eax, 3
		mov dword[a], eax

		mov eax, dword[notb]
		mov ebx, dword[t1]
		xor eax,ebx
		cmp eax, TRUE
		je t2_true
		mov dword[t2], FALSE
		jmp t2_false
		t2_true:
		mov dword[t2],TRUE
		t2_false:
		mov eax, dword[t3]
		mov eax, dword[a]
		cmp eax, ebx
		je	t4_true
		mov dword[t4],FALSE
		jmp t4_false
		t4_true:
		mov dword[t4],TRUE
		t4_false:

		mov eax, dword[t4]
		mov ebx, dword[t3]
		and eax,ebx
		cmp eax, TRUE
		je t5_true
		mov dword[t5], FALSE
		jmp t5_false
		t5_true:
		mov dword[t5],TRUE
		t5_false:
		mov eax, dword[b]
		mov ebx, dword[t5]
		and eax,ebx
		cmp eax, TRUE
		je t6_true
		mov dword[t6], FALSE
		jmp t6_false
		t6_true:
		mov dword[t6],TRUE
		t6_false:
		mov eax, dword[t6]
		mov dword[v], eax

		mov eax, dword[v]
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



	mov eax, 0 
	ret

