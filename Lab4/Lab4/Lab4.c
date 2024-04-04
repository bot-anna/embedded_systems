/*
 * Lab4.c
 *
 * Original Author : Magnus Krampell
 * Edited by: Anna Håkansson, am1163, 2023-12-05
 */ 



#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>			//rand
#include "lcd/lcd.h"
#include "numkey/numkey.h"
#include "delay/delay.h"
#include "hmi/hmi.h"
#include "guess_nr.h"

// for storage of pressed key
char key;
// for generation of variable string
char str[17];


int main(void)

{
	
	
	uint16_t rnd_nr;
	// initialize HMI (LCD and numeric keyboard)
	hmi_init();
	// generate seed for the pseudo-random number generator
		srand(1);															//random_seed();
	// show start screen for the game
	output_msg("WELCOME!", "LET'S PLAY...", 3);
	//play game 
	while (1) {
		// generate a random number
		rnd_nr = rand()  % 101;														//random_get_nr(100) + 1;
		// play a round...
		play_guess_nr(rnd_nr);
	} 
	

	
		
	
	
/******************************************************************************
	OVANF÷R FINNS HUVUDPROGRAMMET, DET SKA NI INTE MODIFIERA!
	NEDANF÷R KAN NI SKRIVA ERA TESTER. GL÷M INTE ATT PROGRAMMET M?STE HA EN
	OƒNDLIG LOOP I SLUTET!

	NƒR DET ƒR DAGS ATT TESTA HUVUDPROGRAMMET KOMMENTERAR NI UT (ELLER RADERAR)
	ER TESTKOD. GL÷M INTE ATT AVKOMMENTERA HUVUDPROGRAMMET
******************************************************************************/

	
	
	/*
	lcd_init();
	lcd_clear();
	
	
	while(1){
		lcd_write_str("HEYOO");
		delay_s(1);
		lcd_write_str(" YO YO YO");
		delay_s(1);
	
		lcd_clear();
		} */
	while (1);
}
