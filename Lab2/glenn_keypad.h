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
 * Input:			void
 * Description:	initiates port D for keypad
 * Dependencies:	none
\*****************************************************************************/
void init_port_d(void)
{
	//disable alternate function
	PDADDR = ALT_FUN;
	PDCTL = 0X00;
	
	//0x70 (0111 0000) = Pins: 6 5 4 INPUT | Pins: others is OUTPUT
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
 * Input: void
 * Description: This function scans for key inputs and index's the value
 * Dependencies: waitkeyreleased | waitkeypressed
\*****************************************************************************/
int keyindex(void)
{
	int 	rowcnt = 0,								//keep track of row numbers
			colcnt = 0, 							//keep track of column value
			coldata = 0;							//read the value of PDIN
			
	char rowselect = 0xF7;						//1111 0111 value to select row
	
	waitkeyreleased();							//wait key to be released
	waitkeypressed();								//wait key to be pressed
	
	while(rowcnt < 4)			
	{
		PDOUT = rowselect;						//set PDOUT = 1111 0111
		coldata = PDIN;							//put value of PDIN into coldata
		coldata = coldata & 0x70;				//read PDIN (PDIN & 0111 000)
		
		/*--
		Break the while loop if coldata does not equal 0111 0000. Otherwise
		procede to the next step below...
		--*/
		if(coldata != 0x70) 						
		{
			break;									
		}
		
		/*--
		Continue with the next row if coldata is still 0111 0000
		--*/
		rowcnt++;									//increment rowcnt
		rowselect = rowselect >> 1;			//shift '0' to next location	
	}
	
	/*--
	After a key has been detected follow each case
	and return the math result to keyindex()
	--*/
	switch (coldata)
	{
	//case for first column pressed (X110 0000)
   case 0x60:										
		colcnt = 0x00; 
		return(3 * rowcnt + colcnt);
		break;
	
	//case for second column pressed (X101 0000)
   case 0x50:										
		colcnt = 0x01;
		return(3 * rowcnt + colcnt);
		break;
	
	//case for third column pressed (X011 0000)	
   case 0x30:										
		colcnt = 0x02;
		return(3 * rowcnt + colcnt);
		break;
		
	/*--
	This is the defult case for giving an error.
	It returns 1111 1111 to keyindex()
	
	This case is usually triggered when the 
	while loop and coldata has a value not
	defined in the case statement..
	--*/	
   default:
		return(0xFF);
   }	
}



/*****************************************************************************\
 * Function: keyval		
 * Input: index		
 * Description: a look up table function using single dimention array
 * Dependencies: none
\*****************************************************************************/
char keyval(char index)
{
	/*--
	Formula for this lookup table is index = (3*row) + column
	array slot:		[0][1][2][3][4][5][6][7][8][9] [10] [11] <--- imaginary slot
	keypad val:		[3][2][1][6][5][4][9][8][7][11] [0] [10] <--- this is what gets printed
	
	for example: 	key #7 is located in row 2, col 2
						using (3*row)+col gives us 
						(3*2)+2 = 8
						
						This means key #7 is in array slot 8 as
						shown above.
	--*/
	char table[] = {3,2,1,6,5,4,9,8,7,11,0,10};
	char key;
	key = table[index];
	return(key);
}



/*****************************************************************************\
 * Function: anykey		
 * Input: void			
 * Description: function for tessting to see if anykey is pressed
 * Dependencies: none
\*****************************************************************************/
unsigned char anykey(void)
{
	unsigned char ip;						//storage for input test result
	char ipval;								//storage for input value
	
	PDOUT = 0x00;							//set all pins to low
	ipval = PDIN & 0x70;					//read PDIN

	/*--
	This if-else statement will test
	to see if PDIN has changed or not.
	
	No change gives input value a '0'
	A change gives input value a '1'
	--*/
	if(ipval == 0x70)	
	{
		//return 0 since nothing changed (still 0x70)
		ip = 0;
		return(ip);
	}
	
	else
	{
		//something changed, return a 1
		ip = 1;
		return(ip);
	}
}



/*****************************************************************************\
 * Function: waitkeypressed		
 * Input: void
 * Description: wait until a key is pressed
 * Dependencies: delay | anykey
\*****************************************************************************/
void waitkeypressed(void)
{
	while(anykey() == 0)
	{
		; 
	}
	
delay(2);
return;
}



/*****************************************************************************\
 * Function: waitkeyreleased		
 * Input: void
 * Description: wait intil key is released
 * Dependencies: delay | anykey
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



#endif		//GLENN_KEYPAD_H_