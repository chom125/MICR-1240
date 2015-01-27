/*---------------------------------------------------------------------------------
888b     d8888888888 .d8888b. 8888888b.     d888   .d8888b.     d8888  .d8888b. 
8888b   d8888  888  d88P  Y88b888   Y88b   d8888  d88P  Y88b   d8P888 d88P  Y88b
88888b.d88888  888  888    888888    888     888         888  d8P 888 888    888
888Y88888P888  888  888       888   d88P     888       .d88P d8P  888 888    888
888 Y888P 888  888  888       8888888P"      888   .od888P" d88   888 888    888
888  Y8P  888  888  888    888888 T88b 888888888  d88P"     8888888888888    888
888   "   888  888  Y88b  d88P888  T88b      888  888"            888 Y88b  d88P
888       8888888888 "Y8888P" 888   T88b   8888888888888888       888  "Y8888P" 

Subject:					Embeded Systems 1
Instructor:				Ken Patzel
Program:					Lab 1
Programmer:				Glenn Lopez
Description:			Writes on line 1 (0x80) and line 2 (0xC0) on a 2x40 LCD
---------------------------------------------------------------------------------*/

// THIS IS A DRAFT - NOT FOR MARKING - THIS IS A DRAFT 

//libraries
#include <ez8.h>
#include "glenn_lcd.h"

//defines for port subregisters
#define DATA_DIR 0x01					// Data Direction
#define ALT_FUN 0x02						// Alternate function
#define OUT_CTL 0x03						// Output Control
#define HDR_EN 0x04						// High Drive Enable
#define SMRS_EN 0x05						// Stop Mode Recovery

//defines for pulse(); function
#define E 0x00								// Enable 
#define RW 0x00							// R/W
#define RS 0x00							// RS

//change the LCD output message
#define LN1 "<line one msg>"			//<--- EDIT THIS LINE
#define LN2 "<line two msg>"			//<--- EDIT THIS LINE

//prototypes
void init_ports(void);



/*****************************************************************************\
 * Function:		main
 * Input:			void
 * Description:	this is the main function
\*****************************************************************************/
void main(void)
{
	char ln1_msg[] = LN1; 	//line 1 (0x80) -- edit in #define
	char ln2_msg[] = LN2;	//line 2 (0xC0) -- edit in #define
	char *msg;

	init_ports();
	init_lcd();

	//line 1 data write
	cmd_write(0x80);
	msg=ln1_msg;
		while(0 != *msg)
			{
				data_write(*msg++);
			}

//line 2 data write
cmd_write(0xC0);
msg=ln2_msg;
	while(0 != *msg)
		{
			data_write(*msg++);
		}
}	



/*****************************************************************************\
 * Function:		init_ports
 * Input:			void
 * Description:	this initiates port E
\*****************************************************************************/
void init_ports(void)
{
	//disable alternate function
	PEADDR = ALT_FUN;
	PECTL = 0X00;
	
	//set pins as output
	PEADDR = DATA_DIR;
	PECTL = 0X00; 
	
	//set pins to push-pull type
	PEADDR = OUT_CTL;
	PECTL = 0X00;
	
	//prevent changes 
	PEADDR = 0X00;
	return;	
}
