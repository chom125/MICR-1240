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
Program:				Lab 3
Programmer:			Glenn Lopez
Description:		This lab introduces interupts to the keypad lab (lab2)	
---------------------------------------------------------------------------------*/

#include <ez8.h>					//used for Z8 uC defines and subroutine stuff
#include <stdio.h>				//used for sprintf (used in main function)
#include "lcd_key.h" 			//includes all the function prototypes
#include "lcd_key.inc" 			//contains lcd and keypad subroutines

//write your error message below
#define ERR_MSG "Too fast bro... Slow down"


/*****************************************************************************\
 * Function:		keypad isr
 * Input:			void
 * Description:	do this when an interupt for the defined ports are triggered
\*****************************************************************************/

//keypad interupt handler (isr)
void interrupt PORTD_isr(void)
{
	int index;
	char val[];
	char error[] = ERR_MSG;
	int key;
	
		while(1)
		{
			index=keyindex();
	
			if(index>11)
				{
					lcd_ready();
					cmd_write(0x01);
					lcd_ready();
					cmd_write(0x84);
					string_write(error);
				}
				
				else
				{
					key = keyval(index);
					//lcd_ready();
					cmd_write(0x01);
					//lcd_ready();x
					cmd_write(0x84);
					sprintf(val,"%d",key);
					//lcd_ready();
					string_write(val);
				}
				
			waitkeyreleased();  	
		}
}


/*****************************************************************************\
 * Function:		main
 * Input:			void
 * Description:	pre-processor routine -- noting in main loop
\*****************************************************************************/
void main(void)
{
	PDOUT=0x00;
	
	
	
	/*--
	note:	this sets up port d to enable interupt any
			time a key is pressed
	--*/
	DI();										
	
	setmodeInterrupt_PortD_F64			//
	(
		PORTPIN_FOUR|						//
		PORTPIN_FIVE|						//
		PORTPIN_SIX,						//
		EDGE_FALLING,						//
		INTPRIORITY_LOW					//
	);
	
	/*--
	note: PDx_IVECT is defined in ez8.h
			PORTD_usr is the name of interupt function
	--*/
	SET_VECTOR(PD4_IVECT, PORTD_isr);	
	SET_VECTOR(PD5_IVECT, PORTD_isr);	
	SET_VECTOR(PD6_IVECT, PORTD_isr);
	
	EI();										
	
	//standard port/lcd setup	
	init_ports();
	init_lcd();
	lcd_ready();
	cmd_write(0x0C);
	
	
	
	while(1)
	{
		/*--
		stay in this loop till 
		an interupt occurs
		--*/
	};
}

