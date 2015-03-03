#ifndef GLENN_KEYPAD_H_
#define GLENN_KEYPAD_H_

//defines for port subregisters
#define DATA_DIR 0x01					// Data Direction
#define ALT_FUN 0x02						// Alternate function
#define OUT_CTL 0x03						// Output Control
#define HDR_EN 0x04						// High Drive Enable
#define SMRS_EN 0x05						// Stop Mode Recovery

//prototypes
void init_port_d(void);					// Initializes ports
int keyindex(void);
unsigned char anykey(void);
int keyval(int);
void waitkeypressed(void);
void waitkeyreleased(void);


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
	
	//set 6 5 4 to i/p, rest to o/p
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
int keyindex(void){
int rowcnt=0, colcnt=0, coldata=0;
char rowselect=0xF7;
	
	waitkeyreleased();
	waitkeypressed();
	
	while(rowcnt<4){
		PDOUT=rowselect;
		coldata=PDIN;
		coldata=coldata&0x70;	
		
		if(coldata!=0x70) 
		{break;}
		
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

#endif		//GLENN_KEYPAD_H_