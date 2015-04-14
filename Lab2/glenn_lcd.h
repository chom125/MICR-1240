#ifndef GLENN_LCD_H_
#define GLENN_LCD_H_

//defines for port subregisters
#define DATA_DIR 0x01						// Data Direction
#define ALT_FUN 0x02							// Alternate function
#define OUT_CTL 0x03							// Output Control
#define HDR_EN 0x04							// High Drive Enable
#define SMRS_EN 0x05							// Stop Mode Recovery

//defines for LCD welcome message
#define MSG1_INIT "Welcome to lab2"		// Line 1 Welcome message
#define MSG2_INIT "Press any key..."	// Line 2 Welcome message

//lcd prototypes
void init_port_e(void);						// Initializes ports
void delay(unsigned int);					// Defined delays (ms)
void init_lcd(void);							// Initializes LCD 
void soft_reset(void);						// Force a software reset
void cmd_write(unsigned char);			// Writes 8bit cmd in 4 bit nibs
void data_write(unsigned char);			// Writes 8bit data in 4 nibs
void lcd_ready(void);						// Return only  when LCD is able to accept
unsigned char rd_busy(void);				// Read LCD busy flag



/*****************************************************************************\
 * Function:		init_port_e
 * Input:			void
 * Description:	initiates port E for LCD
 * Dependencies:	none
\*****************************************************************************/
void init_port_e(void)
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
}



/*****************************************************************************\
 * Function:		delay
 * Input:			count
 * Description:	software delay in ms
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
 * Description:	test the busy flag
 * Dependencies:	none
\*****************************************************************************/
unsigned char rd_busy(void)
{
	//
	unsigned char busyflag=0x00;
	//
	unsigned char busy;
	
	PEOUT=0x02; 						//set RW to 1 (1 = read)
	PEOUT=0x0A; 						//pulse E high, while RW = 1
	
	busyflag=PEIN;						//
	busyflag=busyflag&0x80;			//
	
	PEOUT=0x02;							//set RW to 1 (1 = read)
	PEOUT=0x0A;							//pulse E high, while RW = 1
	PEOUT=0x02;							//set RW to 1 (1 = read)
	
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
	char ln1_msg[] = MSG1_INIT;			
	char ln2_msg[] = MSG2_INIT;
	char *msg;
	
	soft_reset();					//run soft reset
	cmd_write (0x28);
	cmd_write (0x0f);				//display off	
	cmd_write (0x06);				//display clear	
	cmd_write (0x01);				//entry mode set
	cmd_write(0x0C);
	msg=ln1_msg;
	while (23<*msg)				//write welcom message
	{			
	data_write(*msg++);		
	}
	cmd_write(0xC0);
	
	msg=ln2_msg;	
	while (23<*msg)
	{
	data_write(*msg++);
	}
	delay(15000);					//display delay
	cmd_write(0x00);				//reset display
	cmd_write (0x01);				//display clear
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
	delay(16);		//delay > 15ms as per DATASHEET
	PEOUT=0x30;
	PEOUT=0x38;
	PEOUT=0x30;
	
	//
	delay(5);		//delay > 14.1ms as per DATASHEET
	PEOUT=0x30;
	PEOUT=0x38;
	PEOUT=0x30;	
	
	//
	delay(1);		//delay > 100ns as per DATASHEET
	PEOUT=0x30;
	PEOUT=0x38;
	PEOUT=0x30;

	PEOUT=0x20;
	PEOUT=0x28;
	PEOUT=0x20;
}



/*****************************************************************************\
 * Function:		cmd_write
 * Input:			controlval
 * Description:	used to write a control byte to the display in 4bit mode
 * Dependencies:	lcd_ready 
\*****************************************************************************/
void cmd_write(unsigned char controlval)
{
	//initiate variables highnib & lownib as char
	char highnib, lownib;
	
	//assign hignib to accept data but reject changes to control lines
	highnib = controlval&0xF0;
	
	//shift controlval 4 places to the left and assign it to lownib
	lownib = controlval<<4;
	
	//perform lcd_ready function
	lcd_ready();
	
	//set all control bits (E,RS,RW) to LOW
	PEOUT=0x00;
	
	//high nibble
	PEOUT=highnib;					//pulse controlval & keep control bits low
	PEOUT=highnib^0x08;			//pulse above again but + Enable = High (arm the latch)
	PEOUT=highnib;					//pulse Enable=high to latch data to LCD internal register
	
	//low nibble
	PEOUT=lownib;					//same as above
	PEOUT=lownib^0x08;			//dito
	PEOUT=lownib;					//dito
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
												//7654  E RS RW PE0
	PEOUT=0x04;								//0000  0 1  0   0
	
	//high nibble data
	PEOUT=highnibdata;					//
	PEOUT=highnibdata^0x08;				//
	PEOUT=highnibdata;					//
	
	//low nibble data
	PEOUT=lownibdata;						//
	PEOUT=lownibdata^0x08;				//
	PEOUT=lownibdata;						//
}



/*****************************************************************************\
 * Function:		lcd_ready
 * Input:			void
 * Description:	
 * Dependencies:	rd_busy | init_port_e
\*****************************************************************************/
void lcd_ready(void)
{
	//set port E to be inputs
						
	PEADDR=0X01;		//0000 0001 
	PECTL=0XF0;			//1111 0000
	
	PEADDR=0X00;		
	
	while(rd_busy()==1)
	{
		;
	}
		
	init_port_e();
}



/*****************************************************************************\
 * Function: string_write		
 * Input:			
 * Description:	
 * Dependencies:	
\*****************************************************************************/
void string_write(char *message)
{	
	char line=1;
	int n=0x00;
	char *p_message;

	cmd_write(0x80);
	lcd_ready();
	p_message=message;
	
	while(0 != *p_message)
	{
		data_write(*p_message++);
		n++;
		
		if(n==0x10)
		{
			if(line==1)
				line=2;
			else
				line=1;
		
			if (line==1)
				cmd_write(0x80);
			else
				cmd_write(0xC0);
			n=0x00;
			delay(5000);					//delay 2nd line msg
			lcd_ready();
		}		
	}	
}

#endif	//GLENN_LCD_H_