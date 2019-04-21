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
	t3 : times  256  db `\n\tCalculating tests:`,0 
	t4 : times  256  db `\n\t\tWith: N i<<1,j<<1,e; AND D pi<<3.14 AND B bool << True, nbool << False\n\n`,0 
	t5 : times  256  db `\n\t\t bool << `,0 
	t7 : times  256  db `\n`,0 
	t8 : times  256  db `\n\t\t nbool << `,0 
	t10 : times  256  db `\n`,0 
	t11 : times  256  db `\n\t\t bool | nbool << `,0 
	t12 : dd 0
	t13 : times  256  db `\n`,0 
	t14 : times  256  db `\n\t\t nbool & bool << `,0 
	t15 : dd 0
	t16 : times  256  db `\n`,0 
	t17 : times  256  db `\n\t\t bool X nbool << `,0 
	t18 : dd 0
	t19 : times  256  db `\n`,0 
	t20 : times  256  db `\n\t\t !(nbool)  << `,0 
	t21 : dd 0
	t22 : times  256  db `\n`,0 
	t23 : times  256  db `\n\t\t pi/pi=1 << `,0 
	t24 : dd 0 
	t25 : dd 1
	t26 : dd 0
	t27 : times  256  db `\n`,0 
	t28 : times  256  db `\n\t\t pi/pi=1.0 << `,0 
	t29 : dd 0 
	t30 : times 10 db `1.0`
	t31 : dd 0
	t32 : times  256  db `\n`,0 
	t33 : times  256  db `\n\t\t pi/pi=ToDecimal(1) << `,0 
	t34 : dd 0 
	t35 : dd 1
	t36 : dd 0.0
	t37 : dd 0
	t38 : times  256  db `\n`,0 
	t39 : times  256  db `\n\t\t pi<1.0 << `,0 
	t40 : times 10 db `1.0`
	t41 : dd 0
	t42 : times  256  db `\n`,0 
	t43 : times  256  db `\n\t\t pi<=1.0 << `,0 
	t44 : times 10 db `1.0`
	t45 : dd 0
	t46 : times  256  db `\n`,0 
	t47 : times  256  db `\n\t\t pi>1.0 << `,0 
	t48 : times 10 db `1.0`
	t49 : dd 0
	t50 : times  256  db `\n`,0 
	t51 : times  256  db `\n\t\t pi>=1.0 << `,0 
	t52 : times 10 db `1.0`
	t53 : dd 0
	t54 : times  256  db `\n`,0 
	t55 : times  256  db `\n\t\t pi>=1.0 X pi<=1.0 << `,0 
	t56 : times 10 db `1.0`
	t57 : dd 0
	t58 : times 10 db `1.0`
	t59 : dd 0
	t60 : dd 0
	t61 : times  256  db `\n`,0 
	t62 : times  256  db `\nFinished Tests\n`,0 


