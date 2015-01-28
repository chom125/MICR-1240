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

#include <ez8.h>				//used for Z8 uC port
//#include <stdio.h>			//used for sprintf - disabled to save code space
#include "glenn_lcd.h"		//used to isolate LCD functions from main

//LCD OUTPUT - EDIT THIS LINE
#define TICKER "Long message will cycle between line 1 and line 2"			//<--- EDIT THIS LINE



/*****************************************************************************\
 * Function:		main
 * Input:			void
 * Description:	this is the main function
\*****************************************************************************/
void main(void)
{
	char line=1;
	int n=0x00;
	char message[]=TICKER;
	char *msg;
	
	init_ports();
	init_lcd();

	
	cmd_write(0x80);
	lcd_ready();
	msg=message;
		while(0 != *msg){
			data_write(*msg++);
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



