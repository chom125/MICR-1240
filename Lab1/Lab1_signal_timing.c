//include
#include <ez8.h>
#include "glenn_lcd.h"

//main loop
void main(void)
{
	while (1) 
	{
		//initiate ports
		init_ports();
		
		//pulse null
		PEOUT = 0x30;
	
		//pulse enable
		PEOUT = 0x38;
	
		//pulse null
		PEOUT = 0x30;
	
		//data-read
		PEOUT = 0x36;
	}
}
