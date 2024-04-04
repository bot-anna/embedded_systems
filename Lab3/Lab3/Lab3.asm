;
; Lab3.asm
;
; Created: 2023-11-20 10:58:31
; Author : Anna Håkansson, am1163
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
	.INCLUDE		"monitor.inc"
	.INCLUDE		"stats.inc"
	.INCLUDE		"stat_data.inc"
	.INCLUDE		"tarning.inc"

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
	RCALL			init_monitor
	RCALL			init_stat
	RCALL			init_tarning
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
; Write welcome on screen
;==============================================================================
write_welcome:
	LDI			ZH,			high(Str_1<<1)			; load high part of Str_1 into ZH
	LDI			ZL,			low(Str_1<<1)			; load low part of Str_1 into ZL
	CLR			R19
Nxt1: 
	LPM			R24,		Z+						; load Z into R24, then increment Z
	LPM			TEMP,		Z						; temp = Z
	CPI			TEMP,		0						; compare temp (Z) to 0
	BREQ		End1								; if equal (meaning string has ended), go to lbl end1
	PUSH		ZH									; push ZH & ZL to stack 
	PUSH		ZL									; because they are used in lcd_write_char
	RCALL		lcd_write_char						; write char
	POP			ZL									; pop and get values bakc
	POP			ZH
	RJMP		Nxt1								; loop
End1:
	RET

;==============================================================================
; Main part of program
; Uses registers:
;	R16				temp value
;	R25				last key
;==============================================================================
main:

	RCALL	lcd_clear_display					; clear display	

	LDI		R19,		5						
	RCALL	delay_ms							; delay 5ms

	LCD_WRITE_CMD 0x80							; Set position: col 0
	LCD_WRITE_CMD 0x40							; row 0
	

	PRINTSTRING Str_1							; print "WELCOME"

	RCALL delay_1_s								; delay 1 s

menu:

	RCALL	lcd_clear_display					; clear display	

	LCD_WRITE_CMD 0x80							; Set position: col 0
	LCD_WRITE_CMD 0x40							; row 0

	PRINTSTRING Str_2							; print "2. ROLL"

	LCD_WRITE_CMD 0x80							; Set position: col 0	
	LCD_WRITE_CMD 0x41							; row 1 

	PRINTSTRING Str_3							; print "3. SHOW STAT"

	LCD_WRITE_CMD 0x80							; Set position: col 0
	LCD_WRITE_CMD 0x42							; row 2 

	PRINTSTRING Str_4							; print "8. CLEAR STAT"

	LCD_WRITE_CMD 0x80							; Set position: col 0
	LCD_WRITE_CMD 0x43							; row 3

	PRINTSTRING Str_5							; print "9. MONITOR"

	LCD_WRITE_CMD 0x80							; Set position: col 0
	LCD_WRITE_CMD 0x44							; row 4 

	

	LDI		R25,		NO_KEY					; initialize R25 (not sure if necessary but still, why not)



main_loop:

	RCALL			read_keyboard					; read keyboard
	
	CP				R24,			R25				; compare R24 and R25
	BREQ			main_loop						; if equal, go back to main_loop
					
	CPI				R24,			NO_KEY			; compare R24 to NO_KEY
	BRNE			next							; jump to next if not equal

	LDI				R25,			NO_KEY			; load current value with NO_KEY
	RJMP			main_loop						; jump to main loop

next:

	MOV				R25,			R24				; set current value variable to R24

	CPI				R24,			'2'				; compare RVAL to '2'
	BREQ			Prss_2							; if eq, goto Prss_2

	RCALL			delay_1_micros					; delay

	CPI				R24,			'3'				; compare RVAL to '3'
	BREQ			Prss_3							; if eq, goto Prss_3

	RCALL			delay_1_micros					; delay

	CPI				R24,			'8'				; compare RVAL to '8'
	BREQ			Prss_8							; if eq, goto Prss_8

	RCALL			delay_1_micros					; delay

	CPI				R24,			'9'				; compare RVAL to '9'
	BREQ			Prss_9							; if eq, goto Prss_9

	RCALL			delay_1_micros					; delay

	RJMP main_loop									; jump to main_loop


	Prss_2:
		RCALL	lcd_clear_display					; clear display	
		PRINTSTRING Str_6							; print "ROLLING..."
		RCALL	roll_dice							; call  roll dice
		MOV		R24,			R16					; R24 = return value of roll dice
		PUSH	R24									; save R24 for later (used in store_stat)
		RCALL	store_stat							; store the stat
		POP		R24									; pick up r24
		RCALL	convert_integer						; convert the integer value to corresponding char
		RCALL lcd_write_char						; write char

		RCALL delay_1_s								; delay


		RJMP	menu								; jump to lbl menu

	Prss_3:
		RCALL	lcd_clear_display					; clear display
		RCALL	showstat							; call showstat
		RCALL delay_1_s								; delay
		RJMP	menu								; jump back to menu

	Prss_8:
		RCALL	lcd_clear_display					; clear display
		PRINTSTRING Str_8							; print "CLEARING..."

		RCALL	clear_stat							; clear stats

		RCALL	delay_1_s							; delay

		RJMP	menu								; jump back to menu

	Prss_9:
		RCALL	monitor								; call monitor
		RJMP	menu								; jump back to menu
	
	
	
	
	RJMP main_loop									; loop back to main_loop label	


