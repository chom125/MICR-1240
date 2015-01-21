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
Description:			Interface a Character LCD to a 4 bit Z8 (Zilog)
---------------------------------------------------------------------------------*/

// includes	
#include <ez8.h> 							// Z8 pinconfigs header file
#include <stdio.h>						// Standard I/O header file

// defines 
#define E 0x08								// Enable 
#define RW 0x02							// R/W
#define RS 0x04							// RS


// prototypes
void init_ports(void);							// prototype init_ports
void delay(unsigned int time);				// prototype delay
void init_lcd(void);								// prototype init_lcd	
void soft_reset(void);							// prototype soft_reset
void command_write (unsigned int com);		// prototype command_write
void data_write (unsigned int val);			// prototype data_write
void lcd_ready (void);							// prototype lcd_ready
void pulse (unsigned int comm);				// prototype pulse
void read_busy (void);							// prototype read_busy
void write_string (void);						// write string function


// main function
void main (void)							
{	
	init_ports();							// run init_ports									
	init_lcd();								// run init_lcd
	write_string();						// write message
}

// init_port function
void init_ports(void)					// init_ports function
{
	// innitialize port E
	PEADDR=0x02;	// select alt. functions sub-register
	PECTL=0x00;		// all alt. functions are disabled
	PEADDR=0x01;	// select data direction sub-register
	PECTL=0x00;		// bits are o/p
	PEADDR=0x03;	// select output control sub-register
	PECTL=0x00;		// all outputs are push-pull
	PEADDR=0x00; 	// prevents inadvertant changes to sub-register  
}
 
// software delay function
void delay(unsigned int time)				
{
	long int delay;							// declare delay	
	for( ;time>0;time--)						// for loop that runs the passed value times
		for(delay=168;delay>0;delay--);	// nested while loop set to 1 milli seconds
}

void soft_reset (void)
{
 	PEOUT = 0x00;						// reset peout	
	delay(16);							// 16ms delay	
	PEOUT = 0x30;						// set 0x30	
	pulse(E);							// pulse E1
	pulse(E);							// pules E0	
	delay(5);							// 5 mili delay	
	pulse(E);							// pules E1
	pulse(E);							// pulse E0	
	delay(1);							// 1ms  delay	
	pulse(E);							// pulse E1
	pulse(E);							// pulse E0	
	PEOUT = 0x20;						// delta 4 bit	
	pulse(E);							// pulse E1
	pulse(E);							// pulse E0
}

void init_lcd (void)
{	char message[] = "Welcome back";		// innitialize msg
	char message2[] = "Glenn";
	char *p_message;	
	
	soft_reset();								// run soft reset
	command_write (0x28);
	command_write (0x0f);					// display off	
	command_write (0x06);					// display clear	
	command_write (0x01);					// entry mode set
	
	p_message=message;
	
	while (23<*p_message)					// write welcome message
	{			
		data_write(*p_message++);		
	}
	
	command_write(0xC0);
	p_message=message2;	
	
	while (23<*p_message)
	{
		data_write(*p_message++);
	}
		
	delay(1000);							// display message for 1 second
	command_write(0x00);					// reset display
	command_write (0x01);				// display clear
}

void pulse (unsigned int comm)
{
		PEOUT = PEOUT^comm;					// pulse command line	
}

void data_write (unsigned int val)
{
	char high, low;
	lcd_ready();						// check busy
	PEOUT = 0x00;						// clear peout
	pulse(RS);							// set to data read
	high = val & 0xf0;					// set high nibble
	high = high^0x04;
	PEOUT = high;						// send high nibble
	pulse(E);							// make E 1
	pulse(E);							//  make E 0
	low = (val << 4);					// set low nibble
	low = low^0x04;
	PEOUT = low;						// send low nibble
	pulse(E);							// make E 1
	pulse(E);							//  make E 0
	pulse(RS);							// reset to command read
}

void lcd_ready (void)
{
	PEOUT = 0x00;						// set PDOUT low
	pulse(RW);							// set to read
	read_busy();						// check flag
	pulse(RW);							// set to write
}

void command_write (unsigned int com)
{
	lcd_ready();						// check busy
	PEOUT = 0x00;						// clear peout
	PEOUT = com & 0xf0;				// set high nibble
	pulse(E);							// make E 1
	pulse(E);							// make E 0
	PEOUT = com << 4;					// set low nibble
	pulse(E);							// set E 1
	pulse(E);							// set E 0
}
void read_busy (void)
{
	char ready = 1;
	PEADDR=0x01;		// select data direction sub-register
	PECTL=0x80;			// MSB is input
	PEADDR=0x00; 		// prevents inadvertant changes to sub-register

	while (ready != 0)				// check flag
	{
		pulse(E);						// enable
		ready = PEIN & 0x80;			// read flag
		pulse(E);						// disable
		pulse(E);						// dummy pulse
		pulse(E);
	}	

	PEADDR=0x01;	// select data direction sub-register
	PECTL=0x00;		// all bits are output
	PEADDR=0x03;	// select output control sub-register
	PECTL=0x00;		// all outputs are push-pull
	PEADDR=0x00; 	// prevents inadvertant changes to sub-register	
}

void tstmsg (void)			// write message
{
	// array to write to screen
	char message[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit...";
	char *p_message;				// pointer for array to be written
	char new_line = 0;				// keep track of line
	
	p_message=message;				// set pointer to message
		while (23<*p_message)		// write until null
		{
			if (new_line==16)		// check position
			{
				command_write(0xC0);// move cursor
				delay(1500);		// wait 1.5 seconds
			}
			if (new_line==32)		// check position
			{
				command_write(0x80);// move cursor
				new_line = 0;		// reset line
				delay(1500);		// wait 1.5 seconds
				command_write(0x01);// clear screen
			}
			data_write(*p_message++);// write ascii at address
			new_line++;				// increase command
		}
		delay(1500);				// wait 1.5 seconds
		command_write(0x01);		// clear screen
}