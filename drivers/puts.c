#include "reg_io.h"

/* uart data register */
#define UART_UTBR	0	/*DLAB = 0*/

/* uart control register */
#define UART_UIER	1	/*DLAB = 0*/
#define UART_UFCR	2
#define UART_ULCR	3
#define UART_UMCR	4

#define UART_UDLL	0	/*DLAB = 1*/
#define UART_UDLM	1	/*DLAB = 2*/

/*uart status register */
#define UART_ULSR	5
#define UART_UMSR	6
#define UART_USCR	7

/*
 * Memory segments (32bit kernel mode addresses)
 */
#define SERIAL_BASE   0xFB005100	/* it8172 */
#define SER_CMD       5
#define SER_DATA      0x00
#define RX_READY      0x01
#define TX_BUSY       0x20

#define TIMEOUT       0xfffff	


const char digits[16] = "0123456789abcdef";
static volatile unsigned char *const com1 = (unsigned char *) SERIAL_BASE;
/********************************************************************/
unsigned int console_log_level = 4;
unsigned int msg_log_level = 5;	//default is disabled

/********************************************************************/
/*********************************************************************************
		local functions
*********************************************************************************/
/*delay*/
static inline void slow_down(void)
{
	int k;
	for (k = 0; k < 10000; k++);
}


/**********************************************************************************
**********************************************************************************/
/*
put a char, send to serial port in order to display on the host terminal
parameter:	c: the char
return:		void 
*/
void serial_putc (const char c)
//void putch(const unsigned char c)
{
	unsigned char ch;
	//int i = 0;

	do {
		/*read UART line status register*/
		//use MAG's macro, endian issues
		ch = ReadRegByte(SERIAL_BASE+SER_CMD);   //MAG
		/*
		slow_down();
		i++;
		if (i > TIMEOUT) {
			break;
		}
		*/
	} while (0 == (ch & TX_BUSY));
	//use MAG's macro, endian issues
	if(c=='\n')
		WriteRegByte(SERIAL_BASE+SER_DATA, '\r');	//MAG

	WriteRegByte(SERIAL_BASE+SER_DATA, c);		//MAG
}

int putchar(const char c)
{
	serial_putc(c);
	return c;
}


/*********************************************************************************/

/**********************************************************************************
**********************************************************************************/
/*
put a string, send to serial port in ordet to display on the host terminal
parameter:	cp: pointer points to the target string
return:		void
*/
static void internal_puts(const char *cp)
{
	unsigned char ch;
	int i = 0;

	while (*cp) {
		do {
			/*read UART line status register*/
			ch=ReadRegByte(SERIAL_BASE+SER_CMD);  //MAG
			i++;
			if (i > TIMEOUT) {
				break;
			}
		} while (0 == (ch & TX_BUSY));
		if(*cp=='\n')
		{
			WriteRegByte(SERIAL_BASE+SER_DATA, '\r');	//MAG
			WriteRegByte(SERIAL_BASE+SER_DATA, '\n');	//MAG
		}
		else{
			WriteRegByte(SERIAL_BASE+SER_DATA, *cp);	//MAG
		}
		cp++;
	}
}

void puts (const char *s)
{
	internal_puts(s);
	//serial_putc('\n');
	return;
}
/*********************************************************************************/

/**********************************************************************************
**********************************************************************************/
/*
print a 64 bit unsigned number in a hex format on host terminal
parameter:	ul: the 64bit number
return:		void
*/
void put64(unsigned long ul)
{
	int cnt;
	unsigned ch;

	cnt = 16;		/* 16 nibbles in a 64 bit long */
	serial_putc('0');
	serial_putc('x');
	do {
		cnt--;
		ch = (unsigned char) (ul >> cnt * 4) & 0x0F;
		serial_putc(digits[ch]);
	} while (cnt > 0);
}
/**********************************************************************************/

/***********************************************************************************
***********************************************************************************/
/*
print a 32 bit unsigned number in a hex format on host terminal
parameter:	u: the 32bit number
return:		void
*/
void put32(unsigned u)
{
	int cnt;
	unsigned ch;

	cnt = 8;		/* 8 nibbles in a 32 bit long */

	serial_putc('0');
	serial_putc('x');
	do {
		cnt--;
		ch = (unsigned char) (u >> cnt * 4) & 0x0F;
		serial_putc(digits[ch]);
	} while (cnt > 0);

}
/***********************************************************************************/

/***********************************************************************************
***********************************************************************************/
/*
print a 16 bit unsigned number in a hex format on host terminal
parameter:	u: the 16bit number
return:		void
*/
void put16(unsigned short u)
{
	int cnt;
	unsigned ch;

	cnt = 4;		/* 4 nibbles in a 16 bit long */

	serial_putc('0');
	serial_putc('x');
	do {
		cnt--;
		ch = (unsigned char) (u >> cnt * 4) & 0x0F;
		serial_putc(digits[ch]);
	} while (cnt > 0);

}
/***********************************************************************************/

