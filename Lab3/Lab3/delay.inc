/*
 * delay.inc
 *
 * This file contains delay routines.
 *
 * Created by Anna Håkansson, 2023-12-04, for the course DA346A at Malmo University.
 */

;==============================================================================
; Delay of 1 µs (including RCALL)
;==============================================================================
delay_1_micros:
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

	RET

;==============================================================================
; Delay of X µs
;	LDI + RCALL = 4 cycles
; Uses registers:
;	R19				Input parameter data (X µs)
;==============================================================================
delay_micros:
	
	RCALL delay_1_micros
	DEC R19
	BRNE delay_micros
	RET

;==============================================================================
; Delay of X ms
;	LDI + RCALL = 4 cycles
; Uses registers:
;	R18				Copy of parameter data (X ms)
;	R19				Input parameter data (X ms) and
;					also input to 'delay_micros'.
;==============================================================================
delay_ms:


	MOV			R18,		R19			; Copy the value in R19 to R18

L1:
	LDI			R19,		0xFA		; R19=250	

	RCALL		delay_micros			; 250 micros delayed

	LDI			R19,		0xFA		; R19=250	

	RCALL		delay_micros			; 500 micros delayed

	LDI			R19,		0xFA		; R19=250	

	RCALL		delay_micros			; 750 micros delayed

	LDI			R19,		0xFA		; R19=250	

	RCALL		delay_micros			; 1000 micros delayed = 1ms

	DEC		R18

	BRNE	L1

	RET

;==============================================================================
; Delay of 1 s
;	LDI + RCALL = 4 cycles
; Uses registers:
;	R19				Used for storage and
;					also input to 'delay_micros'.
;==============================================================================
delay_1_s:

	LDI			R19,		0xFA		; R19=250	

	RCALL		delay_ms				; 250 ms delayed

	LDI			R19,		0xFA		; R19=250	

	RCALL		delay_ms				; 500 ms delayed

	LDI			R19,		0xFA		; R19=250	

	RCALL		delay_ms				; 750 ms delayed

	LDI			R19,		0xFA		; R19=250	

	RCALL		delay_ms				; 1000 ms delayed = 1s

	RET

;==============================================================================
; Delay of X s
;	LDI + RCALL = 4 cycles
; Uses registers:
;	R18				Input parameter data (X ms)
;==============================================================================

delay_s:
	RCALL		delay_1_s				; delay_1_s
	DEC			R18						; decrement counter
	BRNE		delay_s					; start over if not 0
	RET
	