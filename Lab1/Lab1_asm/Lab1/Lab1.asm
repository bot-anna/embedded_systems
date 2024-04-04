/*
 * Lab1.asm
 *
 * Created: 2018-11-xx
 *
 * Created by N.N, for the course DA346A at Malmo University.
 */
 
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
;	Rnn				xxxx
;==============================================================================
main:
	CALL	read_keyboard_num					; call the keyboard driver
	OUT		PORTD,				R19				; send value of R19 (counter) to the LEDs
	NOP											; pause
	NOP											; pause
	RJMP main									; loop back to main label				

