#ifndef GLENN_KEYPAD_H_
#define GLENN_KEYPAD_H_

//defines for port subregisters
#define DATA_DIR 0x01					// Data Direction
#define ALT_FUN 0x02						// Alternate function
#define OUT_CTL 0x03						// Output Control
#define HDR_EN 0x04						// High Drive Enable
#define SMRS_EN 0x05						// Stop Mode Recovery

//keypad prototypes
void init_port_d(void);					// Initializes port D
int keyindex(void);						// Get the key value for lookup table 
unsigned char anykey(void);			// Poll the keypad
char keyval(char);						// Lookup table
void waitkeypressed(void);				// Wait for key press
void waitkeyreleased(void);			// Wait for key release


/*****************************************************************************\
 * Function:		init_port_d
 * Input:			void`
 * Description:	initiates port D for keypad
 * Dependencies:	none
\*****************************************************************************/
void init_port_d(void)
{
	//disable alternate function
	PDADDR = ALT_FUN;
	PDCTL = 0X00;
	
	//0x70 (0111 0000) = Pins: 6 5 4 input | Pins: others IS output
	PDADDR = DATA_DIR;
	PDCTL = 0X70;
	
	//set to pushpull
	PDADDR = OUT_CTL;
	PDCTL = 0X00;
	
	//prevent changes to subregisters
	PDADDR = 0X00;
}


/*****************************************************************************\
 * Function: keyindex		
 * Input:			
 * Description:	
 * Dependencies:	
\*****************************************************************************/
int keyindex(void)
{
	int 	rowcnt=0,							//keep track of row numbers
			colcnt=0, 							//keep track of column value
			coldata=0;							//read the value of PDIN
			
	char rowselect=0xF7;						//1111 0111 value to select row
	
	waitkeyreleased();						//wait key to be released
	waitkeypressed();							//wait key to be pressed
	
	while(rowcnt<4)
	{
		PDOUT=rowselect;
		coldata=PDIN;
		coldata=coldata&0x70;	
		
		if(coldata!=0x70) 
		{
			break;
		}
		
		rowcnt++;
		rowselect=rowselect>>1;				
	}
	
	switch (coldata)
  {
    case 0x60:
		colcnt=0x00; 
		return(3*rowcnt+colcnt);
		break;
    case 0x50:
		colcnt=0x01;
		return(3*rowcnt+colcnt);
		break;
    case 0x30:
		colcnt=0x02; 
		return(3*rowcnt+colcnt);
		break;
    default:
		return(0xFF);
   }	
}


/*****************************************************************************\
 * Function: anykey		
 * Input:			
 * Description:	
 * Dependencies:	
\*****************************************************************************/
unsigned char anykey(void)
{
	unsigned char ip;
	char ipval;
	
	PDOUT=0x00;
	ipval=PDIN&0x70;

	if(ipval==0x70)
	{
		ip=0;
		return(ip);
	}
	
	else
	{
		ip=1;
		return(ip);
	}
}


/*****************************************************************************\
 * Function: keyval		
 * Input:			
 * Description:	
 * Dependencies:	
\*****************************************************************************/

char keyval(char index)
{
	char table[]={3,2,1,6,5,4,9,8,7,11,0,10};
	char key;
	key=table[index];
	return(key);
}


/*****************************************************************************\
 * Function: waitkeypressed		
 * Input:			
 * Description:	
 * Dependencies:	
\*****************************************************************************/
void waitkeypressed(void)
{
	while(anykey()==0)
	{
		; 
	}
	delay(2);
	return;
}


/*****************************************************************************\
 * Function: waitkeyreleased		
 * Input:			
 * Description:	
 * Dependencies:	
\*****************************************************************************/
void waitkeyreleased(void)
{
	while(anykey()!=0)
	{
		; 
	}
	delay(2);
return;
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

#endif		//GLENN_KEYPAD_H_