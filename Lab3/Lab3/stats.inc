/*
 * stats.inc
 *
 *  Created: 2016-11-16 20:03:40
 *  updated: 2021-10-08 added printdecimal
 *   Author: Magnus Krampell
 */ 


clearstring:
	.DB "CLEARING...",0 
totalstring:
	.DB "THROWS:",0
countones:
	.DB "#  1'S:",0
counttwos:
	.DB "#  2'S:",0
countthrees:
	.DB "#  3'S:",0
countfours:
	.DB "#  4'S:",0
countfives:
	.DB "#  5'S:",0
countsixs:
	.DB "#  6'S:",0


; init statistics module
init_stat:
	RCALL clear_stat	; clear data structure
	RET

; print content of RVAL on display
; uses R24/RVAL, R5, R6

hexconv: .DB "0123456789ABCDEF"
printhex:
	LDI		ZH, HIGH(hexconv<<1)
	LDI		ZL, LOW(hexconv<<1)
	MOV		R5, RVAL		; save content
	SWAP	RVAL			; 
	ANDI	RVAL, 0x0F		; mask out low nibble
	ADD		ZL, RVAL
	CLR		R6
	ADC		ZH, R6			; handle carry, if needed		
	LPM		RVAL, Z			; convert number to ASCII
	RCALL lcd_write_char		; write to LCD
	MOV		RVAL, R5		; retrieve content
	ANDI	RVAL, 0x0F		; mask out low nibble
	LDI		ZH, HIGH(hexconv<<1)
	LDI		ZL, LOW(hexconv<<1) ; reset ZL to start
	ADD		ZL, RVAL
	CLR		R6
	ADC		ZH, R6			; handle carry, if needed		
	LPM		RVAL, Z			; convert number to ASCII
	RCALL lcd_write_char		; write to LCD
	RET

	/* print the content of RVAL as a decimal number 0-255
	Uses R16, R17, R20, R24/RVAL

	Test for 200+ and 100+ separately. For numbers 10-99, loop nad count number of 
	times 10 can be substracted. Use 2 registers to avoid myltiplication
	Finally, print single number 0-9
	*/

printdecimal:
	MOV R20, RVAL			; save value
	CPI R20, 200			; >200?
	BRLO no200				; no
	LDI R24, '2'			; yes, print...
	RCALL lcd_write_char	; ...nunber starting with 2
	SUBI R20, 200			; number is now 0-55
	RJMP tens				; 

no200:
	CPI R20, 100			; >100?
	BRLO no100				; no
	LDI R24, '1'			; yes, print...
	RCALL lcd_write_char	; ...nunber starting with 1
	SUBI R20, 100			; number is now 0-99
	RJMP tens				; 

no100:
	LDI R24, '0'			; print leading 0
	RCALL lcd_write_char	; ...nunber starting with 1

tens:
	LDI R16, 1				; first decade
	LDI R17, 10				; no need to multiply R16 by 10, instead use another register
	
nextten:
	CLC
	CP R20, R17				; remove R16*10 from number
	BRCS ones				; have we reached 0?

	INC R16					; no, next decade
	SUBI R17, -10			; add another 10
	RJMP nextten			; try next

ones:
	SUBI R17, 10			; R17 was now too much, back by 10
	SUB R20, R17			; remove from R20, to only keep 0-9
	PUSH R20				; save, in case R20 is used somewhere

	DEC R16					; R16 was also one step too large...
	MOV R24, R16			; number of tens to print
	SUBI R24, -48			; convert to ASCII
	RCALL lcd_write_char
	POP R24					; get number, now 0-9 to print
	SUBI R24, -48			; convert to ASCII
	RCALL lcd_write_char

	RET


; test routine to print decimal all numbers 0-255

testPrintDec:				
	RCALL lcd_clear_display
	CLR R5
nexttoprint:
	MOV R24, R5
	RCALL printdecimal
	LDI RVAL, ' '
	RCALL lcd_write_char
	RCALL delay_1_s
	INC R5
	BRNE nexttoprint
	RET

showstat:
	RCALL lcd_clear_display
	PRINTSTRING totalstring
	LDI RVAL, 0
	RCALL get_stat			; get total number of throws	
	PUSH RVAL
	RCALL printhex			; print as 2 hex numbers
	LDI RVAL, '('
	RCALL lcd_write_char
	POP RVAL
	RCALL printdecimal		; print same number as decimal 0-255
	LDI RVAL, ')'
	RCALL lcd_write_char
	RCALL delay_1_s

	RCALL lcd_clear_display
	PRINTSTRING countones
	LDI RVAL, 1
	RCALL get_stat			; get total number of throws	
	PUSH RVAL
	RCALL printhex			; print as 2 hex numbers
	LDI RVAL, '('
	RCALL lcd_write_char
	POP RVAL
	RCALL printdecimal		; print same number as decimal 0-255
	LDI RVAL, ')'
	RCALL lcd_write_char
	RCALL delay_1_s

	RCALL lcd_clear_display
	PRINTSTRING counttwos
	LDI RVAL, 2
	RCALL get_stat			; get total number of throws	
	PUSH RVAL
	RCALL printhex			; print as 2 hex numbers
	LDI RVAL, '('
	RCALL lcd_write_char
	POP RVAL
	RCALL printdecimal		; print same number as decimal 0-255
	LDI RVAL, ')'
	RCALL lcd_write_char
	RCALL delay_1_s

	RCALL lcd_clear_display
	PRINTSTRING countthrees
	LDI RVAL, 3
	RCALL get_stat			; get total number of throws	
	PUSH RVAL
	RCALL printhex			; print as 2 hex numbers
	LDI RVAL, '('
	RCALL lcd_write_char
	POP RVAL
	RCALL printdecimal		; print same number as decimal 0-255
	LDI RVAL, ')'
	RCALL lcd_write_char
	RCALL delay_1_s

	RCALL lcd_clear_display
	PRINTSTRING countfours
	LDI RVAL, 4
	RCALL get_stat			; get total number of throws	
	PUSH RVAL
	RCALL printhex			; print as 2 hex numbers
	LDI RVAL, '('
	RCALL lcd_write_char
	POP RVAL
	RCALL printdecimal		; print same number as decimal 0-255
	LDI RVAL, ')'
	RCALL lcd_write_char
	RCALL delay_1_s

	RCALL lcd_clear_display
	PRINTSTRING countfives
	LDI RVAL, 5
	RCALL get_stat			; get total number of throws	
	PUSH RVAL
	RCALL printhex			; print as 2 hex numbers
	LDI RVAL, '('
	RCALL lcd_write_char
	POP RVAL
	RCALL printdecimal		; print same number as decimal 0-255
	LDI RVAL, ')'
	RCALL lcd_write_char
	RCALL delay_1_s

	RCALL lcd_clear_display
	PRINTSTRING countsixs
	LDI RVAL, 6
	RCALL get_stat			; get total number of throws	
	PUSH RVAL
	RCALL printhex			; print as 2 hex numbers
	LDI RVAL, '('
	RCALL lcd_write_char
	POP RVAL
	RCALL printdecimal		; print same number as decimal 0-255
	LDI RVAL, ')'
	RCALL lcd_write_char
	RCALL delay_1_s

	RCALL lcd_clear_display
	RET

clearstat:
	RCALL lcd_clear_display
	PRINTSTRING clearstring
	RCALL clear_stat
	RET
