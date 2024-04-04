/*
 * Tarning.inc
 *
 *  Created: 2023-11-28 15:32:12
 *   Author: Anna Håkansson, am1163
 */ 
 init_tarning:
	.EQU			ROLL_KEY = '2'					; make constant ROLL_KEY = 2
	RET

roll_dice:
	LDI				R16,			6				;load R16 (the dice) with 6
rolling:
	RCALL			delay_1_micros					; delay

	RCALL			read_keyboard					; read keyboard
	CPI				RVAL,			ROLL_KEY		; compare rval with roll_key
	BREQ			roll							; if equal, we are still rolling. jump to roll
	RET												; else return, button is no longer pressed


roll:	
	DEC				R16								;decrement dice
	BREQ			roll_dice						;if 0, goto roll_dice and start over from 6
	RJMP			rolling							;else, keep rolling and decrementing