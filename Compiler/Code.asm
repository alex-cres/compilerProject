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

	azz : dd 0
	t1 : dd 1
	t2 : dd 0
	a : dd 0
	t3 : dd 1
	t4 : dd 0
	t5 :  dd 20 
	decimal : dd 0.0
	numeral : dd 0
	t6 : dd 2.0 
	decimal2 : dd 0.0
	t7 : dd 2.0 
	numeral2 : dd 0
	t8 : dd 0 
	nresultNumeralDecimal : dd 0
	t9 : dd 0 
	dresultNumeralDecimal : dd 0.0
	t10 : dd 0 
	nresultNumeralNumeral : dd 0
	t11 : dd 0 
	dresultNumeralNumeral : dd 0.0
	t12 : dd 0 
	nresultDecimalDecimal : dd 0
	t13 : dd 0 
	dresultDecimalDecimal : dd 0.0


SECTION .bss


SECTION .text
	global _main
	extern _printf
	extern _scanf
	extern _strcat
	extern _strcpy
	_main:

		mov eax, 5
		mov dword[azz], eax

		l1_begin:
		mov eax, dword[azz]
		mov ebx, dword[t1]
		cmp eax, ebx
		jne	t2_true
		mov eax,FALSE
		mov dword[t2],eax
		jmp t2_false
		t2_true:
		mov eax,TRUE
		mov dword[t2],eax
		t2_false:

		mov ebx, dword[t2]
		mov eax, TRUE
		cmp eax, ebx
		je l1_looper_true
		jmp l1_looper_end
		l1_looper_true:
		mov eax, 2
		mov dword[a], eax


		jmp l1_looper_end

		l1_looper_continue:
		jmp l1_begin
		l1_looper_end:
		l2_begin:
		mov eax, 4
		mov dword[a], eax


		jmp l2_looper_end

		mov eax, dword[azz]
		mov ebx, dword[t3]
		cmp eax, ebx
		jne	t4_true
		mov eax,FALSE
		mov dword[t4],eax
		jmp t4_false
		t4_true:
		mov eax,TRUE
		mov dword[t4],eax
		t4_false:

		mov ebx, dword[t4]
		mov eax, TRUE
		cmp eax, ebx
		je l2_looper_true
		jmp l2_looper_end
		l2_looper_true:
		l2_looper_continue:
		jmp l2_begin
		l2_looper_end:
		fild dword[t5]
		fstp dword[decimal]

		mov eax, 2
		mov dword[numeral], eax

		fld	dword[t6]; need to convert 32 - bit to 64 - bit
		fstp dword[decimal2]

		fld	dword[t7]; need to convert 32 - bit to 64 - bit
		fistp dword[numeral2]

		fild dword[numeral] ; Moving First Operand Number Var
		fld dword[decimal] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t8] ;Result 

		fld dword[t8]
		fistp dword[nresultNumeralDecimal]

		fild dword[numeral] ; Moving First Operand Number Var
		fld dword[decimal] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t9] ;Result 

		fld dword[t9]
		fstp dword[dresultNumeralDecimal]

		mov eax, dword[numeral] ; Moving First Operand Number Var
		mov ebx, dword[numeral] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t10] , eax ;Result 

		mov eax, dword[t10]
		mov dword[nresultNumeralNumeral], eax

		mov eax, dword[numeral] ; Moving First Operand Number Var
		mov ebx, dword[numeral] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t11] , eax ;Result 

		fild dword[t11]
		fstp dword[dresultNumeralNumeral]

		fld dword[decimal] ; Moving First Operand Decimal Var
		fld dword[decimal] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t12] ;Result 

		fld dword[t12]
		fistp dword[nresultDecimalDecimal]

		fld dword[decimal] ; Moving First Operand Decimal Var
		fld dword[decimal] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t13] ;Result 

		fld dword[t13]
		fstp dword[dresultDecimalDecimal]



	mov eax, 0 
	ret

