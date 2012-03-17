/*
 * lcd.c - the driver module for my LCD package
 * By: Jimmy Larsson 
 * 
 * 970328 - v0.0001 - Project started
 *
 */

#include <stdlib.h>
#include <asm/io.h>
#include <errno.h>
#include "lcd.h"

/* Pins in the control register */
#define LCD_DISABLE     1
#define LCD_RS          2

/* Timing in microseconds       */
#define LCD_CLR_TIME       1640     /* pause after a clear command */
#define LCD_HOME_TIME      1640     /* pause after a home command  */
#define LCD_COMMAND_TIME   40       /* pause after other commands  */
#define LCD_WRRAM_TIME     120      /* Write ram delay             */
#define LCD_ECYCLE_TIME    1        /* Min. enable cycle time      */
#define LCD_CHARWRITE_TIME 120      /* time to wite a custom char  */

/* timing in nanoseconds        */
#define LCD_EPW_TIME      450      /* Min. enable pulse width      */
#define LCD_AS_TIME       140      /* Min address setup time (min) */
#define LCD_AH_TIME       10       /* address hold time (min)      */
#define LCD_DH_TIME       10       /* data hold time               */
#define LCD_DS_TIME       195      /* data setup time              */

/* base address of the lpt port */
static int   lpt_base = 0x0;

/* custom char definitions (ends with a value of -1)  */
static int   custom_chars[65] = {31,16,16,16,16,16,31, 0,      /* char 0 */
				 31,24,24,24,24,24,31, 0,      /* char 1 */
				 31,28,28,28,28,28,31, 0,      /* char 2 */
				 31,30,30,30,30,30,31, 0,      /* char 3 */
				 31,31,31,31,31,31,31, 0,      /* char 4 */
				 31, 1, 1, 1, 1, 1,31, 0,      /* char 5 */
				 31, 0, 0, 0, 0, 0,31, 0,      /* char 6 */
				 -1};

/*
 * lcd_put
 * puts data/instruction byte to LCD
 *
 */

void
lcd_put (
    int   data,        /* data byte */
    int   rs)          /* register select (0: instructio, 1: data) */
{
    if (rs == 1)
	outb (0 | LCD_DISABLE, lpt_base + 2);      /* rs high, enable low  */
    else
	outb (LCD_RS | LCD_DISABLE, lpt_base + 2); /* rs low, enable low   */

    nanosleep (LCD_AS_TIME);                /* wait tAS       */

    if (rs == 1)                            /* rise enable    */ 
	outb (0, lpt_base + 2);             /* rs+enable high */
    else
	outb (LCD_RS, lpt_base + 2);        /* enable high    */

    outb (data & 0xff, lpt_base);           /* put data       */
    nanosleep (LCD_EPW_TIME);               /* wait PWEH      */

    if (rs == 1)                            /* lower enable   */
	outb (LCD_DISABLE, lpt_base + 2);
    else
	outb (LCD_RS | LCD_DISABLE, lpt_base + 2);

    nanosleep (LCD_DH_TIME);                /* wait tWH       */
                                            /* wait TcycE - PWEH */
    nanosleep (LCD_ECYCLE_TIME * 1000 - LCD_EPW_TIME);
}
    

/*
 * lcd_clear
 * clears and homes the cursor
 *
 */

void
lcd_clear (void)
{
    lcd_put (1, 0);
    usleep (LCD_CLR_TIME);
}

/*
 * lcd_home
 * homes the cursor
 *
 */

void
lcd_home (void)
{
    lcd_pos (1, 1);
}

/*
 * lcd_on
 * turns display,cursor,blink on/off. Use LCD_CURS_ON, LCD_ON, LCD_BLINK
 *
 */

void
lcd_on (
    int   val)
{
    lcd_put ((val & 7) | 8, 0);
    usleep (LCD_COMMAND_TIME);
}

/*
 * lcd_scroll
 * scrolls display or cursor. Use: LCD_SCROLL_DISPLAY, LCD_SCROLL_RIGHT
 *
 */

void
lcd_scroll (
    int   val)
{
    lcd_put ((val & 15) | 16, 0);
    usleep (LCD_COMMAND_TIME);
}

/*
 * lcd_function_set
 * sets up the display functions
 * Use: LCD_8BIT, LCD_16DUTY, LCD_5x11FONT
 *
 */

void
lcd_function_set (
    int   val)
{
    lcd_put ((val & 31) | 32, 0);
    usleep (LCD_COMMAND_TIME);
}

