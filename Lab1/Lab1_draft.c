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
Description:			Interface a character LCD & Write 2 ln msg
---------------------------------------------------------------------------------*/

// THIS IS A DRAFT - NOT FOR MARKING - THIS IS A DRAFT

#include <ez8.h>
//#include <stdio.h> //we can use this for time or delay();

//prototypes
void init_ports(void);
void delay(unsigned int);
void init_lcd(void);
void soft_reset(void);
void cmd_write(unsigned char);
void data_write(unsigned char);
void lcd_ready(void);
unsigned char rd_busy(void);

/*------------------------------------------------------*/			//lab1.c
void main(void)
{
	//ln00 msg
	//ln40 msg
	
	init_ports();
	init_lcd();
	
	//cmd_write ln00
	//cmd_write ln40
}	

/*-------------------------------------------------------*/ 		//ports_gpio.h

void init_ports(void)
{
	//set pins to gpio mode
	PEADDR=0X02;
		PECTL=0X00;
	
	//output config 
	PEADDR=0X01;
		PECTL=0X00; 
	
	//set pins to push-pull
	PEADDR=0X03;
		PECTL=0X00;
	 
	PEADDR=0X00;
	return;	
}

/*-------------------------------------------------------*/			//my_delay.h

void delay(unsigned int count)
{

	unsigned int delay;
	
	while(count>0)
	{
		count--; 
		delay=0xA7; 
		while(delay>0) 
		{
			delay--; 
		}
	}
	return;	
}

/*-------------------------------------------------------*/ 		//lcd.h
void init_lcd(void)
{
	//call soft reset function
	soft_reset();
	
	lcd_ready();
	cmd_write(0x28);
	
	lcd_ready();
	cmd_write(0x0F);
	
	lcd_ready();
	cmd_write(0x06);
	
	lcd_ready();
	cmd_write(0x01);
}

/*-------------------------------------------------------*/			//lcd.h

void soft_reset(void)
{
	delay(16);
		PEOUT=0x30;
		PEOUT=0x38;
		PEOUT=0x30;
	
	delay(5);
		PEOUT=0x30;
		PEOUT=0x38;
		PEOUT=0x30;	

	delay(5);
		PEOUT=0x30;
		PEOUT=0x38;
		PEOUT=0x30;

	PEOUT=0x20;
	PEOUT=0x28;
	PEOUT=0x20;
}

/*-------------------------------------------------------*/			//lcd.h

void cmd_write(unsigned char controlval)
{
	
	char highnib, lownib;
	
	highnib=controlval&0xF0;
	lownib=controlval<<4;
	
	//call lcd_read() function
	lcd_ready();
	
	PEOUT=0x00;
	
	//high nibble
	PEOUT=highnib;
	PEOUT=highnib^0x08;
	PEOUT=highnib;
	
	//low nibble
	PEOUT=lownib;
	PEOUT=lownib^0x08;
	PEOUT=lownib;
}
/*-------------------------------------------------------*/			//lcd.h

void data_write(unsigned char dataval)
{
	char highnibdata, lownibdata;
	
	highnibdata=(dataval&0xF0)|0x04;
	lownibdata=(dataval<<4)|0x04;
	
	lcd_ready();
	
	PEOUT=0x04;
	
	//high nibble data
	PEOUT=highnibdata;
	PEOUT=highnibdata^0x08;
	PEOUT=highnibdata;
	
	//low nibble data
	PEOUT=lownibdata;
	PEOUT=lownibdata^0x08;
	PEOUT=lownibdata;	
}
/*-------------------------------------------------------*/			//lcd.h

void lcd_ready(void)
{
	//set port E to be inputs
	PEADDR=0X01;
	PECTL=0XF0;
	
	PEADDR=0X00;
	
	while(rd_busy()==1)
		{
			//rd_busy(); //--- don't need anymore
			;
		}
		
		init_ports();
		
	//PEADDR=0X01;		//--- don't need anymore
	//PECTL=0X00; 		//--- don't need anymore
	//PEADDR=0X00;		//--- don't need anymore
}
/*-------------------------------------------------------*/			//lcd.h

unsigned char rd_busy(void)
{
	unsigned char busyflag=0x00;
	unsigned char busy;
	
	PEOUT=0x02; 
	PEOUT=0x0A; 
	
	busyflag=PEIN;
	busyflag=busyflag&0x80;
	
	PEOUT=0x02;
	PEOUT=0x0A; 
	PEOUT=0x02;
	
	//check busy flag
	if (busyflag==0x0)
		busy=0;
	else 
		busy=1;
	
	return(busy);
}
/*-------------------------------------------------------*/			//lcd.h