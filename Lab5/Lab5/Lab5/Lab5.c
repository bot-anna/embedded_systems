/*
 * Lab5.c
 *
 * Original Author :  Anna Håkansson, am1163, 2023-12-05
 */ 



#include <avr/io.h>
#include <stdio.h>
#include "lcd/lcd.h"
#include "numkey/numkey.h"
#include "delay/delay.h"
#include "hmi/hmi.h"
#include "regulator/regulator.h"
#include "common.h"
//#include "state/state.h"


char regulator_str[17];
char str[17];	
char key;
uint8_t regulator = 0;

enum state {
	MOTOR_OFF,
	MOTOR_ON_FORWARD,
	MOTOR_RUNNING_FORWARD,
	MOTOR_ON_BACKWARD,
	MOTOR_RUNNING_BACKWARD
};

typedef enum state state_t;
state_t current_state = MOTOR_OFF;
state_t next_state = MOTOR_OFF;

int main(void)

{
	// initialize HMI (LCD and numeric keyboard)
	hmi_init();
	lcd_clear();
	
	//initialize regulator
	regulator_init();

	key = NO_KEY;
	
	while (1) {		
		
		key = numkey_read();
		
		
			delay_ms(20);
		
		if(current_state == MOTOR_OFF)
		{
			regulator = regulator_read();
			sprintf(str, "MOTOR OFF!");
		
			output_msg(str, "", 0);
		
			if((key == '1') && (regulator == 0))
			{
				next_state = MOTOR_ON_BACKWARD;
			}
			else if((key == '3') && (regulator == 0))
			{
				next_state = MOTOR_ON_FORWARD;
			}
		
		}
		else if(current_state == MOTOR_ON_FORWARD)
		{
			sprintf(str, "FORWARD!");
			output_msg(str, "", 0);
			regulator = regulator_read();
			if(key == '0')
			{
				next_state = MOTOR_OFF;
			}
			else if(regulator > 0)
			{
				next_state = MOTOR_RUNNING_FORWARD;
			}
			
		}
		else if(current_state == MOTOR_RUNNING_FORWARD)
		{
			regulator = regulator_read();
			sprintf(str, "RUNNING FW");
			sprintf(regulator_str, "MOTOR SPEED:  %u%%", regulator);
			output_msg(str, regulator_str, 0);
			key = numkey_read();
			if(key == '0')
			{
				next_state = MOTOR_OFF;
			}
		}
		else if(current_state == MOTOR_ON_BACKWARD)
		{
			regulator = regulator_read();
			sprintf(str, "BACKWARD!");
		
			output_msg(str, "", 0);
			if(key == '0')
			{
				next_state = MOTOR_OFF;
			}
			else if(regulator > 0)
			{
				next_state = MOTOR_RUNNING_BACKWARD;
			}
		}
		else if(current_state == MOTOR_RUNNING_BACKWARD)
		{
			regulator = regulator_read();
			sprintf(str, "RUNNING BW");
			sprintf(regulator_str, "MOTOR SPEED:  %u%%", regulator);
			output_msg(str, regulator_str, 0);
			key = numkey_read();
			if(key == '0')
			{
				next_state = MOTOR_OFF;
			}
		}
		current_state = next_state;
		delay_ms(50);
		
		
	} 
	while(1);
}