/*
 * lcd_addr_set
 * set the char-ram addr. for the next read/write
 *
 */

void
lcd_addr_set (
    int   addr)
{
    lcd_put ((addr & 63) | 64, 0);
    usleep (LCD_COMMAND_TIME);
}

/*
 * lcd_entry
 * sets lcd's entry mode. Use: LCD_SCROLLING, LCD_INCR
 *
 */

void
lcd_entry (
    int   val)
{
    lcd_put ((val & 3) | 4, 0);
    usleep (LCD_COMMAND_TIME);
}

/*
 * lcd_init
 * initializes lcd communication and clears it.
 * returns 0 - success, otherwise the errno from ioperm.
 *
 */

int
lcd_init (
    int   lpt_addr)
{
    int   i = 0;

    lpt_base = lpt_addr;

    if (ioperm (lpt_base, 3, 1) == -1)
	return errno;

    outb (0x0, lpt_base);                       /* clear data */
    outb (LCD_DISABLE | LCD_RS, lpt_base + 2);  /* clear enable+rs */

    usleep (15000);
    lcd_put (48, 0);   /* 48 is a senseless init number (repeated) */
    usleep (4100);
    lcd_put (48, 0);
    usleep (100);
    lcd_put (48, 0);
    usleep (4100);

    /* 5x7 font, 8-bit, 1/16 duty cycle */
    lcd_function_set (LCD_8BIT | LCD_16DUTY);
    
    /* disp. off, curs. off, blink off */
    lcd_on (0);

    /* clear, home  */
    lcd_clear ();

    /* incr. right, no scrolling   */
    lcd_entry (LCD_INCR);

    /* send custom chars to display */
    while (custom_chars[i] != -1)
    {
	lcd_addr_set (i);
	lcd_put (custom_chars[i], 1);
	usleep (LCD_CHARWRITE_TIME);
	i++;
    }
    lcd_pos (1, 1);

    /* turn on LCD */
    lcd_on (LCD_ON);
    return 0;
}   

/*
 * lcd_putchar
 * prints a charater on the LCD
 *
 */

void
lcd_putchar (
    int   chr)
{
    lcd_put (chr & 0xff, 1);
    usleep (LCD_COMMAND_TIME);
}

/*
 * lcd_print
 * prints a string to the LCD
 *
 */

void
lcd_print (
    char   *str)
{
    while (*str != '\0')
    {
	lcd_putchar (*str);
	str++;
    }
}

/*
 * lcd_pos
 * positions cursor at line and column, starting at 1,1
 * 
 */

void
lcd_pos (
    int   col,
    int   line)
{
    int   pos;
    if (line == 2)
	pos = 0x40 + col - 1;
    else 
	pos = col - 1;

    lcd_put ((pos & 127) | 128, 0);
    usleep (LCD_COMMAND_TIME);
}

/*
 * lcd_print2
 * prints two strings, one on the first line, one on the second
 *
 */

void
lcd_print2 (
    char   *line1,
    char   *line2)
{
    int     num = 0;

    while (line1[num] != '\0' || line2[num] != '\0')
    {
	lcd_pos (num + 1, 1);
	if (line1[num] != '\0')
	    lcd_putchar (line1[num]);
    
	lcd_pos (num + 1, 2);
    	if (line2[num] != '\0')
	    lcd_putchar (line2[num]);

	num++;
    }
}

/*
 * lcd_makebar
 * make a bar graph of a floating point value between 0 and 1, in terms of LCD chars.
 * Takes a char vetor and length plus the value as arguments.
 *
 */

void
lcd_makebar (
    double   value,
    char    *bar,
    int      barlen)
{
    double   blocks;
    double   frac;
    int      pos = 0;

    if (value > 1)
	value = 1;
    if (value < 0)
	value = 0;

    blocks = value * barlen;
    frac = blocks - (int) blocks;

    while (pos < (int) blocks)
	bar[pos++] = '\x0c';

    if (frac > 0 && frac <= 0.25)
	bar[pos++] = '\x08';
    else if (frac > 0.25 && frac <= 0.5)
	bar[pos++] = '\x09';
    else if (frac > 0.5 && frac <= 0.75)
	bar[pos++] = '\x0a';
    else if (frac > 0.75 && frac < 1.0)
	bar[pos++] = '\x0b';

    while (pos < barlen)
	bar[pos++] = '\x0e';

    bar[pos++] = '\x0d';

    if (bar[0] == '\x0e')
	bar[0] = '\x08';

    bar[pos] = '\0';
}

