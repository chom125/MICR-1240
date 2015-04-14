/*---------------------------------------------------------------------------------
888b     d8888888888 .d8888b. 8888888b.     d888   .d8888b.     d8888  .d8888b. 
8888b   d8888  888  d88P  Y88b888   Y88b   d8888  d88P  Y88b   d8P888 d88P  Y88b
88888b.d88888  888  888    888888    888     888         888  d8P 888 888    888
888Y88888P888  888  888       888   d88P     888       .d88P d8P  888 888    888
888 Y888P 888  888  888       8888888P"      888   .od888P" d88   888 888    888
888  Y8P  888  888  888    888888 T88b 888888888  d88P"     8888888888888    888
888   "   888  888  Y88b  d88P888  T88b      888  888"            888 Y88b  d88P
888       8888888888 "Y8888P" 888   T88b   8888888888888888       888  "Y8888P" 

Subject:				Embeded Systems 1
Instructor:			Ken Patzel
Program:				Lab 2
Programmer:			Glenn Lopez
Description:		This lab introduces inputs to the LCD lab (lab1)	
---------------------------------------------------------------------------------*/

#include <ez8.h>					//used for Z8 uC defines and subroutine stuff
#include <stdio.h>				//used for sprintf (used in main function)
#include "glenn_lcd.h"			//used for displaying inputs from the keypad
#include "glenn_keypad.h"		//used for user inputs

//defines for error msg
#define ERR_MSG "Too fast bro... Slow down"



/*****************************************************************************\
 * Function:		main
 * Input:			void
 * Description:	utilize a matrix keypad for inputs
\*****************************************************************************/
void main(void)
{
	int index;									//holds the 
	char val[];									
	char error[] = ERR_MSG;					//holds the error message
	int key;

	//initializations
	init_port_d();								//initialize keypad port		
	init_port_e();								//initialize lcd port
	init_lcd();									//initialize the lcd
	lcd_ready();								//check to see if lcd is ready first

	cmd_write(0x0C); 
	
	/*--
	This is the loop that doesn't end
   Yes, it goes on and on my friend
   Some people started coding it not knowing what it was,
   And Zilog will continue running it forever just because . . . 
	--*/
	
	while(1)
	{ 
		index = keyindex();					//get the value for the lookup table

		//check for any error
		if(index > 11)							
		{
			lcd_ready();						//check if lcd is ready for commands
			cmd_write(0x01);					//clear the screen
			
			lcd_ready();						//check if lcd is ready for commands
			cmd_write(0x84);					//
			
			string_write(error);
		}
			
		else
		{
			key = keyval(index);
			
			lcd_ready();
			cmd_write(0x01);
			
			lcd_ready();
			cmd_write(0x84);
			sprintf(val,"%d",key); 		
			
			lcd_ready();          
			string_write(val);
		}	
			
	/*--
	This is the loop that doesn't end
   Yes, it goes on and on my friend
   Some people started coding it not knowing what it was,
   And Zilog will continue running it forever just because . . . 
	--*/
		
	}
}