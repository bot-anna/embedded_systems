/*
 * stat_data.inc
 *
 *  Created: 2023-11-28 16:19:20
 *   Author: Anna Håkansson, am1163
 */ 

 /* ---------------------------------------------------
Space in the RAM to save the results from dice throws.
The following functions are provided:
store_stat (R24 contains the dice value)
The function will increment the
Total number of throws and the
number of throws with results equals R24.
get_stat (R24 gives the value for which the
result is retrieved. If R24 = 0, the
total number of throws shall be returned.
The result is returned in R24.
clear_stat (no parameters in nor out)
Clears all data.
--------------------------------------------------------*/

	.DSEG							; The following applies to the RAM:
	.ORG		0x200				; Set starting point
									; address of data
									; segment to 0x200
	stats: .BYTE 7
	.CSEG

store_stat: 
	LDI			XH,			HIGH(stats)		; set XH to high part of stats
	LDI			XL,			LOW(stats)		; set XL to low part of stats

	LD			TEMP,		X				;load value of total rolls into temp
	INC			TEMP						;increment temp
	ST			X,			TEMP			; store in X
	
	ADD			XL,			RVAL			; add rval to X to get right index
	LDI			RVAL, 0						; load 0 into rval
	ADC			XH, RVAL					; add with carry to YH to catch carry
	LD			TEMP,		X				; load the value of X to temp
	INC			TEMP						; increment num of throws
	ST			X,			TEMP			; store incremented value in ram
	RET

get_stat: 
	LDI			XH,			HIGH(stats)		; load high part of stats into XH
	LDI			XL,			LOW(stats)		; load low part of stats into XL

	ADD			XL,			RVAL			; add rval into XL to get the right index
	LDI			RVAL,		0				; load rval with 0
	ADC			XH,			RVAL			; add with carry into XH to catch potential carry

	LD			RVAL,		X				; load X into RVAL
	RET										; return

clear_stat: 
	LDI			XH,			HIGH(stats)		; load high part of stats into XH
	LDI			XL,			LOW(stats)		; load low part of stats into XL

	LDI			TEMP,		0				; load 0 into temp	
	ST			X+,			TEMP			; load 0 into all locations
	ST			X+,			TEMP
	ST			X+,			TEMP
	ST			X+,			TEMP
	ST			X+,			TEMP
	ST			X+,			TEMP
	ST			X,			TEMP



	RET