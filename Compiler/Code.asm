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
	t3 :  dd 20 
	decimal : dd 0.0
	numeral : dd 0
	t4 : dd 2.0 
	decimal2 : dd 0.0
	t5 : dd 2.0 
	numeral2 : dd 0
	t6 : dd 0 
	nresultNumeralDecimal : dd 0
	t7 : dd 0 
	dresultNumeralDecimal : dd 0.0
	t8 : dd 0 
	nresultNumeralNumeral : dd 0
	t9 : dd 0 
	dresultNumeralNumeral : dd 0.0
	t10 : dd 0 
	nresultDecimalDecimal : dd 0
	t11 : dd 0 
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
		fild dword[t3]
		fstp dword[decimal]

		mov eax, 2
		mov dword[numeral], eax

		fld	dword[t4]; need to convert 32 - bit to 64 - bit
		fstp dword[decimal2]

		fld	dword[t5]; need to convert 32 - bit to 64 - bit
		fistp dword[numeral2]

		fild dword[numeral] ; Moving First Operand Number Var
		fld dword[decimal] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t6] ;Result 

		fld dword[t6]
		fistp dword[nresultNumeralDecimal]

		fild dword[numeral] ; Moving First Operand Number Var
		fld dword[decimal] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t7] ;Result 

		fld dword[t7]
		fstp dword[dresultNumeralDecimal]

		mov eax, dword[numeral] ; Moving First Operand Number Var
		mov ebx, dword[numeral] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t8] , eax ;Result 

		mov eax, dword[t8]
		mov dword[nresultNumeralNumeral], eax

		mov eax, dword[numeral] ; Moving First Operand Number Var
		mov ebx, dword[numeral] ; Moving Second Operand Number Var
		add eax, ebx ; Adding First and Second Operand Number
		mov dword[t9] , eax ;Result 

		fild dword[t9]
		fstp dword[dresultNumeralNumeral]

		fld dword[decimal] ; Moving First Operand Decimal Var
		fld dword[decimal] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t10] ;Result 

		fld dword[t10]
		fistp dword[nresultDecimalDecimal]

		fld dword[decimal] ; Moving First Operand Decimal Var
		fld dword[decimal] ; Moving Second Operand Decimal Var
		fadd ; Adding First and Second Operand Number
		fstp dword[t11] ;Result 

		fld dword[t11]
		fstp dword[dresultDecimalDecimal]



	mov eax, 0 
	ret

