#ifndef GLENN_LCD_H_
#define GLENN_LCD_H_

//defines for port subregisters
#define DATA_DIR 0x01					// Data Direction
#define ALT_FUN 0x02						// Alternate function
#define OUT_CTL 0x03						// Output Control
#define HDR_EN 0x04						// High Drive Enable
#define SMRS_EN 0x05						// Stop Mode Recovery

/*-- D I S A B L E D --*
//not yet implemented

//defines for pulse(); function
#define E 0x00								// Enable 
#define RW 0x00							// Read or Write
#define RS 0x00							// Register Select
/*--*/

//prototypes
void init_ports(void);					// Initializes ports
void delay(unsigned int);				// Defined delays (ms)
void init_lcd(void);						// initializes LCD 
void soft_reset(void);					// 
void cmd_write(unsigned char);		// 
void data_write(unsigned char);		// 
void lcd_ready(void);					// 
//void pulse (unsigned int comm);	// DISABLED - not yet implemented
unsigned char rd_busy(void);			// 
//void debug(void);						// DEBUG SIGNAL



/*****************************************************************************\
 * Function:		init_ports
 * Input:			void
 * Description:	this initiates port E
 * Dependencies:	none
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



/*****************************************************************************\
 * Function:		delay
 * Input:			count
 * Description:	this delay is in ms
 * Dependencies:	none
\*****************************************************************************/
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



/*****************************************************************************\
 * Function:		rd_busy
 * Input:			void
 * Description:	
 * Dependencies:	none
\*****************************************************************************/
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



/*****************************************************************************\
 * Function:		init_lcd
 * Input:			void
 * Description:	initializes lcd
 * Dependencies:	soft_reset | lcd_ready | cmd_write
\*****************************************************************************/
void init_lcd(void)
{
	//
	soft_reset();
	
	//
	lcd_ready();
	cmd_write(0x28);
	
	//
	lcd_ready();
	cmd_write(0x0F);

	//
	lcd_ready();
	cmd_write(0x06);
	
	//
	lcd_ready();
	cmd_write(0x01);
}



/*****************************************************************************\
 * Function:		soft_reset
 * Input:			void
 * Description:	
 * Dependencies:	delay
\*****************************************************************************/
void soft_reset(void)
{
	//
	delay(16);
	PEOUT=0x30;
	PEOUT=0x38;
	PEOUT=0x30;
	
	//
	delay(5);
	PEOUT=0x30;
	PEOUT=0x38;
	PEOUT=0x30;	
	
	//
	delay(1);
	PEOUT=0x30;
	PEOUT=0x38;
	PEOUT=0x30;

	//
	PEOUT=0x20;
	PEOUT=0x28;
	PEOUT=0x20;
}



/*****************************************************************************\
 * Function:		cmd_write
 * Input:			controlval
 * Description:	
 * Dependencies:	lcd_ready 
\*****************************************************************************/
void cmd_write(unsigned char controlval)
{
	
	char highnib, lownib;
	
	//
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



/*****************************************************************************\
 * Function:		data_write
 * Input:			dataval
 * Description:
 * Dependencies:	lcd_ready
\*****************************************************************************/
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



/*****************************************************************************\
 * Function:		lcd_ready
 * Input:			void
 * Description:	
 * Dependencies:	rd_busy | init_ports
\*****************************************************************************/
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



/*****************************************************************************\
 * Function:		pulse
 * Input:			comm
 * Description:
 * Dependencies:	none	
\*****************************************************************************/

/*--	D I S A B L E D --*
//not yet implemented

void pulse (unsigned int comm)
{
	PEOUT = comm;
}

/*--*/

#endif	//GLENN_LCD_H_