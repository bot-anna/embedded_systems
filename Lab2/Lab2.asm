;
; Lab2.asm
;
; Created: 2023-11-20 10:58:31
; Author : generic
;


;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU			RESET		 =	0x0000			; reset vector
	.EQU			PM_START	 =	0x0072			; start of program


;==============================================================================
; Start of program
;==============================================================================
	.CSEG
	.ORG			RESET
	RJMP			init

	.ORG			PM_START
	.INCLUDE		"keyboard.inc"
	.INCLUDE		"delay.inc"
	.INCLUDE		"lcd.inc"

;==============================================================================
; Basic initializations of stack pointer, etc.
;==============================================================================
init:
	LDI				R16,			LOW(RAMEND)		; Set stack pointer
	OUT				SPL,			R16				; at the end of RAM.
	LDI				R16,			HIGH(RAMEND)
	OUT				SPH,			R16
	.DEF			TEMP = R16						; give R16 alias temp
	RCALL			init_pins						; Initialize pins
	RCALL			lcd_init						; initialize lcd
	RJMP			main							; Jump to main

;==============================================================================
; Initialize I/O pins
;==============================================================================
init_pins:
	; PORT C
	; output:	7
	LDI		TEMP,		0xFF					; set temp to 0b11111111
	OUT		DDRD,		TEMP					; set the D port to output
	SBI		DDRG,5								; set PG5 (col 0) to output
	SBI		DDRE,3								; set PE3 (col 1) to output
	LDS		R17,		DDRH					; register R16-R31 kan användas
	ORI		R17,		0b00011000				; bit 3&4 ettställs
	STS		DDRH,		R17						; set bit 3&4 to output
	CBI		DDRF,5								; set PF5 (row 0) to input
	CBI		DDRF,4								; set PF4 (row 1) to input
	CBI		DDRE,5								; set PE5 (row 3) to input
	CBI		DDRE,4								; set PE4 (row 2) to input


	RET

;==============================================================================
; Main part of program
; Uses registers:
;	R16				temp value
;	R25				last key
;==============================================================================
main:
	/*LDI		R16,	0x0F						; load with 0x0F
	OUT		PORTD,	R16							; set in portd
	CALL	delay_1_s							; delay 1 s
	LDI		R16,	0x00						; load with 0x00
	OUT		PORTD,	R16							; set in portd
	CALL	delay_1_s							; delay 1 s */
	
	LCD_WRITE_CMD 0x80 ; Set position: col 0
	LCD_WRITE_CMD 0x41 ; row 0
	RCALL	lcd_clear_display

	LDI		R24,		5
	RCALL	delay_ms

	LCD_WRITE_CMD 0x80 ; Set position: col 0
	LCD_WRITE_CMD 0x40 ; row 0
	
	LCD_WRITE_CHR 'K'
	LCD_WRITE_CHR 'E'
	LCD_WRITE_CHR 'Y'
	LCD_WRITE_CHR ':'

	LCD_WRITE_CMD 0x80 ; Set position: col 0
	LCD_WRITE_CMD 0x41 ; row 1

	LDI		R25,		NO_KEY


main_loop:
	RCALL			read_keyboard_num
	
/*	CP				R19,			R25
	BREQ			main_loop
					
	CPI				R19,			NO_KEY
	BRNE			next							; jump to next if not equal

	LDI				R25,			NO_KEY			; load current value with NO_KEY
	RJMP			main_loop						; jump to main loop*/

	MOV		TEMP, R19
	SUBI	TEMP, NO_KEY
	BRNE	next
	LDI		R25, NO_KEY
	RJMP	main_loop

next:
	
	MOV				TEMP,			R25				; move previous current value to temp
	SUB				TEMP,			R19				; compare previous value with R19
	BRNE			next_2							; jump to next_2 if not equal
	
	RCALL			delay_1_micros
	RCALL			delay_1_micros
	RJMP			main_loop						; else jump to main loop 

next_2: 
	MOV				R25,			R19				; set current value variable to R19
	LDI				TEMP,			9				; set temp to 9
	SUB				TEMP,			R19				; subtract R19 from 9
	RCALL			delay_1_micros
	RCALL			delay_1_micros
	BRCS			IF								; if R19>9, jump to if
	RJMP			ELSE							; else, skip "if"
IF:
	LDI				TEMP,			7				; load temp with 7
	ADD				R19,			TEMP			; add 7 to temp
ELSE:												
	LDI				TEMP,			0x30			; load temp with 0x30 (ascii number offset)
	ADD				R19,			TEMP			; add to get correct ascii value
	SBI				PORTB,			4				; set D/C pin
	MOV				R24,			R19				; set command as parameter
	RCALL			lcd_write_char					; write char to the LCD
	LDI				R24,			5
	RCALL			delay_ms 
	
	
	

	RJMP main_loop								; loop back to main_loop label	

