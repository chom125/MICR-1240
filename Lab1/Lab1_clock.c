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
Instructor:				Ken Patzel
Program:				Lab 1
Programmer:				Glenn Lopez
Description:			
---------------------------------------------------------------------------------*/

// THIS IS A DRAFT - NOT FOR MARKING - THIS IS A DRAFT 

//libraries
#include <ez8.h>
#include <stdio.h>
#include "glenn_lcd.h"



/*****************************************************************************\
 * Function:		main
 * Input:			void
 * Description:	this is the main function
\*****************************************************************************/
void main(void)
{
	int sec=55;
	int min=59;
	int hour=23;
	int day=23;
	int month=03;
	int year=1987;
	char time[9];
	char date[11];
	char *msg;
	
	init_ports();
	init_lcd();

	while(1)
	{
		delay(4000);
		sec++;
	
		
		if (sec==60)
		{
			sec=0;
			min++;
		}
	
		if (min==60)
		{
			min=0;
			hour++;
		}
		
		if (hour==24)
		{
			hour=0;
			day++;
		}
		
		if (day==31)
		{
			day=1;
			month++;
		}
	
		if (month==13)
		{
			month=1;
			year++;
		}
		
		cmd_write(0x84);
		sprintf(time,"%02d:%02d:%02d",hour, min, sec);
		lcd_ready();
		msg=time;
	
		while(0 != *msg)
		{
			data_write(*msg++);
		}
		
		cmd_write(0xC3);
		sprintf(date,"%02d/%02d/%4d",day, month, year);
		lcd_ready();
		msg=date;
		
		while(0 != *msg)
		{
			data_write(*msg++);
		}
		
		
	}		
}	



