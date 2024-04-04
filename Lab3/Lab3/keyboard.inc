/*
 * keyboard.inc
 *
 * This file contains a driver for reading the keypad on the
 * "iBridge keypad shield". The driver is adapted for a shield mounted
 * on a Arduino Mega board (ATmega2560).
 *
 * Key mapping:
 *					-----------------   ------------------
 *					| 1 | 2 | 3 | A |   | 0 | 4 | 8  | 12 |
 *					-----------------   ------------------
 *					| 4 | 5 | 6 | B |   | 1 | 5 | 9  | 13 |
 *					-----------------   ------------------
 *					| 7 | 8 | 9 | C |   | 2 | 6 | 10 | 14 |
 *					-----------------   ------------------
 *					| * | 0 | # | D |   | 3 | 7 | 11 | 15 |
 *					-----------------   ------------------
 *
 * Created by N.N, xx Nov 2015, for the course DA346A at Malmo University.
 */

;==============================================================================
; Table of characters, for mapping with keys
;==============================================================================
keys:
	.DB				"147*2580369#ABCD", NO_KEY, NO_KEY

;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU			NO_KEY		=	0x10				; no key was pressed
	.DEF			RVAL		=	R24					; alias R24 rval



;==============================================================================
; Read keyboard
; Returns the corresponding character for the pressed key. If no key is
; pressed, then '\0' (null) is returned.
;
; Uses registers:
;	R24	(RVAL)		Counter and contains key when returning
;
; Important! A minimum delay of 2 NOP's is necessary after setting port,
; before pin can be read.
;==============================================================================
read_keyboard:

	RCALL		read_keyboard_num
	LDI			ZH,			high(keys <<1)		;Initialize Z pointer
	LDI			ZL,			low(keys <<1)
	ADD			ZL,			RVAL				;Add index
	LDI			RVAL,		0x00
	ADC			ZH,			RVAL				;Add 0 to catch Carry, if present
	LPM			RVAL,		Z
	RET

;==============================================================================
; Converts an integer value to the corresponding key on the display
; Uses registers:
;	R24 (RVAL)			as return value
;	R16 (TEMP)			as integer input value
;==============================================================================
convert_integer:
	LDI				TEMP,			9				; set temp to 9
	SUB				TEMP,			RVAL			; subtract RVAL from 9
	RCALL			delay_1_micros                  ; delay
	RCALL			delay_1_micros					; delay
	BRCS			IF								; if RVAL>9, jump to if
	RJMP			ELSE							; else, skip "if"
IF:
	LDI				TEMP,			7				; load temp with 7
	ADD				RVAL,			TEMP			; add 7 to temp
ELSE:												
	LDI				TEMP,			0x30			; load temp with 0x30 (ascii number offset)
	ADD				RVAL,			TEMP			; add to get correct ascii value

RET



;==============================================================================
; Read keyboard
; Returns the corresponding number (0-15) of the key pressed.
; Includes code to handle key bounce.
; If no key is pressed, 16 is returned.
;==============================================================================

read_keyboard_num:

// write code here to implement function in Lab 1

LDI		RVAL,		0x00					; Initiate counter to 0
SBI		PORTG,5								; Set Col0=1
CBI		PORTE,3								; Set Col1=0
LDS		R17,		PORTH					; register R16-R31 kan användas
ANDI	R17,		0b11100111				; bit 3&4 nollställs
STS		PORTH,		R17						; col2 and col3 = 0


LDI		R19,		20
RCALL	delay_ms

SBIC	PINF,5								; Test Row 0, skip if=0
RJMP	END									; Row 0=1, jump to end
INC		RVAL									; Increment counter
SBIC	PINF,4								; Test Row 1, skip if=0
RJMP	END									; Row 1=1, jump to end
INC		RVAL									; Increment counter
SBIC	PINE,4								; Test Row 2, skip if=0
RJMP	END									; Row 2=1, jump to end
INC		RVAL									; Increment counter
SBIC	PINE,5								; Test Row 3, skip if=0
RJMP	END									; Row 3=1, jump to end
INC		RVAL								; Increment	counter


CBI		PORTG,5								; col0=0
SBI		PORTE,3								; col1=1
LDS		R17,		PORTH					; register R16-R31 kan användas
ANDI	R17,		0b11100111				; bit 3&4 nollställs
STS		PORTH,		R17						; col2&3=0

LDI		R19,		20
RCALL	delay_ms

SBIC	PINF,5								; Test Row 0, skip if=0
RJMP	END									; Row 0=1, jump to end
INC		RVAL								; Increment counter
SBIC	PINF,4								; Test Row 1, skip if=0
RJMP	END									; Row 1=1, jump to end
INC		RVAL								; Increment counter
SBIC	PINE,4								; Test Row 2, skip if=0
RJMP	END									; Row 2=1, jump to end
INC		RVAL								; Increment counter
SBIC	PINE,5								; Test Row 3, skip if=0
RJMP	END									; Row 3=1, jump to end
INC		RVAL									; Increment counter


CBI		PORTG,5								; col0=0
CBI		PORTE,3								; col1=0
LDS		R17,		PORTH					; register R16-R31 kan användas
ORI		R17,		0b00001000				; bit 3 ettställs
STS		PORTH,		R17						; col2=1
LDS		R17,		PORTH					; register R16-R31 kan användas
ANDI	R17,		0b11101111				; bit 4 nollställs
STS		PORTH,		R17						; col3=0

LDI		R19,		20
RCALL	delay_ms

SBIC	PINF,5								; Test Row 0, skip if=0
RJMP	END									; Row 0=1, jump to end
INC		RVAL									; Increment counter
SBIC	PINF,4								; Test Row 1, skip if=0
RJMP	END									; Row 1=1, jump to end
INC		RVAL									; Increment counter
SBIC	PINE,4								; Test Row 2, skip if=0
RJMP	END									; Row 2=1, jump to end
INC		RVAL									; Increment counter
SBIC	PINE,5								; Test Row 3, skip if=0
RJMP	END									; Row 3=1, jump to end
INC		RVAL								; Increment counter

CBI		PORTG,5								; col0=0
CBI		PORTE,3								; col1=0
LDS		R17,		PORTH					; register R16-R31 kan användas
ANDI	R17,		0b11110111				; bit 3 nollställs
STS		PORTH,		R17						; col2=0;
LDS		R17,		PORTH
ORI		R17,		0b00010000				; bit 4 ettställs
STS		PORTH,		R17						; col3=1

LDI		R19,		20
RCALL	delay_ms

SBIC	PINF,5								; Test Row 0, skip if=0
RJMP	END									; Row 0=1, jump to end
INC		RVAL								; Increment counter
SBIC	PINF,4								; Test Row 1, skip if=0
RJMP	END									; Row 1=1, jump to end
INC		RVAL								; Increment counter
SBIC	PINE,4								; Test Row 2, skip if=0
RJMP	END									; Row 2=1, jump to end
INC		RVAL								; Increment counter
SBIC	PINE,5								; Test Row 3, skip if=0
RJMP	END									; Row 3=1, jump to end
INC		RVAL								; Increment counter

		

											; If you reach this point, counter should equal NO_KEY
END: 										; You may come here through a jump statement above

RET											; return with correct value in counter