/************************************************************************************
************************************************************************************/
/*
print a 8 bit--BYTE unsigned number in a hex format on host terminal
parameter:	u: the 8bit number
return:		void
*/
void put8(unsigned char u)
{
	int cnt;
	unsigned ch;

	cnt=2;

/*	putch('0');
	putch('x');	*/
	do {
		cnt--;
		ch = (unsigned char)(u>>cnt*4)&0x0F;
		serial_putc(digits[ch]);
	} while(cnt>0);
}


#define LINESZ 200
#if 0
int
printf(const char *fmt, ...)
{
	char buf[LINESZ * 2];
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsprintf(buf, fmt, args);   /* hopefully i < sizeof(buf)-4 */
	va_end(args);

	if(msg_log_level >= console_log_level)
		return i;

	internal_puts(buf);
	return i;
}
#endif

#if 1
/*
get a char, return value is the target char.
parameter:	void
return:		char got
*/
int serial_getc (void)
//unsigned char getch()
{
	unsigned char ch;
	//int i=0;

	do {
		/*read UART line status register*/
//		ch = com1[SER_CMD];
		//add MAG's macro, endian issues
		ch=ReadRegByte(SERIAL_BASE+SER_CMD);  //MAG
		/*
		slow_down();
		i++;
		if (i > TIMEOUT) {
			return;
		}
		*/
	} while (0 == (ch & RX_READY));
	/*UART Receiver buffer register*/
//	ch=com1[SER_DATA];
	ch=ReadRegByte(SERIAL_BASE+SER_DATA);	//MAG
	return ch;
}

unsigned char getch_nodelay(void)
{
	unsigned char ch;

	ch=ReadRegByte(SERIAL_BASE+SER_DATA);	//MAG

	return ch;
}
//new version of _getch() with timeout
unsigned char _getch(void) 
{
    unsigned char ch;
    int i = 0;
    
    do {
	
	    /*read UART line status register */ 
	    ch = ReadRegByte(SERIAL_BASE + SER_CMD);	//MAG
	i++;
	if (i > 0xff) {
	    return 0;
	}
    } while (0 == (ch & RX_READY));
    
	/*UART Receiver buffer register */ 
	ch = ReadRegByte(SERIAL_BASE + SER_DATA);	//MAG
    return ch;
}
#endif

#if 1
int ctrlc (void)
{
	if (getch_nodelay() == 0x03)	/* ^C - Control C */
		return 1;
	return 0;
}
#endif

#if 0
/*
get a string via serial port until get <CR> which equals '\r'
when user hit a key, meanwhile, put the key on the screen
parametre:	p: pointer points to the target string in the end
return 		the length of the string got
*/
int gets(unsigned char * p)
{
	unsigned char * ch;
	int ret=0;

	ch=p;
	
	while(1)
	{
		*ch=serial_getc();
		if(*ch=='\r')
		{
			*ch='\0';
			break;
		}else{
			serial_putc(*ch);
			ret++;
			ch++;
		}
		
	}

	return ret;
}
#endif
void serial_setbrg (void)
{
}

int serial_tstc (void)
{
    	unsigned char ch;

	ch = ReadRegByte(SERIAL_BASE + SER_CMD);	//MAG
     	return (0 != (ch & RX_READY));
}

int serial_init (void)
{
	unsigned char tmp;
	
	unsigned int val =0x0;

#ifdef MACH_SX8
	WriteRegByte(0xf500ee1d, 0x33);     //TXD RXD
#endif
	
	WriteRegByte((SERIAL_BASE+UART_ULCR),0x03);	//each character, 8 bits, stop bit 1 bit
	WriteRegByte((SERIAL_BASE+UART_UFCR),0xc7);	//FIFO trigger level: 14 bytes, enable FIFO and clear FIFO
	tmp=ReadRegByte((SERIAL_BASE+UART_ULCR));	
	WriteRegByte((SERIAL_BASE+UART_ULCR),tmp|0x80);	//set DLAB=1
	WriteRegByte((SERIAL_BASE+UART_UDLL),1);		//set baudrate=115200
	WriteRegByte((SERIAL_BASE+UART_UDLM),0);	
	WriteRegByte((SERIAL_BASE+UART_UMCR),0x06);

	tmp=ReadRegByte((SERIAL_BASE+UART_ULCR));	
	WriteRegByte((SERIAL_BASE+UART_ULCR),tmp&~0x80);	//set DLAB=0
	
	return 0;
	
}
