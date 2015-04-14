/*****************************************************************************\
 * Function:		
 * Input:			
 * Description:	
\*****************************************************************************/
void init_ports(void);
void delay(unsigned int);

void init_lcd(void);
void soft_reset(void);
void cmd_write(unsigned char);
void data_write(unsigned char);
void lcd_ready(void);
unsigned char rd_busy(void);
void string_write(char *);

int keyindex(void);
unsigned char anykey(void);
int keyval(int);
void waitkeypressed(void);
void waitkeyreleased(void);
void numprint(void);