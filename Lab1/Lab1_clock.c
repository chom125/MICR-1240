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
Description:		Displays preset date - uses counters	
---------------------------------------------------------------------------------*/

#include <ez8.h>				//used for Z8 uC port
#include <stdio.h>			//used for sprintf 
#include "glenn_lcd.h"		//used to isolate LCD functions from main function



/*****************************************************************************\
 * Function:		main
 * Input:			void
 * Description:	displays the date and time (pre-defined)
 * Dependencies:	init_ports, init_lcd, delay, cmd_write, sprintf, lcd_ready
						data_write,
\*****************************************************************************/
void main(void)
{
	//initiate and assign values of a date and time
	int sec=50;
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

	//iterate through every variables as per common-sense
	while(1)
	/*--
	The sequential-decition-iterative structure below is
	a counter based on hardware clock NOT realtime-clock.
	It does not account for leap-year or 28 day month cal.
	The flow is relatively simple (read the comments)
	--*/
	{
		//increment sec then....
		delay(4000);
		sec++;
		
		//increment min only when sec==60 then...
		if (sec==60)
		{
			sec=0; //reset sec to 0 then...
			min++;
		}
		
		//increment hour only when min==60 then...
		if (min==60)		
		{
			min=0; //reset min to 0
			hour++;
		}
		
		//increment day only when hour=24 then...
		if (hour==24)		
		{
			hour=0; //reset hour to 0
			day++;
		}
		
		//increment month only when day==31 then...
		if (day==31)		
		{
			day=1; //set day back to 1
			month++;
		}
	
		//increment year only when month==13 then...
		if (month==13)	
		{
			month=1; //set month back to 1
			year++;
		}
		 
		cmd_write(0x84); //1111 
		sprintf(time,"%02d:%02d:%02d",hour, min, sec);	//put var into %02d
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



