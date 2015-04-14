/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
int keyindex(void){
int rowcnt=0, colcnt=0, coldata=0;
char rowselect=0xF7;
	
	//waitkeyreleased(); 
	waitkeypressed();
	
	while(rowcnt<4)
	{
		PDOUT=rowselect;
		coldata=PDIN;
		coldata = coldata & 0x70;
		
		
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


/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
unsigned char anykey(void){
unsigned char ip;
char ipval;
	
PDOUT=0x00;
ipval=PDIN&0x70;

	if(ipval==0x70){
		ip=0;
		return(ip);
	}
	else{
		ip=1;
		return(ip);
	}
}


/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
int keyval(int index){
int table[]={3,2,1,6,5,4,9,8,7,11,0,10};
int key;
key=table[index];
return(key);
}


/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void waitkeypressed(void){

	while(anykey()==0){
		;
		}
	delay(3);
return;
}



/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void waitkeyreleased(void){

	while(anykey()!=0){
		;
		}
	delay(3);
return;
}



/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void string_write(char *message){
	
char line=1;
int n=0x00;
char *p_message;

cmd_write(0x80);
lcd_ready();
p_message=message;
	while(0 != *p_message){
		data_write(*p_message++);
		n++;
		if(n==0x10){
			if(line==1)
				line=2;
			else
				line=1;
		
			if (line==1)
				cmd_write(0x80);
			else
				cmd_write(0xC0);
			n=0x00;
			delay(8000);
			lcd_ready();}
		
		}
	
}



/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void init_ports(void){
	
	PEADDR=0X02;
	PECTL=0X00; //no alt function
	PEADDR=0X01;
	PECTL=0X00; //set to o/p
	PEADDR=0X03;
	PECTL=0X00; //set to pushpull
	PEADDR=0X00;
	
	PDADDR=0X02;
	PDCTL=0X00; //no alt function
	PDADDR=0X01;
	PDCTL=0X70; //set 6 5 4 to i/p, rest to o/p
	PDADDR=0X03;
	PDCTL=0X00; //set to pushpull
	PDADDR=0X00;
return;	
}


/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void delay(unsigned int count){

	unsigned int delay;
	
	while(count>0)
	{
		count--; //decrement time variable
		delay=0xDC; //assign value to variable delay
		while(delay>0) //exit loop when delay equals zero
		{
			delay--; //decrement delay
		}
	}

return;	
}


/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void init_lcd(void){
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


/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void soft_reset(void){

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



/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void cmd_write(unsigned char controlval){
	
	char highnib, lownib;
	
	highnib=controlval&0xF0;
	lownib=controlval<<4;
	
	lcd_ready();
	
	PEOUT=0x00; //write first nib
	PEOUT=highnib;
	PEOUT=highnib^0x08;
	PEOUT=highnib;
	PEOUT=lownib;
	PEOUT=lownib^0x08;
	PEOUT=lownib;
}



/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void data_write(unsigned char dataval){
	
	char highnibdata, lownibdata;
	
	highnibdata=(dataval&0xF0)|0x04;
	lownibdata=(dataval<<4)|0x04;
	
	lcd_ready();
	
	PEOUT=0x04; //write first nib
	PEOUT=highnibdata;
	PEOUT=highnibdata^0x08;
	PEOUT=highnibdata;
	PEOUT=lownibdata;
	PEOUT=lownibdata^0x08;
	PEOUT=lownibdata;	
}



/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void lcd_ready(void){
	
	
	PEADDR=0X01;
	PECTL=0XF0; //4 lines are now I/P
	PEADDR=0X00;
	
	while(rd_busy()==1)
		{
			;
		}
		
	init_ports();
	
}



/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
unsigned char rd_busy(void){
	unsigned char busyflag=0x00;
	unsigned char busy;
	
	
	PEOUT=0x02; //set to read
	PEOUT=0x0A; //enable
	busyflag=PEIN;
	busyflag=busyflag&0x80;
	PEOUT=0x02;
	PEOUT=0x0A; //pulse to read second nib
	PEOUT=0x02;
	
	
	if (busyflag==0x0)
		busy=0;
	else 
		busy=1;
	
	return(busy);
}