SECTION .bss


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

		push t5
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[bool]
		cmp eax, 0
		je t6_false
		push stringTrue
		jmp t6_true
		t6_false:
		push stringFalse
		t6_true:
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t7
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t8
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[nbool]
		cmp eax, 0
		je t9_false
		push stringTrue
		jmp t9_true
		t9_false:
		push stringFalse
		t9_true:
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t10
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t11
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[bool]
		mov ebx, dword[nbool]
		or eax,ebx
		cmp eax, TRUE
		je t12_true
		mov dword[t12], FALSE
		jmp t12_false
		t12_true:
		mov dword[t12],TRUE
		t12_false:
		push dword[t12]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t13
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t14
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[nbool]
		mov ebx, dword[bool]
		and eax,ebx
		cmp eax, TRUE
		je t15_true
		mov dword[t15], FALSE
		jmp t15_false
		t15_true:
		mov dword[t15],TRUE
		t15_false:
		push dword[t15]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t16
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t17
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[bool]
		mov ebx, dword[nbool]
		xor eax,ebx
		cmp eax, TRUE
		je t18_true
		mov dword[t18], FALSE
		jmp t18_false
		t18_true:
		mov dword[t18],TRUE
		t18_false:
		push dword[t18]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t19
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t20
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[nbool]
		cmp eax, FALSE
		je t21_true
		mov eax, FALSE		mov dword[t21], eax
		jmp t21_false
		t21_true:
		mov eax,TRUE
		mov dword[t21],eax
		t21_false:
		push dword[t21]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t22
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t23
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fld dword[pi] ; Moving Second Operand Decimal Var
		fdiv ; Dividing First and Second Operand Number
		fstp dword[t24] ;Result 

		mov eax, dword[t24]
		mov ebx, dword[t25]
		cmp eax, ebx
		je	t26_true
		mov eax,FALSE
		mov dword[t26],eax
		jmp t26_false
		t26_true:
		mov eax,TRUE
		mov dword[t26],eax
		t26_false:

		push dword[t26]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t27
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t28
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fld dword[pi] ; Moving Second Operand Decimal Var
		fdiv ; Dividing First and Second Operand Number
		fstp dword[t29] ;Result 

		mov eax, dword[t29]
		mov ebx, dword[t30]
		cmp eax, ebx
		je	t31_true
		mov eax,FALSE
		mov dword[t31],eax
		jmp t31_false
		t31_true:
		mov eax,TRUE
		mov dword[t31],eax
		t31_false:

		push dword[t31]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t32
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t33
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		fld dword[pi] ; Moving First Operand Decimal Var
		fld dword[pi] ; Moving Second Operand Decimal Var
		fdiv ; Dividing First and Second Operand Number
		fstp dword[t34] ;Result 

		fild dword[t35]
		fstp dword[t36]

		mov eax, dword[t36]
		mov ebx, dword[t36]
		cmp eax, ebx
		je	t37_true
		mov eax,FALSE
		mov dword[t37],eax
		jmp t37_false
		t37_true:
		mov eax,TRUE
		mov dword[t37],eax
		t37_false:

		push dword[t37]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t38
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t39
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[pi]
		mov ebx, dword[t40]
		cmp eax, ebx
		jl	t41_true
		mov eax,FALSE
		mov dword[t41],eax
		jmp t41_false
		t41_true:
		mov eax,TRUE
		mov dword[t41],eax
		t41_false:

		push dword[t41]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t42
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t43
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[pi]
		mov ebx, dword[t44]
		cmp eax, ebx
		jle	t45_true
		mov eax,FALSE
		mov dword[t45],eax
		jmp t45_false
		t45_true:
		mov eax,TRUE
		mov dword[t45],eax
		t45_false:

		push dword[t45]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t46
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t47
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[pi]
		mov ebx, dword[t48]
		cmp eax, ebx
		jg	t49_true
		mov eax,FALSE
		mov dword[t49],eax
		jmp t49_false
		t49_true:
		mov eax,TRUE
		mov dword[t49],eax
		t49_false:

		push dword[t49]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t50
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t51
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[pi]
		mov ebx, dword[t52]
		cmp eax, ebx
		jge	t53_true
		mov eax,FALSE
		mov dword[t53],eax
		jmp t53_false
		t53_true:
		mov eax,TRUE
		mov dword[t53],eax
		t53_false:

		push dword[t53]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t54
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t55
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		mov eax, dword[pi]
		mov ebx, dword[t56]
		cmp eax, ebx
		jge	t57_true
		mov eax,FALSE
		mov dword[t57],eax
		jmp t57_false
		t57_true:
		mov eax,TRUE
		mov dword[t57],eax
		t57_false:

		mov eax, dword[pi]
		mov ebx, dword[t58]
		cmp eax, ebx
		jle	t59_true
		mov eax,FALSE
		mov dword[t59],eax
		jmp t59_false
		t59_true:
		mov eax,TRUE
		mov dword[t59],eax
		t59_false:

		mov eax, dword[t59]
		mov ebx, dword[t58]
		xor eax,ebx
		cmp eax, TRUE
		je t60_true
		mov dword[t60], FALSE
		jmp t60_false
		t60_true:
		mov dword[t60],TRUE
		t60_false:
		push dword[t60]
		push formatoutbool; push message into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t61
		push formatoutchar; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4

		push t62
		push formatoutstring; push format into ESP
		call _printf; call defined function
		add esp, 8; params * 4



	mov eax, 0 
	ret

