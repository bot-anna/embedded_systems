/*
 * monitor.inc
 *
 *  Created: 2016-11-19 12:42:26
 *   Author: Magnus Krampell
 *	 Edited by Anna Håkansson, am1163, 2023-12-05
 */ 
 monitorstr: .DB "MONITOR",0
// moni_start_str: .DB ">0000: ",0

 init_monitor:
	RET

; read keys from keyboard. If * - exit, if # advance address. if number - use as XH.
moni_read_keyboard:
	RCALL read_keyboard
	RCALL	delay_1_micros
	RCALL	delay_1_micros
	RCALL	delay_1_micros
	CPI		RVAL, '*'
	BREQ monireadquit		; yes, go back
	CPI		RVAL, '#'
	BREQ monireadquit		; yes, go back
	CPI		RVAL, NO_KEY 
	BRNE	moni_akey		; a key, which one?
	MOV		R3, RVAL		; save in case another key was pressed before this
	RJMP	moni_read_keyboard
moni_akey:					; some key is pressed
//rcall printhex
	CP		RVAL, R3
	BREQ	moni_read_keyboard			; repeat key, skip...
	MOV		R3, RVAL		; no repeat, save new key to detect repeat next time
			; ok, accept only one key, for now....
monireadquit:
	RET

moni_print_address:
	LDI RVAL, '>'
	SBI                PORTB,            4
	RCALL lcd_write_char
	MOV RVAL, XH
	RCALL printhex
	MOV RVAL, XL
	RCALL printhex
	LDI RVAL, ':'
	SBI                PORTB,            4 
	RCALL lcd_write_char
	RET

; monitor displays memory content on the format:
;	0x0100: 0xAB	The cell '0100' (hex) contains 'AB' (hex)
; the button # means "display the next cell"
; the button * means "go back to the main menu"

monitor:
	RCALL lcd_clear_display
	PRINTSTRING monitorstr
	RCALL delay_1_s
	
	LDI	XH,	0x00	; set start value
	LDI	XL,	0x00
moni_next:
	RCALL lcd_clear_display
	RCALL moni_print_address
	LD	RVAL, X+			; get memory value where X points...
	PUSH RVAL
	RCALL printhex			; ; print that value on LCD, first as HEX...
	LDI RVAL, '('
	RCALL lcd_write_char
	POP RVAL
	RCALL printdecimal		; ...then as DECIMAL
	LDI RVAL, ')'
	RCALL lcd_write_char
		
	RCALL moni_read_keyboard ; returns with "a real key", not NO_KEY!
	CPI		RVAL, '*'
	BREQ moniquit			; yes, quit
	CPI		RVAL, '#'
	BREQ moni_more
	; some number was returned. Use it in the X register
	SUBI RVAL, 0x30			; convert from ASCII to number
	MOV	XH, RVAL			; use number as high start address
	CLR	XL					; set XL to 00
	RJMP moni_next

moni_more:			; continue and display next address
					; X is already pointing at the next address...
	LDI		R19, 200
	RCALL	delay_ms
	RJMP moni_next
	

moniquit:
	RET