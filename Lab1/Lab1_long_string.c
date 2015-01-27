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
Program:				Lab 1
Programmer:			Glenn Lopez
Description:		Tests long string  
---------------------------------------------------------------------------------*/ 

//libraries
#include <ez8.h>
#include "glenn_lcd.h"

//change the LCD output message
#define LONG_MSG "<long msg goes here>"			//<--- EDIT THIS LINE



/*****************************************************************************\
 * Function:		main
 * Input:			void
 * Description:	this is the main function
\*****************************************************************************/
void main(void)
{
	char line=1;
	int n=0x00;
	char message[]="LONG_MSG";
	char *p_message;
	
	init_ports();
	init_lcd();

	
	cmd_write(0x80);
	lcd_ready();
	p_message=message;
		while(0 != *p_message){
			data_write(*p_message++);
			n++;
			if(n==0x10){
			if(line==1)
				line=2;
			else
				line=1;
		
			if (line==1)
				cmd_write(0x80);
			else
				cmd_write(0xC0);
			n=0x00;
			delay(8000);}
		
		}
}	



